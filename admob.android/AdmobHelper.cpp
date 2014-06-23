#include "AdmobHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>


const char* AppActivityClassName = "ca/welcomelm/crossRoad/crossRoad";

static bool isAdShowing = false;

static void hideAd()
{
	if(isAdShowing == false) return;

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "hideAd", "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = false;
    }
}



static void showAd()
{
	if(isAdShowing == true) return;

    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAd", "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        isAdShowing = true;
    }

}

void setBannerViewHidden(bool hidden){
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

void purchase()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "purchase", "()V"))
    {

        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }

}
