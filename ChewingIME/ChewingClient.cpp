// ChewingClient.cpp: implementation of the ChewingClient class.
//
//////////////////////////////////////////////////////////////////////

#include "ChewingClient.h"
#include <string.h>
#include <tchar.h>
#include ".\chewingclient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChewingClient::ChewingClient( int kbLayout, bool spaceAsSel, const char* selKeys, bool AdvAfterSel, bool EscCleanAllBuf )
	: serverWnd(NULL), chewingID(0), sharedMem(INVALID_HANDLE_VALUE)
	, spaceAsSelection(spaceAsSel)
	, keyLayout(kbLayout)
    , advAfterSelection(AdvAfterSel)
	, escCleanAllBuf( EscCleanAllBuf )
{
	ConnectServer();
	SelKey((char*)selKeys);
    SetAdvanceAfterSelection(advAfterSelection);
	SetEscCleanAllBuf( escCleanAllBuf );
    pSelKeys = (char*)selKeys;
}

ChewingClient::~ChewingClient()
{
	SendMessage( serverWnd, ChewingServer::cmdRemoveClient, 0, chewingID );
    if ( sharedMem==INVALID_HANDLE_VALUE )
    {
        CloseHandle(sharedMem);
    }
}

void ChewingClient::SetKeyboardLayout(int kb)
{	SendMessage( serverWnd, ChewingServer::cmdSetKeyboardLayout, kb, chewingID);	}

void ChewingClient::SetHsuSelectionKeyType(int type)
{	SendMessage( serverWnd, ChewingServer::cmdSetHsuSelectionKeyType, type, chewingID);	}

int ChewingClient::Space()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdSpace, 0, chewingID);	}

int ChewingClient::Esc()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdEsc, 0, chewingID);	}

int ChewingClient::Enter()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdEnter, 0, chewingID);	}

int ChewingClient::Delete()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdDelete, 0, chewingID);	}

int ChewingClient::Backspace()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdBackspace, 0, chewingID);	}

int ChewingClient::Tab()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdTab, 0, chewingID);	}

int ChewingClient::ShiftLeft()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdShiftLeft, 0, chewingID);	}

int ChewingClient::ShiftRight()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdShiftRight, 0, chewingID);	}

int ChewingClient::ShiftSpace()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdShiftSpace, 0, chewingID);	}

int ChewingClient::Right()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdRight, 0, chewingID);	}

int ChewingClient::Left()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdLeft, 0, chewingID);	}

int ChewingClient::Up()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdUp, 0, chewingID);	}

int ChewingClient::Down()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdDown, 0, chewingID);	}

int ChewingClient::Home()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdHome, 0, chewingID);	}

int ChewingClient::End()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdEnd, 0, chewingID);	}

int ChewingClient::Capslock()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCapslock, 0, chewingID);	}

int ChewingClient::Key(unsigned int code)
{	return (int)SendMessage( serverWnd, ChewingServer::cmdKey, code, chewingID);	}

int ChewingClient::CtrlNum(unsigned int code)
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCtrlNum, code, chewingID);	}

int ChewingClient::NumPad(unsigned int code)
{	return (int)SendMessage( serverWnd, ChewingServer::cmdNumPad, code, chewingID);	}

int ChewingClient::DoubleTab()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdDoubleTab, 0, chewingID);	}

// Return the i-th selection key, i >= 0.
char ChewingClient::SelKey(int i)
{	return (char)SendMessage( serverWnd, ChewingServer::cmdGetSelKey, i, chewingID);	}

void ChewingClient::SetAdvanceAfterSelection(bool bDo)
{
    SendMessage( serverWnd, ChewingServer::cmdAdvanceAfterSelection, 
                (bDo==true)?1 :0, chewingID);
}

void ChewingClient::SetEasySymbolInput(bool bSet)
{
    SendMessage( serverWnd, ChewingServer::cmdEasySymbolInput, 
                (bSet==true)?1 :0, chewingID);
}

void ChewingClient::SelKey(char* selkey)
{
    sharedMem = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, filemapName);
    if ( sharedMem==NULL )
    {
        sharedMem = INVALID_HANDLE_VALUE; 
        return;
    }

    char* pbuf = (char*)MapViewOfFile( sharedMem, FILE_MAP_WRITE, 
								0, 0, CHEWINGSERVER_BUF_SIZE );
    if ( pbuf!=NULL )
    {
	    strcpy( pbuf, selkey );
	    UnmapViewOfFile( pbuf );
    }
	CloseHandle(sharedMem);
    sharedMem = INVALID_HANDLE_VALUE;

    SendMessage( serverWnd, ChewingServer::cmdSetSelKey, 0, chewingID);
}

char* ChewingClient::ZuinStr()
{
	int len = (int)SendMessage( serverWnd, ChewingServer::cmdZuinStr, 0, chewingID);
	return (char*)GetDataFromSharedMem(len);
}

char* ChewingClient::CommitStr()
{
	int len = (int)SendMessage( serverWnd, ChewingServer::cmdCommitStr, 0, chewingID);
	return (char*)GetDataFromSharedMem(len);
}

int   ChewingClient::CommitReady()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCommitReady, 0, chewingID);	}

char* ChewingClient::Buffer()
{
	int len = (int)SendMessage( serverWnd, ChewingServer::cmdBuffer, 0, chewingID);
	return (char*)GetDataFromSharedMem(len);
}

int   ChewingClient::BufferLen()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdBufferLen, 0, chewingID);	}

int ChewingClient::CursorPos()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCursorPos, 0, chewingID);	}

int ChewingClient::KeystrokeIgnore()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdKeystrokeIgnore, 0, chewingID);	}

int ChewingClient::ChineseMode()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdChineseMode, 0, chewingID);	}

// CandidateWindow-related routines
int ChewingClient::Candidate()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCandidate, 0, chewingID);	}

int ChewingClient::ChoicePerPage()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdChoicePerPage, 0, chewingID);	}

int ChewingClient::TotalChoice()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdTotalChoice, 0, chewingID);	}

int ChewingClient::TotalPage()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdTotalPage, 0, chewingID);	}

int ChewingClient::CurrentPage()
{	return (int)SendMessage( serverWnd, ChewingServer::cmdCurrentPage, 0, chewingID);	}

// Return the i-th selection wchar_t, i >= 0.
char* ChewingClient::Selection(int i)
{
	int len = (int)SendMessage( serverWnd, ChewingServer::cmdSelection, i, chewingID);
	return (char*)GetDataFromSharedMem(len);
}

void ChewingClient::SetFullShape(bool full)
{	SendMessage( serverWnd, ChewingServer::cmdSetFullShape, full, chewingID);	}

bool ChewingClient::GetFullShape(void)
{	return !!SendMessage( serverWnd, ChewingServer::cmdGetFullShape, 0, chewingID);	}

void ChewingClient::SetSpaceAsSelection(bool spaceAsSelection)
{	SendMessage( serverWnd, ChewingServer::cmdSetSpaceAsSelection, spaceAsSelection, chewingID);	}

void ChewingClient::SetEscCleanAllBuf( bool escCleanAllBuf ) {
	SendMessage( serverWnd, ChewingServer::cmdSetEscCleanAllBuf, escCleanAllBuf, chewingID );
}

unsigned char* ChewingClient::GetDataFromSharedMem(int len)
{
	unsigned char* data = NULL;

	if ( len<=0 ){
		return	NULL;
	}

	sharedMem = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, filemapName);
    if ( sharedMem==NULL )
    {
        sharedMem = INVALID_HANDLE_VALUE; 
        return  NULL;
    }

	char* buf = (char*)MapViewOfFile( sharedMem, FILE_MAP_READ, 0, 0, CHEWINGSERVER_BUF_SIZE );
	if( buf )
	{
		data = (unsigned char*)calloc( len, sizeof(unsigned char) );
		memcpy( data, buf, len );
		UnmapViewOfFile(buf);
	}
    CloseHandle(sharedMem);
    sharedMem = INVALID_HANDLE_VALUE;
	return data;
}

char* _gen_event_name(char *buf, int szbuf, const char *prefix)
{
	char temp[512]={'0'};
	DWORD sztemp = sizeof(temp);
	GetUserName(temp, &sztemp);

	strncpy(buf, prefix, szbuf);
	strncat(buf, "_", szbuf);
	strncat(buf, temp, szbuf);
	buf[szbuf-1] = '\0';
	return	buf;
}

void ChewingClient::ConnectServer(void)
{
	char tempname[512];
	_gen_event_name(tempname, sizeof(tempname), chewingServerClassName);
	serverWnd = FindWindow( tempname, NULL );

	if( ! serverWnd )
	{
		char evt_name[512];
		_gen_event_name(evt_name, sizeof(evt_name), "Local\\ChewingServerEvent");
		LPCTSTR evtname = evt_name;

		DWORD osVersion = GetVersion();
 		DWORD major = (DWORD)(LOBYTE(LOWORD(osVersion)));
		DWORD minor =  (DWORD)(HIBYTE(LOWORD(osVersion)));
		if( osVersion >= 0x80000000 || major <= 4 )	// Windows 9x or Windows NT 4
			evtname += 6;	// remove prfix "Local\\"

		HANDLE evt = CreateEvent( NULL, TRUE, FALSE, evtname );
		TCHAR server_path[MAX_PATH];
		GetSystemDirectory( server_path, MAX_PATH );
		_tcscat( server_path, _T("\\IME\\Chewing\\ChewingServer.exe") );
		ShellExecute( NULL, "open", server_path, NULL, NULL, SW_HIDE );
		WaitForSingleObject( evt, 10000 );
		CloseHandle(evt);
		serverWnd = FindWindow( tempname, NULL );
	}

	chewingID = SendMessage( serverWnd, ChewingServer::cmdAddClient, 0, 0 );
	GetWindowText( serverWnd, tempname, sizeof(tempname) );
	_gen_event_name(filemapName, sizeof(filemapName), tempname);

	SetSpaceAsSelection(spaceAsSelection);
	SetKeyboardLayout(keyLayout);
}

int ChewingClient::ShowMsgLen(void)
{	return (int)SendMessage( serverWnd, ChewingServer::cmdShowMsgLen, 0, chewingID);	}

char* ChewingClient::ShowMsg(void)
{
	int len = (int)SendMessage( serverWnd, ChewingServer::cmdShowMsg, 0, chewingID);
	return (char*)GetDataFromSharedMem(len);
}

void ChewingClient::SetAddPhraseForward(bool add_forward)
{	SendMessage( serverWnd, ChewingServer::cmdSetAddPhraseForward, add_forward, chewingID);	}

int ChewingClient::GetAddPhraseForward(void)
{	return (int)SendMessage( serverWnd, ChewingServer::cmdGetAddPhraseForward, 0, chewingID);	}

unsigned int ChewingClient::EchoFromServer()
{	return (unsigned int)SendMessage( serverWnd, ChewingServer::cmdEcho, 0, chewingID);	}

bool ChewingClient::CheckServer()
{
    if ( ChewingClient::EchoFromServer()==(~((unsigned int)chewingID)) )
    {
        return  true;
    }
    ConnectServer();
	SelKey((char*)pSelKeys);
    return  false;
}

void ChewingClient::SetCandPerPage(int len)
{	SendMessage( serverWnd, ChewingServer::cmdSetCandPerPage, len, chewingID);	}

unsigned char* ChewingClient::GetIntervalArray(int& len) {
	len = (int)SendMessage( serverWnd, ChewingServer::cmdIntervalArray, 0, chewingID );
	return GetDataFromSharedMem( len );
}

void ChewingClient::ReloadSymbolTable(void)
{
	SendMessage( serverWnd, ChewingServer::cmdReloadSymbolTable, 0, 0);
}

