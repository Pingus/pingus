LOCAL_PATH := $(call my-dir)

# ---------------------------------------------------------------------------
# Prebuilt OpenAL Soft + libmodplug (per-ABI static libs from AUDIO_ANDROID_LIBS)
# ---------------------------------------------------------------------------
ifeq ($(ENABLE_ANDROID_SOUND),1)

include $(CLEAR_VARS)
LOCAL_MODULE := openal
LOCAL_SRC_FILES := ../audio/$(TARGET_ARCH_ABI)/lib/libopenal.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../audio/include $(LOCAL_PATH)/../audio/include/AL
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := modplug
LOCAL_SRC_FILES := ../audio/$(TARGET_ARCH_ABI)/lib/libmodplug.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../audio/include
include $(PREBUILT_STATIC_LIBRARY)

endif

# ---------------------------------------------------------------------------
# libmain — Pingus + staged external sources
# ---------------------------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := main

# Recursively collect C++ under this tree (game + deps/*).
RWILDCARD = $(foreach d,$(wildcard $1*),$(call RWILDCARD,$d/,$2) $(filter $(subst *,%,$2),$d))
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(call RWILDCARD,$(LOCAL_PATH)/,%.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%,%,$(wildcard $(LOCAL_PATH)/*.c))

# Exclude desktop-only / optional backends when present under the tree.
LOCAL_SRC_FILES := $(filter-out %/win32/% win32/%,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(filter-out %/json_reader_impl.cpp %/json_writer_impl.cpp %/jsonpretty_writer_impl.cpp,$(LOCAL_SRC_FILES))

ifneq ($(ENABLE_ANDROID_SOUND),1)
LOCAL_SRC_FILES := $(filter-out %/sound_real.cpp,$(LOCAL_SRC_FILES))
# Without OpenAL, drop wstsound sources staged under deps/
LOCAL_SRC_FILES := $(filter-out %/deps/wstsound/%,$(LOCAL_SRC_FILES))
endif

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
	$(LOCAL_PATH)/../external_includes/wstsound \
	$(LOCAL_PATH)/deps/argpp \
	$(LOCAL_PATH)/deps/logmich \
	$(LOCAL_PATH)/deps/sexpcpp \
	$(LOCAL_PATH)/deps/strutcpp \
	$(LOCAL_PATH)/deps/priocpp \
	$(LOCAL_PATH)/deps/tinygettext \
	$(LOCAL_PATH)/deps/wstsound

ifeq ($(ENABLE_ANDROID_SOUND),1)
# libmodplug: <libmodplug/modplug.h>  OpenAL Soft install: include/AL/*.h
# Non-Emscripten wstsound uses <al.h>/<alc.h>/<alext.h> (not <AL/al.h>).
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../audio/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../audio/include/AL
endif

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -llog -landroid -lz -lGLESv2 -lEGL

ifeq ($(ENABLE_ANDROID_SOUND),1)
# WHOLE_STATIC: OpenAL Soft registers backends via static constructors;
# regular static linking drops them and alcOpenDevice fails silently.
LOCAL_WHOLE_STATIC_LIBRARIES := openal
LOCAL_STATIC_LIBRARIES := modplug
LOCAL_LDLIBS += -lOpenSLES
LOCAL_CFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_USE_GLES=1
LOCAL_CPPFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_USE_GLES=1 -std=c++20 -D_LIBCPP_ENABLE_EXPERIMENTAL=1
LOCAL_CFLAGS += -DWSTSOUND_WITH_MODPLUG=1
LOCAL_CPPFLAGS += -DWSTSOUND_WITH_MODPLUG=1
else
LOCAL_CFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1
LOCAL_CPPFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1 -std=c++20 -D_LIBCPP_ENABLE_EXPERIMENTAL=1
endif

LOCAL_CFLAGS += -DPRIO_USE_SEXPCPP=1 -DTINYGETTEXT_UTF8_ONLY=1
LOCAL_CPPFLAGS += -DPRIO_USE_SEXPCPP=1 -DTINYGETTEXT_UTF8_ONLY=1
ifndef PINGUS_VERSION
PINGUS_VERSION := 0.8.0-dev
endif
LOCAL_CFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CPPFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CFLAGS += -DPINGUS_DEFAULT_DATADIR=\"\" -DDATA_PREFIX=\"\"
LOCAL_CPPFLAGS += -DPINGUS_DEFAULT_DATADIR=\"\" -DDATA_PREFIX=\"\"

LOCAL_CPP_FEATURES := exceptions rtti

include $(BUILD_SHARED_LIBRARY)
