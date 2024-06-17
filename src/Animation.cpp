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

void AnimTextStatic1Line_::update() // override this shit for text
{
    if (mlife > 0)
        mlife--;
}

void AnimTextStatic1Line_::draw(JoyDisplay_ *pcanvas)
{
    if (mDrawBox) {
        pcanvas->fillRoundRect(mX, mY - 1, mW, mH + 2, 10, MENU_BOX_COLOR_BG);
        pcanvas->drawRoundRect(mX, mY - 1, mW, mH + 2, 10, MENU_BOX_COLOR_FG);
    }

    pcanvas->setFont(MENU_FONT_FACE);
    pcanvas->setTextWrap(false);
    pcanvas->setTextColor(mColor);
    pcanvas->setCursor((mW / 2) - ((float)mText.length() / 2 * TEXT_WIDTH), mY + TEXT_HEIGHT - 2);
    pcanvas->print(mText);
}

//--------------------------------------------------------------------------------------------------//
void AnimTextPrompt_::update() // override this shit for text
{
    Animation_::update();
    if (mdelaycounter > 0)
        return;
}

void AnimTextPrompt_::draw(JoyDisplay_ *pcanvas)
{

    for (auto &it : mvStrings){
        Serial.print(it);
        Serial.printf(", Length: %d\n", it.length());
    }

    if (mDrawBox) {
        pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
        pcanvas->drawRoundRect(mX +1 , mY +1 , mW -2 , mH -2 , 10, MENU_BOX_COLOR_FG);
        pcanvas->drawRoundRect(mX +2 , mY +2 , mW -4 , mH -4 , 10, MENU_BOX_COLOR_FG);
    }
    
    
    pcanvas->setFont(MENU_FONT_FACE);
    pcanvas->setTextColor(mColor);
    int promptCount = mvStrings.size();
    int x = this->mX;
    int y = this->mY +TEXT_HEIGHT + 4;
    int j = mposition;
    pcanvas->setTextColor(mColor);
    // only display 3 prompts at a time, and adjust screen center
    int count = (promptCount > 3) ? 3 : promptCount;
    if (count == 2)
        y += TEXT_HEIGHT / 2;
    if (count == 1)
        y += TEXT_HEIGHT;

    for (int i = 0; i < count; i++, y += TEXT_HEIGHT)
    {
        j = (mposition + i) % promptCount;
        x = (mW / 2) - ((float)mvStrings[j].length() / 2 * TEXT_WIDTH);
        pcanvas->setCursor(x + mX + mTextOffsetX, y + mTextOffsetY);
        pcanvas->println(mvStrings[j]);
    }
}

//------------------------------   Text Selection Prompt   ----------------------------------------//
//------------------------------   Text Selection Prompt   ----------------------------------------//
void AnimInputDialogList_::start(String title, uint8_t * selection, std::initializer_list<String> prompts)
{
    mConfirm = false;
    mCancel = false;
    mpReturnPointer = selection;
    mposition = 0;
    mlife = -1;
    mTitle = title;
    mvStrings = prompts;

}

void AnimInputDialogList_::update()
{
    uint8_t range = mvStrings.size();

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
        *mpReturnPointer = mposition;
        return;
    }
    if (MyJoystickBT.joyJustPressed(JOY_DOWN)) // decrement
    {
        mposition = (mposition + range - 1) % range;
        return;
    }

    if (MyJoystickBT.joyJustPressed(JOY_UP)) // increment
    {
        mposition = (mposition + 1) % range;
        return;
    }
}

void AnimInputDialogList_::draw(JoyDisplay_ *pcanvas)
{

    AnimTextPrompt_::draw(pcanvas);
    
    int x = 64 - (((float)mTitle.length() / 2) * TEXT_WIDTH);
    pcanvas->setCursor(mX + x, mY + TEXT_HEIGHT + 4);
    Serial.printf("Title is %s", mTitle);
    pcanvas->print(mTitle);
}
