// stdafx.h : �i�b�����Y�ɤ��]�t�зǪ��t�� Include �ɡA
// �άO�g�`�ϥΫo�ܤ��ܧ󪺱M�ױM�� Include �ɮ�
//

//#pragma once

#include <iostream>
#include <tchar.h>
#include <windows.h>

// TODO: �b���Ѧұz���{���һݭn����L���Y
#include "chewing-utf8-util.h"
#include "global.h"

typedef struct
{
	const char zuin[10];
}zuintable;

typedef struct
{
	int lastfreq;
	int lasttime;
	int maxfreq;
	int systemfreq;
	int phonenum;
	uint16 phoneserial;
	int bits;
}chewing;