#include "main.h"


//显示数据目录表

BOOL _stdcall _ShowData(LPVOID lpMapFile,HWND hDlg)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	TCHAR	szBuffer[10] = {0};

	SetDlgItemText(hDlg, IDC_EDIT1, TEXT("RVA"));
	SetDlgItemText(hDlg, IDC_EDIT2, TEXT("size"));

	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT3, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT4, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT5, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT6, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT7, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT8, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT9, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT10, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT11, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT12, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT13, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT14, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT15, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT16, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT17, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT18, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT19, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT20, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT21, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT22, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT23, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT24, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT25, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT26, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT27, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT28, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT29, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT30, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT31, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT32, szBuffer);

	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	wsprintf(szBuffer,TEXT("%08lX"),lpData->VirtualAddress);
	SetDlgItemText(hDlg, IDC_EDIT33, szBuffer);
	wsprintf(szBuffer,TEXT("%08lX"),lpData->Size);
	SetDlgItemText(hDlg, IDC_EDIT34, szBuffer);
	return 0;
}


//数据目录表窗口过程

BOOL CALLBACK _ProcDlgData(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		_ShowData((LPVOID)lParam,hWnd);
		break;
	case WM_CLOSE:
		EndDialog(hWnd,NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
