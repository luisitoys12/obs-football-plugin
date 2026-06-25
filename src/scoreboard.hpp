#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct TeamInfo {
    std::string name;
    std::string shortName;   // e.g. "CHI", "AME"
    std::string logoPath;
    uint32_t    primaryColor   = 0xFF0000FF; // RGBA
    uint32_t    secondaryColor = 0xFFFFFFFF;
    int         goals          = 0;
    int         yellowCards    = 0;
    int         redCards       = 0;
};

struct MatchEvent {
    enum class Type { Goal, YellowCard, RedCard, Substitution, VAR };
    Type        type;
    std::string playerName;
    std::string playerOut;   // For substitutions
    int         minute;
    int         teamIndex;   // 0 = home, 1 = away
};

class Scoreboard {
public:
    TeamInfo home;
    TeamInfo away;
    int      period       = 1;    // 1 = 1st half, 2 = 2nd, 3 = ET1, 4 = ET2, 5 = Penalties
    bool     matchStarted = false;
    bool     matchEnded   = false;
    std::vector<MatchEvent> events;

    void Reset();
    void AddGoal(int teamIndex, const std::string &player, int minute);
    void AddCard(MatchEvent::Type cardType, int teamIndex,
                 const std::string &player, int minute);
    void AddSubstitution(int teamIndex, const std::string &playerIn,
                         const std::string &playerOut, int minute);
    std::string GetPeriodString() const;
};
