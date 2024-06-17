#ifndef ANIMATION_H
#define ANIMATION_H
#include <Arduino.h>
#include <stdlib.h>
#include "Display.h"
#include "MyJoystick.h"



class Behavior_
{
public:
    Behavior_() {}
    virtual void update() = 0;
};

class Animation_
{
public:
    // clang-format off
        /**
         * @param x - relative to canvas passed to draw
         * @param y - relative to canvas passed to draw
         * @param w - width
         * @param h - height
         * @param order - draw order 
         * @param life - number of updates before kill
         * @param delay - compositor clock ticks per update
         * @param dx - delta X to move per tick
         * @param dy - delta Y to move per tick
         * 
        */
        Animation_(int16_t x, int16_t y, uint8_t w, uint8_t h
                , uint8_t order, int16_t life = -1, int16_t delay = -1
                , int8_t dx = 0, int8_t dy = 0) 
            : mX(x)
            , mY(y)
            , mW(w)
            , mH(h)
            , mdrawOrder(order)
            , mlife(life)
            , mdelay(delay)
            , mdelaycounter(mdelay)
            , mdeltaX(dx)
            , mdeltaY(dy)
            {}
    // clang-format on
    virtual void update();
    void setParameters(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy);
    virtual void draw(JoyDisplay_ *) = 0;
    void inline setBitmap(const unsigned char bmp[]) { mbitmap = bmp; }
    void inline setBitmap(unsigned char *bmp) { mbitmap = bmp; }
    void inline setBitmapArray(const unsigned char **frames, uint8_t size)
    {
        mframearray = frames;
        mframecount = size;
    }

    bool operator<(const Animation_ &rhs) const { return mdrawOrder < rhs.mdrawOrder; }
    Behavior_ *mpbehavior = nullptr;
    const unsigned char *mbitmap = nullptr;
    const unsigned char **mframearray;
    uint8_t mframecount = 1;
    int16_t mlife = -1;
    int16_t mdelay = -1;
    int16_t mdelaycounter = -1;
    uint8_t mframe = 0;
    int16_t mX = 0;
    int16_t mY = 0;
    int8_t mdeltaX = 0;
    int8_t mdeltaY = 0;
    int8_t mdeltaDelay = -1;
    int8_t mdeltaDelayCounter;
    uint8_t mColor = 0xF;
    uint8_t mW;
    uint8_t mH;
    uint8_t mdrawOrder;
};

/*********************************************************************/

/// @brief 1 bit Animation.  Registered with compositor
class AnimSprite1_ : public Animation_
{
public:
    /**
     * @param x - relative to canvas passed to draw
     * @param y - relative to canvas passed to draw
     * @param w - width
     * @param h - height
     * @param order - draw order
     * @param life - number of updates before kill
     * @param delay - compositor clock ticks per update
     * @param dx - delta X to move per tick
     * @param dy - delta Y to move per tick
     */
    AnimSprite1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy)
        : Animation_(x, y, w, h, order, life, delay, dx, dy) {}
    // (x, y, *bmp, w, h, order, life, delay)
    AnimSprite1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay)
        : Animation_(x, y, w, h, order, life, delay, 0, 0) {}

    void update();

    void draw(JoyDisplay_ *pcanvas);
};

/// @brief 8 bit Animation.  Registered with compositor
class AnimSprite8_ : public Animation_
{
    /**
     * @param x - relative to canvas passed to draw
     * @param y - relative to canvas passed to draw
     * @param w - width
     * @param h - height
     * @param order - draw order
     * @param life - number of updates before kill
     * @param delay - compositor clock ticks per update
     * @param dx - delta X to move per tick
     * @param dy - delta Y to move per tick
     */
public:
    // (x, y, *bmp, w, h, order, life, delay, deltax, deltay)
    AnimSprite8_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy)
        : Animation_(x, y, w, h, order, life, delay, dx, dy) {}
    // (x, y, *bmp, w, h, order, life, delay)
    AnimSprite8_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay)
        : Animation_(x, y, w, h, order, life, delay, 0, 0) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    // void update();

    void draw(JoyDisplay_ *pcanvas);
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Static   ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/// @brief 1 bit Static Sprite.  No Moving, No Animation. Registered with compositor
class AnimStatic1_ : public Animation_
{
public:
    /**
     * @param y - relative to canvas passed to draw
     * @param x - relative to canvas passed to draw
     * @param w - width
     * @param h - height
     * @param order - draw order
     * @param life - number of updates before kill
     */
    AnimStatic1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life)
        : Animation_(x, y, w, h, order, life, 0, 0, 0) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void update();

    void draw(JoyDisplay_ *pcanvas);
};

/// @brief 8 bit Static Sprite.  No Moving, No Animation. Registered with compositor

class AnimStatic4_ : public Animation_
{
public:
    /**
     * @param x - relative to canvas passed to draw
     * @param y - relative to canvas passed to draw
     * @param w - width
     * @param h - height
     * @param order - draw order
     * @param life - number of updates before kill
     */
    AnimStatic4_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life)
        : Animation_(x, y, w, h, order, life, -1, 0, 0) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void update();

    void draw(JoyDisplay_ *pcanvas);
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    Text    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class AnimText1_ : public Animation_
{
public:

    AnimText1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life)
        : Animation_(x, y, w, h, order, life, 0, 0, 0) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void update();

    void draw(JoyDisplay_ *pcanvas);
};

/// @brief 8 bit Static Sprite.  No Moving, No Animation. Registered with compositor

class AnimTextPrompt_ : public Animation_
{
public:

    AnimTextPrompt_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life = -1, int16_t delay = -1, int deltax = 0, int deltay = 0)
        : Animation_(x, y, w, h, order, life, delay, deltax, deltay) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void inline setText(String s)
    {
        mvStrings.clear();
        mvStrings.push_back(s);
    }
    void inline setText(std::vector<String> strings) { mvStrings = strings; };
    void inline setText(std::vector<String> strings, uint8_t pos)
    {
        mvStrings = strings;
        mposition = pos;
    }
    void inline setPosition(uint8_t pos) { mposition = pos; }
    void inline setDrawBox(bool draw) { mDrawBox = draw;}
    void update();
    void draw(JoyDisplay_ *pcanvas);
    void setLife(int16_t life, int16_t delay = 0) {mlife = life; mdelay = delay;}


    std::vector<String> mvStrings;
    uint8_t mposition = 0;
    bool mDrawBox = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////      Input Dialog          /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename I>
class AnimInputDialogInt_ : public Animation_
{
public:
    /**
     * @param x - relative to canvas passed to draw
     * @param y - relative to canvas passed to draw
     * @param w - width
     * @param h - height
     * @param order - draw order
     * @param life - number of updates before kill
     */
    AnimInputDialogInt_(String prompt, I *inputVar, I lower, I upper, int16_t x = 0, int16_t y = 64 - TEXT_HEIGHT - 5, uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 2 + 10, uint8_t order = 10, int16_t life = -1, int16_t delay = -1, int16_t deltax = 0, int16_t deltay = 0)
        : Animation_(x, y, w, h, order, life, delay, deltax, deltay)
        {
            start(prompt, inputVar, lower, upper);
        }
    void update();
    void start(String prompt, I *inputVar, I lower, I upper) {
        mlife = -1;
        mCancel = false;
        mConfirm = false;
        mPrompt = prompt;
        mpReturnPointer = inputVar;
        mNewValue = *inputVar;
        mLowerBound = lower;
        mUpperBound = upper;
        mRange = upper - lower + 1;
        mZeroPad = String(mUpperBound).length();
    }
    void draw(JoyDisplay_ *pcanvas);
    void setPrompt(String p) { mPrompt = p; }
    void setReturnPointer(I *pReturn) { 
        if (pReturn != nullptr)
        {
            mpReturnPointer = pReturn;
            mNewValue = *pReturn;
        }
    }
    bool finished() { return mConfirm || mCancel; }

    I *mpReturnPointer = nullptr;
    I mNewValue;
    String mPrompt;
    String mPrompt2;
    I mUpperBound;
    I mLowerBound;
    I mRange;
    uint8_t mZeroPad = 0;
    uint8_t mSpeed = 0;
    bool mConfirm = false;
    bool mCancel = false;
    uint32_t mLastTime;
};


template <typename I>
void AnimInputDialogInt_<I>::update()
{
    if (MyJoystickBT.buttonJustPressed(0)) // cancel
    {
        mCancel = true;
        mlife = 0;
        return;
    }
    if (MyJoystickBT.buttonJustPressed(4)) // accept
    {
        mConfirm = true;
        mlife = 0;
        *mpReturnPointer = mNewValue;
        return;
    }
    if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
    {
        mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
        mLastTime = millis();
        return;
    }
    if (MyJoystickBT.joyHeld(JOY_DOWN))
    {
        if (mLastTime + 350 < millis())
            mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
        return;
    }
    if (MyJoystickBT.joyJustPressed(JOY_UP)) // increment
    {
        mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
        mLastTime = millis();
        return;
    }
    if (MyJoystickBT.joyHeld(JOY_UP))
    {
        if (mLastTime + 350 < millis())
            mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
        return;
    }
}
template <typename I>
void AnimInputDialogInt_<I>::draw(JoyDisplay_ *pcanvas)
{

    pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
    pcanvas->drawRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_FG);

    pcanvas->setFont(MENU_FONT_FACE);
    pcanvas->setTextColor(0xF);
    pcanvas->setTextWrap(false);
    
    pcanvas->setCursor(mX + 64 - (TEXT_WIDTH * mPrompt.length() / 2), mY + TEXT_HEIGHT * 1.5);
    pcanvas->print(mPrompt);

    mPrompt2 = String(mNewValue);
    while (mPrompt2.length() < mZeroPad)
        mPrompt2 = "0" + mPrompt2;
    pcanvas->setCursor(mX + 64 - (TEXT_WIDTH * mPrompt2.length() / 2), mY + TEXT_HEIGHT * 2.5 + 2);
    pcanvas->print(mPrompt2);
}


#endif