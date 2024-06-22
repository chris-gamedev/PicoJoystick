#ifndef FUNTHINGS_H
#define FUNTHINGS_H


#include <Arduino.h>
#include "Compositor.h"
#include "Animation.h"
#include "Configuration.h"

class FunThings_ : public IConfigurable_
{
    // clang-format off
public:
    FunThings_(Compositor_ *comp);

    void doTheFunThings();
    void configure(Configuration *config)
    {
        mFunTime = config->funThings_on;
    }

    Compositor_ *mpComp;

    AnimSprite8_ aSprite8_1;
    AnimSprite8_ aSprite8_2;
    AnimSprite8_ aSprite8_blinky;
    AnimSprite8_ aSprite8_pinky;
    AnimSprite8_ aScardey8_1;
    AnimSprite8_ aScardey8_2;

    Animation_ *funNotUsed[6] = {&aScardey8_1, &aScardey8_2, &aSprite8_1, &aSprite8_2, &aSprite8_blinky, &aSprite8_pinky};
    Animation_ *funInUse[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

    uint32_t funDelay = 10;
    uint32_t funLastTime = 0;
    bool mFunTime = true;
};

#endif // FUNTHINGS_H