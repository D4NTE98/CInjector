#include "ProcessHelper.h"
#include <tlhelp32.h>
#include <psapi.h>

std::vector<ProcessInfo> ProcessHelper::FindProcessesByName(const std::wstring& name) {
    std::vector<ProcessInfo> results;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return results;

    PROCESSENTRY32W entry = { sizeof(PROCESSENTRY32W) };
    std::wstring loweredName = name;
    for (auto& c : loweredName) c = towlower(c);

    if (Process32FirstW(snapshot, &entry)) {
        do {
            std::wstring procName = entry.szExeFile;
            for (auto& c : procName) c = towlower(c);

            if (procName == loweredName) {
                ProcessInfo pi{ entry.th32ProcessID, entry.szExeFile, L"" };
                pi.path = GetProcessPath(pi.pid);
                results.push_back(pi);
            }
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return results;
}

DWORD ProcessHelper::FindProcessId(const std::wstring& name) {
    auto processes = FindProcessesByName(name);
    return processes.empty() ? 0 : processes[0].pid;
}

std::wstring ProcessHelper::GetProcessPath(DWORD pid) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProc) return L"<access denied>";

    wchar_t path[MAX_PATH] = {};
    DWORD size = MAX_PATH;
    QueryFullProcessImageNameW(hProc, 0, path, &size);
    CloseHandle(hProc);
    return path;
}
