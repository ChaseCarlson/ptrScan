// ptrScan.cpp : Defines the entry point for the application.
//

#include "ptrScan.h"
#include <Windows.h>
#include <vector>

std::vector<MEMORY_BASIC_INFORMATION> proc_get_blocks(HANDLE hProcess)
{
	std::vector<MEMORY_BASIC_INFORMATION> blocks;
	char* lastAddress = 0;
	size_t lastSize = 0;
	while (1)
	{
		MEMORY_BASIC_INFORMATION lpBuffer;
		if (!VirtualQueryEx(hProcess, lastAddress + lastSize, &lpBuffer, sizeof(lpBuffer)))
			break;
		lastAddress = (char*)lpBuffer.BaseAddress;
		lastSize = lpBuffer.RegionSize;
		blocks.push_back(lpBuffer);
	}

	return blocks;
}

// BUILD FOR THE CORRECT ARCHITECHTURE...
// IF YOUR TARGET IS X86, BUILD FOR X86! 

int main()
{
	int test = 100;

	HWND hwnd = FindWindowA(NULL, "");
	if (!hwnd)
	{
		std::cout << "Can't find window" << std::endl;
		return 1;
	}
	std::cout << "Window found" << std::endl;
	
	DWORD processId;
	GetWindowThreadProcessId(hwnd, &processId);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	if (!processId)
	{
		std::cout << "Process couldn't be acquired" << std::endl;
		return 1;
	}

	std::cout << "Starting memory filtration" << std::endl;

	auto blocks = proc_get_blocks(hProcess);

	void* empty = malloc(10000);
	int i = 0;
	for (MEMORY_BASIC_INFORMATION block : blocks)
	{
		WriteProcessMemory(hProcess, block.BaseAddress, empty, block.RegionSize, 0);
		i++;
	}




	return 0;
}
