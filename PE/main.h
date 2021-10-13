#include <Windows.h>
#include "resource.h"
#pragma comment( lib, "comctl32.lib" )
#include <commctrl.h>
#include <wchar.h>
DWORD _stdcall _RVAtoFileOffset(LPVOID lpMapFile,DWORD dwRVA);
BOOL _stdcall _FileOffsetToRVA(LPVOID lpMapFile, DWORD dwOffset);