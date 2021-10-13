#include "main.h"

//显示输出表
BOOL _stdcall _ShowExport(LPVOID lpMapFile,HWND hDlg)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	IMAGE_EXPORT_DIRECTORY * lpExport = (IMAGE_EXPORT_DIRECTORY *)lpMapFile;
	lpExport = (IMAGE_EXPORT_DIRECTORY *)((BYTE *)lpExport + _RVAtoFileOffset(lpMapFile, lpData->VirtualAddress));
	
	if(lpExport->NumberOfFunctions == 0)
	{
		MessageBox(NULL, TEXT("此文件无输出表！"), TEXT("sorry"), MB_OK);
		return FALSE;
	}
	TCHAR szBuffer[256] = {0};
	TCHAR szName[256]	= {0};
	char * lpExportName = (char *)lpMapFile;
	lpExportName = lpExportName + _RVAtoFileOffset(lpMapFile, lpExport->Name);
	for(int i = 0;;i++)
	{
		if(lpExportName[i] == 0)
			break;
		szName[i] = lpExportName[i];
	}

	SetDlgItemText(hDlg, IDC_EDIT3, szName);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->Base);
	SetDlgItemText(hDlg, IDC_EDIT4, szBuffer);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->NumberOfFunctions);
	SetDlgItemText(hDlg, IDC_EDIT5, szBuffer);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->NumberOfNames);
	SetDlgItemText(hDlg, IDC_EDIT6, szBuffer);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->AddressOfFunctions);
	SetDlgItemText(hDlg, IDC_EDIT7, szBuffer);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->AddressOfNames);
	SetDlgItemText(hDlg, IDC_EDIT8, szBuffer);

	wsprintf(szBuffer, TEXT("%08lX"), lpExport->AddressOfNameOrdinals);
	SetDlgItemText(hDlg, IDC_EDIT9, szBuffer);


	HWND         hList;							
	LVCOLUMN     lvColumn;						
	LVITEM       lvItem;						
	memset(&lvColumn, 0, sizeof(lvColumn));
	hList	= GetDlgItem(hDlg, IDC_LIST1);

	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 120;
	lvColumn.pszText   = TEXT("序号");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 120;
	lvColumn.pszText   = TEXT("RVA");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 210;
	lvColumn.pszText   = TEXT("函数名");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

	int * lpENT = (int *)lpMapFile;
	lpENT = (int *)((BYTE *)lpENT + _RVAtoFileOffset(lpMapFile,lpExport->AddressOfNames));
	char * lpProcName = (char *)lpMapFile; 

	int * lpEAT = (int *)lpMapFile;
	lpEAT = (int *)((BYTE *)lpEAT + _RVAtoFileOffset(lpMapFile,lpExport->AddressOfFunctions));

	WORD * lpNum = (WORD *)lpMapFile;
	lpNum = (WORD *)((BYTE*)lpNum + _RVAtoFileOffset(lpMapFile,lpExport->AddressOfNameOrdinals));

	
	for(int i = 0; i < lpExport->NumberOfFunctions; i++)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		memset(szBuffer, 0, sizeof(szBuffer));
		memset(szName, 0, sizeof(szName));
		lvItem.iItem	= i;
		lvItem.mask		= LVIF_TEXT;
		lvItem.pszText	= szBuffer;

		
		wsprintf(szBuffer, TEXT("%04lX"), (*lpNum));
		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_INSERTITEM, 0,(LPARAM)&lvItem);
		


		lvItem.iSubItem = 1;
		wsprintf(szBuffer, TEXT("%08lX"), (*lpEAT));
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);


		lvItem.iSubItem = 2;
		lvItem.pszText	= szName;
		lpProcName = (char *)lpMapFile;
		lpProcName = (char *)((BYTE*)lpProcName + _RVAtoFileOffset(lpMapFile,(*lpENT)));
		for(int i = 0;;i++)
		{
			if(lpProcName[i] == 0)
				break;
			szName[i] = lpProcName[i];
		}
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);


		

	
		lpENT++;
		lpEAT++;
		lpNum++;
	}
	return TRUE;
}

//输出表窗口过程
BOOL CALLBACK _ProcDlgExport(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		if(FALSE == _ShowExport((LPVOID)lParam, hWnd))
			EndDialog(hWnd,NULL);
		break;
	case WM_CLOSE:
		EndDialog(hWnd, NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

