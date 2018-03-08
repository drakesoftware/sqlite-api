CC = g++

CFLAGS = -c

LFLAGS = -o 

DEBUG = -g

TARGET = telsql

SQLITECORE_HEADERS = sqlite-core/Headers/dbmanager.h \

SQLITECORE_SOURCES = main.cpp sqlite-core/Sources/dbmanager.cpp \

OBJS = main.o dbmanager.o \

all: $(OBJS)
	$(CC) $(DEBUG) $(LFLAGS) $(TARGET) $(OBJS) 

objects: $(SQLITECORE_HEADERS) $(SQLITECORE_SOURCES)
	$(CC) $(DEBUG) $(CFLAGS) $(SQLITECORE_SOURCES) 

clean: 
	rm -rf $(OBJS) $(TARGET)

