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

SQLITECORE_SOURCES = 	sqlite-core/Sources/dbmanager.cpp \
						sqlite-core/Sources/db.cpp \
						sqlite-core/Sources/table.cpp \
						sqlite-core/Sources/field.cpp \

SOURCES = 	main.cpp \
			$(SQLITECORE_SOURCES) \

OBJECTS = 	$(addprefix $(BUILDIR)/, \
			main.o \
			dbmanager.o \
			db.o \
			table.o \
			field.o)

all: $(OBJECTS)
	$(CXX) -o $(BUILDIR)/$(TARGET) $(OBJECTS)

$(BUILDIR)/main.o: main.cpp
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) main.cpp -o $@

$(BUILDIR)/dbmanager.o: sqlite-core/Sources/dbmanager.cpp sqlite-core/Headers/dbmanager.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) sqlite-core/Sources/dbmanager.cpp -o $@	

$(BUILDIR)/db.o: sqlite-core/Sources/db.cpp sqlite-core/Headers/db.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) sqlite-core/Sources/db.cpp -o $@	

$(BUILDIR)/table.o: sqlite-core/Sources/table.cpp sqlite-core/Headers/table.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) sqlite-core/Sources/table.cpp -o $@

$(BUILDIR)/field.o: sqlite-core/Sources/field.cpp sqlite-core/Headers/field.h 
	$(CXX) $(DEBUG) $(CFLAGS) $(INCLUDE) sqlite-core/Sources/field.cpp -o $@

clean:
	$(RM) $(BUILDIR)/*
