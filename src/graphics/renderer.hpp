#pragma once
#include <string>
#include <obs-module.h>
#include "../scoreboard.hpp"
#include "../timer.hpp"

enum class Theme { LigaMX, ESPN, FOX, CopaAmerica };
enum class ScoreBugPos { TopLeft, TopRight, BottomLeft, BottomRight };

class Renderer {
public:
    struct ThemeColors {
        uint32_t bugBackground  = 0xFF1A1A2E; // Dark navy
        uint32_t bugAccent      = 0xFF16213E; // Deeper navy
        uint32_t textPrimary    = 0xFFFFFFFF; // White
        uint32_t textSecondary  = 0xFFCCCCCC; // Light gray
        uint32_t goalFlash      = 0xFFFFD700; // Gold (Liga MX trophy color)
        uint32_t scoreColor     = 0xFFFFD700; // Gold score
        uint32_t timerColor     = 0xFF00FF88; // Green timer
        uint32_t periodBg       = 0xFF8B0000; // Deep red (Liga MX)
    };

    bool Init(uint32_t width, uint32_t height);
    void Render(const Scoreboard &board, const MatchTimer &timer);
    void Tick(float deltaSeconds);
    void SetTheme(const std::string &themeStr);
    void SetScoreBugPosition(const std::string &posStr);
    void TriggerGoalAnimation(int teamIndex);
    void TriggerCardAnimation(int teamIndex, bool isRed);
    void TriggerSubAnimation(const std::string &pIn, const std::string &pOut);
    void Destroy();

private:
    uint32_t     width, height;
    Theme        theme     = Theme::LigaMX;
    ScoreBugPos  bugPos    = ScoreBugPos::TopLeft;
    float        goalTimer = 0.0f;
    int          goalTeam  = -1;
    gs_texture_t *renderTarget = nullptr;

    void RenderScoreBug(const Scoreboard &board, const MatchTimer &timer);
    void RenderLowerThird();
    void RenderGoalAnimation();

    // Theme colors (Liga MX palette as default)
    ThemeColors colors;

    void ApplyThemeColors();
};
