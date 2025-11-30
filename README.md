# ProcHider

A DLL for hiding a process from the Windows process list through intercepting NtQuerySystemInformation.

## Dependencies

- [MinHook](https://github.com/TsudaKageworker/minhook ) - a library for intercepting the API
- CMake 3.10+
- C11-enabled compiler (MSVC, MinGW)

## How to build

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

How it works

1. The DLL intercepts the NtQuerySystemInformation from ntdll.dll
2. When requesting a list of processes (SystemProcessInformation) filters the results.
3. The process whose name you specify in g_ProcessName will be hidden.

Using

Inject the DLL into the target process using any injector.
