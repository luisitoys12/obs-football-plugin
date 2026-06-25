#pragma once
#include <string>
#include <vector>
#include <functional>
#include "scoreboard.hpp"

struct LiveMatch {
    int         id;
    std::string homeTeam;
    std::string awayTeam;
    std::string homeShort;
    std::string awayShort;
    std::string homeLogoUrl;
    std::string awayLogoUrl;
    int         homeGoals;
    int         awayGoals;
    int         minute;
    std::string status;    // "1H", "2H", "HT", "FT", etc.
    std::string league;
    std::string leagueLogo;
};

class APIFootball {
public:
    using MatchesCallback = std::function<void(std::vector<LiveMatch>)>;
    using ErrorCallback   = std::function<void(std::string)>;

    void SetApiKey(const std::string &key);
    void FetchLiveMatches(MatchesCallback onSuccess, ErrorCallback onError);
    void FetchMatchEvents(int matchId, MatchesCallback onSuccess, ErrorCallback onError);
    void ApplyToScoreboard(const LiveMatch &match, Scoreboard &board);
    void StartAutoRefresh(int intervalSeconds, int matchId,
                          MatchesCallback onSuccess, ErrorCallback onError);
    void StopAutoRefresh();

    static std::string DownloadTeamLogo(const std::string &url,
                                        const std::string &savePath);

private:
    std::string apiKey;
    bool        autoRefreshRunning = false;

    std::string HttpGet(const std::string &url);
};
