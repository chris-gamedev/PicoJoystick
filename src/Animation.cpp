#include "Animation.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  Animation_  ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void Animation_::update()
{

    if (mlife == 0)
        return;
    else if (mlife > 0)
        mlife--;

    if (mdelaycounter > 0)
    {
        mdelaycounter--;
        return;
    }
    else if (mdelaycounter == 0)
        mdelaycounter = mdelay;

    if (mpbehavior != nullptr)
        mpbehavior->update();
    mX += mdeltaX;
    mY += mdeltaY;

    if (mframecount > 1)
    {
        mframe = (mframe + 1) % mframecount;
        mbitmap = mframearray[mframe];
    }
}

void inline Animation_::setParameters(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy)
{
    mX = x, mY = y, mW = w, mH = h, mdrawOrder = order, mlife = life, mdelay = delay, mdelaycounter = mdelay, mdeltaX = dx, mdeltaY = dy;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Sprite 1   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimSprite1_::draw(JoyDisplay_ *pcanvas)
{
}

void AnimSprite8_::draw(JoyDisplay_ *pcanvas)
{
    pcanvas->drawBitmap4Bit(mX, mY, mbitmap, mW, mH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////   Static   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimStatic1_::update() // override this shit for static sprites.
{
}

void AnimStatic1_::draw(JoyDisplay_ *pcanvas)
{
}

void AnimStatic4_::update() // override this shit for static sprites.
{
}

void AnimStatic4_::draw(JoyDisplay_ *pcanvas)
{
    pcanvas->drawBitmap4Bit(mX, mY, mbitmap, mW, mH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////   Text   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimText1_::update() // override this shit for text
{
}

void AnimText1_::draw(JoyDisplay_ *pcanvas)
{
}

void AnimTextPrompt_::update() // override this shit for text
{
    Animation_::update();
    if (mdelaycounter > 0)
        return;
}

void AnimTextPrompt_::draw(JoyDisplay_ *pcanvas)
{
    // Serial.println("inside Display Prompts. Cursor is " +String(currentPrompt));
    // Serial.println("prompts are: ");

    for (auto &it : mvStrings)
        Serial.println(it);

    
    
    pcanvas->setFont(MENU_FONT_FACE);
    pcanvas->setTextColor(mColor);
    int promptCount = mvStrings.size();
    int x = this->mX;
    int y = this->mY;
    int j = mposition;
    pcanvas->setTextColor(mColor);
    // do some number ajusting
    int count = (promptCount > 3) ? 3 : promptCount;
    if (count == 2)
        y += TEXT_HEIGHT / 2;
    if (count == 1)
        y += TEXT_HEIGHT;

    for (int i = 0; i < count; i++, y += TEXT_HEIGHT)
    {
        j = (mposition + i) % promptCount;
        x = (mW / 2) - (((float)mvStrings[j].length() * TEXT_WIDTH) / 2) - TEXT_WIDTH / 2;
        pcanvas->setCursor(mX + x, y + MENU_TRIM_WIDTH);

        pcanvas->println(mvStrings[j]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////      Input Dialog          /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// template <typename I>
// void AnimInputDialogInt_<I>::update()
// {
//     if (MyJoystickBT.buttonJustPressed(0)) // cancel
//     {
//         mCancel = true;
//         mlife = 0;
//         return;
//     }
//     if (MyJoystickBT.buttonJustPressed(4)) // accept
//     {
//         mConfirm = true;
//         mlife = 0;
//         *mpReturnPointer = mNewValue;
//         return;
//     }
//     if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
//     {
//         mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
//         mLastTime = millis();
//         return;
//     }
//     if (MyJoystickBT.joyHeld(JOY_DOWN))
//     {
//         if (mLastTime + 350 < millis())
//             mNewValue = ((mNewValue + mRange - mLowerBound - 1) % mRange) + mLowerBound;
//         return;
//     }
//     if (MyJoystickBT.joyJustPressed(JOY_UP)) // increment
//     {
//         mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
//         mLastTime = millis();
//         return;
//     }
//     if (MyJoystickBT.joyHeld(JOY_UP))
//     {
//         if (mLastTime + 350 < millis())
//             mNewValue = (mNewValue - mLowerBound + 1) % mRange + mLowerBound;
//         return;
//     }
// }
// template <typename I>
// void AnimInputDialogInt_<I>::draw(JoyDisplay_ *pcanvas)
// {

//     pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
//     pcanvas->drawRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_FG);

//     pcanvas->setFont(MENU_FONT_FACE);
//     pcanvas->setTextColor(0xF);
//     pcanvas->setTextWrap(false);
    
//     pcanvas->setCursor(mX + 64 - (TEXT_WIDTH * mPrompt.length() / 2), mY + TEXT_HEIGHT);
//     pcanvas->print(mPrompt);

//     mPrompt2 = String(mNewValue);
//     while (mPrompt2.length() < mZeroPad)
//         mPrompt2 = "0" + mPrompt2;
//     pcanvas->setCursor(mX + 64 - (TEXT_WIDTH * mPrompt2.length() / 2), mY + TEXT_HEIGHT * 2 + 2);
//     pcanvas->print(mPrompt2);
// }
