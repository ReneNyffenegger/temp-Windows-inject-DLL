//
//       https://github.com/ReneNyffenegger/temp-Windows-inject-DLL
//

#include <windows.h>

#include <stdio.h>

#include "win-proc.h"




void injectDll(DWORD procId, char* modulePath) {

     HANDLE process = procIdToHandle(procId);

     if (!process) {
        printf("could not open process\n");
        return;
     }

//   size_t Size  = (Path.length() + 1) * sizeof(wchar_t);
     void* mem = VirtualAllocEx(process, NULL, strlen(modulePath+1), MEM_COMMIT, PAGE_READWRITE);

     if (!mem) {
        printf("could not alloc mem\n");
        return;
     }

     if (!WriteProcessMemory(
        process,
        mem,
        modulePath,
        strlen(modulePath),
        NULL)
      ) {
        printf("could not write process memory.\n");
        return;
     }

     HMODULE hKernel32 = GetModuleHandle("Kernel32");

     if (!hKernel32) {
        printf("! hKernel32.\n");
     }

     PTHREAD_START_ROUTINE addrLoadLibrary = 
    (PTHREAD_START_ROUTINE) GetProcAddress(hKernel32, "LoadLibraryA");


     if (!addrLoadLibrary) {
        printf("! addrLoadLibrary\n");
     }

     HANDLE thread = CreateRemoteThread(
        process,
        NULL           , // lpThreadAttributes
        0              , // dwStackSize
        addrLoadLibrary, // lpStartAddress
        mem            , // lpParameter   (mem contains name of library)
        0              , // dwCreationFlags
        NULL             // lpThreadId
     );

	   WaitForSingleObject(thread, INFINITE);

  // Get thread exit code
      DWORD ExitCode;
      if (!GetExitCodeThread(thread, &ExitCode)) {
        printf("! get exit code\n");
        return;
      }

  // Check LoadLibrary succeeded and returned a module base
     if(!ExitCode) {
        printf("! call to LoadLibraryW failed\n");
        return;
     }
}


int main(int argc, char* argv[]) {

    DWORD procID = 0;

    procID = procNameToId(argv[1]);

    if (!procID) {
       printf("Process not found\n");
       return 1;
    }
    printf("ProcID %d\n", procID);


    injectDll(procID, argv[2]);

}
