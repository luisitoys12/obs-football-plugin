#pragma once

class MatchTimer {
public:
    void Start();
    void Pause();
    void Reset();
    void Tick(float deltaSeconds);
    void SetAddedTime(int minutes);
    void SetTime(int minutes, int seconds);

    int  GetMinutes()  const { return static_cast<int>(elapsed) / 60; }
    int  GetSeconds()  const { return static_cast<int>(elapsed) % 60; }
    int  GetAddedTime() const { return addedTime; }
    bool IsRunning()   const { return running; }
    bool IsInAddedTime() const;
    float GetElapsed() const { return elapsed; }

private:
    float elapsed    = 0.0f;
    int   addedTime  = 0;
    int   halfLength = 45 * 60; // seconds
    bool  running    = false;
};
