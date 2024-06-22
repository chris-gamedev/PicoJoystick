#include "Compositor.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////  Compositor  ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Compositor_::registerAnimation(Animation_ *pAnim, CanvasType::TCanvasType canvas)
{
    using CanvasType ::TCanvasType;

    switch (canvas)
    {
    case TCanvasType::FG:
        mvFG.push_back(pAnim);
        std::sort(mvFG.begin(), mvFG.end(), [](Animation_ *a, Animation_ *b)
                  { return *a < *b; });
        break;
    case TCanvasType::BG:
        mvBG.push_back(pAnim);
        std::sort(mvBG.begin(), mvBG.end(), [](Animation_ *a, Animation_ *b)
                  { return *a < *b; });
        break;
    case TCanvasType::TOP:
        mvTop.push_back(pAnim);
        std::sort(mvTop.begin(), mvTop.end(), [](Animation_ *a, Animation_ *b)
                  { return *a < *b; });
        break;
    case TCanvasType::BOTTOM:
        mvBottom.push_back(pAnim);
        std::sort(mvBottom.begin(), mvBottom.end(), [](Animation_ *a, Animation_ *b)
                  { return *a < *b; });
        break;
    }
}

void Compositor_::killAnimation(Animation_ *pAnim)
{
}

void Compositor_::killAnimation(Animation_ *pAnim, CanvasType::TCanvasType canvas)
{
    std::vector<Animation_ *> *pvpCanvas;
    switch (canvas)
    {
    case CanvasType::FG:
        pvpCanvas = &mvFG;
        break;
    case CanvasType::BG:
        pvpCanvas = &mvBG;
        break;
    case CanvasType::TOP:
        pvpCanvas = &mvTop;
        break;
    case CanvasType::BOTTOM:
        pvpCanvas = &mvBottom;
        break;
    }

    std::vector<Animation_ *>::iterator it = pvpCanvas->begin();
    while ( it != pvpCanvas->end())
    {
        if (*it == pAnim)
        {
            pvpCanvas->erase(it);
            break;
        }
        else
            it++;
    }
}

void Compositor_::purgeAll()
{
    mvFG.clear();
    mvBG.clear();
    mvTop.clear();
    mvBottom.clear();
}

void Compositor_::update()
{
    Serial.print("In comp, ");
    if (mclock + mclockdelay >= millis())
        return;
    mclock += mclockdelay;

    updateVectors(&mvFG);
    updateVectors(&mvBG);
    updateVectors(&mvTop);
    updateVectors(&mvBottom);
}

void Compositor_::updateVectors(std::vector<Animation_ *> *v)
{
    for (std::vector<Animation_ *>::iterator &&it = v->begin(); it != v->end();)
    {
        (*it)->updateAnim();
        if ((*it)->mlife == 0)
            it = v->erase(it);
        else
            ++it;
    }
}

void Compositor_::draw()
{
    // TODO Sort out these canvases!
    mpDisplay->clearDisplay();

    for (auto &it : mvBG)
        it->drawAnim(mpDisplay);
        Serial.print("comp. after bg, ");
    for (auto &it : mvTop)
        it->drawAnim(mpDisplay);
        Serial.print("comp. after top, ");

    for (auto &it : mvBottom)
        it->drawAnim(mpDisplay);
        Serial.print("comp. after bot, ");
        
    for (auto &it : mvFG)
        it->drawAnim(mpDisplay);
        Serial.print("comp. after fg, ");

    mpDisplay->display();
}
