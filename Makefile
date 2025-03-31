run_win:
ifeq ($(OS), Windows_NT)
	./bin/MyProgram.exe
else
	wine ./bin/MyProgram.exe
endif

run_linux:
ifeq ($(OS), Windows_NT)
	echo "Can't run"
else
	./bin/MyProgram.x86_64
endif


linux:
ifeq ($(OS), Windows_NT)
	wsl
	gcc -o  bin/MyProgram.x86_64 src/Main.c -lX11
	exit
else
	gcc -o  bin/MyProgram.x86_64 src/Main.c -lX11
endif

wayland:
	gcc -o server src/WaylandTest.c -lwayland-server

win:
ifeq ($(OS), Windows_NT)
	gcc -o bin/MyProgram.exe src/Main.c -luser32 -mwindows -lgdi32
else

	x86_64-w64-mingw32-gcc -o bin/MyProgram.exe src/Main.c -luser32
endif

all: linux win
