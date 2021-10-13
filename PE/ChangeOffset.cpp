#include "main.h"

//RVA转换为文件偏移地址
DWORD _stdcall _RVAtoFileOffset(LPVOID lpMapFile,DWORD dwRVA)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
		
	if(dwRVA < lpBlock->VirtualAddress)
		return dwRVA;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if(dwRVA >= lpBlock->VirtualAddress )
		{
			
			if(dwRVA < lpBlock->VirtualAddress + lpBlock->SizeOfRawData)
				return (dwRVA - lpBlock->VirtualAddress + lpBlock->PointerToRawData);
		}
		lpBlock++;
	}

	return -1;

}
//RVA转化为VA
DWORD _stdcall _RVAtoVA(LPVOID lpMapFile, DWORD dwRVA)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
	if(dwRVA < 0)
		return -1;
	else if(dwRVA < lpBlock->VirtualAddress)
		return dwRVA + lpNT->OptionalHeader.ImageBase;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if(dwRVA >= lpBlock->VirtualAddress)
		{
			if(dwRVA < lpBlock->VirtualAddress + lpBlock->SizeOfRawData)
				return dwRVA + lpNT->OptionalHeader.ImageBase;
		}
		lpBlock++;
	}
	return -1;
}


//文件偏移转化为RVA
BOOL _stdcall _FileOffsetToRVA(LPVOID lpMapFile, DWORD dwOffset)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
	if(dwOffset < 0)
		return -1;
	else if(dwOffset < lpBlock->PointerToRawData)
		return dwOffset;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if(dwOffset >= lpBlock->PointerToRawData)
		{
			if(dwOffset < lpBlock->PointerToRawData + lpBlock->SizeOfRawData)
				return lpBlock->VirtualAddress + (dwOffset - lpBlock->PointerToRawData);
		}
		lpBlock++;
	}
	return -1;
}


//文件偏移转化为VA
BOOL _stdcall _FileOffsetToVA(LPVOID lpMapFile, DWORD dwOffset)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
	if(dwOffset < 0)
		return -1;
	else if(dwOffset < lpBlock->PointerToRawData)
		return lpNT->OptionalHeader.ImageBase + dwOffset;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if(dwOffset >= lpBlock->PointerToRawData)
		{
			if(dwOffset < lpBlock->PointerToRawData + lpBlock->SizeOfRawData)
				return (lpNT->OptionalHeader.ImageBase + _FileOffsetToRVA(lpMapFile, dwOffset));
			
		}
		lpBlock++;
	}
	return -1;
}


//VA转化为RVA
BOOL _stdcall _VAtoRVA(LPVOID lpMapFile, DWORD dwVA)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
	if((dwVA - lpNT->OptionalHeader.ImageBase) < 0)
		return -1;
	else if((dwVA - lpNT->OptionalHeader.ImageBase) < lpBlock->VirtualAddress)
		return dwVA - lpNT->OptionalHeader.ImageBase;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if((dwVA - lpNT->OptionalHeader.ImageBase) >= lpBlock->VirtualAddress)
		{
			lpBlock++;
			if((dwVA - lpNT->OptionalHeader.ImageBase) < lpBlock->VirtualAddress)
				return dwVA - lpNT->OptionalHeader.ImageBase;
			lpBlock--;
		}
		lpBlock++;
	}
	return -1;
}

//VA转化为文件偏移
BOOL _stdcall _VAtoFileOffset(LPVOID lpMapFile, DWORD dwVA)
{
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);	
	IMAGE_SECTION_HEADER * lpBlock = (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));

	if((dwVA - lpNT->OptionalHeader.ImageBase) < 0)
		return -1;
	else if((dwVA - lpNT->OptionalHeader.ImageBase) < lpBlock->VirtualAddress)
		return dwVA - lpNT->OptionalHeader.ImageBase;
	int x;
	for(int i = 0; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		x = _RVAtoFileOffset(lpMapFile, dwVA - lpNT->OptionalHeader.ImageBase);
		if(x >= 0)
			return x;
	}
	return -1; 
}







//将16进制字符串转化为数值
int htoi(TCHAR s[])  
{  
    int n = 0;  
    for (int i = 0;i < wcslen(s); i++)  
    {  
		if((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >='A' && s[i] <= 'F'))
		{
			if (tolower(s[i]) > '9')  
				n = 16 * n + (10 + tolower(s[i]) - 'a');  
			else  
				n = 16 * n + (tolower(s[i]) - '0');  
		}
		else
			return 0;
    }  
    return n;  
}  




//地址转换窗口过程
BOOL CALLBACK _ProcDlgToRVA(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	TCHAR	szRVA[20]	 = {0};
	TCHAR	szVA[20]	 = {0};
	TCHAR	szOffset[20] = {0};
	static  LPVOID lpMapFile;
	switch(message)
	{
	case WM_INITDIALOG:
		lpMapFile = (LPVOID)lParam;
		break;
	case WM_COMMAND:
		switch(short int(wParam))
		{
		case IDC_EDIT5:
			GetDlgItemText(hWnd, IDC_EDIT5, szRVA, sizeof(szRVA));
			if(wcslen(szRVA) > 0)
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), TRUE);
			}
			break;
		case IDC_EDIT1:
			GetDlgItemText(hWnd, IDC_EDIT1, szVA, sizeof(szVA));
			if(wcslen(szVA) > 0)
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), FALSE);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), TRUE);
			}
			break;
		case IDC_EDIT3:
			GetDlgItemText(hWnd, IDC_EDIT3, szOffset, sizeof(szOffset));
			if(wcslen(szOffset) > 0)
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), FALSE);
			}
			else
			{
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), TRUE);
			}
			break;
		case IDC_BUTTON1:
			if(IsWindowEnabled(GetDlgItem(hWnd, IDC_EDIT5)))
			{
				SetDlgItemText(hWnd, IDC_EDIT1, NULL);
				SetDlgItemText(hWnd, IDC_EDIT3, NULL);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
				GetDlgItemText(hWnd, IDC_EDIT5, szRVA, sizeof(szRVA));
				if(htoi(szRVA))
				{
					int x = _RVAtoVA(lpMapFile, htoi(szRVA));
					if(x >= 0)
					{
						wsprintf(szVA, TEXT("%X"), x);
						SetDlgItemText(hWnd, IDC_EDIT1, szVA);
						x = _RVAtoFileOffset(lpMapFile, htoi(szRVA));
						if(x >= 0)
						{
							wsprintf(szOffset, TEXT("%X"), x);
							SetDlgItemText(hWnd, IDC_EDIT3, szOffset);
							EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), TRUE);
						}
					}
				}
				else
					MessageBox(NULL, TEXT("输入地址有误！"),NULL, MB_OK);
			}

			if(IsWindowEnabled(GetDlgItem(hWnd, IDC_EDIT3)))
			{
				SetDlgItemText(hWnd, IDC_EDIT1, NULL);
				SetDlgItemText(hWnd, IDC_EDIT5, NULL);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), FALSE);
				GetDlgItemText(hWnd, IDC_EDIT3, szOffset, sizeof(szOffset));
				if(htoi(szOffset))
				{
					int x = _FileOffsetToVA(lpMapFile, htoi(szOffset));
					if(x >= 0)
					{
						wsprintf(szVA, TEXT("%X"), x);
						SetDlgItemText(hWnd, IDC_EDIT1, szVA);
						x = _FileOffsetToRVA(lpMapFile, htoi(szOffset));
						if(x >= 0)
						{
							wsprintf(szRVA, TEXT("%X"), x);
							SetDlgItemText(hWnd, IDC_EDIT5, szRVA);
							EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), TRUE);
						}
					}
				}
				else
					MessageBox(NULL, TEXT("输入地址有误！"),NULL, MB_OK);
			}

			if(IsWindowEnabled(GetDlgItem(hWnd, IDC_EDIT1)))
			{
				SetDlgItemText(hWnd, IDC_EDIT3, NULL);
				SetDlgItemText(hWnd, IDC_EDIT5, NULL);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), FALSE);
				GetDlgItemText(hWnd, IDC_EDIT1, szVA, sizeof(szVA));
				if(htoi(szVA))
				{
					int x = _VAtoRVA(lpMapFile, htoi(szVA));
					if(x >= 0)
					{
						wsprintf(szRVA, TEXT("%X"), x);
						SetDlgItemText(hWnd, IDC_EDIT5, szRVA);
						x = _VAtoFileOffset(lpMapFile, htoi(szVA));
						if(x >= 0)
						{
							wsprintf(szOffset, TEXT("%X"), x);
							SetDlgItemText(hWnd, IDC_EDIT3, szOffset);
							EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), TRUE);
						}
					}
				}
				else 
					MessageBox(NULL, TEXT("输入地址有误！"), NULL, MB_OK);
			}
			break;
		case IDC_BUTTON2:
			SetDlgItemText(hWnd, IDC_EDIT1, NULL);
			SetDlgItemText(hWnd, IDC_EDIT3, NULL);
			SetDlgItemText(hWnd, IDC_EDIT5, NULL);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT3), TRUE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT5), TRUE);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hWnd, NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}