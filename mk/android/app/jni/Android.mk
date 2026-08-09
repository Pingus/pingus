LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# Sources are copied under jni/src/ by build-apk.sh (entire src/ tree).
RWILDCARD = $(foreach d,$(wildcard $1*),$(call RWILDCARD,$d/,$2)$(filter $2,$d))
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(call RWILDCARD,$(LOCAL_PATH)/,%.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%,%,$(wildcard $(LOCAL_PATH)/*.c))
# Exclude desktop-only / optional backends when present under the tree.
LOCAL_SRC_FILES := $(filter-out %/sound_real.cpp,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(filter-out %/win32/%,$(LOCAL_SRC_FILES))

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../SDL/include \
	$(LOCAL_PATH)/../SDL/include/SDL2 \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../external_includes \
	$(LOCAL_PATH)/../external_includes/argpp \
	$(LOCAL_PATH)/../external_includes/geom \
	$(LOCAL_PATH)/../external_includes/logmich \
	$(LOCAL_PATH)/../external_includes/prio \
	$(LOCAL_PATH)/../external_includes/strut \
	$(LOCAL_PATH)/../external_includes/sexp \
	$(LOCAL_PATH)/../external_includes/tinygettext \
	$(LOCAL_PATH)/../external_includes/uitest \
	$(LOCAL_PATH)/../external_includes/wstsound \
	$(LOCAL_PATH)/../external_includes/xdgcpp

# SDL2_image is not a prebuilt here — IMG_* comes from img_stb_min.c
# (compiled into this module via build-apk.sh). Prebuilt mk only ships
# SDL2 + optional SDL2_mixer.
LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -llog -landroid -lz -lGLESv2 -lEGL

LOCAL_CFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1
LOCAL_CPPFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1 -std=c++17
ifndef PINGUS_VERSION
PINGUS_VERSION := 0.8.0-dev
endif
LOCAL_CFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CPPFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
# AssetManager root holds images/, levels/, … (contents of data/).
# PINGUS_DEFAULT_DATADIR is what pingus_main uses.
LOCAL_CFLAGS += -DPINGUS_DEFAULT_DATADIR=\".\" -DDATA_PREFIX=\".\"
LOCAL_CPPFLAGS += -DPINGUS_DEFAULT_DATADIR=\".\" -DDATA_PREFIX=\".\"

LOCAL_CPP_FEATURES := exceptions rtti

include $(BUILD_SHARED_LIBRARY)
