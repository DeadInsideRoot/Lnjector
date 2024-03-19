#include "include.h"

int getDLLpath(char* dll) {
	cout << "Please enter the path to your DLL file: ";
	cin >> dll;
	return 1;
}

int getPID(int* PID) {
	cout << "Please enter the PID to your target process: ";
	cin >> *PID;
	return 1;
}

int getProc(HANDLE*handleToProc, DWORD pid) {
	*handleToProc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	DWORD dwlasterror = GetLastError();

	if (*handleToProc == NULL) {
		std::cout << "Unable to open process.\n";
		return -1;
	}
	else { 
		std::cout << "Process Opened.\n";
		return 1;
	}
}

int InjectDLL(DWORD PID, char* dll) {
	HANDLE handleToProc;
	LPVOID LoadLibAddr;
	LPVOID baseAddr;
	HANDLE remTread;

	// Получить длину dll
	int dllLength = strlen(dll) + 1;

	//Получаем обработку процесса
	if (getProc(&handleToProc, PID) < 0)
		return -1;

	// Загрузить kerne132 библиотеку
	LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

	if (!LoadLibAddr)
		return -1;

	baseAddr = VirtualAllocEx(handleToProc, NULL, dllLength, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (!baseAddr)
		return -1;

	//Записываем путь к DLL
	if (!WriteProcessMemory(handleToProc, baseAddr, dll, dllLength, NULL))
		return -1;

	// Создаем удаленный поток
	remThread = CreateRemoteThread(handleToProc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, baseAddr, 0, NULL);

	if (!remThread)
		return -1;

	WaitForSingleObject(remThread, INFINITE);

	VirtualFreeEx(handleToProc, baseAddr, dllLength, MEM_RELEASE);

	//Закрываем обработчик
	if (CloseHandle(remThread) == 0) {
		:cout << "Failed to close handle to remote thread";
		return -1;
	}
	if (CloseHandle(handleToProc) == 0)	{
		cout << "Failed to close handle to target process.\n";
		return -1;
	}
}

int main() {
	SetConsoleTitle("Injector 2.0 by ZXC");

	int PID = -1;
	char* dll = new char[255];

	getDLLpath(dll);
	getPID(&PID);
	InjectDLL(PID, dll);

	system("pause");
	return 0;
}