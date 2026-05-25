#include <iostream>
#include <string>
#include <windows.h>
#include "DLLInjector.h"
#include "ProcessHelper.h"
#include "InjectionMethods.h"

int wmain()
{
    SetConsoleTitleW(L"CInjector - Advanced DLL Injector");
    std::wcout << L"=== CInjector - Advanced DLL Injector ===\n\n";

    std::wstring processName, dllName;
    int methodChoice;

    std::wcout << L"Enter process name (e.g. cs2.exe): ";
    std::getline(std::wcin, processName);

    if (processName.empty())
    {
        std::wcout << L"[!] No process name entered.\n";
        return 1;
    }

    std::wcout << L"Enter DLL name (e.g. cheat.dll): ";
    std::getline(std::wcin, dllName);

    if (dllName.empty())
    {
        std::wcout << L"[!] No DLL name entered.\n";
        return 1;
    }

    std::wcout << L"\nInjection Method:\n";
    std::wcout << L"1. Auto (CreateRemoteThread)\n";
    std::wcout << L"2. QueueUserAPC\n";
    std::wcout << L"Choose (1-2): ";
    std::cin >> methodChoice;

    InjectionMethod method = (methodChoice == 2) ?
        InjectionMethod::QueueUserAPC :
        InjectionMethod::CreateRemoteThread;

    DLLInjector::Inject(processName, dllName, method);

    std::wcout << L"\nPress Enter to exit...";
    std::wstring dummy;
    std::getline(std::wcin, dummy);
    std::getline(std::wcin, dummy);

    return 0;
}
