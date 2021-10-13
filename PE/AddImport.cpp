#include "main.h"

//�ж�����ĺϷ���
BOOL _stdcall _SelectProc(LPVOID lpMapFile, HWND hDlg,int num)
{
	TCHAR	szDllName[40]	= {0};
	TCHAR	szProcName[40]	= {0};
	TCHAR	szName[256]		= {0};
	HANDLE	hFind;
	WIN32_FIND_DATA stFind;
	BOOL	bFlag			= 0;	//��ʶ���Ƿ����ҵ�ָ������
	IMAGE_DOS_HEADER * lpDosMZ = (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS * lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_DATA_DIRECTORY * lpData = lpNT->OptionalHeader.DataDirectory;
	char * lpProcName = NULL;		//ָ�򵼳�������

	HWND         hList;							//ListView�ؼ��ľ��
	LVITEM       lvItem;						//ListView�б�������


	RtlZeroMemory(&stFind, sizeof(stFind));
	GetDlgItemText(hDlg, IDC_EDIT1, szDllName, sizeof(szDllName));
	if(wcslen(szDllName) == 0)
		MessageBox(NULL, TEXT("�㻹û������DLL���ƣ�"), NULL, MB_OK);
	else
	{
		hFind = FindFirstFile(szDllName, &stFind);
		
		if(hFind != INVALID_HANDLE_VALUE)
		{
			FindClose(hFind);
			GetDlgItemText(hDlg, IDC_EDIT2, szProcName, sizeof(szProcName));
			if(wcslen(szProcName) == 0)
				MessageBox(NULL, TEXT("û����DLL�з��ֶ�Ӧ�ĺ�����"), NULL, MB_OK);
			else
			{

				HANDLE hFile		= CreateFile(szDllName,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
				HANDLE hMapFile		= CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);			//�����ڴ�ӳ���ļ�
				LPVOID lpMapFile1	= MapViewOfFile(hMapFile,FILE_MAP_READ,0,0,0);
				
				IMAGE_DOS_HEADER * lpDosMZ1 = (IMAGE_DOS_HEADER *)lpMapFile1;
				IMAGE_NT_HEADERS * lpNT1 = (IMAGE_NT_HEADERS *)lpMapFile1;
				lpNT1 = (IMAGE_NT_HEADERS *)((BYTE *)lpNT1 + lpDosMZ1->e_lfanew);
				IMAGE_DATA_DIRECTORY * lpData1 = lpNT1->OptionalHeader.DataDirectory;
				IMAGE_EXPORT_DIRECTORY * lpExport1 = (IMAGE_EXPORT_DIRECTORY *)lpMapFile1;
				lpExport1 = (IMAGE_EXPORT_DIRECTORY *)((BYTE *)lpExport1 + _RVAtoFileOffset(lpMapFile1, lpData1->VirtualAddress));
				int * lpENT1 = (int *)lpMapFile1;
				lpENT1 = (int *)((BYTE *)lpENT1 + _RVAtoFileOffset(lpMapFile1,lpExport1->AddressOfNames));
				




				if(lpExport1->NumberOfNames != 0)
				{
					
					for(int i = 0; i < lpExport1->NumberOfFunctions; i++)
					{
						memset(szName, 0, sizeof(szName));
						lpProcName = (char *)lpMapFile1;
						lpProcName = (char *)((BYTE*)lpProcName + _RVAtoFileOffset(lpMapFile1,(*lpENT1)));
						for(int i = 0;;i++)
						{
							if(lpProcName[i] == 0)
								break;
							szName[i] = lpProcName[i];
						}
						if(wcscmp(szName, szProcName) == 0)
						{
							bFlag = 1;
							break;
						}
						lpENT1++;
					}

				}
				if(bFlag == 0)
					MessageBox(NULL, TEXT("û����DLL�з��ֶ�Ӧ�ĺ�����"), NULL, MB_OK);
				else if(bFlag == 1)
				{

					lpData = (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
					IMAGE_IMPORT_DESCRIPTOR * lpIID = (IMAGE_IMPORT_DESCRIPTOR *)lpMapFile;
					lpIID = (IMAGE_IMPORT_DESCRIPTOR *)((BYTE *)lpIID + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));
					IMAGE_THUNK_DATA * lpThunk = (IMAGE_THUNK_DATA *)lpMapFile;
					IMAGE_IMPORT_BY_NAME * lpDllName = (IMAGE_IMPORT_BY_NAME *)lpMapFile;
					lpThunk = (IMAGE_THUNK_DATA *)((BYTE *)lpThunk + _RVAtoFileOffset(lpMapFile,lpIID->OriginalFirstThunk));
					TCHAR	szName1[256] = {0};
					while(lpIID->Name != 0)
					{
						lpThunk = (IMAGE_THUNK_DATA *)lpMapFile;
						lpThunk = (IMAGE_THUNK_DATA *)((BYTE *)lpThunk + _RVAtoFileOffset(lpMapFile,lpIID->OriginalFirstThunk));
						while(lpThunk->u1.AddressOfData != 0)
						{
							memset(szName, 0, sizeof(szName));
							lpDllName = (IMAGE_IMPORT_BY_NAME *)lpMapFile;
							lpDllName =(IMAGE_IMPORT_BY_NAME *)((BYTE *)lpDllName + _RVAtoFileOffset(lpMapFile,lpThunk->u1.AddressOfData));	
							for(int i = 0;; i++)
							{
								if(lpDllName->Name[i] == 0)
								{
									szName1[i] = 0;
									break;
								}
								szName1[i] = lpDllName->Name[i];
							}

							if(wcscmp(szName1, szProcName) == 0)
							{
								MessageBox(NULL, TEXT("�����ظ���ӣ��������DLL�д�����˺��������ĺ�������"), NULL, MB_OK);
								UnmapViewOfFile(lpMapFile1);
								CloseHandle(hMapFile);				
								CloseHandle(hFile);
								return 0;
							}

							lpThunk++;
						}
						lpIID++;

					}

					UnmapViewOfFile(lpMapFile1);
					CloseHandle(hMapFile);				
					CloseHandle(hFile);

					
					hList	= GetDlgItem(hDlg, IDC_LIST1);
					SendMessage(hList,LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_FULLROWSELECT);
					memset(&lvItem, 0, sizeof(lvItem));
					lvItem.mask		 = LVIF_TEXT;
					lvItem.iItem	 = num;
					lvItem.pszText	 = szProcName;
					SendDlgItemMessage(hDlg,IDC_LIST1,LVM_INSERTITEM,0,(LPARAM)&lvItem);

				}

			}
		}
		else
			MessageBox(NULL, TEXT("û���ҵ�DLL�ļ�"), NULL, MB_OK);

		


	}
	
	return 0;
}


//����������
BOOL _stdcall _AddImport(LPVOID lpMapFile, HWND hDlg, int num)
{
	IMAGE_DOS_HEADER *			lpDosMZ	= (IMAGE_DOS_HEADER *)lpMapFile;
	IMAGE_NT_HEADERS *			lpNT	= (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT								= (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
	IMAGE_SECTION_HEADER *		lpBlock	= (IMAGE_SECTION_HEADER *)((BYTE *)lpNT + sizeof(IMAGE_NT_HEADERS));
	IMAGE_DATA_DIRECTORY *		lpData	= lpNT->OptionalHeader.DataDirectory;
	lpData								= (IMAGE_DATA_DIRECTORY *)((BYTE *)lpData + sizeof(IMAGE_DATA_DIRECTORY));
	
	IMAGE_IMPORT_DESCRIPTOR *	lpIID	= (IMAGE_IMPORT_DESCRIPTOR *)lpMapFile;
	lpIID								= (IMAGE_IMPORT_DESCRIPTOR *)((BYTE *)lpIID + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));
	int dwFileOffset					= lpBlock->PointerToRawData;			//���һ��������ļ�ƫ�Ƶ�ַ
	IMAGE_IMPORT_DESCRIPTOR *	lpIIDTrue = NULL;


	TCHAR szDllName[40]					= {0};
	TCHAR szProcName[40]				= {0};
	BOOL bFlag							= FALSE; //��־���ӵ�dll�Ƿ���ڣ�
	WORD *  lpFlagg= (WORD *)((BYTE*)lpNT - 2);	//��־���ļ����Ƿ�Ķ���;
	//��λ���һ������
	for(int i = 1; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		lpBlock++;
		if(lpBlock->PointerToRawData > dwFileOffset)
			dwFileOffset = lpBlock->PointerToRawData;
	}
	for(int i = 1; i < lpNT->FileHeader.NumberOfSections; i++)
	{
		if(lpBlock->PointerToRawData == dwFileOffset)
			break;
		lpBlock--;
	}
	//Ϊ���һ����������д���ԣ���Ϊ�½���IAT���������һ�������У�������Ҫ��д���ԣ�
	if(lpBlock->Characteristics < 0x80000000)
		lpBlock->Characteristics = lpBlock->Characteristics | 0x80000000;
	
	int FlagFirstNot = 0;					//�ж��ǲ��ǵ��뺯����DLL�Ѿ�����
	BYTE * lpDword = (BYTE *)lpMapFile;
	lpDword =lpDword + lpBlock->PointerToRawData ;		//���һ�������Ӧ���ļ�ƫ�ƣ��ڴ�ӳ���ļ���ʽ��





	//��������ӵ�Dll�Ƿ��Ѵ���
	GetDlgItemText(hDlg, IDC_EDIT1, szDllName, sizeof(szDllName));
	TCHAR 		lpIDName[100] = {0};
	while(lpIID->Name != 0)
	{
		memset(lpIDName, 0, sizeof(lpIDName));
		for(int i = 0;;i++)
		{
			if(((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->Name))[i] == 0)
			{
				((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->Name))[i] = 0;
				break;
			}
			lpIDName[i] = ((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->Name))[i];
		}
		if(wcscmp(lpIDName, szDllName) == 0)
		{
			bFlag = TRUE;
			lpIIDTrue = lpIID;
			break;
		}

		lpIID++;

	}






	lpIID	= (IMAGE_IMPORT_DESCRIPTOR *)lpMapFile;
	lpIID	= (IMAGE_IMPORT_DESCRIPTOR *)((BYTE *)lpIID + _RVAtoFileOffset(lpMapFile,lpData->VirtualAddress));



	if(bFlag == FALSE)
	{
		if(lpFlagg[0] != 0xF0F0)		//�״����ӽ���dosͷ������һ��0XF0F0�ı�־��������ļ�Ϊ�򿪹�����
		{
			lpDword = lpDword + lpBlock->SizeOfRawData;
			RtlMoveMemory(lpDword, lpIID, lpData->Size);
			//lpBlock->Misc.VirtualSize = lpBlock->Misc.VirtualSize + 0x200;
			lpBlock->SizeOfRawData = lpBlock->SizeOfRawData + 0x200;
			lpData->VirtualAddress = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpDword - int(lpMapFile)));
			lpData->Size	=	lpData->Size + sizeof(IMAGE_IMPORT_DESCRIPTOR);
			lpFlagg[0] = 0xF0F0;
		}
		else
		{
			lpDword = (BYTE *)lpIID;
			int n = 0;
			BYTE * f =(BYTE *)lpIID + 0x200;
			while(f[0] != 0)
			{
				n++;
				f = f + 0x100;
			}
			if( n/2 == 0)
				lpBlock->SizeOfRawData = lpBlock->SizeOfRawData + 0x200;
		}
		
	}
	else
	{
		lpDword = (BYTE *)lpIID; 
	}
	BYTE * lpName = lpDword +  0x100;				//�洢����ӵ�DLL�������ַ��ȵȣ����е�IID��һ�����Ϊ0x100��
	lpIID =(IMAGE_IMPORT_DESCRIPTOR *)lpDword; 
	
	

	if(bFlag == FALSE)
	{
		while(1)
		{
			if(lpIID->Name == 0)
				break;
			lpIID++;
		}
	
		while(lpName[0] != 0)
			lpName = lpName + 0x100;
		for(int i = 0;;i++)
		{
			if(szDllName[i] == 0)
				break;
			lpName[i] = szDllName[i];
		}
		lpIID->Name = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
		lpName = lpName + 0x50;
		lpName[0] = 0;					//���������ʱû��
		lpName[1] = 0;
		lpName = lpName + 2;
		
		LVITEM       lvItem;						//ListView�б�������
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask		 = LVIF_TEXT;
		lvItem.iItem	 = num - 1;
		lvItem.cchTextMax = 128;
		lvItem.pszText	 = szProcName; 
		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_GETITEMTEXT,lvItem.iItem,(LPARAM)&lvItem);
	
		for(int i = 0;;i++)
		{
			if(szProcName[i] == 0)
				break;
			lpName[i] = szProcName[i];
		}
		lpName = lpName - 2;
		//////////////////////////////////
		int ProcRVA = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
		lpName = lpName - 0x20;
		((DWORD *)lpName)[0] = ProcRVA;
	
		lpIID->FirstThunk = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
		lpName = lpName - 0x20;
		((DWORD *)lpName)[0] = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile) + 0x40));
		lpIID->OriginalFirstThunk = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
	}
	else if(bFlag == TRUE)
	{
		lpIID = lpIIDTrue;

		LVITEM       lvItem;						//ListView�б�������
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.mask		 = LVIF_TEXT;
		lvItem.iItem	 = num - 1;
		lvItem.cchTextMax = 128;
		lvItem.pszText	 = szProcName; 
		SendDlgItemMessage(hDlg,IDC_LIST1,LVM_GETITEMTEXT,lvItem.iItem,(LPARAM)&lvItem);			//��ȡlistcontral�ĵ�lvitem.iItem�е�����

		int FlagBreak = 0;
		for(int i = 0;;i++)
		{	
			if( ((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i] == 0)
			{
				i--;
				for(int x = 0;;x++)
				{
					if(((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile,((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i]))[x] == 0);
					{
						if(((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile,((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i]))[x + 1] == 0)
						{
							if(((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile,((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i]))[x + 2] == 0)
							{	
								x++;
								lpName = ((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile,((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i])) + x;
								FlagBreak = 1;
								break;
							}
						}
					}
				}
			}
			if(FlagBreak == 1)
				break;
		}

		lpName[0] = 0;
		lpName[1] = 0;
		lpName = lpName + 2; 
		for(int i = 0;;i++)
		{
			if(szProcName[i] == 0)
				break;
			lpName[i] = szProcName[i];
		}
		lpName = lpName - 2;
	for(int i = 0;;i++)
		if(((DWORD *)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i] == 0)
		{
			((DWORD*)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->OriginalFirstThunk)))[i] = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
			((DWORD*)((BYTE *)lpMapFile + _RVAtoFileOffset(lpMapFile, lpIID->FirstThunk)))[i]		  = _FileOffsetToRVA(lpMapFile,int((BYTE *)lpName - int(lpMapFile)));
			break;
		}
	}
	
	return 0;
}




//����������ڹ���
BOOL CALLBACK _ProcDlgAdd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static	LPVOID lpMapFile;
	TCHAR	szProcName[256] = {0};
	LVITEM  lvItem;
	int		num;
	switch(message)
	{

	case WM_COMMAND:
		switch(short int(wParam))
		{
		case IDC_BUTTON1:
			
			memset(&lvItem, 0, sizeof(lvItem));
			num = SendDlgItemMessage(hWnd,IDC_LIST1,LVM_GETITEMCOUNT,0,(LPARAM)&lvItem);			
			_SelectProc(lpMapFile, hWnd, num);
			break;
		case IDC_BUTTON2:						
								
			memset(&lvItem, 0, sizeof(lvItem));
			num = SendDlgItemMessage(hWnd,IDC_LIST1,LVM_GETITEMCOUNT,0,(LPARAM)&lvItem);			//��ȡlistcontral����
			if(num == 0)
				MessageBox(NULL, TEXT("�㻹û��ѡ����ĺ�����"), NULL, MB_OK);
			else
			{	
				while(num != 0)
				{
					_AddImport(lpMapFile, hWnd, num);
					num--;
				}
				memset(&lvItem, 0, sizeof(lvItem));
				SendDlgItemMessage(hWnd, IDC_LIST1, LVM_DELETEALLITEMS,0, (LPARAM)&lvItem);				//ɾ��ȫ����listcontral��
			}
			break;
		case IDC_BUTTON3:
			EndDialog(hWnd, NULL);
			break;
		}
		break;
	case WM_INITDIALOG:
		lpMapFile = (LPVOID)lParam;
		break;
	case WM_CLOSE:
		EndDialog(hWnd, NULL);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}




//bug�Ͳ���


//����༭ѡ��༭�ļ����ڴ�ӳ���ļ������ļ�һ��������0x1600����

//���е�IID��һ�����ռ�ռ�Ϊ0x100

//��Ҫѡ��PE�༭���ٴ��ļ����ܱ༭pE�ļ�