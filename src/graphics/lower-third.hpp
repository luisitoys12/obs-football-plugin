#pragma once
#include <string>
#include "renderer.hpp"

enum class LowerThirdType {
    PlayerStat,       // Player name + stats
    YellowCard,       // Yellow card + player + minute
    RedCard,          // Red card
    Substitution,     // Player in / player out
    GoalScorer,       // GOOOL + scorer name
    VAR,              // VAR review
    Lineup,           // Team lineup display
    MatchPreview      // Pre-match team names
};

struct LowerThirdData {
    LowerThirdType type;
    std::string    primaryText;
    std::string    secondaryText;
    int            minute     = 0;
    int            teamIndex  = 0;
    float          duration   = 5.0f;  // seconds on screen
};

class LowerThird {
public:
    void Show(const LowerThirdData &data);
    void Hide();
    void Tick(float deltaSeconds);
    void Render(const Renderer::ThemeColors &colors,
                uint32_t canvasW, uint32_t canvasH);
    bool IsVisible() const { return visible; }

private:
    bool           visible    = false;
    float          timeLeft   = 0.0f;
    float          animIn     = 0.0f;  // 0.0 → 1.0 slide-in
    LowerThirdData current;
};
