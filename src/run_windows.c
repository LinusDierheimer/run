#include "run.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int run(RunOptions* runOptions)
{

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));


	if (!CreateProcess(
		"cmd",               //program
		runOptions->command, //command line
		NULL,                //Process Handle
		NULL,                //Thread Handle
		FALSE,               //Handle Inheritance
		0,                   //Creation Flags
		NULL,                //inherit environment block
		NULL,                //inherit starting directory 
		&si,                 //startup info
		&pi                  //process info
	)) {
		return -1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}