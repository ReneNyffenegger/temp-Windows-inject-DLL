#include <windows.h>
#include "testProg.h"

// __declspec(dllimport) char* var;

DWORD DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
//  var = "the number is 42";

}

__declspec(dllexport) void doSomething() {

//  char* var = GetProcAddress(
    MessageBox(0, "doSomething", "doSomething", 0);

}
