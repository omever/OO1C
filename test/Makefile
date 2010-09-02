OOPATH=/usr/lib/ooo3
BASIS=$(OOPATH)/basis-link
URE=$(OOPATH)/ure
PRJ=$(BASIS)/sdk
SETTINGS=$(PRJ)/settings

include $(SETTINGS)/settings.mk
include $(SETTINGS)/std.mk
include $(SETTINGS)/dk.mk

TARGET=ootest

SOURCES=main.cpp \
	OOTest.cpp \
	warpcontainer.cpp

LIBS=pthread

OBJECTS=$(SOURCES:.cpp=.o)
INCLUDES=-Iinclude -I/usr/lib/ooo3/basis-link/sdk/include
CXXLAGS=-g $(CXXFLAGS) $(INCLUDES) $(CFLAGS) $(CC_FLAGS) $(CC_INCLUDES) $(CC_DEFINES) -m32 -finput-charset=UTF-8 -fPIC

all: $(TARGET)

include $(SETTINGS)/stdtarget.mk

-include $(OBJECTS:.o=.d)

%.o: %.cpp
	g++ -c  $(CXXLAGS) $*.cpp -o $*.o
	g++ -MM $(CXXLAGS) $*.cpp >  $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(TARGET): $(OBJECTS) Makefile
	g++ $(INCLUDES) $(EXE_LINK_FLAGS) $(LINK_LIBS) $(OBJECTS) -o $(TARGET) $(addprefix -l, $(LIBS)) $(CPPUHELPERLIB) $(CPPULIB) $(SALHELPERLIB) $(SALLIB) $(STLPORTLIB) $(STDC++LIB) $(CPPUHELPERDYLIB) $(CPPUDYLIB) $(SALHELPERDYLIB) $(SALDYLIB)

cppumk: 
	$PRJ/bin/cppumaker -Gc -BUCR -O/home/om/openoffice.org3.2_sdk/LINUXexample.out/inc "$(URE)/share/misc/types.rdb" "$(BASIS)/program/offapi.rdb"


clean:
	-rm $(TARGET) *.o *.d

deploy:
	zip oolink.zip MANIFEST.XML oolink.so

ifeq "$(OS)" "WIN"
INI_EXTENSION=.ini
else
INI_EXTENSION=rc
endif

