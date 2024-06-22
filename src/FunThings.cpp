#include "FunThings.h"
#include "Pacman.h"


FunThings_::FunThings_(Compositor_ *comp)
    // clang-format off
        : mpComp(comp)
        , aSprite8_1(0, 20, 16, 16, 5, 200, 1, 3, 0, 1)
        , aSprite8_2(0, 45, 16, 16, 4, 200, 4, 2, 0, 1)
        , aSprite8_blinky(0, 55, 16, 16, 5, 80, 4, 3, 0, 2)
        , aSprite8_pinky(0, 60, 16, 16, 4, 200, 4, 3, 0, 2)
        , aScardey8_1(0, 115, 16, 16, 5, 125, 4, 1, 0, 2)
        , aScardey8_2(0, 60, 16, 16, 4, 150, 4, 1, 0, 2)
// clang-format on
{
    
        aSprite8_1.setBitmapArray(msPacMan_spritesheetR, msPacMan_spritesheet_size);
        aSprite8_2.setBitmapArray(msPacMan_spritesheetR, msPacMan_spritesheet_size);
        aSprite8_blinky.setBitmapArray(blinky_spritesheet, blinky_spritesheet_size);
        aSprite8_pinky.setBitmapArray(pinky_spritesheet, pinky_spritesheetR_size);
        aScardey8_1.setBitmapArray(scaredy_spritesheet, scaredy_spritesheet_size);
        aScardey8_2.setBitmapArray(scaredy_spritesheet, scaredy_spritesheet_size);
    
}

void FunThings_::doTheFunThings()
{
    if (!mFunTime)
        return;

    // delay for starting a random animation
    if ((funLastTime + funDelay) > millis())
        return;

    funLastTime += funDelay;
    Animation_ *useMe = nullptr;
    Animation_ *temp = nullptr;
    // sort out the dead ones
    for (int i = 0; i < 6; i++)
    {
        if (funInUse[i] == nullptr)
            continue;
        if (funInUse[i]->mlife == 0)
        {
            temp = funInUse[i];
            funInUse[i] = nullptr;
            funNotUsed[i] = temp;
        }
    }
    // pick a free one
    for (int i = 0; i < 6; i++)
    {
        if (funNotUsed[i] != nullptr)
        {
            useMe = funNotUsed[i];
            funNotUsed[i] = nullptr;
            funInUse[i] = useMe;
            break;
        }
    }

    // set up and release our new friend
    if (useMe != nullptr)
    {
        int deltax = random(5) + 1;
        int y = random(100) + 10;
        useMe->mX = -20;
        useMe->mY = y;
        useMe->mdrawOrder = deltax;
        // useMe->mlife = 800 * (1 / deltax) + random(200) + 100;
        useMe->mdelay = -1;
        useMe->mdelaycounter = useMe->mdelay;
        useMe->mdeltaX = deltax;
        useMe->mlife = 168 / deltax + random(50);
        CanvasType::TCanvasType layer = (random(6) + 1 > 4) ? CanvasType::FG : CanvasType::BG;

        mpComp->registerAnimation(useMe, layer);
    }
}