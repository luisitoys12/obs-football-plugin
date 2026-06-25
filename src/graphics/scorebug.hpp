#pragma once
#include "renderer.hpp"

// ScoreBug: The corner widget showing scores, time and period.
// Rendered as a native OBS graphics element via gs_* API.
class ScoreBug {
public:
    void Render(const Scoreboard &board, const MatchTimer &timer,
                ScoreBugPos pos, const Renderer::ThemeColors &colors,
                uint32_t canvasW, uint32_t canvasH);
private:
    void GetPosition(ScoreBugPos pos, uint32_t cW, uint32_t cH,
                     float &x, float &y);
};
