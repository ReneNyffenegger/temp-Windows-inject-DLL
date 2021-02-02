all: payload.dll dllInjector.exe testProg.exe testLib.dll target.exe

target.exe: target.c
	gcc target.c -o target.exe

payload.dll: payload.c
	gcc -shared payload.c

dllInjector.exe: injectLib.c win-proc.c win-proc.h
	gcc injectLib.c win-proc.c -o dllInjector.exe

testProg.exe: testProg.c testProg.h win-proc.h
	gcc testProg.c -o testProg.exe

testLib.dll: testLib.c testProg.h
	gcc -shared testLib.c -o testLib.dll

