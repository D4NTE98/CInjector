#pragma once
#include <windows.h>
#include <string>
#include <vector>

struct ProcessInfo {
    DWORD pid;
    std::wstring name;
    std::wstring path;
};

class ProcessHelper {
public:
    static std::vector<ProcessInfo> FindProcessesByName(const std::wstring& name);
    static DWORD FindProcessId(const std::wstring& name);
    static std::wstring GetProcessPath(DWORD pid);
};
