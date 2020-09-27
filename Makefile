CC=gcc

#CFLAGS=-Wno-write-strings -ldl -lm -lpthread -pthread -lGL $(shell pkg-config --cflags glfw3) -lsndfile -lportaudio
#CLIBS= $(shell pkg-config --libs glfw3)
CFLAGS=-ggdb -g -O0 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#CLIBS= -I/opt/raylib/src -L/opt/raylib/release/libs/linux

#WINCFLAGS=-Iinclude -Llib-mingw -L"C:\Program Files (x86)\CodeBlocks\MinGW\lib"
#WINCLIBS=-lglfw3 -lopengl32 -lgdi32 -lsndfile -lportaudio

OUTPUT=osuc
WINOUTPUT=$(OUTPUT).exe

FILES=main.c 
#glad.c

Linux: $(FILES)
	$(CC)  -o $(OUTPUT) $(FILES) $(CLIBS) $(CFLAGS)

Windows: $(FILES)
	$(CC)  -o $(WINOUTPUT) $(FILES) $(WINCFLAGS) $(WINCLIBS) 
