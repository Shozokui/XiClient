#include "RuntimeConfig.h"
#define TOML_ENABLE_WINDOWS_COMPAT true
#include "toml++/toml.hpp"

FFXI::Config::RuntimeConfig FFXI::Config::RuntimeConfig::instance{};

void ParseOptionalKeys(const toml::table& tbl)
{
    FFXI::Config::RuntimeConfig* cfg = &FFXI::Config::RuntimeConfig::instance;
    //Process
    cfg->process_priority = tbl["Process"]["priority"].value_or<unsigned int>(1);
    //Sound
    cfg->sound_enable = tbl["Sound"]["enable"].value_or<bool>(true);
    //Video
    int window_mode = tbl["Video"]["window_mode"].value_or<int>((int)FFXI::Config::RuntimeConfig::WindowMode::Windowed);
    if (window_mode < 0 || window_mode > 3) {
        window_mode = 1;
    }
    cfg->window_mode = (FFXI::Config::RuntimeConfig::WindowMode)window_mode;
    cfg->resolution_main.X = tbl["Video"]["main"][0].value_or<unsigned short>(640);
    cfg->resolution_main.Y = tbl["Video"]["main"][1].value_or<unsigned short>(480);
    cfg->resolution_ui.X = tbl["Video"]["ui"][0].value_or<unsigned short>(0);
    cfg->resolution_ui.Y = tbl["Video"]["ui"][1].value_or<unsigned short>(0);
    cfg->resolution_bg.X = tbl["Video"]["bg"][0].value_or<unsigned short>(512);
    cfg->resolution_bg.Y = tbl["Video"]["bg"][1].value_or<unsigned short>(512);
}

int FFXI::Config::RuntimeConfig::Initialize(std::filesystem::path config_file)
{
    if (std::filesystem::exists(config_file) == false) {
        return -1;
    }

    toml::table tbl;
    try
    {
        tbl = toml::parse_file(config_file.c_str());
        //Install dir is a required key
         toml::v3::optional<std::wstring> install_dir = tbl["Install"]["dir"].value<std::wstring>();
        if (install_dir.has_value() == false) {
            return -3;
        }

        std::filesystem::path dir = install_dir.value();

        if (dir.is_absolute() == true) {
            FFXI::Config::RuntimeConfig::instance.ffxi_install_path = dir;
        }
        else {
            FFXI::Config::RuntimeConfig::instance.ffxi_install_path = config_file.parent_path() / dir;
        }
    }
    catch (const toml::parse_error& err)
    {
        return -2;
    }
    
    ParseOptionalKeys(tbl);
    return 1;
}
