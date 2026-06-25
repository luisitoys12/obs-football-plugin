#include "scorebug.hpp"
#include <algorithm>
#include <obs-module.h>

void ScoreBug::GetPosition(ScoreBugPos pos, uint32_t cW, uint32_t cH,
                           float &x, float &y)
{
    const float margin = 24.0f;
    const float bugW = 460.0f;
    const float bugH = 92.0f;

    switch (pos) {
    case ScoreBugPos::TopLeft:
        x = margin;
        y = margin;
        break;
    case ScoreBugPos::TopRight:
        x = static_cast<float>(cW) - bugW - margin;
        y = margin;
        break;
    case ScoreBugPos::BottomLeft:
        x = margin;
        y = static_cast<float>(cH) - bugH - margin;
        break;
    case ScoreBugPos::BottomRight:
        x = static_cast<float>(cW) - bugW - margin;
        y = static_cast<float>(cH) - bugH - margin;
        break;
    }
}

void ScoreBug::Render(const Scoreboard &board, const MatchTimer &timer,
                      ScoreBugPos pos, const Renderer::ThemeColors &colors,
                      uint32_t canvasW, uint32_t canvasH)
{
    float x = 0.0f;
    float y = 0.0f;
    GetPosition(pos, canvasW, canvasH, x, y);

    // Placeholder render hook until full gs_* primitives/text implementation lands.
    (void)board;
    (void)timer;
    (void)colors;
    (void)x;
    (void)y;
}
