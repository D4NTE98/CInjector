#pragma once
#include <string>
#include "InjectionMethods.h"

class DLLInjector {
public:
    static bool Inject(const std::wstring& processName, const std::wstring& dllName, InjectionMethod preferredMethod = InjectionMethod::CreateRemoteThread);

private:
    static std::wstring GetExeDirectory();
    static bool FileExists(const std::wstring& path);
};
