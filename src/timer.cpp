#include "timer.hpp"

void MatchTimer::Start()  { running = true; }
void MatchTimer::Pause()  { running = false; }
void MatchTimer::Reset()  { running = false; elapsed = 0.0f; addedTime = 0; }

void MatchTimer::Tick(float deltaSeconds)
{
    if (running) elapsed += deltaSeconds;
}

void MatchTimer::SetAddedTime(int minutes)
{
    addedTime = minutes;
}

void MatchTimer::SetTime(int minutes, int seconds)
{
    elapsed = static_cast<float>(minutes * 60 + seconds);
}

bool MatchTimer::IsInAddedTime() const
{
    return running && (elapsed > halfLength) && (addedTime > 0);
}
