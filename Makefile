all: dllInjector.exe


dllInjector.exe: injectLib.c
	gcc injectLib.c -o dllInjector.exe
