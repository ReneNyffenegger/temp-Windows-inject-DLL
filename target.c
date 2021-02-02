//
//    https://github.com/KooroshRZ/Windows-DLL-Injector/blob/master/TargetProgram/src/Target.cpp
//

#include <stdio.h>
#include <Windows.h>


int main() {

	/*LPCSTR DllPath = "C:\\Users\\k.rajabzadeh\\source\\repos\\SimpleDllInjector\\DLLFileBuild\\bin\Win32\\Debug\\DLLFile.dll";
	HINSTANCE h = LoadLibrary(DllPath);
	
	if (h) {
		printf("success, 0x%x\n", GetLastError());
		LogMessage("Helllllllloooooooooooo from DLL");
	}
	else {
		printf("failure, 0x%x\n", GetLastError());
	}*/

  printf("process id of target: %d\n", GetCurrentProcessId());
  int cnt=0;
	while (++cnt) {
		printf("%d\n", cnt);
		SleepEx(1000, 1);// Call Sleep function with alertable flag to put the thread in suspended state for using in QueueUserAPC method
	}

}
