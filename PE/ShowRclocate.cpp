#include "main.h"

//显示重定位表
BOOL _stdcall _ShowRclocate(LPVOID lpMapFile, HWND hDlg)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	
	if(lpNT->FileHeader.Characteristics & 0x0001)
	{
		MessageBox(NULL, TEXT("文件不存在重定位信息"), TEXT("sorry"), MB_OK);
		return FALSE;
	}
	
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	lpData = lpData + 5;

	IMAGE_BASE_RELOCATION * lpRelocate = (IMAGE_BASE_RELOCATION *)lpMapFile;
	lpRelocate = (IMAGE_BASE_RELOCATION *)((BYTE *)lpRelocate + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));

	HWND         hList;							
	LVCOLUMN     lvColumn;						
	LVITEM       lvItem;						
	TCHAR		 szBuffer[256] = {0};
	memset(&lvColumn, 0, sizeof(lvColumn));
	hList	= GetDlgItem(hDlg, IDC_LIST1);

	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 160;
	lvColumn.pszText   = TEXT("重定位数据开始RVA");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);


	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 150;
	lvColumn.pszText   = TEXT("重定位块长度");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 150;
	lvColumn.pszText   = TEXT("重定位项数组");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);

	int i = 0;
	while(lpRelocate->VirtualAddress != 0)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		memset(szBuffer, 0, sizeof(szBuffer));
		lvItem.iItem	= i;
		lvItem.mask		= LVIF_TEXT;
		lvItem.pszText	= szBuffer;

		wsprintf(szBuffer, TEXT("%08lX"), lpRelocate->VirtualAddress);
		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_INSERTITEM, 0,(LPARAM)&lvItem);
		
		lvItem.iSubItem = 1;
		wsprintf(szBuffer, TEXT("%08lX"), lpRelocate->SizeOfBlock);
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);

		lvItem.iSubItem = 2;
		wsprintf(szBuffer, TEXT("%08lX"), *((WORD *)((BYTE *)lpRelocate + 8)));
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
		
		lpRelocate = (IMAGE_BASE_RELOCATION *)((BYTE *)lpRelocate + lpRelocate->SizeOfBlock);
		i++;
	}




	return TRUE;
}







//重定位表窗口过程

BOOL CALLBACK _ProcDlgRclocate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		if(FALSE == _ShowRclocate((LPVOID)lParam,hWnd))
			EndDialog(hWnd, NULL);
		break;
	case WM_CLOSE:
		EndDialog(hWnd, NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}