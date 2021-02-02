#include <Windows.h>
#include <tlhelp32.h>

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

HANDLE procIdToHandle(DWORD procId) {

     HANDLE process = OpenProcess (
         PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
         PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
         PROCESS_VM_WRITE,             // For WriteProcessMemory
         FALSE,
         procId
     );


   return process;
}
