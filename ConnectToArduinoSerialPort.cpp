#include <atlstr.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
CString ComName;
HANDLE hSerial;
DCB params;
int buffSize = 5000;
DWORD bytes_written, total_bytes_written = 0;
using namespace std;
LPCSTR port;


CString SelectComPort(){

	TCHAR lpTargetPath[5000];
	DWORD test;
	bool gotPort = false;

	for (int i = 255; i > 0; i--)
	{
		CString str;
		str.Format(_T("%d"), i);
		ComName = CString("COM") + CString(str);

		test = QueryDosDevice(ComName, (LPSTR)lpTargetPath, buffSize);


		if (test != 0)
		{
			//cout << ComName << endl;
			gotPort = 1; // found port
			break;
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			buffSize = buffSize * 2;
			lpTargetPath[buffSize];
			continue;
		}

	}

	if (!gotPort) // if not port
		cout << "No Ports Found" << endl;
	return ComName;
}
void sendData(char bytes_to_send[10]) {
	//WriteFile(hSerial, bytes_to_send, 10, NULL, NULL);

	if (!WriteFile(hSerial, bytes_to_send, 10, &bytes_written, NULL)) {
		cout << "Error" << endl;
		CloseHandle(hSerial);
	}

}
bool connectToArduino() {

	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };


	cout << "Opening Serial Port..." << endl;
	hSerial = CreateFile(
		port,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hSerial == INVALID_HANDLE_VALUE) {
		cout << "Error\n" << endl;
		return false;
	}
	else {
		cout << "Connected!" << endl;
	}
	params.DCBlength = sizeof(params);

	if (GetCommState(hSerial, &params) == 0) {
		cout << "Error getting Device State" << endl;
		CloseHandle(hSerial);
		return true;
	}
	else {
		cout << "Device state received." << endl;
	}

	if (SetCommState(hSerial, &dcbSerialParams) == 0) {
		cout << "Error getting Device State" << endl;
		CloseHandle(hSerial);
		return true;
	}
	else { cout << "Device parameters succesfully set." << endl; }

}
int main() {
	CString c1 = SelectComPort();
	CString c = "\\\\.\\" + c1;
	port = c;
	connectToArduino();
}