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
    void addHornSounds();
private:
    CCSize winSize;
    CCSpriteBatchNode * node;
    void addRoad();
    void addCityObj();
    int road_tile_number;
};

#endif /* CITY_H_ */
