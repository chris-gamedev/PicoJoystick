#ifndef CREATEMACRO_H
#define CREATEMACRO_H
#include "Applications.h"
#include "Images.h"
#include "Joystick.h"
#include "manifold6pt7b.h"
#include "Manifold_ArrowsAt21_12pt7b.h"

extern MyJoystickBT_ MyJoystickBT;
class PhraseTable_;

typedef void (PhraseTable_::*tableCommand)();

// struct MacroWord
// {
//     uint16_t mButtonStateMap = 0;
//     uint8_t mJoyState = 0;
//     uint16_t mDuration = 50;
// };

// struct Macro
// {
//     std::vector<MacroWord> phrase;
//     String name;
//     uint16_t disabledButtonsMap = 0;
// };

class ICell_
{
public:
    ICell_() {}
    tableCommand onSelect() { return command; };
    virtual void draw(JoyDisplay_ *pcanvas, uint8_t x, uint8_t y)
    {
        pcanvas->fillRect(mX, mY, mW, mH, 0x8);
    }
    tableCommand command = nullptr;
    uint8_t mRow = 0;
    uint8_t mX = 99;
    uint8_t mY = 99;
    uint8_t mW = 99;
    uint8_t mH = 99;
};
class IRow_
{
public:
    IRow_(uint8_t x, uint8_t y, struct MacroWord word = {0, 0, 0}) : mX(x), mY(y), mWord(word) {}
    virtual void draw(JoyDisplay_ *pcanvas)
    {
        // pcanvas->drawRect(mX, mY, mW, mH, 0xF);
    }
    struct MacroWord mWord = {1, 2, 3};
    ICell_ *maCells[3];
    uint8_t mX = 0;
    uint8_t mY = 0;
    uint8_t mW = 0;
    uint8_t mH = 0;
};

///////////////-------------------------  PhraseTable_ ----------------------////////////////////
class PhraseTable_ : public Animation_
{
public:
    PhraseTable_(uint8_t x, uint8_t y, std::vector<MacroWord> phrase);
    ~PhraseTable_();
    void initTable(std::vector<MacroWord> phrase);
    void moveCursor(int8_t dx, int8_t dy);
    uint8_t advanceTable(int8_t direction);
    void makeRowCoordinates();
    bool update();
    void addRowBefore();
    void removeRow();
    void StartEditStatemap()
    {
        mEditingStateMap = true;
        manimTopMessage.mText = "Edit Buttons";
    }
    void StartEditWordDuration()
    {
        mEditingWordDuration = true;
        manimTopMessage.mText = "Edit Duration";
    }
    bool editWordDuration();
    bool editStatemap();
    void updateAnim() {};
    void drawAnim(JoyDisplay_ *pcanvas);
    std::vector<MacroWord> savePhrase();
    void deleteTable();
    void endDialog();

    std::vector<IRow_ *> mvpRows;
    int8_t mcursorX = 0;
    int8_t mcursorY = 0;
    uint8_t mTableHead = 0;
    uint32_t mLastTime = 0;
    uint8_t mMaxEditRows = 20;
    AnimTextStatic1Line_ manimTopMessage;
    bool mEditingStateMap = false;
    bool mEditingWordDuration = false;
    uint16_t const mUpperDelayBound = 4000;
    uint16_t const mLowerDelayBound = 50;
};

///////////////-------------------------     Cells    ----------------------////////////////////

class AddCell_ : public ICell_
{
public:
    AddCell_()
    {
        mX = 0;
        mW = 128;
        mH = 8;
        command = &PhraseTable_::addRowBefore;
    }
};

class DeleteCell_ : public ICell_
{
public:
    DeleteCell_()
    {
        mX = 0;
        mW = 8;
        mH = 20;
        command = &PhraseTable_::removeRow;
    }
    void draw(JoyDisplay_ *pcanvas, uint8_t x, uint8_t y)
    {
        pcanvas->drawFastHLine(x, y + 6, 6, 0xC);
        pcanvas->drawFastHLine(x, y + 7, 6, 0xC);
        pcanvas->drawFastHLine(x, y + 8, 6, 0xC);
    }
};

class EditCell_ : public ICell_
{
public:
    EditCell_(struct MacroWord *word) : mpWord(word)
    {
        mX = 9;
        mW = 86;
        mH = 20;
        command = &PhraseTable_::StartEditStatemap;
    }
    void draw(JoyDisplay_ *pcanvas, uint8_t x, uint8_t y)
    {
        pcanvas->setFont(&Manifold_ArrowsAt21_12pt7b);
        pcanvas->setCursor(x + xOffset - 18, y + yOffset + 15);
        pcanvas->write(0X21 + mpWord->mJoyState);
        pcanvas->drawBitmap(x + xOffset, y + yOffset, bmpBackground, 64, 16, 0x9);

        for (int i = 0; i < 16; i++)
        {
            if (mpWord->mButtonStateMap & MAKE_BUTTON_BITMASK_16(i))
                pcanvas->drawBitmap(map_bitmap_ButtonLayoutMacroView64x16[i][0] + x + xOffset, map_bitmap_ButtonLayoutMacroView64x16[i][1] + y + yOffset, bmpButton, 8, 8, 0xF);
        }
    }
    const unsigned char *bmpBackground = bitmap_ButtonLayoutMacroView64x16;
    const unsigned char *bmpButton = bitmap_ButtonMacroView8x8;
    struct MacroWord *mpWord;
    int8_t const xOffset = 28;
    int8_t const yOffset = 2;
};

class DurationCell_ : public ICell_
{
public:
    DurationCell_(struct MacroWord *word) : mpWord(word)
    {
        mX = 95;
        mW = 33;
        mH = 20;
        command = &PhraseTable_::StartEditWordDuration;
    }
    void draw(JoyDisplay_ *pcanvas, uint8_t x, uint8_t y)
    {
        char duration[5];
        sprintf(duration, "%04d", mpWord->mDuration);
        pcanvas->setFont(&manifold6pt7b);
        pcanvas->setCursor(x + xOffset, y + yOffset);
        pcanvas->setTextWrap(false);
        pcanvas->print(duration);
    }
    struct MacroWord *mpWord;
    int8_t const xOffset = 96;
    int8_t const yOffset = 15;
};

///////////////-------------------------      Rows      ----------------------////////////////////
class WordRow_ : public IRow_
{
public:
    WordRow_(uint8_t x, uint8_t y, MacroWord word) : IRow_(x, y, word), mEditCell(&mWord), mDurCell(&mWord)
    {
        mW = 128;
        mH = 20;
        maCells[0] = &mDelCell;
        maCells[1] = &mEditCell;
        maCells[2] = &mDurCell;
    }
    void draw(JoyDisplay_ *pcanvas)
    {
        mDelCell.draw(pcanvas, mX, mY);
        mEditCell.draw(pcanvas, mX, mY);
        mDurCell.draw(pcanvas, mX, mY);
        // pcanvas->drawRect(mX, mY, mW, mH, 0xF);
    }

    DeleteCell_ mDelCell;
    EditCell_ mEditCell;
    DurationCell_ mDurCell;
};

class AddRow_ : public IRow_
{
public:
    AddRow_(int8_t x, int8_t y) : IRow_(x, y)
    {
        mW = 127;
        mH = 8;
        maCells[0] = &mAddCell;
        maCells[1] = &mAddCell;
        maCells[2] = &mAddCell;
    }
    void draw(JoyDisplay_ *pcanvas)
    {
        pcanvas->setFont(&manifold8pt7b);
        pcanvas->drawFastHLine(mX, mY + (mH / 2), (mW / 2) - 10, 0xF);
        pcanvas->drawFastHLine(mX + (mW / 2) + 10, mY + (mH / 2), (mW / 2) - 10, 0xF);
        pcanvas->setCursor(mX + (mW / 2) - 3, mY + 9);
        pcanvas->write("+");
    }
    AddCell_ mAddCell;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////      Create Macro       ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CreateMacroApp_ : public Applet_
{
public:
    CreateMacroApp_(Compositor_ *comp, Applet_ *drawkeyapp)
            : Applet_(comp)
            , mPhraseTable(0, 18, {})
            , mpappDrawKeysApp(drawkeyapp)
            , manimBottom(0, 112, 128, 15)
            , manimInputDialogSave(comp, &mSave, "Save?")
            , manimInputMacroName(comp, &mFilename)

    {
        manimBottom.setText("Edit Macro");
        manimBottom.mpFont = &manifold6pt7b;
        manimBottom.mFontWidth = 7;
        manimBottom.mYOffset = -3;
        manimBottom.mDrawBox = true;
    }

    // for applet
    void initApp();
    void startFromScratch();
    AppletStatus::TAppletStatus updateApp();
    bool exitApp() { return false; }
    void cleanupApp();
    // for Animation {}_

    String mFilename;
    Applet_ *mpappDrawKeysApp;
    AnimTextStatic1Line_ manimBottom;
    PhraseTable_ mPhraseTable;
    AnimInputDialogList_ manimInputDialogSave;
    AnimInputDialogString_ manimInputMacroName;
    Macro mMacro;
    uint32_t mAppLastTime;
    uint16_t mAppDelay;
    uint8_t mSave;
    bool mEditingPhrase = false;
    bool mConfirmingSave = false;
    bool mNamingMacro = false;
};
#endif