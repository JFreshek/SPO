#include <iostream>
#include <stack>
#include <Windows.h>
#include <string>
#include <conio.h>

using namespace std;

bool createChild(PROCESS_INFORMATION* processInfo, LPSTR eventName, STARTUPINFO startUpInfo, LPCSTR childPath);
void deleteChild(PROCESS_INFORMATION& processInfo);
void menu();

int main(int argc, char* argv[]) {
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startUpInfo;
	ZeroMemory(&startUpInfo, sizeof(startUpInfo));
	startUpInfo.cb = sizeof(startUpInfo);
	stack<PROCESS_INFORMATION> childsProcesses;
	LPSTR eventName = const_cast<char*>("myEvent");
	HANDLE event = CreateEvent(NULL, FALSE, TRUE, eventName);
	//HANDLE event = CreateEvent(NULL, TRUE, FALSE, eventName);
	LPCSTR childPath = argv[1];
	bool quit = false;
	menu();
	while (!quit) {
		switch (_getch()) {
		case '+':
			if (createChild(&processInfo, eventName, startUpInfo, childPath)) {
				childsProcesses.push(processInfo);
			}
			break;
		case '-':
			WaitForSingleObject(event, INFINITE);
			if (!childsProcesses.empty()) {
				deleteChild(childsProcesses.top());
				childsProcesses.pop();
			}
			else {
				cout << "No processes" << endl;
			}
			SetEvent(event);
			break;
		case 'q':
			while (!childsProcesses.empty()) {
				deleteChild(childsProcesses.top());
				childsProcesses.pop();
			}
			quit = true;
			break;
		}
	}
	CloseHandle(event);
	return 0;
}

void menu() {
	cout << "Enter:" << endl << "'+' to add child process." << endl;
	cout << "'-' to delete child process." << endl << "'q' to quit." << endl;
	return;
}

void deleteChild(PROCESS_INFORMATION& processInfo) {
	TerminateProcess(processInfo.hProcess, 0);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}

bool createChild(PROCESS_INFORMATION* processInfo, LPSTR eventName, STARTUPINFO startUpInfo, LPCSTR childPath) {
	if (!CreateProcess(childPath, eventName, NULL, NULL, FALSE, NULL, NULL, NULL, &startUpInfo, processInfo)) {
		cout << "Error! Can't create process!" << endl;
		return false;
	}
	return true;
}