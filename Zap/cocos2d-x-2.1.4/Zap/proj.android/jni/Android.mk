LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/Scenes/GameWorldScene.cpp \
                   ../../Classes/Scenes/GameMenuScene.cpp \
                   ../../Classes/Scenes/GameOverScene.cpp \
                   ../../Classes/Utilities/GameManager.cpp \
                   ../../Classes/Effects/LightningLine.cpp \
                   ../../Classes/Entities/BugBase.cpp \
                   ../../Classes/Entities/BlueBug.cpp \
                   ../../Classes/Entities/RedBug.cpp \
                   ../../Classes/Entities/GreenBug.cpp \
                   ../../Classes/Entities/Lantern.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static
            
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
