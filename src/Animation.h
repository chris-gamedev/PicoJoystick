#ifndef ANIMATION_H
#define ANIMATION_H
#include <Arduino.h>
#include <vector>
#include <initializer_list>
#include "Display.h"

class Behavior_;

class IScreenObject_
{
public:
    IScreenObject_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life = -1)
        : mX(x), mY(y), mW(w), mH(h), mdrawOrder(order), mlife(life)
    {
    }
    virtual void updateAnim() = 0;
    virtual void drawAnim(JoyDisplay_ *) = 0;
    bool operator<(const IScreenObject_ &rhs) const { return mdrawOrder < rhs.mdrawOrder; }
    int16_t mX = 0;
    int16_t mY = 0;
    uint8_t mW;
    uint8_t mH;
    uint8_t mdrawOrder;
    int16_t mlife = -1;
    uint8_t mColor = 0xF;
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
         * @param life - number of updates before kill. -1 for no kill
         * @param delay - compositor clock ticks per update. -1 for no delay
         * @param dx - delta X to move per tick
         * @param dy - delta Y to move per tick
         * @param framedelay - ticks between each spritesheet frame
         * 
        */
        Animation_(int16_t x, int16_t y, uint8_t w, uint8_t h
                , uint8_t order, int16_t life = -1, int16_t delay = -1
                , int8_t dx = 0, int8_t dy = 0, int8_t framedelay = -1) 
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
            , mframeDelay(framedelay)
            , mframeDelayCounter(framedelay)
            {}
    // clang-format on
    virtual void updateAnim();
    void setParameters(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order = 1, int16_t life = -1, int16_t delay = -1, int8_t dx = 0, int8_t dy = 0, int8_t frameDelay = -1);
    virtual void drawAnim(JoyDisplay_ *) = 0;
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
    uint8_t mframeDelay = 0;
    uint8_t mframeDelayCounter = 0;
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
    bool mFaceR = true;
    bool mFaceUp = true;
};

/// @brief 1 bit Animation.  Registered with compositor
class AnimSprite1_ : public Animation_
{
public:
    AnimSprite1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy)
        : Animation_(x, y, w, h, order, life, delay, dx, dy) {}
    // (x, y, *bmp, w, h, order, life, delay)
    AnimSprite1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay)
        : Animation_(x, y, w, h, order, life, delay, 0, 0) {}

    void updateAnim();

    void drawAnim(JoyDisplay_ *pcanvas);
};

/// @brief 4 bit Animation.  Registered with compositor
class AnimSprite4_ : public Animation_
{

public:
    AnimSprite4_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order = 1, int16_t life = -1, int16_t delay = -1, int8_t dx = 0, int8_t dy = 0, int8_t framedelay = -1)
        : Animation_(x, y, w, h, order, life, delay, dx, dy, framedelay) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    // void update();

    void drawAnim(JoyDisplay_ *pcanvas);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class PacManSprite_ : public AnimSprite4_
{

public:
    // (x, y, *bmp, w, h, order, life, delay, deltax, deltay)
    PacManSprite_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order = 1, int16_t life = -1, int16_t delay = -1, int8_t dx = 0, int8_t dy = 0, int8_t framedelay = -1)
        : AnimSprite4_(x, y, w, h, order, life, delay, dx, dy, framedelay) {}

    void drawAnim(JoyDisplay_ *pcanvas);
    int8_t mLastDeltaX;
    bool mFacingRight = true;
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   Static   ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/// @brief 1 bit Static Sprite.  No Moving, No Animation.
class AnimStatic1_ : public Animation_
{
public:
    AnimStatic1_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life)
        : Animation_(x, y, w, h, order, life, 0, 0, 0) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void updateAnim();

    void drawAnim(JoyDisplay_ *pcanvas);
};

/// @brief 4 bit Static Sprite.  No Moving, No Animation.
class AnimStatic4_ : public Animation_
{
public:
    AnimStatic4_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life)
        : Animation_(x, y, w, h, order, life) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void updateAnim();

    void drawAnim(JoyDisplay_ *pcanvas);
};

class AnimStaticBGBox_ : public Animation_
{
public:
    AnimStaticBGBox_(int16_t x, int16_t y, uint8_t w = 128, uint8_t h = TEXT_HEIGHT + 2, uint8_t order = 1)
        : Animation_(x, y, w, h, order) {}

    void updateAnim() {}
    void drawAnim(JoyDisplay_ *pcanvas)
    {
        if (mDrawBox)
        {
            pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
            pcanvas->drawRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_FG);
        }
    }
    bool mDrawBox = true;
};

//////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    Text    ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class AnimTextStatic1Line_ : public Animation_
{
public:
    AnimTextStatic1Line_(int16_t x, int16_t y, uint8_t w = 128, uint8_t h = TEXT_HEIGHT + 2, uint8_t order = 10, int16_t life = -1)
        : Animation_(x, y, w, h, order, life, -1, 0, 0) {}

    // void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void inline setText(const String &t) { mText = t; }
    void inline setDrawBox(bool drawBox) { mDrawBox = drawBox; }
    void updateAnim();
    void drawAnim(JoyDisplay_ *pcanvas);

    bool mDrawBox = false;

    String mText;
    GFXfont const *mpFont = MENU_FONT_FACE;
    uint8_t mFontWidth = TEXT_WIDTH;
    uint8_t mFontHeight = TEXT_HEIGHT;
    int8_t mXOffset = 0;
    int8_t mYOffset = -2;
    bool mCenterText = true;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class AnimTextCursor1Line_ : public AnimTextStatic1Line_
{
public:
    AnimTextCursor1Line_(int16_t x, int16_t y, uint8_t w = 128, uint8_t h = TEXT_HEIGHT + 2, uint8_t order = 10, int16_t life = -1)
        : AnimTextStatic1Line_(x, y, w, h, order, life)
    {
        mCenterText = false;
        mXOffset = 5;
    }
    void moveCursor(int8_t dir);
    void inline setText(const String &text)
    {
        mFullString = text;
        mCursor = (mCursor > mFullString.length() - 1) ? mFullString.length() - 1 : mCursor;
    }
    void drawAnim(JoyDisplay_ *pcanvas);

    String mFullString;
    int8_t mCursor = 0;
    uint8_t mMaxStringLength = 20;
    uint8_t mMaxDisplayLength = 12;
    int8_t mStartChar = 0;
    int8_t mEndChar = mMaxDisplayLength;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class AnimTextPrompt_ : public Animation_
{
public:
    AnimTextPrompt_(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life = -1, int16_t delay = -1, int deltax = 0, int deltay = 0)
        : Animation_(x, y, w, h, order, life, delay, deltax, deltay) {}

    void inline setBitmap(const unsigned char *bmp) { mbitmap = bmp; }
    void inline setText(const String &s)
    {
        mvStrings.clear();
        mvStrings.push_back(s);
    }
    void inline setText(std::initializer_list<String> strings) { mvStrings = strings; };
    void inline setText(const std::vector<String> &strings, uint8_t pos = 0)
    {
        mvStrings = strings;
        mposition = pos;
    }
    void inline setPosition(uint8_t pos) { mposition = pos; }
    void inline setDrawBox(bool draw) { mDrawBox = draw; }
    void updateAnim();
    void drawAnim(JoyDisplay_ *pcanvas);
    void setLife(int16_t life, int16_t delay = 0)
    {
        mlife = life;
        mdelay = delay;
    }

    std::vector<String> mvStrings;
    uint8_t mposition = 0;
    bool mDrawBox = false;
    uint8_t mTextOffsetX = 0;
    uint8_t mTextOffsetY = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////    Behaviors    /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Behavior_
{
public:
    Behavior_() {}
    virtual void update(Animation_ *parent) = 0;
};

class UDLRRandom : public Behavior_
{
public:
    UDLRRandom(int8_t threshold = 10) : mthreshold(threshold) {}
    void update (Animation_ *parent);

    int8_t mthreshold;
};

#endif