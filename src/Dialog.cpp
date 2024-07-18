#include "Dialog.h"

extern MyJoystickBT_ MyJoystickBT;


void AnimInputDialogList_::start(const char * title, uint8_t *selection, std::initializer_list<const char*> prompts)
{

    mConfirm = false;
    mCancel = false;
    mpReturnPointer = selection;
    mPosition = 0;
    mH = (TEXT_HEIGHT * 5);
    manimBGBox.mH = mH;
    mpCompositor->registerAnimation(&manimBGBox, CanvasType::TOP);

    manimTitle.mlife = -1;
    manimTitle.setText(title);
    mpCompositor->registerAnimation(&manimTitle, CanvasType::FG);

    manimPromptList.mlife = -1;
    manimPromptList.setText(prompts);
    manimPromptList.setPosition(mPosition);
    mpCompositor->registerAnimation(&manimPromptList, CanvasType::FG);
}

bool AnimInputDialogList_::updateDialog()
{
    if (mpReturnPointer == nullptr)
        return false;
    // TODO : use a getter here
    uint8_t range = manimPromptList.mvStrings.size();

    if (MyJoystickBT.buttonJustPressed(0)) // cancel
    {
        mCancel = true;
        endDialog();
        return false;
    }
    if (MyJoystickBT.buttonJustPressed(4)) // accept
    {
        mConfirm = true;
        *mpReturnPointer = mPosition;
        endDialog();
        return false;
    }
    if (MyJoystickBT.joyJustPressed(JOY_UP)) // decrement
    {
        mPosition = (mPosition + range - 1) % range;
        manimPromptList.setPosition(mPosition);
    }

    if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // increment
    {
        mPosition = (mPosition + 1) % range;
        manimPromptList.setPosition(mPosition);
    }
    return true;
}

void AnimInputDialogList_::endDialog()
{
    
    manimBGBox.mlife = 0;
    manimPromptList.mlife = 0;
    manimTitle.mlife = 0;
    // mpCompositor->killAnimation(&manimBGBox, CanvasType::TOP);
    // mpCompositor->killAnimation(&manimPromptList, CanvasType::FG);
    // mpCompositor->killAnimation(&manimTitle, CanvasType::FG);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////    Input Dialog - String     ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimInputDialogString_::start(const char *title, String *returnString)
{

    mConfirm = false;
    mCancel = false;
    mpReturnPointer = returnString;
    mEditString = *returnString;
    if (mEditString.length() == 0)
        mEditString = " ";
    mH = manimTitle.mH + TEXT_HEIGHT * 2;

    manimBGBox.mH = mH;
    manimBGBox.mlife=-1;
    mpCompositor->registerAnimation(&manimBGBox, CanvasType::TOP);

    manimTitle.mlife = -1;
    manimTitle.setText(title);
    mpCompositor->registerAnimation(&manimTitle, CanvasType::TOP);

    manimEditTextBox.mlife = -1;
    manimEditTextBox.setText(mEditString);
    manimEditTextBox.mCursor = 0;
    mCursor = 0;
    mpCompositor->registerAnimation(&manimEditTextBox, CanvasType::TOP);
}

bool AnimInputDialogString_::updateDialog()
{
    if (mpReturnPointer == nullptr)
        return false;

    if (MyJoystickBT.buttonJustPressed(0)) // cancel
    {
        if (mEditString.length() == 0)
        {
            mCancel = true;
            endDialog();
            return false;
        }
        deleteCharacter();
        return true;
    }
    if (MyJoystickBT.buttonJustPressed(4)) // accept
    {
        mConfirm = true;
        mEditString.trim();
        *mpReturnPointer = mEditString;
        endDialog();
        return false;
    }
    uint8_t joyState = MyJoystickBT.joyJustPressed();
    uint8_t joyHeldState = MyJoystickBT.joyHeld();
    if (joyState)
    {
        mLastTime = millis();
    }
    if (joyHeldState && mLastTime + 300 < millis())
        joyState = joyHeldState;

    switch (joyState)
    {
    case JOY_LEFT: // decrement
        moveCursor(-1);
        break;
    case JOY_RIGHT: // increment
        moveCursor(1);
        break;
    case JOY_UP: // increment
        changeCharacter(1);
        break;
    case JOY_DOWN: // decrement
        changeCharacter(-1);
        break;
    }

    return true;
}

void AnimInputDialogString_::endDialog()
{
    manimBGBox.mlife = 0;
    manimTitle.mlife = 0;
    manimEditTextBox.mlife = 0;
    // mpCompositor->killAnimation(&manimBGBox, CanvasType::TOP);
    // mpCompositor->killAnimation(&manimTitle, CanvasType::FG);
    // mpCompositor->killAnimation(&manimEditTextBox, CanvasType::FG);
}

void AnimInputDialogString_::moveCursor(int8_t dir)
{
#ifdef DEADBEEF
    Serial.printf("Dialog:  moving cursor %d, cursor=%d\n", dir, mCursor);
#endif
    if (mCursor + dir > (int32_t)mEditString.length() - 1 && mEditString.length() < mMaxStringLength && mEditString[mCursor] != ' ')
    {
#ifdef DEADBEEF
        Serial.printf("(%d > %d && %d < %d && %d\n", (mCursor + dir), (mEditString.length() - 1), mEditString.length(), mMaxStringLength, (mEditString[mCursor] != ' '));
        Serial.printf("adding space.  dir:%d, cursor=%d, mEditString[cursor]=%c\n", dir, mCursor, mEditString[mCursor]);
#endif
        mEditString += " ";
        manimEditTextBox.setText(mEditString);
    }
    manimEditTextBox.moveCursor(dir);
    mCursor = manimEditTextBox.mCursor;
}

void AnimInputDialogString_::changeCharacter(int8_t dir)
{
    char c = mEditString[mCursor];
    // 0x30 - 0x39 : numeric
    // 0x41 - 0x5A : ALPHA
    // 0x61 - 0x7A : alpha

    if (dir < 0)
    {
        switch (c)
        {
        case ' ':
            c = '9';
            break;
        case '0':
            c = 'Z';
            break;
        case 'A':
            c = 'z';
            break;
        case 'a':
            c = '9';
            break;
        default:
            c -= 1;
        }
    }
    if (dir > 0)
    {
        switch (c)
        {
        case ' ':
            c = 'a';
            break;
        case 'z':
            c = 'A';
            break;
        case 'Z':
            c = '0';
            break;
        case '9':
            c = 'a';
            break;
        default:
            c += 1;
        }
    }
    mEditString[mCursor] = c;
    manimEditTextBox.setText(mEditString);
}

void AnimInputDialogString_::deleteCharacter()
{
    mEditString = mEditString.substring(0, mCursor) + mEditString.substring(mCursor + 1);
    manimEditTextBox.setText(mEditString);
    mCursor = manimEditTextBox.mCursor;
}
