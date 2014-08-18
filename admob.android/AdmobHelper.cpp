#include "AdmobHelper.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <string.h>

const char* AppActivityClassName = "ca/welcomelm/crossRoad/crossRoad";

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

void changeAdmobId(const char * adsId)
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, AppActivityClassName, "changeAdmobId", "(Ljava/lang/String;)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID ,
				(t.env)->NewStringUTF(adsId));
		t.env->DeleteLocalRef(t.classID);
	}
}
