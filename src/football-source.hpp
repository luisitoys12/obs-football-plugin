#pragma once
#include <obs-module.h>
#include <string>
#include "scoreboard.hpp"
#include "timer.hpp"
#include "graphics/renderer.hpp"

class FootballSource {
public:
    static void Register();
    static const char *GetName(void *unused);
    static void *Create(obs_data_t *settings, obs_source_t *source);
    static void Destroy(void *data);
    static void VideoRender(void *data, gs_effect_t *effect);
    static void VideoTick(void *data, float seconds);
    static uint32_t GetWidth(void *data);
    static uint32_t GetHeight(void *data);
    static obs_properties_t *GetProperties(void *data);
    static void GetDefaults(obs_data_t *settings);
    static void Update(void *data, obs_data_t *settings);

private:
    obs_source_t *source;
    Scoreboard scoreboard;
    MatchTimer timer;
    Renderer renderer;
    uint32_t baseWidth  = 1920;
    uint32_t baseHeight = 1080;
};
