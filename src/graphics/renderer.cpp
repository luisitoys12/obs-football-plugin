#include "renderer.hpp"
#include <obs-module.h>
#include <graphics/vec4.h>
#include <algorithm>
#include <string>

bool Renderer::Init(uint32_t w, uint32_t h)
{
    width  = w;
    height = h;
    ApplyThemeColors();
    blog(LOG_INFO, "[OBS Football Plugin] Renderer initialized %dx%d", w, h);
    return true;
}

void Renderer::SetTheme(const std::string &themeStr)
{
    if      (themeStr == "ligamx") theme = Theme::LigaMX;
    else if (themeStr == "espn")   theme = Theme::ESPN;
    else if (themeStr == "fox")    theme = Theme::FOX;
    else if (themeStr == "copa")   theme = Theme::CopaAmerica;
    ApplyThemeColors();
}

void Renderer::ApplyThemeColors()
{
    switch (theme) {
    case Theme::LigaMX:
        // Liga MX: Dark navy + gold (colores oficiales)
        colors.bugBackground = 0xE61A1A2E;
        colors.bugAccent     = 0xE616213E;
        colors.goalFlash     = 0xFFFFD700; // Gold
        colors.scoreColor    = 0xFFFFD700;
        colors.periodBg      = 0xFF8B0000; // Rojo oscuro
        break;
    case Theme::ESPN:
        // ESPN: Dark charcoal + red accent
        colors.bugBackground = 0xE6121212;
        colors.bugAccent     = 0xE61F1F1F;
        colors.goalFlash     = 0xFFCC0000; // ESPN Red
        colors.scoreColor    = 0xFFFFFFFF;
        colors.periodBg      = 0xFFCC0000;
        break;
    case Theme::FOX:
        // FOX Sports: Black + blue
        colors.bugBackground = 0xE6000000;
        colors.bugAccent     = 0xE6001F5B; // Fox Blue
        colors.goalFlash     = 0xFF0066CC;
        colors.scoreColor    = 0xFFFFFFFF;
        colors.periodBg      = 0xFF0066CC;
        break;
    case Theme::CopaAmerica:
        // Copa America: Blue gradient
        colors.bugBackground = 0xE60D1B4B;
        colors.bugAccent     = 0xE6162451;
        colors.goalFlash     = 0xFFFFCC00;
        colors.scoreColor    = 0xFFFFCC00;
        colors.periodBg      = 0xFF0033A0;
        break;
    }
}

void Renderer::SetScoreBugPosition(const std::string &posStr)
{
    if      (posStr == "tl") bugPos = ScoreBugPos::TopLeft;
    else if (posStr == "tr") bugPos = ScoreBugPos::TopRight;
    else if (posStr == "bl") bugPos = ScoreBugPos::BottomLeft;
    else if (posStr == "br") bugPos = ScoreBugPos::BottomRight;
}

void Renderer::TriggerGoalAnimation(int teamIndex)
{
    goalTimer = 3.0f; // 3 seconds animation
    goalTeam  = teamIndex;
}

void Renderer::Tick(float deltaSeconds)
{
    if (goalTimer > 0.0f) goalTimer -= deltaSeconds;
    if (lowerThirdTimer > 0.0f) lowerThirdTimer -= deltaSeconds;
}

void Renderer::Render(const Scoreboard &board, const MatchTimer &timer)
{
    UpdateLowerThirdFromEvents(board);
    RenderScoreBug(board, timer);
    if (lowerThirdTimer > 0.0f) RenderLowerThird();
    if (goalTimer > 0.0f) RenderGoalAnimation();
}

void Renderer::UpdateLowerThirdFromEvents(const Scoreboard &board)
{
    while (processedEvents < board.events.size()) {
        const MatchEvent &ev = board.events[processedEvents++];
        lowerThirdSecondary.clear();

        switch (ev.type) {
        case MatchEvent::Type::Substitution:
            lowerThirdPrimary = "SUBSTITUTION";
            lowerThirdSecondary = "IN: " + ev.playerName + " | OUT: " + ev.playerOut;
            lowerThirdTimer = 5.0f;
            break;
        case MatchEvent::Type::Goal:
            lowerThirdPrimary = "GOOOOL";
            lowerThirdSecondary = ev.playerName;
            lowerThirdTimer = 4.0f;
            break;
        case MatchEvent::Type::YellowCard:
            lowerThirdPrimary = "YELLOW CARD";
            lowerThirdSecondary = ev.playerName;
            lowerThirdTimer = 4.0f;
            break;
        case MatchEvent::Type::RedCard:
            lowerThirdPrimary = "RED CARD";
            lowerThirdSecondary = ev.playerName;
            lowerThirdTimer = 4.0f;
            break;
        case MatchEvent::Type::VAR:
            lowerThirdPrimary = "SUBTITLE";
            lowerThirdSecondary = ev.playerName;
            lowerThirdTimer = 6.0f;
            break;
        }

        blog(LOG_INFO, "[OBS Football Plugin] Lower-third: %s | %s",
             lowerThirdPrimary.c_str(), lowerThirdSecondary.c_str());
    }
}

void Renderer::RenderScoreBug(const Scoreboard &board, const MatchTimer &timer)
{
    // TODO (Phase 2): Full OpenGL scorebug render using gs_* OBS graphics API
    // Placeholder: draws a simple colored rectangle via OBS effect pipeline
    // Final implementation will render:
    //   [LOGO] TEAM_SHORT  SCORE - SCORE  TEAM_SHORT [LOGO]  | MM:SS | PERIOD
    (void)board;
    (void)timer;
}

void Renderer::RenderGoalAnimation()
{
    // TODO (Phase 4): GOOOL flash animation
    // Flashes the bug gold, expands momentarily, shows scorer name
}

void Renderer::RenderLowerThird()
{
    gs_effect_t *effect = obs_get_base_effect(OBS_EFFECT_SOLID);
    if (!effect)
        return;

    gs_eparam_t *colorParam = gs_effect_get_param_by_name(effect, "color");
    if (!colorParam)
        return;

    const float progress = std::clamp(lowerThirdTimer / 6.0f, 0.0f, 1.0f);
    const float barW = std::min(900.0f, width * 0.7f);
    const float barH = 86.0f;
    const float x = (static_cast<float>(width) - barW) * 0.5f;
    const float y = static_cast<float>(height) - 150.0f + (1.0f - progress) * 40.0f;

    vec4 rgba;
    vec4_set(&rgba, 0.07f, 0.09f, 0.18f, 0.82f);
    gs_effect_set_vec4(colorParam, &rgba);

    gs_matrix_push();
    gs_matrix_translate3f(x, y, 0.0f);
    gs_matrix_scale3f(barW, barH, 1.0f);
    while (gs_effect_loop(effect, "Solid")) {
        gs_draw_sprite(nullptr, 0, 1, 1);
    }
    gs_matrix_pop();
}

void Renderer::Destroy()
{
    if (renderTarget) {
        obs_enter_graphics();
        gs_texture_destroy(renderTarget);
        obs_leave_graphics();
        renderTarget = nullptr;
    }
}
