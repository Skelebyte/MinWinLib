default: linux win

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
	$(warning You are on windows! Please use WSL (Windows Subsystem for Linux) and run this command again!)
else
	gcc -o  bin/MyProgram.x86_64 src/Main.c src/glad/*.c -lX11 -lXfixes -lEGL
endif

win:
ifeq ($(OS), Windows_NT)
	gcc -o bin/MyProgram.exe src/Main.c src/glad/*.c -luser32 -lgdi32
else
	x86_64-w64-mingw32-gcc -o bin/MyProgram.exe src/Main.c src/glad/*.c -luser32 -lgdi32
endif

lib_so:
	gcc -fPIC -shared -o libMWL.so src/MinWinLib.c -lX11 -lXfixes

lib_dll:
	x86_64-w64-mingw32-gcc -shared -o libMWL.dll src/MinWinLib.c -DBUILD_DLL -luser32 -lgdi32 -mwindows

all: linux win

all_lib: lib_dll lib_so