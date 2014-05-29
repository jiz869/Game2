/*
 * City.cpp
 *
 *  Created on: May 29, 2014
 *      Author: mli
 */

#include "City.h"
#include "Lane.h"

City::City() {
	// TODO Auto-generated constructor stub

}

City::~City() {
	// TODO Auto-generated destructor stub
}

bool City::init(){
	if(!CCLayer::init()){
		return false;
	}

	winSize = CCDirector::sharedDirector()->getWinSize();

	node = CCSpriteBatchNode::create("sprites.png");

	addChild(node);

	road_tile_number = getRandom(0 , 6);

	addCityObj();

	addRoad();

	return true;
}

void City::addRoad(){

	for(float i = 0 ; i < 0.9 ; i+=0.1){
		CCString * roadName = CCString::createWithFormat("road%d.png", road_tile_number);

		CCSprite * road = CCSprite::createWithSpriteFrameName(roadName->getCString());

		road->setAnchorPoint(ccp(1 , 0.5));

		road->setPosition(ccp(winSize.width , i * winSize.height));

		road->setScaleY(0.8);

		node->addChild(road);
	}
}

void City::addCityObj(){
    int number;
	CCString * spriteName;
	CCSprite * sprite;
	float scaleX, scaleY;

	for(int i = 0 ; i < 8 ; i++){
		for(int j = 0 ; j < 21 ; j++){
			if(j == 10 || j == 5 || j == 15 ) {
				spriteName = CCString::createWithFormat("tile%d.png", road_tile_number);
				scaleX = 1;
				scaleY = 0.8;
			}else if(toss(0.4)){
				continue;
			}else if(toss(0.5)){
                number = getRandom(0 , 38);
                spriteName = CCString::createWithFormat("house%d.png", number);
                scaleX = 0.6;
                scaleY = 0.6;
            }else{
                number = getRandom(0 , 16);
                spriteName = CCString::createWithFormat("tree%d.png", number);
                scaleX = 0.6;
                scaleY = 0.6;
            }
            sprite = CCSprite::createWithSpriteFrameName(spriteName->getCString());
			sprite->setPosition(ccp(j * 0.05 * winSize.width , (0.05 + i * 0.1) * winSize.height));
			sprite->setScaleX(scaleX);
			sprite->setScaleY(scaleY);
			node->addChild(sprite);
		}
	}
}
