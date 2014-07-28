//
//  PWDField.cpp
//  crossRoad
//
//  Created by Min Li on 2014-07-27.
//
//

#include "PWDField.h"

PWDField::PWDField(){
    
}

PWDField::~PWDField(){
    
}

void PWDField::setString(const char *text){
    CCTextFieldTTF::setString(text);
    replaceWithDots();
}

void PWDField::replaceWithDots(){
    
    int length = strlen(CCTextFieldTTF::getString());
    
    if (length == 0) {
        return;
    }
    
    string dots = "";
    
    for (int i = 0; i < length; i++) {
        dots.append("*");
    }
    
    CCLabelTTF::setString(dots.c_str());
    
}

PWDField * PWDField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize){
    PWDField *pRet = new PWDField();
    if(pRet && pRet->initWithString("", fontName, fontSize))
    {
        pRet->autorelease();
        if (placeholder)
        {
            pRet->setPlaceHolder(placeholder);
        }
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

