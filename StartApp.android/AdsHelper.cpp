#include "AdsHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string.h>

const char* AppActivityClassName = "ca/welcomelm/crossRoadNow/crossRoadNow";

void showAds()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "showAds", "()V"))
	{

		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
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

void changeAdsId(const char * dev_id , const char * app_id)
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "changeAdsId", "(Ljava/lang/String;Ljava/lang/String;)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID ,
				(t.env)->NewStringUTF(dev_id) , (t.env)->NewStringUTF(app_id));
		t.env->DeleteLocalRef(t.classID);
	}
}
