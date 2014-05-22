/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 Copyright (c) 2010      Ray Wenderlich

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameOverScene.h"
#include "GameController.h"
#include "PlayScene.h"

using namespace cocos2d;

bool GameOverScene::init()
{
	if( CCScene::init() )
	{
		this->_layer = GameOverLayer::create();
		this->addChild(_layer);

		return true;
	}
	else
	{
		return false;
	}
}

GameOverScene::~GameOverScene()
{
	if (_layer)
	{
		_layer = NULL;
	}
}


bool GameOverLayer::init()
{
	if ( CCLayerColor::initWithColor( ccc4(255,255,255,255) ) )
	{
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->_label = CCLabelTTF::create("","American Typewriter", 32);
		_label->setColor( ccc3(0, 0, 128) );
		_label->setPosition( ccp(winSize.width/2, winSize.height/2) );
		this->addChild(_label);

		//set score
		GameController *gc = GameController::getGameController();
		char s[200];
		snprintf(s, 200, "Score: %d ", gc->lastScore );
		this->getLabel()->setString(s);

		this->runAction( CCSequence::create(
                                CCDelayTime::create(1),
                                CCCallFunc::create(this,
                                callfunc_selector(GameOverLayer::gameOverDone)),
                                NULL));

		return true;
	}
	else
	{
		return false;
	}
}

void GameOverLayer::gameOverDone()
{
	CCDirector::sharedDirector()->replaceScene( PlayScene::scene() );
}

GameOverLayer::~GameOverLayer()
{
	if (_label)
	{
		_label = NULL;
	}
}
