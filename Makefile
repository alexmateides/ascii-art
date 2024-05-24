default_target:all


CXX = g++
CXXFLAGS = -std=c++20 -O2 -g
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer -Wno-format-security
ASAN_LIBS  = -static-libasan
LD = g++ -Wall -Wextra -pedantic
LDFLAGS =-lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libasan
SRC=$(wildcard ./src/*.cpp)
HDR=$(wildcard ./src/*.h)
OBJ=$(patsubst %.cpp, %.o, $(SRC))
#LIBS=`sdl2-config --libs` -L\\wsl.localhost\Mint\usr\lib -lSDL2_image -lSDL2_ttf -lSDL2_gfx
LIBS=`sdl2-config --libs` -L/usr/include/SDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx

all: clean compile run clean

run:
	./mateial1 "./assets/config/config1.json"

doc:

compile: $(OBJ)
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(LDFLAGS) $^ $(LIBS) $(ASAN_LIBS) -o mateial1
	#$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o mateial1

clean:
	#rm -f .\src\*.o mateial1
	rm -f ./src/*.o mateial1 out.txt

.PHONY: clean
