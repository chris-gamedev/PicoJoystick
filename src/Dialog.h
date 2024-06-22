#ifndef DIALOG_H
#define DIALOG_H
#include <Arduino.h>
#include <initializer_list>
#include "Compositor.h"
#include "Animation.h"
#include "MyJoystick.h"

class IDialog_
{
public:
    // clang-format off
    IDialog_(Compositor_ *comp, int16_t x, int16_t y, uint8_t order) 
        : manimBGBox(x, y, 128, TEXT_HEIGHT * 4 + 4, order)
        , manimTitle(x, y + 2, 128, TEXT_HEIGHT, 5)
        , mpCompositor(comp)
        , mX(x)
        , mY(y)
        , mOrder(order)
    {
        
    }
    // clang-format on
    Compositor_ * const mpCompositor;
    AnimTextStatic1Line_ manimTitle;
    AnimStaticBGBox_ manimBGBox;
    virtual bool updateDialog() = 0;
    virtual void endDialog() = 0;
    int16_t mX = 0;
    int16_t mY = 0;
    uint8_t mH = 0;
    uint8_t mOrder = 10;
    bool mConfirm = false;
    bool mCancel = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////      Input Dialog          /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnimInputDialogList_ : public IDialog_
{
public:
    AnimInputDialogList_(Compositor_ *comp, uint8_t *inputVar, String prompt = "", int16_t x = 0, int16_t y = 64 - (TEXT_HEIGHT * 2) - 5, uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 4 + 10, uint8_t order = 1, int16_t life = -1, int16_t delay = -1, int16_t deltax = 0, int16_t deltay = 0)
        : IDialog_(comp, x, y, order)
          // , manimBGBox(x, y, w, TEXT_HEIGHT * 4 + 4, 1)
          // , manimTitle(x, y + 2, 128, TEXT_HEIGHT, 5)
          ,
          manimPromptList(x, y + 2 + TEXT_HEIGHT, 128, TEXT_HEIGHT * 2, order), mpReturnPointer(inputVar)

    {
        manimTitle.setText(prompt);
        manimTitle.setDrawBox(false);
        manimPromptList.setDrawBox(false);

        // start("", nullptr, {});
    }

    AnimTextPrompt_ manimPromptList;
    void start(String title, uint8_t *selection, std::initializer_list<String> prompts);
    // void updateAnim();
    // void drawAnim(JoyDisplay_ *pcanvas);
    bool updateDialog();
    void endDialog();
    bool finished() { return mConfirm || mCancel; }
    void setTitle(String title) { manimTitle.setText(title); }

    String mTitle;
    uint8_t *mpReturnPointer;
    uint8_t mPosition = 0;
    bool mConfirm;
    bool mCancel;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////      Input Dialog Template         ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename I>
class AnimInputDialogInt_ : public IDialog_
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
    AnimInputDialogInt_(Compositor_ *comp, I *inputVar, I lower, I upper, String prompt = "", int16_t x = 0, int16_t y = 64 - (TEXT_HEIGHT * 2) - 5, uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 4 + 10, uint8_t order = 1, int16_t life = -1)
        : IDialog_(comp, x, y, order), manimValueBox(x, y + TEXT_HEIGHT * 2)
    {
        manimBGBox.mH = TEXT_HEIGHT * 3 + 4;
        // start(prompt, inputVar, lower, upper);
    }
    bool updateDialog();
    void endDialog()
    {
        mpCompositor->killAnimation(&manimBGBox, CanvasType::TOP);
        mpCompositor->killAnimation(&manimTitle, CanvasType::FG);
        mpCompositor->killAnimation(&manimValueBox, CanvasType::FG);
    }
    void start(String prompt, I *inputVar, I lower, I upper)
    {

        mCancel = false;
        mConfirm = false;
        mpReturnPointer = inputVar;
        mNewValue = (*inputVar < lower) ? lower : *inputVar;
        mNewValue = (*inputVar > upper) ? upper : *inputVar;
        mLowerBound = lower;
        mUpperBound = upper;
        mRange = upper - lower + 1;
        mZeroPad = String(mUpperBound).length();

        mpCompositor->registerAnimation(&manimBGBox, CanvasType::TOP);

        manimTitle.setText(prompt);
        mpCompositor->registerAnimation(&manimTitle, CanvasType::FG);

        mValueString = String(mNewValue);
        while (mValueString.length() < String(mUpperBound).length())
            mValueString = "0" + mValueString;

        manimValueBox.setText(String(mValueString));
        mpCompositor->registerAnimation(&manimValueBox, CanvasType::FG);
    }
    void setReturnPointer(I *pReturn)
    {
        if (pReturn != nullptr)
        {
            mpReturnPointer = pReturn;
            mNewValue = *pReturn;
        }
    }
    bool finished() { return mConfirm || mCancel; }

    AnimTextStatic1Line_ manimValueBox;

    I *mpReturnPointer = nullptr;
    I mNewValue;
    String mValueString;
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
bool AnimInputDialogInt_<I>::updateDialog()
{
    if (MyJoystickBT.buttonJustPressed(0)) // cancel
    {
        mCancel = true;
        endDialog();
        return false;
    }
    if (MyJoystickBT.buttonJustPressed(4)) // accept
    {
        mConfirm = true;
        *mpReturnPointer = mNewValue;
        endDialog();
        return false;
    }
    if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
    {
        mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
        mLastTime = millis();
    }
    if (MyJoystickBT.joyHeld(JOY_DOWN))
    {
        if (mLastTime + 350 < millis())
            mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
    }
    if (MyJoystickBT.joyJustPressed(JOY_UP)) // increment
    {
        mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
        mLastTime = millis();
    }
    if (MyJoystickBT.joyHeld(JOY_UP))
    {
        if (mLastTime + 350 < millis())
            mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
    }
    
    mValueString = String(mNewValue);
    while (mValueString.length() < String(mUpperBound).length())
        mValueString = "0" + mValueString;
    manimValueBox.setText(mValueString);

    return true;
}

#endif // DIALOG_H