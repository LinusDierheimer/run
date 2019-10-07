#include "run.h"
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma warning(disable : 4996)

int run(RunOptions* runOptions)
{

	if (runOptions->showDebugInfos)
		puts("[debug] using implementation: win32");

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = NULL;

	if (runOptions->keepIO)
	{
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	}

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	char buf[4096];
	buf[0] = '\0';

	strcat(buf, "cmd /c ");
	strcat(buf, runOptions->command);

	if (runOptions->showDebugInfos)
		printf("[debug] using windows specific command: %s\n", buf);

	if (!CreateProcess(
		NULL,               //program, identify by command line
		buf,                 //command line
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

	if (runOptions->showDebugInfos)
	{
		printf("[debug] using new process id: %lu\n", pi.dwProcessId);
		printf("[debug] using new thread id: %lu\n", pi.dwThreadId);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}