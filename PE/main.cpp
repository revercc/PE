#include "main.h"
HWND		hWinMain;

BOOL _stdcall _CleanPE(HWND hWinMain);
BOOL _stdcall _ShowBasicInformation(LPVOID lpMapFile,HWND);
BOOL CALLBACK _ProcDlgToRVA(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgAdd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgRclocate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgExport(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgImport(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgData(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProcDlgBlock(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _DialogMain(HWND, UINT, WPARAM, LPARAM) ;
BOOL CALLBACK _ProcDlgHelp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
BOOL OpenPE = FALSE;			//��־�Ƿ�ɹ���PE�ļ�
int WINAPI	WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{	
	MSG		stMsg;
	hWinMain = CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG2),NULL,_DialogMain,NULL);
	while(1)
	{
		if(0 == GetMessage(&stMsg,NULL,0,0))
			break;
		TranslateMessage(&stMsg);
		DispatchMessage(&stMsg);
	}
	return stMsg.wParam;
}


//�����ڴ��ڹ���
BOOL CALLBACK _DialogMain(HWND hWnd, UINT message,WPARAM wParam,LPARAM lParam)
{
	OPENFILENAME	stOF;
	static HANDLE	hFile;				//�ļ����
	static HANDLE	hMapFile;			//�ڴ�ӳ���ļ�������
	static LPVOID	lpMapFile;			//�ڴ�ӳ���ļ�ָ��
	HICON			hIcon;
	RtlZeroMemory(&stOF,sizeof(stOF));
	TCHAR			szFileName[256] = {0};
	


	switch(message)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_BUTTON1:
			memset(&stOF,0,sizeof(stOF));
			stOF.lStructSize	= sizeof(stOF);
			stOF.hwndOwner		= hWinMain;
			stOF.lpstrFilter	= TEXT("��ִ���ļ�(*.exe,*.dll)\0*.exe;*.dll\0�����ļ�(*.*)\0*.*\0\0");
			stOF.lpstrFile		= szFileName;												//�˻������������
			stOF.nMaxFile		= MAX_PATH;
			stOF.Flags			= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			if(GetOpenFileName(&stOF))
			{
				SetDlgItemText(hWnd,IDC_EDIT1,szFileName);
				if(OpenPE == TRUE)
				{
					UnmapViewOfFile(lpMapFile);		//����ӳ��
					CloseHandle(hMapFile);			//�ر��ڴ�ӳ�������
					CloseHandle(hFile);				//�ر��ļ�
				}

				hFile		= CreateFile(szFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
				if(IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED)
				{
					hMapFile	= CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
					lpMapFile	= MapViewOfFile(hMapFile,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
					IMAGE_DOS_HEADER *			lpDosMZ	= (IMAGE_DOS_HEADER *)lpMapFile;
					IMAGE_NT_HEADERS *			lpNT	= (IMAGE_NT_HEADERS *)lpMapFile;
					lpNT								= (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
					WORD *  lpFlagg= (WORD *)((BYTE*)lpNT - 4);
					if(lpFlagg[0] != 0xE0E0)									//ͨ����Dosͷ�����0xE0E0��־�������ļ��Ƿ��޸Ĺ���
					{
						UnmapViewOfFile(lpMapFile);		//����ӳ��
						CloseHandle(hMapFile);			//�ر��ڴ�ӳ�������
						hMapFile	= CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,GetFileSize(hFile, NULL) + 0x1600,NULL);			//�����ڴ�ӳ���ļ�
						lpMapFile	= MapViewOfFile(hMapFile,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
						IMAGE_DOS_HEADER *			lpDosMZ	= (IMAGE_DOS_HEADER *)lpMapFile;
						IMAGE_NT_HEADERS *			lpNT	= (IMAGE_NT_HEADERS *)lpMapFile;
						lpNT								= (IMAGE_NT_HEADERS *)((BYTE *)lpNT + lpDosMZ->e_lfanew);
						WORD *  lpFlagg= (WORD *)((BYTE*)lpNT - 4);
						lpFlagg[0] = 0xE0E0;
					}
				}
				else
				{
					hMapFile	= CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
					lpMapFile	= MapViewOfFile(hMapFile,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
				}
				if(_ShowBasicInformation(lpMapFile,hWinMain))
				{
					OpenPE = TRUE;
					if(IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED)				
						EnableMenuItem(GetMenu(hWnd), ID_40008, MF_ENABLED);
					EnableMenuItem(GetMenu(hWnd), ID_40006, MF_ENABLED);
					
				}
				else
				{
					OpenPE = FALSE;
					EnableMenuItem(GetMenu(hWnd), ID_40006, MF_GRAYED);
					EnableMenuItem(GetMenu(hWnd), ID_40008, MF_GRAYED);
					UnmapViewOfFile(lpMapFile);		//����ӳ��
					CloseHandle(hMapFile);			//�ر��ڴ�ӳ�������
					CloseHandle(hFile);				//�ر��ļ�
					_CleanPE(hWinMain);				

				}
			}
			break;
		case IDC_BUTTON6:
			if(OpenPE == TRUE)
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG7),hWnd, _ProcDlgRclocate,(LPARAM)lpMapFile);
			break;
		case IDC_BUTTON5:
			if(OpenPE == TRUE)
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG6),hWnd,_ProcDlgExport,(LPARAM)lpMapFile);
			break;
		case IDC_BUTTON3:
			if(OpenPE == TRUE)
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG3),hWnd,_ProcDlgBlock,(LPARAM)lpMapFile);
			break;
		case IDC_BUTTON4:
			if(OpenPE == TRUE)
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG5),hWnd,_ProcDlgImport,(LPARAM)lpMapFile);
			break;
		case IDC_BUTTON2:
			if(OpenPE == TRUE)
				DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG4),hWnd,_ProcDlgData,(LPARAM)lpMapFile);
			break;
		case ID_40006:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DLGTORVA),hWnd,_ProcDlgToRVA,(LPARAM)lpMapFile);
			
			break;
		case ID_40008:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOGADD),hWnd, _ProcDlgAdd, (LPARAM)lpMapFile);
			break;
		case ID_40001:
			DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hWnd,_ProcDlgHelp,NULL);
			break;

		case ID_40003:
			SendMessage(hWnd,WM_CLOSE,NULL,NULL);
			break;
		}
		break;
	case WM_DROPFILES:
		DragQueryFile((HDROP)wParam, 0, szFileName, sizeof(szFileName));	//����ļ���
		SetDlgItemText(hWnd,IDC_EDIT1,szFileName);
		if(OpenPE == TRUE)
		{
			UnmapViewOfFile(lpMapFile);		//����ӳ��
			CloseHandle(hMapFile);			//�ر��ڴ�ӳ�������
			CloseHandle(hFile);				//�ر��ļ�
		}
		hFile		= CreateFile(szFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED)
			hMapFile	= CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,GetFileSize(hFile, NULL) + 0x600,NULL);			//�����ڴ�ӳ���ļ�
		else
			hMapFile	= CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
		lpMapFile	= MapViewOfFile(hMapFile,FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);
		if(_ShowBasicInformation(lpMapFile,hWinMain))
		{
			OpenPE = TRUE;
			if(IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED)
				EnableMenuItem(GetMenu(hWnd), ID_40008, MF_ENABLED);
			EnableMenuItem(GetMenu(hWnd), ID_40006, MF_ENABLED);
			
		}
		else
		{
			OpenPE = FALSE;
			EnableMenuItem(GetMenu(hWnd), ID_40006, MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd), ID_40008, MF_GRAYED);
			UnmapViewOfFile(lpMapFile);		//����ӳ��
			CloseHandle(hMapFile);			//�ر��ڴ�ӳ�������
			CloseHandle(hFile);				//�ر��ļ�
			_CleanPE(hWinMain);				

		}
		break;
	case WM_INITDIALOG:
		hIcon = LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hWnd,WM_SETICON,ICON_BIG,LPARAM(hIcon));
		
		

		break;
	case WM_CLOSE:
		if(IDOK == MessageBox(NULL,TEXT("ȷ��Ҫ�˳���"),TEXT("Chek_PE"),MB_OKCANCEL | MB_ICONEXCLAMATION))
			DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		
		UnmapViewOfFile(lpMapFile);			//����ӳ��
		CloseHandle(hMapFile);				//�ر��ڴ�ӳ���ļ�������
		CloseHandle(hFile);					//�ر��ļ�
		PostQuitMessage(NULL);
		break;
	
	default:
		return FALSE;
	}
	return TRUE;
}



//�������ڴ��ڹ���
BOOL CALLBACK _ProcDlgHelp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_COMMAND:
		if(wParam == IDC_BUTTON1)
			EndDialog(hWnd,NULL);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}




