CFLAGS = -c -std=c++11

LDFLAGS = 

DEBUG = -g

TARGET = telsql

BUILDIR = build

INCLUDE = -Isqlite-core/Headers

SQLITECORE_HEADERS = 	sqlite-core/Headers/dbmanager.h \
						sqlite-core/Headers/db.h \
						sqlite-core/Headers/table.h \
						sqlite-core/Headers/field.h \
						sqlite-core/Headers/entity.h \

SQLITECORE_SOURCES = 	sqlite-core/Sources/dbmanager.cpp \
						sqlite-core/Sources/db.cpp \
						sqlite-core/Sources/table.cpp \
						sqlite-core/Sources/field.cpp \
						sqlite-core/Sources/entity.cpp \

SOURCES = 	main.cpp \
			$(SQLITECORE_SOURCES) \

OBJECTS = 	$(addprefix $(BUILDIR)/, \
			main.o \
			dbmanager.o \
			db.o \
			table.o \
			field.o \
			entity.o)

all: $(OBJECTS)
	$(CXX) -o $(BUILDIR)/$(TARGET) $(OBJECTS)

$(BUILDIR)/dbmanager.o: sqlite-core/Sources/dbmanager.cpp sqlite-core/Headers/dbmanager.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/dbmanager.cpp	

$(BUILDIR)/db.o: sqlite-core/Sources/db.cpp sqlite-core/Headers/db.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/db.cpp	

$(BUILDIR)/table.o: sqlite-core/Sources/table.cpp sqlite-core/Headers/table.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/table.cpp

$(BUILDIR)/field.o: sqlite-core/Sources/field.cpp sqlite-core/Headers/field.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/field.cpp

$(BUILDIR)/entity.o: sqlite-core/Sources/entity.cpp sqlite-core/Headers/entity.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ sqlite-core/Sources/entity.cpp

$(BUILDIR)/main.o: main.cpp
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) -o $@ main.cpp

clean:
	$(RM) $(BUILDIR)/*
