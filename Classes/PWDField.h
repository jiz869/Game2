//
//  PWDField.h
//  crossRoad
//
//  Created by Min Li on 2014-07-27.
//
//

#ifndef __crossRoad__PWDField__
#define __crossRoad__PWDField__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class PWDField : public CCTextFieldTTF {
public:
    
    PWDField();
    virtual ~PWDField();
    
    virtual void setString(const char *text);
    
    static PWDField * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

private:
    
    void replaceWithDots();
};

#endif /* defined(__crossRoad__PWDField__) */
