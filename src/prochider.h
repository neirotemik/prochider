#ifndef PROCHIDER_H
#define PROCHIDER_H

#include <windows.h>
#include <winternl.h>

typedef NTSTATUS(WINAPI* PNT_QUERY_SYSTEM_INFORMATION)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

#endif // PROCHIDER_H
