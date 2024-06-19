#include "Dialog.h"

extern MyJoystickBT_ MyJoystickBT;

void AnimInputDialogList_::start(String title, uint8_t *selection, std::initializer_list<String> prompts)
{

    mConfirm = false;
    mCancel = false;
    mpReturnPointer = selection;
    mPosition = 0;
    mH = (TEXT_HEIGHT * 3) + (prompts.size() / 2.0F * TEXT_HEIGHT) + 4;
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
    if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
    {
        mPosition = (mPosition + range - 1) % range;
        manimPromptList.setPosition(mPosition);
    }

    if (MyJoystickBT.joyJustPressed(JOY_UP)) // increment
    {
        mPosition = (mPosition + 1) % range;
        manimPromptList.setPosition(mPosition);
    }
    return true;
}

void AnimInputDialogList_::endDialog()
{
    mpCompositor->killAnimation(&manimBGBox, CanvasType::TOP);
    mpCompositor->killAnimation(&manimPromptList, CanvasType::FG);
    mpCompositor->killAnimation(&manimTitle, CanvasType::FG);
}

// void AnimInputDialogList_::drawAnim(JoyDisplay_ *pcanvas)
// {

//     AnimTextPrompt_::drawAnim(pcanvas);

//     int x = 64 - (((float)mTitle.length() / 2) * TEXT_WIDTH);
//     pcanvas->setCursor(mX + x, mY + TEXT_HEIGHT + 4);
//     pcanvas->print(mTitle);
// }
