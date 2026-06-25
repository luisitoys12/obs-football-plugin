#include "football-source.hpp"
#include <obs-module.h>

static obs_source_info football_source_info = {};

void FootballSource::Register()
{
    football_source_info.id             = "football_overlay_source";
    football_source_info.type           = OBS_SOURCE_TYPE_INPUT;
    football_source_info.output_flags   = OBS_SOURCE_VIDEO | OBS_SOURCE_CUSTOM_DRAW;
    football_source_info.get_name       = FootballSource::GetName;
    football_source_info.create         = FootballSource::Create;
    football_source_info.destroy        = FootballSource::Destroy;
    football_source_info.video_render   = FootballSource::VideoRender;
    football_source_info.video_tick     = FootballSource::VideoTick;
    football_source_info.get_width      = FootballSource::GetWidth;
    football_source_info.get_height     = FootballSource::GetHeight;
    football_source_info.get_properties = FootballSource::GetProperties;
    football_source_info.get_defaults   = FootballSource::GetDefaults;
    football_source_info.update         = FootballSource::Update;

    obs_register_source(&football_source_info);
    blog(LOG_INFO, "[OBS Football Plugin] Source registered.");
}

const char *FootballSource::GetName(void *)
{
    return obs_module_text("FootballOverlay");
}

void *FootballSource::Create(obs_data_t *settings, obs_source_t *source)
{
    FootballSource *fs = new FootballSource();
    fs->source = source;
    fs->renderer.Init(fs->baseWidth, fs->baseHeight);
    FootballSource::Update(fs, settings);
    return fs;
}

void FootballSource::Destroy(void *data)
{
    delete static_cast<FootballSource *>(data);
}

void FootballSource::VideoRender(void *data, gs_effect_t *)
{
    FootballSource *fs = static_cast<FootballSource *>(data);
    fs->renderer.Render(fs->scoreboard, fs->timer);
}

void FootballSource::VideoTick(void *data, float seconds)
{
    FootballSource *fs = static_cast<FootballSource *>(data);
    fs->timer.Tick(seconds);
    fs->renderer.Tick(seconds);
}

uint32_t FootballSource::GetWidth(void *data)
{
    return static_cast<FootballSource *>(data)->baseWidth;
}

uint32_t FootballSource::GetHeight(void *data)
{
    return static_cast<FootballSource *>(data)->baseHeight;
}

obs_properties_t *FootballSource::GetProperties(void *)
{
    obs_properties_t *props = obs_properties_create();

    // Theme selector
    obs_property_t *theme = obs_properties_add_list(
        props, "theme", obs_module_text("Theme"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
    obs_property_list_add_string(theme, "Liga MX",     "ligamx");
    obs_property_list_add_string(theme, "ESPN",        "espn");
    obs_property_list_add_string(theme, "FOX Sports",  "fox");
    obs_property_list_add_string(theme, "Copa America","copa");

    // Scorebug position
    obs_property_t *pos = obs_properties_add_list(
        props, "scorebug_pos", obs_module_text("ScoreBugPosition"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);
    obs_property_list_add_string(pos, obs_module_text("TopLeft"),     "tl");
    obs_property_list_add_string(pos, obs_module_text("TopRight"),    "tr");
    obs_property_list_add_string(pos, obs_module_text("BottomLeft"),  "bl");
    obs_property_list_add_string(pos, obs_module_text("BottomRight"), "br");

    return props;
}

void FootballSource::GetDefaults(obs_data_t *settings)
{
    obs_data_set_default_string(settings, "theme",        "ligamx");
    obs_data_set_default_string(settings, "scorebug_pos", "tl");
}

void FootballSource::Update(void *data, obs_data_t *settings)
{
    FootballSource *fs = static_cast<FootballSource *>(data);
    std::string theme  = obs_data_get_string(settings, "theme");
    std::string pos    = obs_data_get_string(settings, "scorebug_pos");
    fs->renderer.SetTheme(theme);
    fs->renderer.SetScoreBugPosition(pos);
}
