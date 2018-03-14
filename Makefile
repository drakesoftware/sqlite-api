CFLAGS = -c -std=c++11

LDFLAGS = -lsqlite3

DEBUG = -g

TARGET = telsql

BUILDIR = build

INCLUDE = -Isqlite-core/Headers

SQLITECORE_HEADERS = 	sqlite-core/Headers/dbmanager.h \
						sqlite-core/Headers/db.h \
						sqlite-core/Headers/table.h \
						sqlite-core/Headers/entity.h \
						sqlite-core/Headers/columns.h \

SQLITECORE_SOURCES = 	sqlite-core/Sources/dbmanager.cpp \
						sqlite-core/Sources/db.cpp \
						sqlite-core/Sources/table.cpp \
						sqlite-core/Sources/entity.cpp \
						sqlite-core/Sources/columns.cpp \

SOURCES = 	main.cpp \
			$(SQLITECORE_SOURCES) \

OBJECTS = 	$(addprefix $(BUILDIR)/, \
			main.o \
			dbmanager.o \
			db.o \
			table.o \
			entity.o \
			columns.o)

all: $(OBJECTS)
	$(CXX) -o $(BUILDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS)

$(BUILDIR)/dbmanager.o: sqlite-core/Sources/dbmanager.cpp sqlite-core/Headers/dbmanager.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/dbmanager.cpp	

$(BUILDIR)/db.o: sqlite-core/Sources/db.cpp sqlite-core/Headers/db.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/db.cpp	

$(BUILDIR)/table.o: sqlite-core/Sources/table.cpp sqlite-core/Headers/table.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/table.cpp

$(BUILDIR)/entity.o: sqlite-core/Sources/entity.cpp sqlite-core/Headers/entity.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/entity.cpp

$(BUILDIR)/columns.o: sqlite-core/Sources/columns.cpp sqlite-core/Headers/columns.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/columns.cpp

$(BUILDIR)/main.o: main.cpp
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ main.cpp

clean:
	$(RM) $(BUILDIR)/*
