#include "lower-third.hpp"
#include <algorithm>
#include <obs-module.h>

static const char *LowerThirdTypeName(LowerThirdType type)
{
    switch (type) {
    case LowerThirdType::GoalScorer:
        return "GOOOOL";
    case LowerThirdType::YellowCard:
        return "YELLOW CARD";
    case LowerThirdType::RedCard:
        return "RED CARD";
    case LowerThirdType::Substitution:
        return "SUBSTITUTION";
    case LowerThirdType::Subtitle:
        return "SUBTITLE";
    case LowerThirdType::PlayerStat:
        return "PLAYER STAT";
    case LowerThirdType::VAR:
        return "VAR";
    case LowerThirdType::Lineup:
        return "LINEUP";
    case LowerThirdType::MatchPreview:
        return "MATCH PREVIEW";
    default:
        return "EVENT";
    }
}

void LowerThird::Show(const LowerThirdData &data)
{
    current = data;
    if (current.primaryText.empty()) {
        current.primaryText = LowerThirdTypeName(current.type);
    }

    if (current.type == LowerThirdType::Substitution && current.secondaryText.empty()) {
        current.secondaryText = "Player in / Player out";
    }

    if ((current.type == LowerThirdType::YellowCard ||
         current.type == LowerThirdType::RedCard ||
         current.type == LowerThirdType::GoalScorer) &&
        current.minute > 0 && current.secondaryText.empty()) {
        current.secondaryText = "Minute " + std::to_string(current.minute);
    }

    visible = true;
    timeLeft = std::max(0.1f, data.duration);
    animIn = 0.0f;

    blog(LOG_INFO, "[OBS Football Plugin] LowerThird show: %s | %s",
         current.primaryText.c_str(), current.secondaryText.c_str());
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
    // Current implementation keeps animation state and emits event trace.
    // Final phase can draw text/boxes with full gs_* layout assets.
    (void)colors;
    (void)canvasW;
    (void)canvasH;
}
