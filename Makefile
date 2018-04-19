

INCLUDEDIRS= sqlite-core/Headers

EGINC = example/uat example/uat/adsb_uat example/utilities

LIBS=   -l sqlite3 -lreadline -lzmq

VPATH =  sqlite-core/Sources
EGVPATH = example/uat example/uat/adsb_uat

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


PROGRAM= telsql


C_SOURCES=

CPP_SOURCES= 	main.cpp \
		dbmanager.cpp \
		entity.cpp \
		table.cpp \
		db.cpp \
		columns.cpp \
		appdata.cpp

EG_CPP_SOURCES = 	SelectedAltitude.cpp \
    			CapabilityCodes.cpp \
    			AVSize.cpp \
    			icaocodes.cpp \
    			BaroSetting.cpp \
			Callsign.cpp \
			VerticalVelocity.cpp \
    			HorizontalVelocity.cpp \
    			Latitude.cpp \
    			GroundUplink.cpp \
    			ADSB_Fields.cpp \
    			OperationalModes.cpp \
    			Altitude.cpp \
    			SelectedHeading.cpp \
    			Longitude.cpp \
    			UATData.cpp 

C_OBJECTS=$(patsubst %.c,%.o, $(C_SOURCES))

CPP_OBJECTS=$(patsubst %.cpp,%.o, $(CPP_SOURCES))

EG_CPP_OBJECTS = $(patsubst %.cpp,%.o, $(EG_CPP_SOURCES))

CFLAGS=$(DEBUG)  -MMD $(INCPATH)  $(DEFINE_ARGS) -lpthread

CPPFLAGS=$(DEBUG) -MMD $(INCPATH) -Wno-pmf-conversions -std=c++11 $(DEFINE_ARGS) -lpthread



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
