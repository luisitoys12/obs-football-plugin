#pragma once

#include <string>

enum class TransitionType {
    None,
    Fade,
    WipeHorizontal,
};

class OverlayTransition {
public:
    void Start(TransitionType type, float durationSeconds = 0.5f);
    void Tick(float deltaSeconds);
    bool IsActive() const { return active; }
    float GetProgress() const;
    TransitionType GetType() const { return currentType; }

private:
    TransitionType currentType = TransitionType::None;
    float elapsed = 0.0f;
    float duration = 0.0f;
    bool active = false;
};
