/*
 * City.h
 *
 *  Created on: May 29, 2014
 *      Author: mli
 */

#ifndef CITY_H_
#define CITY_H_

#include "GameController.h"

class City: public cocos2d::CCLayer {
public:
	City();
	virtual ~City();
    CREATE_FUNC(City);
    virtual bool init();
    void addHornSounds(float dt);
    void addSpecial(float dt);
	void expireSpecial(CCNode * special);
    void scheduleSpecial();
    void step(float dt);
private:
    CCSize winSize;
    void addRoad();
    void addCityObj();
    PlaySceneData * playSceneData;
    UserData * userData;
    float timeSinceLastSpecial;
};

#endif /* CITY_H_ */
