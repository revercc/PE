#include"main.h"
//��ʾ������Ϣ
BOOL _stdcall _ShowBasicInformation(LPVOID lpMapFile,HWND hWinMain)
{
	TCHAR szBuffer[256] = {0};
	IMAGE_DOS_HEADER * lpDosMZ;
	lpDosMZ				= (IMAGE_DOS_HEADER *)lpMapFile;
	TCHAR szNotPE[]		= TEXT("�ļ�������Ч��PE�ļ���");
	TCHAR szDosFile[]	= TEXT("���ļ�Ϊ16λ����д�Ŀ�ִ���ļ���");
	IMAGE_NT_HEADERS * lpNT = NULL;
	char  szToHex[]		= "%x";
	if(lpDosMZ->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(NULL,szNotPE,TEXT("Chek_PE"),MB_OK);
		return FALSE;
	}
	lpNT = (IMAGE_NT_HEADERS *)lpMapFile;
	lpNT = (IMAGE_NT_HEADERS *)((BYTE *)lpNT + (lpDosMZ->e_lfanew));		//ע��ָ���ڲ�����ʱ���1�Ǽӵ�������ָ�ṹ��������
	if(lpNT->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL,szDosFile,TEXT("Chek_PE"),MB_OK);
		return FALSE;
	}
	SetDlgItemInt(hWinMain,IDC_EDIT10,lpNT->FileHeader.NumberOfSections,FALSE);			//������Ŀ
	wsprintf(szBuffer,TEXT("%x"),lpNT->FileHeader.Characteristics);
	SetDlgItemText(hWinMain,IDC_EDIT7,szBuffer);										//�ļ�����
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.MajorImageVersion);
	SetDlgItemText(hWinMain,IDC_EDIT6,szBuffer);										//�������汾��
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.AddressOfEntryPoint,FALSE);
	SetDlgItemText(hWinMain,IDC_EDIT3,szBuffer);										//��ڵ�ַ
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.ImageBase);
	SetDlgItemText(hWinMain,IDC_EDIT9,szBuffer);										//����װ�ص�ַ
	wsprintf(szBuffer,TEXT("%x"),lpNT->OptionalHeader.SizeOfImage);
	SetDlgItemText(hWinMain,IDC_EDIT8,szBuffer);										//�ڴ�ӳ��ߴ�
	if(0 == lpNT->OptionalHeader.Subsystem)			
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("δ֪!"));								//��ϵͳ
	else if(1 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("����Ҫ��ϵͳ��"));
	else if(2 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("ͼ�νӿ���ϵͳ(GUI)"));
	else if(3 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("�ַ���ϵͳ(CUI)"));
	else if(5 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("OS/2�ַ���ϵͳ"));
	else if(7 == lpNT->OptionalHeader.Subsystem)
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("POSIX�ַ���ϵͳ"));
	else if(9 == lpNT->OptionalHeader.Subsystem )
		SetDlgItemText(hWinMain,IDC_EDIT5,TEXT("Windows CEͼ�ν���"));
	if(lpNT->FileHeader.Characteristics & IMAGE_FILE_DLL)									//����ַ
		SetDlgItemText(hWinMain,IDC_EDIT4,TEXT("��ȷ��"));
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