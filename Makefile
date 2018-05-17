

INCLUDEDIRS= sqlite-core/Headers example example/src/utilities example/uat example/uat/adsb_uat example/utilities example/common


#src/utilities src/hardware  src/uat src/uat/adsb_uat src/common src/Log messages src/routingmsg src/applications \

LIBS=   -l sqlite3 -lreadline -lzmq

VPATH =  sqlite-core/Sources  example example/uat example/uat/adsb_uat example/utilities

DEBUG=-g


TOPDIR=${PWD}

CPP=g++


ifdef DISABLE_READLINE
DEFINES+=DISABLE_READLINE
endif

APPVERSION:= $(shell scripts/gversion.sh)


SOSUBREV=0


#Clean/dirty state


DEFINE_ARGS=$(patsubst %,-D%, $(DEFINES))


PROGRAM=telsql


C_SOURCES=

UAT_SOURCES=UATData.cpp HorizontalVelocity.cpp VerticalVelocity.cpp Altitude.cpp  \
			Callsign.cpp BaroSetting.cpp SelectedAltitude.cpp  SelectedHeading.cpp \
			UATData_persistable.cpp appdata.cpp




CPP_SOURCES= 	main.cpp \
		dbmanager.cpp \
		persistablebase.cpp \
		table.cpp \
		db.cpp \
		columns.cpp \
		objecttype.cpp \
		$(UAT_SOURCES)


C_OBJECTS=$(patsubst %.c,%.o, $(C_SOURCES))

CPP_OBJECTS=$(patsubst %.cpp,%.o, $(CPP_SOURCES))

EG_CPP_OBJECTS = $(patsubst %.cpp,%.o, $(EG_CPP_SOURCES))

CFLAGS=$(DEBUG)  -MMD $(INCPATH)  $(DEFINE_ARGS) -lpthread

CPPFLAGS=$(DEBUG) -MMD $(INCPATH) -Wno-pmf-conversions -std=c++11 $(DEFINE_ARGS) -pthread



C_DEPS=$(C_SOURCES:.c=.d)

CPP_DEPS=$(CPP_SOURCES:.cpp=.d)

EG_CPP_DEPS=$(EG_CPP_SOURCES:.cpp=.d)

OBJECTS=$(C_OBJECTS) $(CPP_OBJECTS)

EGOBJECTS=$(OBJECTS) $(EG_CPP_OBJECTS)

DEPS=$(C_DEPS) $(CPP_DEPS)


INCLUDEDIR_ARGS=$(patsubst %,-I%, $(INCLUDEDIRS))

EGINCDIR_ARGS=$(patsubst %,-I%, $(EGINC))


%.o:%.c
	$(CC) $(CFLAGS) -c $(INCLUDEDIR_ARGS) $(DEFINE_ARGS) -o $@  $<

%.o:%.cpp
	$(CPP) $(CPPFLAGS) -c $(INCLUDEDIR_ARGS) $(DEFINE_ARGS) -o $@  $<

all: $(PROGRAM) 

lib:
	$(CPP) -shared -Wl,-soname, $(SONAME) -o $(SONAME).$(SOSUBREV) $(SOMODULES)


example: $(EG)


$(PROGRAM):$(OBJECTS) 
	$(CPP) $(CPPFLAGS) -o $(PROGRAM)  $(OBJECTS)  $(LIBS) 



$(OBJECTS): Makefile version.h


$(EG):$(EGOBJECTS)
	$(CPP) $(CPPFLAGS) -o $(PROGRAM)  $(OBJECTS) $(EGOBJECTS)  $(LIBS)


version.h: 
	@echo "const char *appversionstring=\"$(APPVERSION)\";" >$@


test:
	@echo $(CPP)
	@echo $(CPPFLAGS)
	@echo $(DEPS)
	@echo $(DEFINES)
	@echo $(OBJECTS)
	@echo $(CPP_SOURCES)
dox doc docs:
	@doxygen

clean: 
	rm -f $(OBJECTS) $(PROGRAM) $(DEPS) version.h *.db
	


-include $(DEPS)
