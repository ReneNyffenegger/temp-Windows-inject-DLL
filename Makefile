all: dllInjector.exe testProg.exe testLib.dll


dllInjector.exe: injectLib.c win-proc.c win-proc.h
	gcc injectLib.c win-proc.c -o dllInjector.exe

testProg.exe: testProg.c testProg.h win-proc.h
	gcc testProg.c -o testProg.exe

testLib.dll: testLib.c testProg.h
	gcc -shared testLib.c -o testLib.dll
