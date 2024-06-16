#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <Arduino.h>
#include <stdlib.h>
#include <algorithm>
#include "Display.h"
#include "Animation.h"

namespace CanvasType
{
    enum TCanvasType
    {
        FG = 1,
        BG = 2,
        TOP = 4,
        BOTTOM = 8
    };
}


class Compositor_
{
public:
    JoyDisplay_ *const mpDisplay;

    Compositor_(JoyDisplay_ *disp) : mpDisplay(disp)
    {
        mclock = millis();
    }
    void registerAnimation(Animation_ *pAnim, CanvasType::TCanvasType canvas);
    void killAnimation(Animation_ *pAnim);
    void purgeAll();
    void update();
    void draw();

    uint32_t mclock;
    uint32_t mclockdelay = 1;

    std::vector<Animation_ *> mvFG;
    std::vector<Animation_ *> mvBG;
    std::vector<Animation_ *> mvTop;
    std::vector<Animation_ *> mvBottom;

protected:
    void inline updateVectors(std::vector<Animation_ *> *v);
};


#endif
