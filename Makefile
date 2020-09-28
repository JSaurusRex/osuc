CC=gcc

#CFLAGS=-Wno-write-strings -ldl -lm -lpthread -pthread -lGL $(shell pkg-config --cflags glfw3) -lsndfile -lportaudio
#CLIBS= $(shell pkg-config --libs glfw3)
CFLAGS=-ggdb -g -O0 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#CLIBS= -I/opt/raylib/src -L/opt/raylib/release/libs/linux

WINCFLAGS=-Iinclude -Llib-mingw -L"C:\Program Files (x86)\CodeBlocks\MinGW\lib" lib-mingw/libraylib_static.a lib-mingw/libglfw3.a 
WINCLIBS=-Wl,-Bdynamic -lopengl32 -lgdi32 -Wl,-Bstatic -lwinmm \
		-std=c99 -Wl,-allow-multiple-definition -Wl,--subsystem,windows
#/usr/lib/gcc/i686-w64-mingw32/10.2.0/../../../../i686-w64-mingw32/lib/../lib/libwinmm.a /usr/lib/gcc/i686-w64-mingw32/10.2.0/../../../../i686-w64-mingw32/lib/../lib/libwinpthread.a -lopengl32 -lgdi32
#-lglfw3
#-lraylib_static
#WINCLIBS=-lglfw3 -lopengl32 -lgdi32 -lraylib_static

OUTPUT=osuc
WINOUTPUT=$(OUTPUT).exe

FILES=main.c 
#glad.c

Linux: $(FILES)
	$(CC)  -o $(OUTPUT) $(FILES) $(CLIBS) $(CFLAGS)

Windows: $(FILES)
	i686-w64-mingw32-gcc -o $(WINOUTPUT) $(FILES) $(WINCFLAGS) $(WINCLIBS) 
