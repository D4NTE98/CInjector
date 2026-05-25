# CInjector

**Advanced DLL Injector for Windows**  
A reliable DLL injector written in C++ with multiple injection methods.

![Windows](https://img.shields.io/badge/Platform-Windows-0078D4)
![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/License-MIT-green)

## Features

- **Process detection** by name
- **Automatic DLL path resolution** (looks for DLL in the same folder as injector)
- Two injection methods:
  - `CreateRemoteThread` (default)
  - `QueueUserAPC`
- Smart fallback system (if one method fails, tries another)
- Detailed console feedback with status indicators
- Requires administrator privileges

## Usage

1. Run `CInjector.exe` as **Administrator**
2. Enter target process name (e.g. `cs2.exe`)
3. Enter DLL name (e.g. `cheat.dll`)
4. Choose injection method
5. Wait for confirmation

# Created by D4NTE
