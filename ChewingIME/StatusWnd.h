#pragma once

#include "IMEChildWnd.h"
#include "XPToolbar.h"

class StatusWnd : public IMEChildWnd, public XPToolbar
{
public:
	StatusWnd();
	~StatusWnd(void);
	static BOOL registerClass(void);
protected:
//	HWND toolbar;
	static LRESULT wndProc( HWND hwnd , UINT msg, WPARAM wp , LPARAM lp);
//	void onPaint(PAINTSTRUCT& ps);

public:
protected:
	void onCommand(UINT id, HIMC hIMC);
	HICON iconChi;
	HICON iconEng;
	HICON iconFull;
	HICON iconHalf;
	HICON iconConfig;
	bool dragging;
	LRESULT wndProc(UINT msg, WPARAM wp, LPARAM lp);
public:
	virtual void getSize(int* w, int* h);
	bool create(HWND imeUIWnd);
	void enableChiEng(bool enable=true);
	void disableChiEng(){	enableChiEng(false);	}
	void toggleChiEngMode(HIMC hIMC);
	void toggleShapeMode(HIMC hIMC);
	void updateIcons(HIMC hIMC);
};
