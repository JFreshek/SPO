#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	string str;
	DWORD numberOfBytesWritten = 0;
	char input;
	LPCSTR semaphoreName = const_cast<char*>(argv[1]);
	LPCSTR pipeName = const_cast<char*>(argv[2]);
	DWORD numberOfBytesOfTheQuiteCommand = str.size();
	HANDLE semaphore = CreateSemaphore(NULL, 0, 1, semaphoreName);
	if (!semaphore) {
		cout << "Error! Can't create semaphore!" << endl;
		return -1;
	}
	HANDLE pipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 512, 512,
		5000, NULL);
	if (pipe == INVALID_HANDLE_VALUE) {
		cout << "Error! Can't create pipe!" << endl;
		CloseHandle(semaphore);
		return -1;
	}
	cout << "Client not connected!" << endl;
	if (!ConnectNamedPipe(pipe, NULL)) {
		cout << "Error! Can't connect to client!" << endl;
		CloseHandle(pipe);
		CloseHandle(semaphore);
		return -1;
	}
	cout << "Connected!" << endl;
	cout << "('/exit' to exit)" << endl;
	while (1) {
		cout << "Enter your message:" << endl;	
		numberOfBytesWritten = 0;
		cin >> str;
		WriteFile(pipe, str.c_str(), str.size(), &numberOfBytesWritten, NULL);
		ReleaseSemaphore(semaphore, 1, NULL);
		system("CLS");
		if (str == "/exit") break;
	}
	if (!DisconnectNamedPipe(pipe)) {
		cout << "Error! Can't disconnected!" << endl;
	}
	CloseHandle(pipe);
	CloseHandle(semaphore);
	return 0;
}