#ifndef MENU_CONFIGURATION_H
#define MENU_CONFIGURATION_H

#include <map>

#define MAIN_MENU_MODE MenuMode::MAIN_MENU
#define OFF_MENU_MODE MenuMode::MENU_OFF
#define DEBUG_MENU_MODE MenuMode::MENU_DEBUG_MODE

enum class MenuMode {
    MENU_OFF,
    MAIN_MENU,
    MENU_SETTINGS,
    MENU_DEVICES_INFO,
    MENU_SYSTEM_STATUS,
    MENU_CURRENT_STATE,
    MENU_SYSTEM_INFO,
    MENU_ERRORS_LOG,
    MENU_DEBUG_MODE};

struct MenuOption {
    String title;
    MenuMode nextScreen;
};

struct MenuScreen {
    String title;
    MenuMode previousScreen;
    bool autoRefresh;
    std::vector<MenuOption> options;
};

struct MenuConfiguration {
public:
    MenuScreen getMenuScreen(MenuMode menuMode){
        return getMenuBindings()[menuMode];
    }

private:
    std::map<MenuMode, MenuScreen>& getMenuBindings()
    {
        static std::map<MenuMode, MenuScreen> menuBindings = {
            {MenuMode::MAIN_MENU, MenuScreen {
                .title = "Main menu",
                .previousScreen = MenuMode::MENU_OFF,
                .autoRefresh = false,
                .options = {
                    MenuOption {
                        .title = "Settings",
                        .nextScreen = MenuMode::MENU_SETTINGS
                    }, 
                    MenuOption {
                        .title = "Devices info",
                        .nextScreen = MenuMode::MENU_DEVICES_INFO
                    }, 
                    MenuOption {
                        .title = "System status",
                        .nextScreen = MenuMode::MENU_SYSTEM_STATUS
                    }, 
                }
            }},
            {MenuMode::MENU_SETTINGS, MenuScreen {
                .title = "Settings",
                .previousScreen = MenuMode::MAIN_MENU,
                .autoRefresh = false,
                .options = {
                    MenuOption {
                        .title = "Not implemented",
                        .nextScreen = MenuMode::MENU_SETTINGS
                    }
                }
            }},
            {MenuMode::MENU_SYSTEM_STATUS, MenuScreen {
                .title = "System status",
                .previousScreen = MenuMode::MAIN_MENU,
                .autoRefresh = false,
                .options = {
                    MenuOption {
                        .title = "Current state",
                        .nextScreen = MenuMode::MENU_CURRENT_STATE
                    },
                    MenuOption {
                        .title = "System info",
                        .nextScreen = MenuMode::MENU_SYSTEM_INFO
                    },
                    MenuOption {
                        .title = "Errors Log",
                        .nextScreen = MenuMode::MENU_ERRORS_LOG
                    }
                }
            }},
            {MenuMode::MENU_DEVICES_INFO, MenuScreen {
                .title = "Devices info",
                .previousScreen = MenuMode::MAIN_MENU,
                .autoRefresh = true
            }},
            {MenuMode::MENU_CURRENT_STATE, MenuScreen {
                .title = "Current state",
                .previousScreen = MenuMode::MENU_SYSTEM_STATUS,
                .autoRefresh = true
            }},
            {MenuMode::MENU_SYSTEM_INFO, MenuScreen {
                .title = "System info",
                .previousScreen = MenuMode::MENU_SYSTEM_STATUS,
                .autoRefresh = true
            }},
            {MenuMode::MENU_ERRORS_LOG, MenuScreen {
                .title = "Errors Log",
                .previousScreen = MenuMode::MENU_SYSTEM_STATUS,
                .autoRefresh = true
            }}
        };
        return menuBindings;
    }
};

#endif