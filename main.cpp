#include <stdio.h>
#include <string>
#include <windows.h>
#include <dbgeng.h>

bool LoadDLL(const char* name)
{
    if (!LoadLibraryA(name))
    {
        printf("Failed to load %s", name);
        return false;
    }
    return true;
}


int main(int argc, char** argv)
{
    // dbgsrv.exe -t npipe:pipe=qv8b20xidxsd886j,Server=localhost
    if (argc < 3)
    {
        printf("%s -t <connection string>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "-t") != 0)
    {
        printf("This version of dbgsrv only recognizes -t");
        return -1;
    }

    if (!LoadDLL("dbgeng.dll")) return -1;
    if (!LoadDLL("dbgcore.dll")) return -1;
    if (!LoadDLL("dbghelp.dll")) return -1;
    if (!LoadDLL("dbgmodel.dll")) return -1;

    auto handle = GetModuleHandleA("dbgeng.dll");
    if (handle == nullptr)
    {
        printf("Failed to get module handle for dbgeng.dll\n");
        return -1;
    }

    //    HRESULT DebugCreate(
    //    [in]  REFIID InterfaceId,
    //    [out] PVOID  *Interface
    //    );
    typedef HRESULT(__stdcall * pfunDebugCreate)(REFIID, PVOID*);
    auto DebugCreate = (pfunDebugCreate)GetProcAddress(handle, "DebugCreate");
    if (DebugCreate == nullptr)
    {
        printf("Failed to get the address of DebugCreate function\n");
        return -1;
    }

    IDebugClient7* m_debugClient = nullptr;
    if (const auto result = DebugCreate(__uuidof(IDebugClient7), reinterpret_cast<void**>(&m_debugClient));
            result != S_OK)
    {
        printf("Failed to create IDebugClient7\n");
        return -1;
    }

    auto result = m_debugClient->StartProcessServer(DEBUG_CLASS_USER_WINDOWS, argv[2], NULL);
    if (result != S_OK)
    {
        printf("StartProcessServer failed with: 0x%lx\n", result);
        return -1;
    }

    printf("Process server started!\n");
    m_debugClient->WaitForProcessServerEnd(INFINITE);
    return 0;
}