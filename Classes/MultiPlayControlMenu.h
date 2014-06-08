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
    void enemyOver();
    virtual void showUp();
    void updateEnemyScore(float score);

protected:

    virtual void initBloodBar();
    virtual void initScoreLabel();
    virtual void initMenu();
    virtual void initMisc();
    virtual void initLevelSplash();

    virtual void GameOver();

    CCLabelTTF * scoreLabelEnemy;
    float enemyScore;
    virtual void updateScore(bool isGood);
    bool isEnemyOver;
};

#endif /* MULTIPLAYCONTROLMENU_H_ */
