#include <obs-module.h>
#include <obs-frontend-api.h>
#include "football-source.hpp"
#include "ui/control-dock.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-football-plugin", "en-US")

MODULE_EXPORT const char *obs_module_description(void)
{
    return "OBS Football Plugin — Broadcast overlays for football / soccer (Liga MX, ESPN, FOX style)";
}

bool obs_module_load(void)
{
    blog(LOG_INFO, "[OBS Football Plugin] Loading v1.0.0...");

    // Register overlay source
    FootballSource::Register();

    // Add dock panel to OBS UI
    obs_frontend_add_event_callback(
        [](enum obs_frontend_event event, void *) {
            if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
                ControlDock::AddToOBS();
            }
        },
        nullptr
    );

    blog(LOG_INFO, "[OBS Football Plugin] Loaded successfully.");
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "[OBS Football Plugin] Unloaded.");
}
