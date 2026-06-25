#include "scoreboard.hpp"

void Scoreboard::Reset()
{
    home.goals = away.goals = 0;
    home.yellowCards = away.yellowCards = 0;
    home.redCards = away.redCards = 0;
    period = 1;
    matchStarted = matchEnded = false;
    events.clear();
}

void Scoreboard::AddGoal(int teamIndex, const std::string &player, int minute)
{
    if (teamIndex == 0) home.goals++;
    else                away.goals++;
    events.push_back({MatchEvent::Type::Goal, player, "", minute, teamIndex});
}

void Scoreboard::AddCard(MatchEvent::Type cardType, int teamIndex,
                         const std::string &player, int minute)
{
    if (teamIndex == 0) {
        if (cardType == MatchEvent::Type::YellowCard) home.yellowCards++;
        else                                          home.redCards++;
    } else {
        if (cardType == MatchEvent::Type::YellowCard) away.yellowCards++;
        else                                          away.redCards++;
    }
    events.push_back({cardType, player, "", minute, teamIndex});
}

void Scoreboard::AddSubstitution(int teamIndex, const std::string &pIn,
                                  const std::string &pOut, int minute)
{
    events.push_back({MatchEvent::Type::Substitution, pIn, pOut, minute, teamIndex});
}

std::string Scoreboard::GetPeriodString() const
{
    switch (period) {
        case 1:  return "1T";
        case 2:  return "2T";
        case 3:  return "TE1";
        case 4:  return "TE2";
        case 5:  return "PEN";
        default: return "FT";
    }
}
