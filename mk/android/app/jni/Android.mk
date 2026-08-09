LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# Sources are copied under jni/src/ by build-apk.sh (entire src/ tree).
RWILDCARD = $(foreach d,$(wildcard $1*),$(call RWILDCARD,$d/,$2)$(filter $2,$d))
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(call RWILDCARD,$(LOCAL_PATH)/,%.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%,%,$(wildcard $(LOCAL_PATH)/*.c))
# Exclude desktop-only / optional backends when present under the tree.
LOCAL_SRC_FILES := $(filter-out %/sound_real.cpp,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(filter-out %/win32/% win32/%,$(LOCAL_SRC_FILES))
LOCAL_SRC_FILES := $(filter-out %/json_reader_impl.cpp %/json_writer_impl.cpp %/jsonpretty_writer_impl.cpp,$(LOCAL_SRC_FILES))

# external_includes holds include/<ns>/… (geom/, prio/, sexp/, glm/, …).
# Per-ns paths support #include "argpp.hpp" style used inside external sources.
# deps/* hold private headers next to compiled .cpp (float.hpp, etc.).
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
	$(LOCAL_PATH)/deps/argpp \
	$(LOCAL_PATH)/deps/logmich \
	$(LOCAL_PATH)/deps/sexpcpp \
	$(LOCAL_PATH)/deps/strutcpp \
	$(LOCAL_PATH)/deps/priocpp \
	$(LOCAL_PATH)/deps/tinygettext

# SDL2_image is not a prebuilt here — IMG_* comes from img_stb_min.c
# (compiled into this module via build-apk.sh). Prebuilt mk only ships
# SDL2 + optional SDL2_mixer.
LOCAL_SHARED_LIBRARIES := SDL2

# Sound (wstsound + OpenAL Soft + libmodplug) is tracked in TODO.md.
# Until those ABIs are prebuilt and linked here, keep the dummy backend:
#   - PINGUS_NO_SOUND=1
#   - sound_real.cpp filtered out of LOCAL_SRC_FILES
# Planned:
#   LOCAL_STATIC_LIBRARIES += openal modplug
#   stage external/wstsound/src (wav + modplug only; no vorbis/opus/mpg123/efx)
#   drop PINGUS_NO_SOUND and the sound_real.cpp filter

LOCAL_LDLIBS := -llog -landroid -lz -lGLESv2 -lEGL

LOCAL_CFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1
LOCAL_CPPFLAGS += -DUSE_SDL2 -DANDROID -DPINGUS_NO_SOUND=1 -DPINGUS_USE_GLES=1 -std=c++20 -D_LIBCPP_ENABLE_EXPERIMENTAL=1
# prio: sexpr only (no jsoncpp). tinygettext: avoid libiconv on Android.
LOCAL_CFLAGS += -DPRIO_USE_SEXPCPP=1 -DTINYGETTEXT_UTF8_ONLY=1
LOCAL_CPPFLAGS += -DPRIO_USE_SEXPCPP=1 -DTINYGETTEXT_UTF8_ONLY=1
ifndef PINGUS_VERSION
PINGUS_VERSION := 0.8.0-dev
endif
LOCAL_CFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
LOCAL_CPPFLAGS += -DPROJECT_VERSION=\"$(PINGUS_VERSION)\" -DPROJECT_NAME=\"pingus\"
# AssetManager root holds images/, levels/, … (contents of data/).
# Empty datadir → paths like "images/…" (no "./") for SDL_RWFromFile.
LOCAL_CFLAGS += -DPINGUS_DEFAULT_DATADIR=\"\" -DDATA_PREFIX=\"\"
LOCAL_CPPFLAGS += -DPINGUS_DEFAULT_DATADIR=\"\" -DDATA_PREFIX=\"\"

LOCAL_CPP_FEATURES := exceptions rtti

include $(BUILD_SHARED_LIBRARY)
