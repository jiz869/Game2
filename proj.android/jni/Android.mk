LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

COCOS2DX_FILES  := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
COCOS2DX_FILES  := $(COCOS2DX_FILES:$(LOCAL_PATH)/%=%)

AD_FILES     := $(wildcard $(LOCAL_PATH)/../../StartApp.android/*.cpp)
AD_FILES     := $(AD_FILES:$(LOCAL_PATH)/%=%)

APP42_FILES     := $(wildcard $(LOCAL_PATH)/../../App42/cJSON/*.c)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/Common/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/Common/Exception/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/Crypto/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/GameService/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/PushNotificationService/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/StorageService/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/UserService/*.cpp)
APP42_FILES     += $(wildcard $(LOCAL_PATH)/../../App42/Util/*.cpp)
APP42_FILES     := $(APP42_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(COCOS2DX_FILES) $(APP42_FILES) $(AD_FILES) hellocpp/main.cpp                 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../App42/cJSON \
					$(LOCAL_PATH)/../../App42/Common $(LOCAL_PATH)/../../App42/Crypto \
					$(LOCAL_PATH)/../../App42/GameService $(LOCAL_PATH)/../../App42/PushNotificationService \
					$(LOCAL_PATH)/../../App42/StorageService $(LOCAL_PATH)/../../App42/UserService \
					$(LOCAL_PATH)/../../App42/Util $(LOCAL_PATH)/../../App42/Common/Exception

$(call import-add-path, /Users/welcomelm/cocos2d-x-2.2.3)
$(call import-add-path, /Users/welcomelm/cocos2d-x-2.2.3/cocos2dx/platform/third_party/android/prebuilt)

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
