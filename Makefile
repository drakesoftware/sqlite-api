CFLAGS = -c

LDFLAGS = 

DEBUG = -g

TARGET = telsql

BUILDIR = build

SQLITECORE_HEADERS = sqlite-core/Sources/dbmanager.h \

SQLITECORE_SOURCES = sqlite-core/Sources/dbmanager.cpp \

SOURCES = 	main.cpp \
			$(SQLITECORE_SOURCES) \

OBJECTS = 	main.o \
			dbmanager.o \

all: $(OBJECTS)
	$(CXX) $(BUILDIR)/$< -o $(BUILDIR)/$(TARGET)

$(OBJECTS): $(SOURCES)
	$(CXX) $(DEBUG) $(CFLAGS) $< -o $(BUILDIR)/$@

clean:
	$(RM) $(BUILDIR)/*
