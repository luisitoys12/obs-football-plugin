#include "transitions.hpp"

#include <algorithm>

void OverlayTransition::Start(TransitionType type, float durationSeconds)
{
    currentType = type;
    duration = std::max(0.01f, durationSeconds);
    elapsed = 0.0f;
    active = (type != TransitionType::None);
}

void OverlayTransition::Tick(float deltaSeconds)
{
    if (!active)
        return;

    elapsed += deltaSeconds;
    if (elapsed >= duration) {
        elapsed = duration;
        active = false;
    }
}

float OverlayTransition::GetProgress() const
{
    if (duration <= 0.0f)
        return 1.0f;
    return std::clamp(elapsed / duration, 0.0f, 1.0f);
}
