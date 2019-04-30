#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 1) {
		cout << "Data transfer error!" << endl;
		return 1;
	}
	LPSTR eventName = argv[0];
	HANDLE event = CreateEvent(NULL, FALSE, TRUE, eventName);
	DWORD pid = GetCurrentProcessId();
	string msg = "Process with id: ";
	msg += to_string(pid);
	while(1) {

		WaitForSingleObject(event, INFINITE);
		for (auto& c : msg) {
			printf("%c", c);
			Sleep(1000/200);
		}
		printf("\n");
		SetEvent(event);
	}
	return 0;
}