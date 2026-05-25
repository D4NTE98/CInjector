#pragma once
#include <windows.h>
#include <string>

enum class InjectionMethod {
    CreateRemoteThread,
    QueueUserAPC,
    // ManualMapping
};

class InjectionMethods {
public:
    static bool CreateRemoteThreadInject(DWORD pid, const std::wstring& dllPath);
    static bool QueueUserAPCInject(DWORD pid, const std::wstring& dllPath);
    
    static bool Inject(DWORD pid, const std::wstring& dllPath, InjectionMethod method);
};
