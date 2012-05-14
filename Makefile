INCLUDE=-Isrc `sdl-config --cflags`
LIBS=`sdl-config --libs`
#-L../gamepad/Linux-Debug -lgamepad -lpthread -lGL -lfreetype

CFLAGS=-fPIE

SOURCES=\
src/main.cpp

OBJECTS=$(patsubst src/%.cpp,Linux-Debug/%.o,$(SOURCES))

all: Linux-Debug/reztest

Linux-Debug/reztest: $(OBJECTS)
	g++ -o $@ $^ $(LIBS)

Linux-Debug/%.o: src/%.cpp
	test -d Linux-Debug || mkdir -p Linux-Debug
	g++ $(CFLAGS) -I $(INCLUDE) -o $@ -c $<

clean:
	rm -rf Linux-Debug
