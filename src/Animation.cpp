#include "Animation.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  Animation_  ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void Animation_::updateAnim()
{

    if (mlife == 0)
        return;
    else if (mlife > 0)
        mlife--;

    if (mframecount > 1)
    {
        if (mframeDelayCounter == 0)
        {
            mframe = (mframe + 1) % mframecount;
            mbitmap = mframearray[mframe];
            mframeDelayCounter = mframeDelay;
        }
        else
            mframeDelayCounter--;
    }

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
}

void inline Animation_::setParameters(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t order, int16_t life, int16_t delay, int8_t dx, int8_t dy, int8_t frameDelay)
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;
    mdrawOrder = order;
    mlife = life;
    mdelay = delay;
    mdelaycounter = mdelay;
    mdeltaX = dx;
    mdeltaY = dy;
    mframeDelay = frameDelay;
    mframeDelayCounter = frameDelay;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////   Sprite 1   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimSprite1_::drawAnim(JoyDisplay_ *pcanvas)
{
}

void AnimSprite8_::drawAnim(JoyDisplay_ *pcanvas)
{
    pcanvas->drawBitmap4Bit(mX, mY, mbitmap, mW, mH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////   Static   ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimStatic1_::updateAnim() // override this shit for static sprites.
{
}

void AnimStatic1_::drawAnim(JoyDisplay_ *pcanvas)
{
}

void AnimStatic4_::updateAnim() // override this shit for static sprites.
{
}

void AnimStatic4_::drawAnim(JoyDisplay_ *pcanvas)
{
    pcanvas->drawBitmap4Bit(mX, mY, mbitmap, mW, mH);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////   Text   /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimTextStatic1Line_::updateAnim() 
{
    if (mlife > 0)
        mlife--;
}

void AnimTextStatic1Line_::drawAnim(JoyDisplay_ *pcanvas)
{
    if (mDrawBox)
    {
        pcanvas->fillRoundRect(mX, mY - 1, mW, mH + 2, 10, MENU_BOX_COLOR_BG);
        pcanvas->drawRoundRect(mX, mY - 1, mW, mH + 2, 10, MENU_BOX_COLOR_FG);
    }

    pcanvas->setFont(mpFont);
    pcanvas->setTextWrap(false);
    pcanvas->setTextColor(mColor);
    int8_t x = mXOffset;
    if (mCenterText)
        x = (mW / 2) - ((float)mText.length() / 2 * mFontWidth) + mXOffset;
    pcanvas->setCursor(x , mY + TEXT_HEIGHT + mYOffset);
    pcanvas->print(mText);
}

//--------------------------------------------------------------------------------------------------//
void AnimTextPrompt_::updateAnim() // override this shit for text
{
    Animation_::updateAnim();
    if (mdelaycounter > 0)
        return;
}

void AnimTextPrompt_::drawAnim(JoyDisplay_ *pcanvas)
{
#ifdef DEADBEEF
    for (auto &it : mvStrings)
    {
        Serial.print(it);
        Serial.printf(", Length: %d\n", it.length());
    }
#endif
    if (mDrawBox)
    {
        pcanvas->fillRoundRect(mX, mY, mW, mH, 10, MENU_BOX_COLOR_BG);
        pcanvas->drawRoundRect(mX + 1, mY + 1, mW - 2, mH - 2, 10, MENU_BOX_COLOR_FG);
        pcanvas->drawRoundRect(mX + 2, mY + 2, mW - 4, mH - 4, 10, MENU_BOX_COLOR_FG);
    }

    pcanvas->setFont(MENU_FONT_FACE);
    pcanvas->setTextColor(mColor);
    int promptCount = mvStrings.size();
    int x = this->mX;
    int y = this->mY + TEXT_HEIGHT + 4;
    int j = mposition;
    pcanvas->setTextColor(mColor);
    // only display 3 prompts at a time, and adjust screen center
    int count = (promptCount > 3) ? 3 : promptCount;
    if (count == 2)
        y += (TEXT_HEIGHT / 2.0F);
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
