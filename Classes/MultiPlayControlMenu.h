/*
 * MultiPlayControlMenu.h
 *
 *  Created on: Jun 6, 2014
 *      Author: mli
 */

#ifndef MULTIPLAYCONTROLMENU_H_
#define MULTIPLAYCONTROLMENU_H_

#include "ControlMenu.h"

class MultiPlayControlMenu: public ControlMenu
{
public:
    MultiPlayControlMenu();
    virtual ~MultiPlayControlMenu();
    CREATE_FUNC(MultiPlayControlMenu);
    virtual bool init();
    virtual void step(float dt);
    virtual bool doScore();
    virtual void showUp();
    void updateEnemyScore(float score);
    bool isEnemyOver;
    virtual void gameOver();

protected:

    virtual void initBloodBar();
    virtual void initScoreLabel();
    virtual void initMenu();
    virtual void initMisc();
    virtual void initLevelSplash();

    CCLabelTTF * scoreLabelEnemy;
    float enemyScore;
    virtual void updateScore(bool isGood);
};

#endif /* MULTIPLAYCONTROLMENU_H_ */
