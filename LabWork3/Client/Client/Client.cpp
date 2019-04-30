#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	DWORD numberOfBytesRead = 0;
	char buffer[512];
	LPCSTR semaphoreName = const_cast<char*>(argv[1]);
	LPCSTR pipeName = const_cast<char*>(argv[2]);
	HANDLE semaphore = OpenSemaphore(SYNCHRONIZE, FALSE, semaphoreName);
	if (!semaphore) {
		cout << "Error! Can't open semaphore!" << endl;
		cin.get();
		return -1;
	}
	HANDLE pipe = CreateFile(pipeName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		cout << "Error! Can't open pipe!" << endl;
		CloseHandle(semaphore);
		return -1;
	}
	cout << "Connected!" << endl;
	while (1) {
		if (!WaitForSingleObject(semaphore, INFINITE)) {
			numberOfBytesRead = 0;
			ReadFile(pipe, buffer, 512, &numberOfBytesRead, NULL);
			buffer[numberOfBytesRead] = '\0';
			if (!strcmp(buffer,"/exit")) break;
			cout << buffer << endl;
		}
	}
	CloseHandle(pipe);
	CloseHandle(semaphore);
	return 0;
}