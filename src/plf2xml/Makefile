OBJECTS = plf2xml.o ../PingusResource.o ../PLFPLF.o ../PLF.o ../PLFParser.o ../PSMParser.o ../PingusError.o ../ResDescriptor.o ../globals.o ../algo.o ../System.o
LIBS    = -lclanCore -lxml

plf2xml : $(OBJECTS)
	g++ -o  $@ $(OBJECTS) $(LIBS)

plf2xml.o : plf2xml.cc

# EOF #