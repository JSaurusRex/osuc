CC=gcc
CFLAGS=-Wno-write-strings -ldl -lm -lpthread -pthread -lGL $(shell pkg-config --cflags glfw3)
CLIBS= $(shell pkg-config --libs glfw3)


WINCFLAGS=-Iinclude -Llib-mingw -L"C:\Program Files (x86)\CodeBlocks\MinGW\lib"
WINCLIBS=-lglfw3 -lopengl32 -lgdi32

OUTPUT=osuc
WINOUTPUT=$(OUTPUT).exe

FILES=main.c glad.c

Linux: $(FILES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(FILES) $(CLIBS)

Windows: $(FILES)
	$(CC)  -o $(WINOUTPUT) $(FILES) $(WINCFLAGS) $(WINCLIBS) 