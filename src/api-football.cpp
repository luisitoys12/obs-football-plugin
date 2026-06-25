#include "api-football.hpp"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <obs-module.h>
#include <thread>
#include <chrono>

using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *out)
{
    out->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

void APIFootball::SetApiKey(const std::string &key)
{
    apiKey = key;
}

std::string APIFootball::HttpGet(const std::string &url)
{
    CURL *curl = curl_easy_init();
    std::string response;
    if (!curl) return response;

    struct curl_slist *headers = nullptr;
    std::string authHeader = "x-apisports-key: " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL,            url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER,     headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,      &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,        10L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

void APIFootball::FetchLiveMatches(MatchesCallback onSuccess, ErrorCallback onError)
{
    std::thread([this, onSuccess, onError]() {
        try {
            // League 262 = Liga MX; use "live=all" for all live matches
            std::string url = "https://v3.football.api-sports.io/fixtures?live=all&league=262";
            std::string raw = HttpGet(url);
            if (raw.empty()) { onError("Empty response from API-Football"); return; }

            json j = json::parse(raw);
            std::vector<LiveMatch> matches;

            for (auto &fix : j["response"]) {
                LiveMatch m;
                m.id          = fix["fixture"]["id"];
                m.homeTeam    = fix["teams"]["home"]["name"];
                m.awayTeam    = fix["teams"]["away"]["name"];
                m.homeShort   = fix["teams"]["home"]["name"].get<std::string>().substr(0, 3);
                m.awayShort   = fix["teams"]["away"]["name"].get<std::string>().substr(0, 3);
                m.homeLogoUrl = fix["teams"]["home"]["logo"];
                m.awayLogoUrl = fix["teams"]["away"]["logo"];
                m.homeGoals   = fix["goals"]["home"].is_null() ? 0 : fix["goals"]["home"].get<int>();
                m.awayGoals   = fix["goals"]["away"].is_null() ? 0 : fix["goals"]["away"].get<int>();
                m.minute      = fix["fixture"]["status"]["elapsed"].is_null() ? 0 : fix["fixture"]["status"]["elapsed"].get<int>();
                m.status      = fix["fixture"]["status"]["short"];
                m.league      = fix["league"]["name"];
                m.leagueLogo  = fix["league"]["logo"];
                matches.push_back(m);
            }
            onSuccess(matches);
        } catch (std::exception &e) {
            onError(std::string("API parse error: ") + e.what());
        }
    }).detach();
}

void APIFootball::ApplyToScoreboard(const LiveMatch &match, Scoreboard &board)
{
    board.home.name      = match.homeTeam;
    board.home.shortName = match.homeShort;
    board.home.goals     = match.homeGoals;
    board.away.name      = match.awayTeam;
    board.away.shortName = match.awayShort;
    board.away.goals     = match.awayGoals;
    // Map status to period
    if      (match.status == "1H")  board.period = 1;
    else if (match.status == "2H")  board.period = 2;
    else if (match.status == "ET")  board.period = 3;
    else if (match.status == "P")   board.period = 5;
}

void APIFootball::StartAutoRefresh(int intervalSec, int matchId,
                                    MatchesCallback onSuccess, ErrorCallback onError)
{
    autoRefreshRunning = true;
    std::thread([this, intervalSec, matchId, onSuccess, onError]() {
        while (autoRefreshRunning) {
            FetchLiveMatches(onSuccess, onError);
            std::this_thread::sleep_for(std::chrono::seconds(intervalSec));
        }
    }).detach();
}

void APIFootball::StopAutoRefresh()
{
    autoRefreshRunning = false;
}

std::string APIFootball::DownloadTeamLogo(const std::string &url, const std::string &savePath)
{
    CURL *curl = curl_easy_init();
    if (!curl) return "";
    FILE *fp = fopen(savePath.c_str(), "wb");
    if (!fp) { curl_easy_cleanup(curl); return ""; }
    curl_easy_setopt(curl, CURLOPT_URL,      url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
    return savePath;
}
