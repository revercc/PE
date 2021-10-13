#include"main.h"
//显示基本信息
BOOL _stdcall _ShowBasicInformation(LPVOID lpMapFile,HWND hWinMain)
{
	TCHAR szBuffer[256] = {0};
	IMAGE_DOS_HEADER * lpDosMZ;
	lpDosMZ				= (IMAGE_DOS_HEADER *)lpMapFile;
	TCHAR szNotPE[]		= TEXT("文件不是有效的PE文件！");
	TCHAR szDosFile[]	= TEXT("此文件为16位汇编编写的可执行文件！");
	IMAGE_NT_HEADERS * lpNT = NULL;
	char  szToHex[]		= "%x";
	if(lpDosMZ->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(NULL,szNotPE,TEXT("Chek_PE"),MB_OK);
		return FALSE;
	}
	lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + (lpDosMZ->e_lfanew));		//注意指针在操作的时候加1是加的自身所指结构的整数倍
	if(lpNT->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL,szDosFile,TEXT("Chek_PE"),MB_OK);
		return FALSE;
	}
	SetDlgItemInt(hWinMain,IDC_EDIT10,lpNT->FileHeader.NumberOfSections,FALSE);			//区块数目
	wsprintf(szBuffer,TEXT("%x"),lpNT->FileHeader.Characteristics);
	SetDlgItemText(hWinMain,IDC_EDIT7,szBuffer);										//文件属性
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.MajorImageVersion);
	SetDlgItemText(hWinMain,IDC_EDIT6,szBuffer);										//链接器版本号
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.AddressOfEntryPoint,FALSE);
	SetDlgItemText(hWinMain,IDC_EDIT3,szBuffer);										//入口地址
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.ImageBase);
	SetDlgItemText(hWinMain,IDC_EDIT9,szBuffer);										//建议装载地址
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.SizeOfImage);
	SetDlgItemText(hWinMain,IDC_EDIT8,szBuffer);										//内存映像尺寸
	if(0 == lpNT->OptionalHeader.Subsystem)			
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("未知!"));								//子系统
	else if(1 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("不需要子系统！"));
	else if(2 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("图形接口子系统(GUI)"));
	else if(3 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("字符子系统(CUI)"));
	else if(5 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("OS/2字符子系统"));
	else if(7 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("POSIX字符子系统"));
	else if(9 == lpNT->OptionalHeader.Subsystem )
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("Windows CE图形界面"));
	if(lpNT->FileHeader.Characteristics & IMAGE_FILE_DLL)									//基地址
		SetDlgItemText(hWinMain,IDC_EDIT4,TEXT("不确定"));
	else
	{
		wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.ImageBase);
		SetDlgItemText(hWinMain,IDC_EDIT4,szBuffer);
	}
	return TRUE;
}



BOOL _stdcall _CleanPE(HWND hWinMain)
{
	TCHAR	szBuffer[10] = {0};
	SetDlgItemText(hWinMain,IDC_EDIT10,szBuffer);										
	SetDlgItemText(hWinMain,IDC_EDIT7,szBuffer);										
	SetDlgItemText(hWinMain,IDC_EDIT6,szBuffer);										
	SetDlgItemText(hWinMain,IDC_EDIT3,szBuffer);											
	SetDlgItemText(hWinMain,IDC_EDIT9,szBuffer);										
	SetDlgItemText(hWinMain,IDC_EDIT8,szBuffer);										
	SetDlgItemText(hWinMain,IDC_EDIT5,szBuffer);
	SetDlgItemText(hWinMain,IDC_EDIT4,szBuffer);
	return 0;
}