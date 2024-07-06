#ifndef MENUMANAGER_H
#define MENUMANAGER_H
#include <Arduino.h>
#include <vector>
#include <array>
#include "MyJoystick.h"
#include "Display.h"
#include "Compositor.h"
#include "Applet.h"

/// @brief Abstract class for menu entries. Contains vector of children Menu_ entries
class Menu_
{
public:
    Menu_(String name, Menu_ *parent) : mName(name), mpParent(parent) {}
    void inline addChild(Menu_ *child)
    {
        mChildren.reserve(5);
        mChildren.push_back(child);
        mChildPrompts.push_back(child->mName);
    }
    virtual Menu_ *action()
    {
        Serial.println("-----ERROR!!!      INSIDE MENU_ ACTION");
        return nullptr;
    }

    String mName;
    // parent node.
    Menu_ *const mpParent;
    std::vector<Menu_ *> mChildren;
    static AnimTextPrompt_ *mUpperText;
    static AnimTextPrompt_ *mLowerText;
    static uint32_t mLastTime;
    std::vector<String> mChildPrompts;
};

/// @brief Concrete class for menu entries.  Contains vector of Menu_ entries
class SubMenu_ : public Menu_
{
public:
    int8_t selection = 0;
    SubMenu_(String name, Menu_ *parent) : Menu_(name, parent) {}
    virtual Menu_ *action() override;
};

/// @brief Terminal menu entry contianing an action
class Leaf_ : public Menu_
{
public:
    /// @brief Terminal menu entry containing an action
    /// @param name String name of node
    /// @param cb callback function pointer of type (void)(*)(void)
    Leaf_(String name, void (*cb)(), Menu_ *parent) : Menu_(name, parent), pCallBack{cb} {}
    /// @brief Call the callback function and return the parent menu
    virtual Menu_ *action() override
    {
        pCallBack();
        return mpParent;
    }
    void (*pCallBack)();
};

class MenuManager_ : public Applet_, public Animation_
{
public:
    Menu_ *currentMenuNode = nullptr;
    Compositor_ *const mpCompositor;

    AnimTextPrompt_ mUpperTextSpriteStatic;
    AnimTextPrompt_ mLowerTextSpriteStatic;
    MenuManager_(Compositor_ *comp);
    ~MenuManager_() { destroyMenuNodes(this->mMenuRoot.mChildren); }
    void destroyMenuNodes(std::vector<Menu_ *> v)
    {
        if (!v.empty())
            for (auto it : v)
            {
                destroyMenuNodes(it->mChildren);
                delete it;
            }
    }
    void initApp();
    AppletStatus::TAppletStatus updateApp();
    void cleanupApp();

    void drawAnim(JoyDisplay_ *pcanvas); // TODO: Possibly inherit from animation
    SubMenu_ mMenuRoot;

private:
    void buildMenu();
};

#endif