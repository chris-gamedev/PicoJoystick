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
    inline IDialog_(Compositor_ *comp, int16_t x, int16_t y, uint8_t order) 
        : manimBGBox(x, y, 128, TEXT_HEIGHT * 4 + 4, order)
        , manimTitle(x, y + 2, 128, TEXT_HEIGHT, 5)
        , mpCompositor(comp)
        , mX(x)
        , mY(y)
        , mOrder(order)
    {}
    // clang-format on
    Compositor_ *const mpCompositor;
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
///////////////////////////////////////      Input Dialog - List     ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnimInputDialogList_ : public IDialog_
{
public:
    inline AnimInputDialogList_(Compositor_ *comp, uint8_t *inputVar, const char *title = "", int16_t x = 0, int16_t y = 64 - (TEXT_HEIGHT * 2) - 5, uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 4 + 10, uint8_t order = 1)
        : IDialog_(comp, x, y, order), manimPromptList(x, y + 2 + TEXT_HEIGHT, 128, TEXT_HEIGHT * 2, order),
          mpReturnPointer(inputVar)

    {
        manimTitle.setText(title);
        manimTitle.setDrawBox(false);
        manimPromptList.setDrawBox(false);
    }

    AnimTextPrompt_ manimPromptList;
    void start(const char *title, uint8_t *selection, std::initializer_list<const char *> prompts = {});
    // void updateAnim();
    // void drawAnim(JoyDisplay_ *pcanvas);
    bool updateDialog();
    void endDialog();
    bool finished() { return mConfirm || mCancel; }
    void setTitle(const char *title) { manimTitle.setText(title); }

    const char *mTitle;
    uint8_t *mpReturnPointer;
    uint8_t mPosition = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    Input Dialog - String     ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnimInputDialogString_ : public IDialog_
{
    constexpr static uint8_t mMaxStringLength = JOYSTICK_FILENAME_MAX_LENGTH;
    constexpr static uint8_t mMaxDisplayLength = 10;

public:
    inline AnimInputDialogString_(Compositor_ *comp, String *inputVar, const char *title = "", int16_t x = 0, int16_t y = 64 - (TEXT_HEIGHT * 2) - 5, uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 4 + 10, uint8_t order = 1)
        : IDialog_(comp, x, y, order), manimEditTextBox(x, y + TEXT_HEIGHT + 2), mpReturnPointer(inputVar)

    {
        manimTitle.setText(title);
        manimTitle.mDrawBox = false;
        manimEditTextBox.mDrawBox = false;
        manimEditTextBox.mCenterText = false;
        manimEditTextBox.mXOffset = 5;
        manimEditTextBox.mYOffset = 0;

        if (inputVar != nullptr)
            manimEditTextBox.setText(*inputVar);
    }

    void start(const char *title, String *selection);
    bool updateDialog();
    void endDialog();
    void moveCursor(int8_t dir);
    void changeCharacter(int8_t dir);
    void deleteCharacter();
    bool finished() { return mConfirm || mCancel; }
    void setTitle(const char *title) { manimTitle.setText(title); }

    AnimTextCursor1Line_ manimEditTextBox;
    // String mTitle;
    String *mpReturnPointer;
    int8_t mStartChar = 0;
    int8_t mEndChar = mMaxDisplayLength;
    int8_t mCursor = 0;
    String mEditString;
    uint32_t mLastTime;
    // bool mConfirm;
    // bool mCancel;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////      Input Dialog Integer - Template         //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename I>
class AnimInputDialogInt_ : public IDialog_
{
    static_assert(std::is_integral_v<I>, "Type I must be integral");
    constexpr static uint8_t mcU64Int_stringWidth = 20;

public:
    AnimInputDialogInt_(Compositor_ *comp, I *inputVar, I lower, I upper, int16_t x = 0, int16_t y = 64 - (TEXT_HEIGHT * 2), uint8_t w = 128, uint8_t h = TEXT_HEIGHT * 4, uint8_t order = 6)
        : IDialog_(comp, x, y, order), manimValueBox(x, y + TEXT_HEIGHT * 2)
    {
        manimBGBox.mH = TEXT_HEIGHT * 3 + 4;
    }
    bool updateDialog();
    inline void endDialog()
    {
        manimBGBox.mlife = 0;
        manimTitle.mlife = 0;
        manimValueBox.mlife = 0;
    }
    void start(const char *prompt, I *inputVar, I lower, I upper)
    {

        mCancel = false;
        mConfirm = false;
        mpReturnPointer = inputVar;
        mNewValue = (*inputVar < lower) ? lower : *inputVar;
        mNewValue = (*inputVar > upper) ? upper : *inputVar;
        mLowerBound = lower;
        mUpperBound = upper;
        mRange = upper - lower + 1;
        char buf[mcU64Int_stringWidth];
        sprintf(buf, "%d", mUpperBound);
        mZeroPad = strlen(buf);

        mpCompositor->registerAnimation(&manimBGBox, CanvasType::TOP);

        manimTitle.setText(prompt);
        mpCompositor->registerAnimation(&manimTitle, CanvasType::FG);

        sprintf(mValueString, "%0*d", mZeroPad, mNewValue);
        manimValueBox.setText(mValueString);
        mpCompositor->registerAnimation(&manimValueBox, CanvasType::FG);
    }
    inline void setReturnPointer(I *pReturn)
    {
        if (pReturn != nullptr)
        {
            mpReturnPointer = pReturn;
            mNewValue = *pReturn;
        }
    }
    inline bool finished() { return mConfirm || mCancel; }

    AnimTextStatic1Line_ manimValueBox;

    I *mpReturnPointer = nullptr;
    I mNewValue;
    char mValueString[mcU64Int_stringWidth];
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

    sprintf(mValueString, "%0*d", mZeroPad, mNewValue);
    manimValueBox.setText(mValueString);

    return true;
}

#endif // DIALOG_H