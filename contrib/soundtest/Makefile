PINGUS_DIR=../../src
LIBS=-lclanCore -lclanApp -lclanDisplay -lclanSound -lclanVorbis -lclanMikMod
PINGUS_LIBS=$(PINGUS_DIR)/sound/libpingus_sound.a \
  $(PINGUS_DIR)/globals.o \
  $(PINGUS_DIR)/debug.o   \
  $(PINGUS_DIR)/path_manager.o \
  $(PINGUS_DIR)/debug_stream.o \
  $(PINGUS_DIR)/system.o

soundtest : soundtest.cxx soundtest.hxx $(PINGUS_LIBS)
	$(CXX) -I$(PINGUS_DIR) $< $(PINGUS_LIBS) -o $@ $(LIBS)

# EOF #