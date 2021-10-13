#include "main.h"




//显示区块表
BOOL _stdcall _ShowBlock(LPVOID lpMapFile,HWND hDlg)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));


	HWND         hList;							//ListView控件的句柄
	LVCOLUMN     lvColumn;						//ListView列表头属性
	LVITEM       lvItem;						//ListView列表项属性
	memset(&lvColumn, 0, sizeof(lvColumn));
	hList	= GetDlgItem(hDlg, IDC_LIST1);

	TCHAR		szBuffer[256] = {0};
	TCHAR		szName[256] = {0};
	SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("区块");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 0, (LPARAM)&lvColumn);

	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("内存RVA");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 1, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("实际大小");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 2, (LPARAM)&lvColumn);
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("文件地址");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 3, (LPARAM)&lvColumn);
	
	
    lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("文件大小");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 4, (LPARAM)&lvColumn);
	
	lvColumn.mask      = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx        = 80;
	lvColumn.pszText   = TEXT("文件属性");
	SendDlgItemMessage(hDlg, IDC_LIST1, LVM_INSERTCOLUMN, 5, (LPARAM)&lvColumn);



	
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections ; i++)
	{
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask		 = LVIF_TEXT;
		lvItem.iItem	 = i;
		//ListView_InsertItem(hWnd,&lvColumn);


		
		memset(szName, 0, sizeof(szName));
		for(int i = 0; i <= 7; i++)
			szName[i] = lpBlock->Name[i];
		
		//ListView_SetItemText(hWnd, i-1, 0,szName);
		
		lvItem.pszText = szName;

		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_INSERTITEM,0,(LPARAM)&lvItem);
		


		lvItem.pszText  = szBuffer;
		wsprintf(szBuffer, TEXT("%08lX"), lpBlock->VirtualAddress);
		lvItem.iSubItem = 1;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
       
		wsprintf(szBuffer, TEXT("%08lX"), lpBlock->Misc.VirtualSize);
		lvItem.iSubItem = 2;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);

		wsprintf(szBuffer, TEXT("%08lX"), lpBlock->PointerToRawData);
		lvItem.iSubItem = 3;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
        
		wsprintf(szBuffer, TEXT("%08lX"), lpBlock->SizeOfRawData);
		lvItem.iSubItem = 4;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
		
		wsprintf(szBuffer, TEXT("%08lX"), lpBlock->Characteristics);
		lvItem.iSubItem = 5;
		SendDlgItemMessage(hDlg,IDC_LIST1, LVM_SETITEM, 0, (LPARAM)&lvItem);
		lpBlock ++;

	}
	return 0;
}
	



//区块表窗口过程
BOOL CALLBACK _ProcDlgBlock(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		_ShowBlock((LPVOID)lParam,hWnd);
		break;
	case WM_CLOSE:
		EndDialog(hWnd,NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
