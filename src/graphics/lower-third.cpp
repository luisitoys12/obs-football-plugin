#include "lower-third.hpp"
#include <algorithm>
#include <obs-module.h>

void LowerThird::Show(const LowerThirdData &data)
{
    current = data;
    visible = true;
    timeLeft = std::max(0.1f, data.duration);
    animIn = 0.0f;
}

void LowerThird::Hide()
{
    visible = false;
    timeLeft = 0.0f;
    animIn = 0.0f;
}

void LowerThird::Tick(float deltaSeconds)
{
    if (!visible)
        return;

    animIn = std::min(1.0f, animIn + deltaSeconds * 6.0f);
    timeLeft -= deltaSeconds;
    if (timeLeft <= 0.0f)
        Hide();
}

void LowerThird::Render(const Renderer::ThemeColors &colors,
                        uint32_t canvasW, uint32_t canvasH)
{
    // Placeholder render hook until full animated lower-third is implemented.
    (void)colors;
    (void)canvasW;
    (void)canvasH;
}
