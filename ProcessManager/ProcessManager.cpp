#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tchar.h>

#pragma comment(lib, "psapi.lib")

void PrintProcessNameAndID(DWORD processID) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (NULL != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

    CloseHandle(hProcess);
}

int main() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return 1;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    _tprintf(TEXT("Running processes:\n"));
    for (unsigned int i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }

    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

    std::cout << "\nSystem usage:\n";
    std::cout << "Total physical memory: " << totalPhysMem / 1024 / 1024 << " MB\n";
    std::cout << "Physical memory used: " << physMemUsed / 1024 / 1024 << " MB\n";
    std::cout << "Total virtual memory: " << totalVirtualMem / 1024 / 1024 << " MB\n";
    std::cout << "Virtual memory used: " << virtualMemUsed / 1024 / 1024 << " MB\n";

    return 0;
}
