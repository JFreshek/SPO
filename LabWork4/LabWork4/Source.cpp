#include <iostream>
#include <windows.h>
#include <stack>
#include <string>
#include <conio.h>

using namespace std;

CRITICAL_SECTION csStringWriter;

DWORD WINAPI writeString(PVOID pvParam);
void menu();

int main() {
	InitializeCriticalSection(&csStringWriter);
	HANDLE thread;
	stack<HANDLE> threads;
	bool quit = false;

	menu();
	while (!quit) {
		switch (_getch()) {
		case '+':
			thread = CreateThread(NULL,	0, writeString, NULL, 0, NULL);
			if (thread) {
				threads.push(thread);
			}
			break;
		case '-':
			if (!threads.empty()) {
				EnterCriticalSection(&csStringWriter);
				TerminateThread(threads.top(), EXIT_SUCCESS);
				threads.pop();
				LeaveCriticalSection(&csStringWriter);
			}
			else {
				cout << "No treads" << endl;
			}
			break;
		case 'q':
			EnterCriticalSection(&csStringWriter);
			while (!threads.empty()) {
				TerminateThread(threads.top(), EXIT_SUCCESS);
				threads.pop();
			}
			LeaveCriticalSection(&csStringWriter);
			quit = true;
			break;
		}
	}
	DeleteCriticalSection(&csStringWriter);
	return 0;
}

void menu() {
	cout << "Enter:" << endl << "'+' to add thread." << endl;
	cout << "'-' to delete thread." << endl << "'q' to quit." << endl;
	return;
}

DWORD WINAPI writeString(PVOID pvParam) {
	string msg = "Thread id: ";
	msg += to_string(GetCurrentThreadId());
	while(1) {
		if (TryEnterCriticalSection(&csStringWriter)) {
			for (auto& c : msg) {
				printf("%c", c);
				Sleep(5);
			}
			printf("\n");
			LeaveCriticalSection(&csStringWriter);
		}	
		Sleep(10);
	}
}


