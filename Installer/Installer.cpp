// Installer.cpp : �w�q���ε{�����i�J�I�C
//
#include <windows.h>
#include <imm.h>
#include "Installer.h"
#include <winreg.h>

// �����ܼ�:
HINSTANCE hInst;								// �ثe�������

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	char path[MAX_PATH];
	GetSystemDirectory(path, MAX_PATH);
	SetCurrentDirectory(path);

	HKL kl;

	if( strstr( lpCmdLine, "/uninstall" ) )
	{
/*		HKEY hk = NULL;
		if( ERROR_SUCCESS != RegOpenKey( HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ChewingIME", &hk ) )
			return -1;
		RegCloseKey(hkey);
*/		
	}
	else
	{
		kl = ImmInstallIME( "Chewing.ime", "���� (�c��) - �s�ŭ���J�k" );
		char regpath[256];
		wsprintf( regpath, "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\%X", kl );
	}


	return 0;
}