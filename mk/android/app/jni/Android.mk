LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# Sources are copied under jni/src/ by build-apk.sh (entire src/ tree + optional shims).
# Recursive wildcard: every .cpp under this module directory.
RWILDCARD = $(foreach d,$(wildcard $1*),$(call RWILDCARD,$d/,$2)$(filter $2,$d))
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(call RWILDCARD,$(LOCAL_PATH)/,%.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%,%,$(wildcard $(LOCAL_PATH)/*.c))

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../SDL/include \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../SDL/include/SDL2

# Prefer SDL2_image from the prebuilt layer when present; otherwise STB shim may be used.
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image

LOCAL_LDLIBS := -llog -landroid -lz -lGLESv2 -lEGL

LOCAL_CFLAGS += -DUSE_SDL2 -DANDROID
LOCAL_CPPFLAGS += -DUSE_SDL2 -DANDROID -std=c++17
ifndef PINGUS_VERSION
PINGUS_VERSION := 0.8.0-dev
endif
LOCAL_CFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CPPFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CFLAGS += -DDATA_PREFIX=\".\"
LOCAL_CPPFLAGS += -DDATA_PREFIX=\".\"

LOCAL_CPP_FEATURES := exceptions rtti

include $(BUILD_SHARED_LIBRARY)
