//
//       https://github.com/ReneNyffenegger/temp-Windows-inject-DLL
//

#include <windows.h>
#include <tlhelp32.h>

#include <stdio.h>


DWORD procNameToId(char* name) {

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snap == INVALID_HANDLE_VALUE) {
       printf("CreateToolhelp32Snapshot -> invalid\n");
    }

    PROCESSENTRY32 ProcEntry = { sizeof(ProcEntry) };

    BOOL MoreMods = Process32First(snap, &ProcEntry);
    for (; MoreMods; MoreMods = Process32Next(snap, &ProcEntry)) {
     
//     printf("%s\n", ProcEntry.szExeFile);

       if (! strncmp(name, ProcEntry.szExeFile, 255)) {
//        printf("Found %s\n", ProcEntry.szExeFile);
          return ProcEntry.th32ProcessID; 
       }

//      std::tstring CurrentProcess(ProcEntry.szExeFile);
//          CurrentProcess = toLower(CurrentProcess);
//      Found = (CurrentProcess == Name);
//      if (Found) break;
    }

    return 0;

}


void injectDll(DWORD procId, char* modulePath) {

     HANDLE process = OpenProcess (
         PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
         PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
         PROCESS_VM_WRITE,             // For WriteProcessMemory
         FALSE,
         procId
     );

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
    (PTHREAD_START_ROUTINE) GetProcAddress(hKernel32, "LoadLibraryW");


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
      if (!GetExitCodeThread(thread,&ExitCode)) {
        printf("! get exit code\n");
        return;
      }

  // Check LoadLibrary succeeded and returned a module base
     if(!ExitCode) {
        printf("! call to LoadLibraryW failed\n");
        return;
     }

}


int main() {

    DWORD procID = 0;

    procID = procNameToId("notepad.exe");

    if (!procID) {
       printf("Process not found\n");
       return 1;
    }
    printf("ProcID %d\n", procID);


   

}
