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

protected:

    virtual void initBloodBar();
    virtual void initScoreLabel();
    virtual void initMenu();
    virtual void initMisc();
    virtual void initLevelSplash();
};

#endif /* MULTIPLAYCONTROLMENU_H_ */
