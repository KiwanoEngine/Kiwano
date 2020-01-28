// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano/core/Library.h>
#include <kiwano/core/Logger.h>
#include <kiwano/macros.h>

#pragma warning(push)
#pragma warning(disable : 4091)
#include <dbghelp.h>
#pragma warning(pop)

namespace kiwano
{
namespace win32
{
namespace
{

// SymInitialize()
typedef BOOL(__stdcall* PFN_SymInitialize)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);

// SymCleanup()
typedef BOOL(__stdcall* PFN_SymCleanup)(IN HANDLE hProcess);

// SymGetLineFromAddr64()
typedef BOOL(__stdcall* PFN_SymGetLineFromAddr64)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD pdwDisplacement,
                                                  OUT PIMAGEHLP_LINE64 Line);

// SymGetSymFromAddr64()
typedef BOOL(__stdcall* PFN_SymGetSymFromAddr64)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD64 pdwDisplacement,
                                                 OUT PIMAGEHLP_SYMBOL64 Symbol);

// StackWalk64()
typedef BOOL(__stdcall* PFN_StackWalk64)(DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame,
                                         PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
                                         PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
                                         PGET_MODULE_BASE_ROUTINE64       GetModuleBaseRoutine,
                                         PTRANSLATE_ADDRESS_ROUTINE64     TranslateAddress);

// SymFunctionTableAccess64()
typedef PVOID(__stdcall* PFN_SymFunctionTableAccess64)(HANDLE hProcess, DWORD64 AddrBase);

// SymGetModuleBase64()
typedef DWORD64(__stdcall* PFN_SymGetModuleBase64)(IN HANDLE hProcess, IN DWORD64 dwAddr);

struct DbgHelp
{
    Library                      dbgLib;
    PFN_SymInitialize            SymInitialize;
    PFN_SymCleanup               SymCleanup;
    PFN_SymGetLineFromAddr64     SymGetLineFromAddr64;
    PFN_SymGetSymFromAddr64      SymGetSymFromAddr64;
    PFN_StackWalk64              StackWalk64;
    PFN_SymFunctionTableAccess64 SymFunctionTableAccess64;
    PFN_SymGetModuleBase64       SymGetModuleBase64;

    DbgHelp()
        : SymInitialize(nullptr)
        , SymCleanup(nullptr)
        , SymGetLineFromAddr64(nullptr)
        , SymGetSymFromAddr64(nullptr)
        , StackWalk64(nullptr)
        , SymFunctionTableAccess64(nullptr)
        , SymGetModuleBase64(nullptr)
    {
    }

    bool Load()
    {
        if (IsValid())
            return true;

        if (!dbgLib.Load("dbghelp.dll"))
            return false;

        SymInitialize            = dbgLib.GetProcess<PFN_SymInitialize>("SymInitialize");
        SymCleanup               = dbgLib.GetProcess<PFN_SymCleanup>("SymCleanup");
        SymGetLineFromAddr64     = dbgLib.GetProcess<PFN_SymGetLineFromAddr64>("SymGetLineFromAddr64");
        SymGetSymFromAddr64      = dbgLib.GetProcess<PFN_SymGetSymFromAddr64>("SymGetSymFromAddr64");
        StackWalk64              = dbgLib.GetProcess<PFN_StackWalk64>("StackWalk64");
        SymFunctionTableAccess64 = dbgLib.GetProcess<PFN_SymFunctionTableAccess64>("SymFunctionTableAccess64");
        SymGetModuleBase64       = dbgLib.GetProcess<PFN_SymGetModuleBase64>("SymGetModuleBase64");

        if (!IsValid())
        {
            dbgLib.Free();
            return false;
        }
        return true;
    }

    bool IsValid() const
    {
        return SymInitialize && SymCleanup && SymGetLineFromAddr64 && SymGetSymFromAddr64 && StackWalk64
               && SymFunctionTableAccess64 && SymGetModuleBase64;
    }
};

DbgHelp g_DbgHelp;
}  // namespace

void PrintErrorCode(LPCWSTR lpszFunction)
{
    KGE_ERROR(L"%s failed with HRESULT of %08X", lpszFunction, HRESULT_FROM_WIN32(GetLastError()));
}

void PrintCallStackOnContext(PCONTEXT pContext)
{
    if (!g_DbgHelp.Load())
        return;

    if (pContext == nullptr)
        return;

    DWORD        dwMachineType;
    STACKFRAME64 sf;
    HANDLE       hProcess = GetCurrentProcess();
    HANDLE       hThread  = GetCurrentThread();

    ZeroMemory(&sf, sizeof(sf));

    sf.AddrPC.Mode     = AddrModeFlat;
    sf.AddrFrame.Mode  = AddrModeFlat;
    sf.AddrStack.Mode  = AddrModeFlat;
    sf.AddrBStore.Mode = AddrModeFlat;

#ifdef _M_IX86
    dwMachineType       = IMAGE_FILE_MACHINE_I386;
    sf.AddrPC.Offset    = pContext->Eip;
    sf.AddrFrame.Offset = pContext->Ebp;
    sf.AddrStack.Offset = pContext->Esp;
#elif _M_X64
    dwMachineType       = IMAGE_FILE_MACHINE_AMD64;
    sf.AddrPC.Offset    = pContext->Rip;
    sf.AddrFrame.Offset = pContext->Rsp;
    sf.AddrStack.Offset = pContext->Rsp;
#elif _M_IA64
    dwMachineType        = IMAGE_FILE_MACHINE_IA64;
    sf.AddrPC.Offset     = pContext->StIIP;
    sf.AddrFrame.Offset  = pContext->IntSp;
    sf.AddrBStore.Offset = pContext->RsBSP;
    sf.AddrStack.Offset  = pContext->IntSp;
#else
#error "Platform not supported!"
#endif

    constexpr int STACKWALK_MAX_NAMELEN = 1024;
    BYTE          symbolBuffer[sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN];

    KGE_ERROR(L"==========  Stack trace  ==========");

    while (true)
    {
        if (!g_DbgHelp.StackWalk64(dwMachineType, hProcess, hThread, &sf, pContext, NULL,
                                   g_DbgHelp.SymFunctionTableAccess64, g_DbgHelp.SymGetModuleBase64, NULL))
        {
            PrintErrorCode(L"StackWalk64");
            break;
        }

        if (sf.AddrFrame.Offset == 0)
        {
            break;
        }

        ZeroMemory(symbolBuffer, sizeof(symbolBuffer));

        IMAGEHLP_SYMBOL64* pSymbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symbolBuffer);
        pSymbol->SizeOfStruct      = sizeof(IMAGEHLP_SYMBOL64);
        pSymbol->MaxNameLength     = STACKWALK_MAX_NAMELEN;

        DWORD64 dwDisplacement;
        if (!g_DbgHelp.SymGetSymFromAddr64(hProcess, sf.AddrPC.Offset, &dwDisplacement, pSymbol))
        {
            PrintErrorCode(L"SymGetSymFromAddr64");
            break;
        }

        IMAGEHLP_LINE64 lineInfo;
        ZeroMemory(&lineInfo, sizeof(IMAGEHLP_LINE64));
        lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

        DWORD dwLineDisplacement;
        if (g_DbgHelp.SymGetLineFromAddr64(hProcess, sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo))
        {
            String functionName = MultiByteToWide(pSymbol->Name);
            String fileName     = MultiByteToWide(lineInfo.FileName);
            KGE_ERROR(L"%s (%d): %s", fileName.c_str(), lineInfo.LineNumber, functionName.c_str());
        }
        else
        {
            String functionName = MultiByteToWide(pSymbol->Name);
            KGE_ERROR(L"(filename not available): %s", functionName.c_str());
        }

        if (sf.AddrReturn.Offset == 0)
        {
            SetLastError(ERROR_SUCCESS);
            break;
        }
    }
}

void PrintCallStack()
{
    CONTEXT ctx;
    HANDLE  hProcess = GetCurrentProcess();

    if (!g_DbgHelp.Load())
        return;

    if (!g_DbgHelp.SymInitialize(hProcess, NULL, TRUE))
        return;

    RtlCaptureContext(&ctx);

    PrintCallStackOnContext(&ctx);

    g_DbgHelp.SymCleanup(hProcess);
}
}  // namespace win32
}  // namespace kiwano
