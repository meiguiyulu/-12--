// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__8C972371_1C22_4E15_A2A8_3CE42319C9CB__INCLUDED_)
#define AFX_VECTOR_H__8C972371_1C22_4E15_A2A8_3CE42319C9CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P3.h"

class CVector  
{
public:
	CVector();	
	CVector(CP3);
	CVector(CP3,CP3);
	virtual ~CVector();
	CVector Unit();//单位矢量
	double X();//矢量的x坐标
	double Y();//矢量的y坐标
	double Z();//矢量的z坐标
	double Mold();//矢量的模
	friend CVector operator +(CVector &,CVector &);//矢量和
	friend CVector operator -(CVector &,CVector &);//矢量差
	friend CVector operator *(CVector &,double);//矢量数乘
	friend CVector operator /(CVector &,double);//矢量数除
	friend double Dot(CVector &,CVector &);//矢量点积
	friend CVector operator *(CVector &,CVector &);//矢量叉积
public:
	double x,y,z;
};

#endif // !defined(AFX_VECTOR_H__8C972371_1C22_4E15_A2A8_3CE42319C9CB__INCLUDED_)
