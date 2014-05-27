#include "AdmobHelper.h"

bool AdmobHelper::isAdShowing = true;

#include "platform/android/jni/JniHelper.h"
#include <jni.h>
//#include <android/log.h>


const char* AppActivityClassName = "ca/welcomelm/crossRoad/crossRoad";

void AdmobHelper::hideAd()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = false;
    }
}



void AdmobHelper::showAd()
{

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = true;
    }

}

void AdmobHelper::setBannerViewHidden(bool hidden){
	switch(hidden){
		case true:
			hideAd();
			break;
		case false:
			showAd();
			break;
		default:
			break;
	}
}
