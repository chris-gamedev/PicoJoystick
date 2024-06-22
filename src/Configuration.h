#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <vector>
#include <array>

namespace MacroMapType
{
    enum TMacroMapType {
        none,
        macro1,
        macro2,
        macro3,
        macro4,
        macro5,
        macro6,
        macro7,
        macro8,
        turbo = 9,
        latchedTurbo = 10
    };
}

struct Configuration
{
    // Global
    bool global_menuHotkey_on = true;

    // MyJoystick
    uint8_t joystick_joyValueMap[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t joystick_buttonValueMap[12] = {1, 2, 4, 5, 7, 8, 11, 12, 13, 14, 15, 20};
    bool joystick_transmitToHost = true;
    // Menu

    // Apps
    std::array<uint8_t, 12> drawKeypresses_macroMap = {MacroMapType::none};

    // funStuff
    bool funThings_on = true;
};

class IConfigurable_
{
public:
    virtual void configure(Configuration *pconfig) = 0;
};

class Configurator_
{
public:
    Configurator_(std::initializer_list<IConfigurable_ *> list)
    {
        mvpConfigurables = list;
    }

    struct Configuration mConfig;
    std::vector<IConfigurable_ *> mvpConfigurables;
    void registerConfigurable(IConfigurable_ *c) { mvpConfigurables.push_back(c); }
    void configurate()
    {
        for (auto it : mvpConfigurables)
            it->configure(&mConfig);
    }
};

#endif // CONFIGURATION_H