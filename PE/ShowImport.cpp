#include "main.h"

//显示输入表
BOOL _stdcall _ShowImport(LPVOID lpMapFile, HWND hDlg)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	
	IMAGE_IMPORT_DESCRIPTOR * lpIID = (IMAGE_IMPORT_DESCRIPTOR *)lpMapFile;
	lpIID = (IMAGE_IMPORT_DESCRIPTOR *)((BYTE *)lpIID + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));

	HWND         hList;							
	LVCOLUMN     lvColumn;						
	LVITEM       lvItem;						
	memset(&lvColumn, 0, sizeof(lvColumn));
	hList	= GetDlgItem(hDlg, IDC_LIST1);
	

	TCHAR		szBuffer[256] = {0};
	TCHAR		szName[256] = {0};
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("DLL名称");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);


	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("输入名称表(INT)");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("TimeDataStamp");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("ForwarderChain");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("DLL");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);
	
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 98;
	lvColumn.pszText   = TEXT("输入地址表(IAT)");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);
	


	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 270;
	lvColumn.pszText   = TEXT("函数名");
	SendDlgItemMessage(hDlg, IDC_LIST2, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);


	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 100;
	lvColumn.pszText   = TEXT("Hint(序号)");
	SendDlgItemMessage(hDlg, IDC_LIST2, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 110;
	lvColumn.pszText   = TEXT("提示名表(RVA)");
	SendDlgItemMessage(hDlg, IDC_LIST2, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 110;
	lvColumn.pszText   = TEXT("提示名表(offset)");
	SendDlgItemMessage(hDlg, IDC_LIST2, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);

	

	







	int i = 0;
	while(lpIID->Name != 0)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask		 = LVIF_TEXT;
		lvItem.iItem	 = i;
		memset(szName, 0, sizeof(szName));
		char * lpDllName = (char *)lpMapFile;
		lpDllName = (char *)((BYTE *)lpDllName + _RVAtoFileOffset(lpMapFile,lpIID->Name));
		for(int x = 0;;x++)
		{
			if(lpDllName[x] == 0)
				break;
			szName[x] = lpDllName[x];
		}
			
		lvItem.pszText = szName;

		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_INSERTITEM,0,(LPARAM)&lvItem);
	
	
	
		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpIID->OriginalFirstThunk);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
	
		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpIID->TimeDateStamp);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
	
		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpIID->ForwarderChain);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
	
		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpIID->Name);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
	
		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpIID->FirstThunk);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
	
		lpIID++;
		i++;
	
	}
	
	return 0;

}

//显示输入地址表
BOOL _stdcall _ShowDll(HWND hDlg, LPVOID lpMapFile, DWORD iltem)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	
	IMAGE_IMPORT_DESCRIPTOR * lpIID = (IMAGE_IMPORT_DESCRIPTOR *)lpMapFile;
	lpIID = (IMAGE_IMPORT_DESCRIPTOR *)((BYTE *)lpIID + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));
	
	HWND hList2	= GetDlgItem(hDlg, IDC_LIST2);
	SendMessage(hList2,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);	
	SendMessage(hList2 ,LVM_DELETEALLITEMS ,0,0);

	LVITEM       lvItem;						
	TCHAR		szBuffer[256] = {0};
	TCHAR		szName[256] = {0};
	for(int i = 0; i < iltem; i++)
		lpIID++;
	IMAGE_THUNK_DATA * lpThunk = (IMAGE_THUNK_DATA *)lpMapFile;
	IMAGE_IMPORT_BY_NAME * lpDllName = (IMAGE_IMPORT_BY_NAME *)lpMapFile;

	lpThunk = (IMAGE_THUNK_DATA *)((BYTE *)lpThunk + _RVAtoFileOffset(lpMapFile,lpIID->OriginalFirstThunk));
	int i  = 0;						
	while(lpThunk->u1.AddressOfData != 0)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask		 = LVIF_TEXT;
		lvItem.iItem	 = i;
		memset(szName, 0, sizeof(szName));
		lpDllName = (IMAGE_IMPORT_BY_NAME *)lpMapFile;
		lpDllName =(IMAGE_IMPORT_BY_NAME *)((BYTE *)lpDllName + _RVAtoFileOffset(lpMapFile,lpThunk->u1.AddressOfData));	
		for(int i = 0;; i++)
		{
			if(lpDllName->Name[i] == 0)
				break;
			szName[i] = lpDllName->Name[i];
		}

		lvItem.pszText	= szName;
		SendDlgItemMessage(hDlg,IDC_LIST2, LVM_INSERTITEM, 0, (LPARAM)&lvItem);


		lvItem.pszText	= szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpDllName->Hint);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_LIST2, LVM_SETITEM, 0, (LPARAM)&lvItem);

		lvItem.pszText	= szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpThunk->u1.AddressOfData);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_LIST2, LVM_SETITEM, 0, (LPARAM)&lvItem);

		lvItem.pszText	= szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), _RVAtoFileOffset(lpMapFile,lpThunk->u1.AddressOfData));
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_LIST2, LVM_SETITEM, 0, (LPARAM)&lvItem);
		
		lpThunk++;
		i++;
	}
	return 0;
}









//显示输入表窗口过程
BOOL CALLBACK _ProcDlgImport(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int		wmId;
	LPNMHDR pnmh;
	LPNMLISTVIEW	lpnmlv;
	static LPVOID	lpMapFile;
	
	switch(message)
	{
	case WM_INITDIALOG:
		lpMapFile = (LPVOID)lParam;
		_ShowImport((LPVOID)lParam,hWnd);
		break;
	
	case WM_NOTIFY:
		wmId	= LOWORD(wParam); 
		pnmh	= (LPNMHDR) lParam;
		if(wmId == IDC_LIST1)
		{
			switch(pnmh->code)
			{
			case LVN_ITEMCHANGED:
				lpnmlv = (LPNMLISTVIEW) lParam;
				if(lpnmlv->uNewState)
					_ShowDll(hWnd,lpMapFile,lpnmlv->iItem);
			}
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd,NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

