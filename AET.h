// AET.h: interface for the CAET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
#define AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Pi2.h"
#include "Vector.h"

class CAET  
{
public:
	CAET();
	virtual ~CAET();
public:
	double x;//当前x
	int    yMax;//边的最大y值
	double k;//斜率的倒数(x的增量)
	CPi2    pb;//起点
	CPi2    pe;//终点
	CAET    *next;
	CVector ns;//起点法矢量
	CVector ne;//终点法矢量
};

#endif // !defined(AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
