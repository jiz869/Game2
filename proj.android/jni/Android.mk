LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/B2Sprite.cpp \
                   ../../Classes/CarObj.cpp \
                   ../../Classes/ControlMenu.cpp \
                   ../../Classes/GameController.cpp \
                   ../../Classes/GameObj.cpp \
                   ../../Classes/GameOverScene.cpp \
                   ../../Classes/Lane.cpp \
                   ../../Classes/MenuForArrowButton.cpp \
                   ../../Classes/PlayObj.cpp \
                   ../../Classes/SpecialObj.cpp \
                   ../../Classes/StartMenuScene.cpp \
                   ../../Classes/PlayScene.cpp \
                   ../../Classes/City.cpp \
                   ../../Classes/MultiPlayScene.cpp \
                   ../../Classes/MultiPlayControlMenu.cpp \
                   ../../admob.android/AdmobHelper.cpp \
                   ../../AppWarpX/appwarp.cpp \
                   ../../AppWarpX/appwarp_extended.cpp \
                   ../../AppWarpX/base64.cpp \
                   ../../AppWarpX/cJSON.c \
                   ../../AppWarpX/HMAC_SHA1.cpp \
                   ../../AppWarpX/requests.cpp \
                   ../../AppWarpX/SHA1.cpp \
                   ../../AppWarpX/socket.cpp \
                   ../../AppWarpX/udpsocket.cpp \
                   ../../AppWarpX/urlencode.cpp \
                   ../../AppWarpX/utilities.cpp                   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../AppWarpX

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
