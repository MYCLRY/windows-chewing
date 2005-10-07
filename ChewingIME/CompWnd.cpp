#include ".\compwnd.h"
#include "ChewingIME.h"
#include "DrawUtil.h"
#include "CandWnd.h"
#include "CompStr.h"
#include "IMCLock.h"
#include "IMEUI.h"
#include "IMEUILock.h"

CompWnd::CompWnd(void)
{
	font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject( font, sizeof(lf), &lf);
	lf.lfHeight = 16;
	font = CreateFontIndirect( &lf );
}

CompWnd::~CompWnd(void)
{
	DeleteObject( font );
}

BOOL CompWnd::registerClass(void)
{
	WNDCLASSEX wc;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS| CS_IME;
	wc.lpfnWndProc		= (WNDPROC)CompWnd::wndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= g_dllInst;
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hIcon			= NULL;
	wc.lpszMenuName		= (LPTSTR)NULL;
	wc.lpszClassName	= g_compWndClass;
	wc.hbrBackground	= NULL;
	wc.hIconSm			= NULL;
	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	return TRUE;
}

LRESULT CompWnd::wndProc( HWND hwnd , UINT msg, WPARAM wp , LPARAM lp)
{
	HIMC hIMC = getIMC(hwnd);
	IMCLock imc( hIMC );
	IMEUILock lock( GetParent(hwnd) );
	IMEUI* ui = lock.getIMEUI();
	switch (msg)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint( hwnd, &ps );
				if( ui )
					ui->compWnd.onPaint( imc, ps );
				EndPaint(hwnd, &ps);
				break;
			}
		case WM_ERASEBKGND:
			return TRUE;
			break;
		case WM_LBUTTONDOWN:
			if( ui )
				ui->compWnd.onLButtonDown(wp, lp);
			break;
		case WM_MOUSEMOVE:
			if( ui )
				ui->compWnd.onMouseMove(wp, lp);
			break;
		case WM_LBUTTONUP:
			if( ui )
				ui->compWnd.onLButtonUp(wp, lp);
			break;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
		default:
			if (!IsImeMessage(msg))
				return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}



void CompWnd::onPaint(IMCLock& imc, PAINTSTRUCT& ps)
{
	string compStr = getDisplayedCompStr(imc);
	int cursorPos = getDisplayedCursorPos(imc);

	HFONT oldFont;
	RECT rc;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

	GetClientRect( hwnd, &rc );
	HDC memdc = CreateCompatibleDC( ps.hdc );
	HBITMAP membmp = CreateCompatibleBitmap( ps.hdc, rc.right, rc.bottom );
	HGDIOBJ oldbmp = SelectObject( memdc, membmp );

	InflateRect( &rc, -1, -1 );

	oldFont = (HFONT)SelectObject(memdc, font);
	if( !compStr.empty() )
	{
		SetTextColor( memdc, GetSysColor( COLOR_WINDOWTEXT ) );
		SetBkColor( memdc, GetSysColor( COLOR_WINDOW ) );
		ExtTextOut( memdc, 1, 1, ETO_OPAQUE, &rc, compStr.c_str(), 
			compStr.length(), NULL);
//		int selstart = indexToXPos( CompSelStart );
//		int selend = indexToXPos( CompSelEnd );
		int cursor = indexToXPos( compStr, cursorPos );
//		BitBlt( memdc, selstart, 0, selend-selstart, rc.bottom, memdc, selstart, 0, NOTSRCCOPY );
		BitBlt( memdc, cursor, 0, 1, rc.bottom, memdc, cursor, 0, SRCINVERT );
	}

	InflateRect( &rc, 1, 1 );
	Draw3DBorder( memdc, &rc, GetSysColor(COLOR_3DFACE), 0/*GetSysColor(COLOR_3DDKSHADOW)*/);

	BitBlt( ps.hdc, ps.rcPaint.left, ps.rcPaint.top, 
		(ps.rcPaint.right-ps.rcPaint.left), 
		(ps.rcPaint.bottom-ps.rcPaint.top), 
		memdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

	SelectObject( memdc, oldbmp );
	DeleteObject( membmp );
	DeleteDC( memdc );

	SelectObject(memdc, oldFont);
}

void CompWnd::setFont(LOGFONT* lf)
{
	LOGFONT lf2;
	memcpy( &lf2, lf, sizeof( lf2) );

	if( abs(lf2.lfHeight) < 16 )
		lf2.lfHeight = 16;

	if( font )
		DeleteObject( font );

	font = CreateFontIndirect( &lf2 );
}

void CompWnd::getSize(int* w, int* h)
{
	HDC dc = GetDC( hwnd );
	HGDIOBJ oldfont = SelectObject( dc, font );
	SIZE size;
	string compStr = getDisplayedCompStr();
	GetTextExtentPoint( dc, compStr.c_str(), compStr.length(), &size );
	SelectObject( dc, oldfont );
	ReleaseDC( hwnd, dc );
	*w = size.cx + 4;
	*h = size.cy + 4;
}

/*
void CompWnd::setCursorPos(int pos)
{
	int x = indexToXPos( cursorPos );
	RECT rc;
	GetClientRect( hwnd, &rc );
	HDC dc = GetDC( hwnd );
	BitBlt( dc, x, 0, 1, rc.bottom, dc, x, 0, SRCINVERT );
	cursorPos = pos;
	x = indexToXPos( pos );
	BitBlt( dc, x, 0, 1, rc.bottom, dc, x, 0, SRCINVERT );
}
*/

int CompWnd::indexToXPos( string compStr, int idx)
{
	if( compStr.empty() || idx <=0 )
		return 2;
	HDC dc = GetDC( hwnd );
	HGDIOBJ oldfont = SelectObject( dc, font );
	SIZE size;
	GetTextExtentPoint( dc, compStr.c_str(), idx, &size );
	SelectObject( dc, oldfont );
	ReleaseDC( hwnd, dc );
	return size.cx + 2;
}



string CompWnd::getDisplayedCompStr(IMCLock& imc)
{
	CompStr* compStr = imc.getCompStr();
	if( compStr )
		return string( compStr->getCompStr() );
	return string("");
}

int CompWnd::getDisplayedCursorPos(IMCLock& imc)
{
	INPUTCONTEXT* ic = imc.getIC();
	if( !ic )
		return 0;
	CompStr* compStr = imc.getCompStr();
	if( !compStr )
		return 0;
	int cursorPos = compStr->getCursorPos();
	return cursorPos;
}

bool CompWnd::create(HWND imeUIWnd)
{
	hwnd = CreateWindowEx(0, g_compWndClass, NULL,
					WS_POPUP|WS_CLIPCHILDREN,
					0, 0, 0, 0, imeUIWnd, NULL, g_dllInst, NULL);
	return !!hwnd;
}

void CompWnd::getCandPos(IMCLock& imc, POINT* pt)
{
	RECT rc;
	GetWindowRect(hwnd, &rc );
	INPUTCONTEXT* ic = imc.getIC();
	if(!ic)
		return;
	ic->cfCandForm->ptCurrentPos = imc.getIC()->cfCompForm.ptCurrentPos;
	ic->cfCandForm->ptCurrentPos.y += (rc.bottom - rc.top);
	*pt = imc.getIC()->cfCandForm->ptCurrentPos;
	string compstr = imc.getCompStr()->getCompStr();
	pt->x += indexToXPos( compstr, getDisplayedCursorPos());

}
