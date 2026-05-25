#include "DLLInjector.h"
#include "ProcessHelper.h"
#include <iostream>
#include <windows.h>

std::wstring DLLInjector::GetExeDirectory()
{
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring fullPath = path;
    size_t pos = fullPath.find_last_of(L"\\");
    return (pos != std::wstring::npos) ? fullPath.substr(0, pos) : L"";
}

bool DLLInjector::FileExists(const std::wstring& path)
{
    DWORD attrib = GetFileAttributesW(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES);
}

bool DLLInjector::Inject(const std::wstring& processName, const std::wstring& dllName, InjectionMethod preferredMethod)
{
    std::wstring dllFullPath = GetExeDirectory() + L"\\" + dllName;

    if (!FileExists(dllFullPath))
    {
        std::wcout << L"[!] DLL file not found: " << dllName << L"\n";
        std::wcout << L"    Expected path: " << dllFullPath << L"\n";
        return false;
    }

    std::wcout << L"[+] DLL found: " << dllFullPath << L"\n";

    DWORD pid = ProcessHelper::FindProcessId(processName);
    if (pid == 0)
    {
        std::wcout << L"[!] Process not found: " << processName << L"\n";
        return false;
    }

    std::wcout << L"[+] Process found: " << processName << L" (PID: " << pid << L")\n";

    bool success = InjectionMethods::Inject(pid, dllFullPath, preferredMethod);

    if (success)
    {
        std::wcout << L"[+] Injection successful using "
            << (preferredMethod == InjectionMethod::CreateRemoteThread ? L"CreateRemoteThread" : L"QueueUserAPC")
            << L"!\n";
        return true;
    }

    if (preferredMethod != InjectionMethod::CreateRemoteThread)
    {
        std::wcout << L"[!] Injection failed with preferred method. Trying fallback (CreateRemoteThread)...\n";
        success = InjectionMethods::Inject(pid, dllFullPath, InjectionMethod::CreateRemoteThread);

        if (success)
        {
            std::wcout << L"[+] Injection successful using fallback method (CreateRemoteThread)!\n";
            return true;
        }
    }

    std::wcout << L"[!] All injection methods failed.\n";
    return false;
}
