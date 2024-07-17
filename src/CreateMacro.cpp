#include "CreateMacro.h"

extern MyJoystickBT_ MyJoystickBT;
extern Configurator_ Configurator;

PhraseTable_::PhraseTable_(uint8_t x, uint8_t y, std::vector<MacroWord> phrase)
    : Animation_(x, y, 128, 100, 10), manimTopMessage(x, 1, 128, 15, 10)

{
    manimTopMessage.setText("Edit");
    manimTopMessage.mDrawBox = true;
    manimTopMessage.mpFont = &manifold6pt7b;
    manimTopMessage.mFontWidth = 7;
    manimTopMessage.mYOffset = -3;
    initTable(phrase);
}

PhraseTable_::~PhraseTable_()
{
    std::vector<IRow_ *>::iterator it = mvpRows.begin();
    while (it != mvpRows.end())
    {
        delete *it;
        it++;
    }
}

void PhraseTable_::initTable(std::vector<MacroWord> phrase)
{
    if (mvpRows.size() > 0)
        deleteTable();
    mcursorX = 0;
    mcursorY = 0;
    mTableHead = 0; //new
    IRow_ *plastTemp = new AddRow_(mX, mY);
    IRow_ *ptemp;
    mvpRows.push_back(plastTemp);
    for (auto it : phrase)
    {
        ptemp = new WordRow_(mX, plastTemp->mY + plastTemp->mH, it);
        mvpRows.push_back(ptemp);
        plastTemp = ptemp;
        ptemp = new AddRow_(0, plastTemp->mY + plastTemp->mH);
        mvpRows.push_back(ptemp);
        plastTemp = ptemp;
    }
}

void PhraseTable_::moveCursor(int8_t dx, int8_t dy)
{
    if (mcursorY + dy >= 0 && mcursorY + dy < mvpRows.size())
        mcursorY += dy;

    if (mcursorY < mTableHead)
    {
        mTableHead = advanceTable(-1);
        // mcursorY = mTableHead;
    }
    else if (mcursorY > mTableHead + 6)
    {
        mTableHead = advanceTable(1);
        // mcursorY = mTableHead + 6;
    }

    mcursorX += dx;
    if (mcursorX < 0)
        mcursorX = 0;
    if (mcursorX > 2)
        mcursorX = 2;
}

uint8_t PhraseTable_::advanceTable(int8_t direction)
{
    direction *= 2;

    if (mTableHead + direction < mvpRows.size() && mTableHead + direction >= 0)
    {
        mTableHead += direction;
        makeRowCoordinates();
    }

    return mTableHead;
}

void PhraseTable_::makeRowCoordinates()
{

    std::vector<IRow_ *>::iterator it = mvpRows.begin() + mTableHead;
    std::vector<IRow_ *>::iterator itlast = it;
    (*it)->mX = this->mX;
    (*it)->mY = this->mY;
    it++;
    for (int i = 0; i < 6 && it != mvpRows.end(); i++)
    {
        (*it)->mX = this->mX;
        (*it)->mY = (*itlast)->mY + (*itlast)->mH;
        itlast = it;
        it++;
    }
}

bool PhraseTable_::update()
{
    Serial.printf("point 3 \t");
    if (mEditingStateMap)
    {
        Serial.printf("point 4 \t");
        mEditingStateMap = editStatemap();
        return true;
    }
    if (mEditingWordDuration)
    {
        Serial.printf("point 5 \t");
        mEditingWordDuration = editWordDuration();
        return true;
    }

    if (MyJoystickBT.buttonJustPressed(0))
        return false;
    if (MyJoystickBT.buttonJustPressed(4))
    {
        Serial.printf("point 6 \t");
        tableCommand c = mvpRows[mcursorY]->maCells[mcursorX]->onSelect();
        (*this.*c)();
    }
    uint8_t joystate = MyJoystickBT.joyJustPressed();
    switch (joystate)
    {
    case JOY_UP:
        moveCursor(0, -1);
        break;
    case JOY_DOWN:
        moveCursor(0, 1);
        break;
    case JOY_LEFT:
        moveCursor(-1, 0);
        break;
    case JOY_RIGHT:
        moveCursor(1, 0);
    }
    Serial.printf("point 7 \t");
    return true;
}

void PhraseTable_::drawAnim(JoyDisplay_ *pcanvas)
{
    if (mvpRows.size() == 0)
        return;

    if (mEditingStateMap || mEditingWordDuration)
    {
        manimTopMessage.drawAnim(pcanvas);
    }

    // draw cursor
    // pcanvas->fillRect(mvpRows[mcursorY]->mX, mvpRows[mcursorY]->mY - 1, mvpRows[mcursorY]->mW, mvpRows[mcursorY]->mH + 2, 0x6);
    pcanvas->fillRect(mvpRows[mcursorY]->maCells[mcursorX]->mX,
                      mvpRows[mcursorY]->mY - 1,
                      mvpRows[mcursorY]->maCells[mcursorX]->mW,
                      mvpRows[mcursorY]->maCells[mcursorX]->mH +
                          2,
                      0x5);
#ifdef DEADBEEF
    Serial.printf("cell x %d, y %d, w %d, h %d,   cursorx %d, cursory %d\n",
                  mvpRows[mcursorY]->maCells[mcursorX]->mX,
                  mvpRows[mcursorY]->mY,
                  mvpRows[mcursorY]->maCells[mcursorX]->mW,
                  mvpRows[mcursorY]->maCells[mcursorX]->mH,
                  mcursorX, mcursorY);
#endif
    std::vector<IRow_ *>::iterator it = mvpRows.begin() + mTableHead;
    // start at an add row
    (*it)->draw(pcanvas);
    it++;
    // starts at an edit row
    for (int i = 0; i < 3 && it != mvpRows.end(); i++)
    {
        (*it)->draw(pcanvas);
        it++;
        (*it)->draw(pcanvas);
        it++;
    }
}

std::vector<MacroWord> PhraseTable_::savePhrase()
{
    std::vector<MacroWord> phrase;
    if (mvpRows.size() > 1)
    {
        std::vector<IRow_ *>::iterator it = mvpRows.begin() + 1;
        while (it != mvpRows.end())
        {
            phrase.push_back((*it)->mWord);
            it += 2;
        }
    }
    return phrase;
}

void PhraseTable_::deleteTable()
{
    std::vector<IRow_ *>::iterator it = mvpRows.begin();
    while (it != mvpRows.end())
    {
        delete (*it);
        it++;
    }
    mvpRows.clear();
    mcursorX = 0;
    mcursorY = 0;
    mTableHead = 0; // new
}

void PhraseTable_::addRowBefore()
{
    if (mvpRows.size() / 2 >= mMaxEditRows)
        return;
    std::vector<IRow_ *>::iterator it = mvpRows.begin() + mcursorY;
    it = mvpRows.insert(it, new WordRow_(0, 0, {0, 0, mLowerDelayBound}));
    it = mvpRows.insert(it, new AddRow_(0, 0));
    moveCursor(0, 1);
    makeRowCoordinates();
}

void PhraseTable_::removeRow()
{
    std::vector<IRow_ *>::iterator it = mvpRows.begin() + mcursorY;
    delete *it;
    it = mvpRows.erase(it);
    delete *it;
    it = mvpRows.erase(it);
    mcursorY = (mcursorY >= mvpRows.size()) ? mvpRows.size() - 1 : mcursorY;
    makeRowCoordinates();
}

bool PhraseTable_::editStatemap()
{

    static bool sStartRecording = false;
    static uint32_t sLastTime = 0;

    if (sLastTime == 0)
    {
        sLastTime = millis();
        sStartRecording = true;
        return true;
    }
    if (sStartRecording)
    {
        if (sLastTime + 1800 > millis())
        {
            uint16_t StateMap = MyJoystickBT.mPackedButtonStates;
            uint16_t LastStateMap = MyJoystickBT.mLastPackedButtonStates;
            uint8_t joystate = MyJoystickBT.mJoyState;
            if (joystate || StateMap)
            {
                sLastTime = millis();
                uint16_t mask = (LastStateMap ^ StateMap) & (0xFFFF & StateMap); // get ONLY the buttons which 'turned on' this pass
                mvpRows[mcursorY]->mWord.mButtonStateMap ^= mask;                // flip the bits of those buttons in the word we're editing
                if (joystate && MyJoystickBT.joyJustPressed(joystate))
                {
                    if (joystate == mvpRows[mcursorY]->mWord.mJoyState)
                        mvpRows[mcursorY]->mWord.mJoyState = 0;
                    else
                        mvpRows[mcursorY]->mWord.mJoyState = joystate;
                }
            }
        }
        else
        {
            // mvpRows[mcursorY]->mWord = sEditedWord;
            sLastTime = 0;
            sStartRecording = false;
            return false;
        }
    }
    return true;
}

bool PhraseTable_::editWordDuration()
{
    static bool sStartRecording = false;
    static uint8_t sScrollSpeed = 0;
    static uint32_t sLastTime = 0;
    static const uint16_t mRange = mUpperDelayBound - mLowerDelayBound + 1;
    static uint16_t sLastValue = 0;

    if (!sStartRecording)
    {
        sLastValue = mvpRows[mcursorY]->mWord.mDuration;
        sStartRecording = true;
    }

    if (MyJoystickBT.buttonJustPressed(0)) // cancel
    {
        sStartRecording = false;
        mvpRows[mcursorY]->mWord.mDuration = sLastValue;
        return false;
    }
    if (MyJoystickBT.buttonJustPressed(4)) // accept
    {
        sStartRecording = false;
        return false;
    }

    uint8_t joyState = MyJoystickBT.joyJustPressed();
    switch (joyState)
    {
    case JOY_DOWN: // decrement
        mvpRows[mcursorY]->mWord.mDuration = ((mvpRows[mcursorY]->mWord.mDuration + mRange - mLowerDelayBound - 1) % mRange) + mLowerDelayBound;
        sLastTime = millis();
        break;
    case JOY_UP: // increment
        mvpRows[mcursorY]->mWord.mDuration = (mvpRows[mcursorY]->mWord.mDuration - mLowerDelayBound + 1) % mRange + mLowerDelayBound;
        sLastTime = millis();
        break;
    }

    joyState = MyJoystickBT.joyHeld();
    switch (joyState)
    {
    case JOY_UP:
        if (sLastTime + 350 < millis())
            mvpRows[mcursorY]->mWord.mDuration = (mvpRows[mcursorY]->mWord.mDuration - mLowerDelayBound + 1) % mRange + mLowerDelayBound;
        break;
    case JOY_DOWN:
        if (sLastTime + 350 < millis())
            mvpRows[mcursorY]->mWord.mDuration = ((mvpRows[mcursorY]->mWord.mDuration + mRange - mLowerDelayBound - 1) % mRange) + mLowerDelayBound;
        break;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////      Create Macro       ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateMacroApp_::initApp()
{
    // TODO: Disable menu hotkey.  need a clean exit.

    manimBottom.mlife = -1;
    mPhraseTable.mlife = -1;
    mpCompositor->registerAnimation(&manimBottom, CanvasType::FG);
    mpCompositor->registerAnimation(&mPhraseTable, CanvasType::FG);
    MyJoystickBT.forceDisableCustomMacros(true);
    MyJoystickBT.toggleJoyTransmit(false);
    startFromScratch();
}

void CreateMacroApp_::startFromScratch()
{
    mEditingPhrase = true;
    mNamingMacro = false;
    mConfirmingSave = false;
    mSave = 0;
    mPhraseTable.initTable({}); // TODO: init with mMacro to let someone cancel filename.
}

AppletStatus::TAppletStatus CreateMacroApp_::updateApp()
{
    
    if (mEditingPhrase)
    {
        if (mPhraseTable.update())
            return AppletStatus::ALIVE;
        mMacro.phrase = mPhraseTable.savePhrase();
        mPhraseTable.deleteTable();

        // mPhraseTable.mMaxEditRows = 1;
        // manimBottom.mText = "Disabled Keys";
        manimInputMacroName.start("Name:", &mFilename);
        bool joyEnable = true;
        for (auto it : mMacro.phrase) {
            joyEnable = joyEnable && !(bool)it.mJoyState;
        }
        mMacro.enabledJoystickState = joyEnable;
        mMacro.enabledButtonsMap = 0; // TODO: Custom Enabled Buttons.
        mEditingPhrase = false;
        mNamingMacro = true;
        return AppletStatus::ALIVE;
    }
    else if (mNamingMacro)
    {
        if (manimInputMacroName.updateDialog())
            return AppletStatus::ALIVE;

        if (manimInputMacroName.mCancel)
            return AppletStatus::RETURN;

        if (mFilename.length() > 0)
        {
            mMacro.name = mFilename;
            mNamingMacro = false;
            mConfirmingSave = true;
            manimInputDialogSave.start("Save?", &mSave, {"Yes", "No"});
            return AppletStatus::ALIVE;
        }
        else
            startFromScratch();
    }
    else if (mConfirmingSave)
    {
        if (manimInputDialogSave.updateDialog())
            return AppletStatus::ALIVE;

        switch (mSave)
        {
        case 0:
            Configurator.saveMacroToFile(mMacro);
            break;
        case 1:
            return AppletStatus::RETURN;
            break;
        }
        startFromScratch();
        return AppletStatus::ALIVE;
    }
    return AppletStatus::ALIVE;
}

void CreateMacroApp_::cleanupApp()
{
    // need to deregister compositor shit before it is deleted.
    mPhraseTable.deleteTable();
    mPhraseTable.mlife = 0;
    manimBottom.mlife = 0;
    MyJoystickBT.forceDisableCustomMacros(false);
    MyJoystickBT.toggleJoyTransmit(true);
}
