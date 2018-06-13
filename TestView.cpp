// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)//四舍五入
#define PI 3.1415926//圆周率
#include "cmath"//数学头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MMGREEN, OnMmgreen)
	ON_COMMAND(ID_MMBLUE, OnMmblue)
	ON_COMMAND(ID_MMWHITE, OnMmwhite)
	ON_COMMAND(ID_MMRED, OnMmred)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MPLAY, OnMplay)
	ON_UPDATE_COMMAND_UI(ID_MPLAY, OnUpdateMplay)
	ON_COMMAND(ID_MLRED, OnMlred)
	ON_COMMAND(ID_MLWHITE, OnMlwhite)
	ON_COMMAND(ID_MLGREEN, OnMlgreen)
	ON_COMMAND(ID_MLBLUE, OnMlblue)
	ON_COMMAND(ID_MMSuiBian, OnMMSuiBian)
	ON_COMMAND(IDMMQingTong, OnMMQingTong)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here
	Play=FALSE;//动画按钮状态
	R=800.0,d=1000,Phi=-45,Theta=-50;//视点位置球坐标	
	LightNum=3;//光源数量
	pLight=new CLighting(LightNum);//光源动态数组
	pLight->Light[0].SetGlobal(R,Phi,Theta);//光源位置球坐标
	pLight->Light[1].SetGlobal(R,Theta,Phi);
	pLight->Light[2].SetGlobal(R/2,2*Theta,Phi/2);
	//设置光源参数
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(0.8,0.8,0.8);//光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(0.508,0.508,0.508);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光是否开启
	}	
	//设置材质参数
	pMaterial=new CMaterial;//材质指针
	pMaterial->M_Ambient=CRGB(0.192,0.192,0.192);//材质对环境光的反射率
	pMaterial->M_Diffuse=CRGB(0.508,0.508,0.508);//材质对漫反射光的反射率
	pMaterial->M_Specular=CRGB(1.0,1.0,1.0);//材质对镜面反射光的反射率
	pMaterial->M_Exp=5.0;//高光指数
}

CTestView::~CTestView()
{
	if(pLight)
		delete [] pLight;
	if(pMaterial)
		delete pMaterial;
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here	
	ReadPoint();
	ReadFace();
	InitParameter();
	DoubleBuffer();	
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::ReadPoint()//点表
{
	double a=150;//正方体边长
	//顶点的三维坐标(x,y,z)
	//四棱柱
/*	P[0].x=-a/2;P[0].y=-a/2;P[0].z=-a/2;
	P[1].x=a/2; P[1].y=-a/2;P[1].z=-a/2;
	P[2].x=a/4; P[2].y=a/2; P[2].z=-a/4;
	P[3].x=-a/4;P[3].y=a/2; P[3].z=-a/4;
	P[4].x=-a/2;P[4].y=-a/2;P[4].z=a/2;
	P[5].x=a/2; P[5].y=-a/2;P[5].z=a/2;
	P[6].x=a/4; P[6].y=a/2; P[6].z=a/4;
	P[7].x=-a/4;P[7].y=a/2; P[7].z=a/4;
*/
	//三棱锥
	P[0].x=0;P[0].y=-a/2;P[0].z=0;
	P[1].x=a;P[1].y=-a/2;P[1].z=0;
	P[2].x=0;P[2].y=-a/2;P[2].z=a;
	P[3].x=a/2;P[3].y= a/2;P[3].z=a/2;
}
void CTestView::ReadFace()//面表
{
	//面的边数、面的顶点编号和面的颜色
	//四棱柱
/*	F[0].En=4;F[0].p[0]=0;F[0].p[1]=3;F[0].p[2]=2;F[0].p[3]=1;//后面
	F[1].En=4;F[1].p[0]=4;F[1].p[1]=5;F[1].p[2]=6;F[1].p[3]=7;//前面
	F[2].En=4;F[2].p[0]=0;F[2].p[1]=4;F[2].p[2]=7;F[2].p[3]=3;//左面
	F[3].En=4;F[3].p[0]=1;F[3].p[1]=2;F[3].p[2]=6;F[3].p[3]=5;//右面
	F[4].En=4;F[4].p[0]=3;F[4].p[1]=7;F[4].p[2]=6;F[4].p[3]=2;//顶面
	F[5].En=4;F[5].p[0]=0;F[5].p[1]=1;F[5].p[2]=5;F[5].p[3]=4;//底面
*/
	//三棱锥
	F[0].En=3;F[0].p[0]=1;F[0].p[1]=2;F[0].p[2]=3;
	F[1].En=3;F[1].p[0]=2;F[1].p[1]=0;F[1].p[2]=3;
	F[2].En=3;F[2].p[0]=0;F[2].p[1]=1;F[2].p[2]=3;
	F[3].En=3;F[3].p[0]=0;F[3].p[1]=2;F[3].p[2]=1;
}
void CTestView::InitParameter()//透视变换参数初始化
{	
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[3]*k[2];
	k[6]=k[1]*k[2];
	k[7]=k[3]*k[4];
	k[8]=k[1]*k[4];
	ViewPoint.x=R*k[5];//用户坐标系的视点球坐标
	ViewPoint.y=R*k[6];
	ViewPoint.z=R*k[4];
}
void CTestView::PerProject(CP3 P)//透视变换
{
	CP3 ViewP;
	ViewP.x=-k[1]*P.x+k[3]*P.y;//观察坐标系三维坐标
	ViewP.y=ROUND(-k[7]*P.x-k[8]*P.y+k[2]*P.z);
	ViewP.z=-k[5]*P.x-k[6]*P.y-k[4]*P.z+R;
	ViewP.c=P.c;
	ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系二维坐标
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
	ScreenP.c=ViewP.c;
}
void CTestView::DoubleBuffer()//双缓冲
{
	CRect Rect;
	GetClientRect(&Rect);
	CDC *pDC=GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);//自定义坐标系
	pDC->SetWindowExt(Rect.Width(),Rect.Height());
	pDC->SetViewportExt(Rect.Width(),-Rect.Height());//x轴垂直向右，y轴垂直向上
	pDC->SetViewportOrg(Rect.Width()/2,Rect.Height()/2);//屏幕中心为圆心	
	CDC	MemDC,Picture;
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SetMapMode(MM_ANISOTROPIC);
	MemDC.SetWindowExt(Rect.Width(),Rect.Height());
	MemDC.SetViewportExt(Rect.Width(),-Rect.Height());
	MemDC.SetViewportOrg(Rect.Width()/2,Rect.Height()/2);
	CBitmap NewBitmap,*OldBitmap;
	NewBitmap.LoadBitmap(IDB_BITMAP2);
	OldBitmap=MemDC.SelectObject(&NewBitmap);
	MemDC.BitBlt(-Rect.Width()/2,-Rect.Height()/2,Rect.Width(),Rect.Height(),&Picture,-Rect.Width()/2,-Rect.Height()/2,SRCCOPY);
	DrawObject(&MemDC);
	pDC->BitBlt(-Rect.Width()/2,-Rect.Height()/2,Rect.Width(),Rect.Height(),&MemDC,-Rect.Width()/2,-Rect.Height()/2,SRCCOPY);
	ReleaseDC(pDC);
	MemDC.SelectObject(OldBitmap);	
	NewBitmap.DeleteObject();
}
void CTestView::DrawObject(CDC* pDC)//绘制正方体
{
	CPi2  Point[3];
	for(int nFace=0;nFace<4;nFace++)
	{
		CVector VS(P[F[nFace].p[1]],ViewPoint);//面的视矢量
		CVector V01(P[F[nFace].p[0]],P[F[nFace].p[1]]);//面的一个边矢量
		CVector V12(P[F[nFace].p[1]],P[F[nFace].p[2]]);//面的另一个边矢量		
		CVector VN=V01*V12;//面的法矢量	
		if(Dot(VS,VN)>=0)//背面剔除
		{
			for(int nEdge=0;nEdge<F[nFace].En;nEdge++)
			{
				PerProject(P[F[nFace].p[nEdge]]);
				Point[nEdge].x=ScreenP.x;
				Point[nEdge].y=ROUND(ScreenP.y);
				Point[nEdge].c=pLight->Lighting(ViewPoint,P[F[nFace].p[nEdge]],VN,pMaterial);
			}
			fill.SetPoint(Point,3);//利用多边形填充类来填充
			fill.CreateBucket();
			fill.CreateEdge();
			fill.Gouraud(pDC);
		}
	}
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键函数
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	DoubleBuffer();
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point)//鼠标右键函数 
{
	// TODO: Add your message handler code here and/or call default
	R=R-100;
	InitParameter();
	DoubleBuffer();
	CView::OnRButtonDown(nFlags, point);
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!Play)
	{
		switch(nChar)
		{
		case VK_UP:
			Phi+=5;
			break;
		case VK_DOWN:
			Phi-=5;
			break;
		case VK_LEFT:
			Theta-=5;
			break;
		case VK_RIGHT:
			Theta+=5;
			break;
		default:
			break;
		}
		pLight->Light[0].SetGlobal(R,Phi,Theta);
		InitParameter();
		DoubleBuffer();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnMplay()//动画按钮函数 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//设置动画的时间步
		SetTimer(1,15,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent)//动画时间函数 
{
	// TODO: Add your message handler code here and/or call default
	Phi-=10;
	Theta-=10;
	pLight->Light[0].SetGlobal(R,Phi,Theta);
	InitParameter();	
	DoubleBuffer();
}

void CTestView::OnUpdateMplay(CCmdUI* pCmdUI)//动画按钮状态函数 
{
	// TODO: Add your command update UI handler code here
	if(Play)
 	{
 		pCmdUI->SetCheck(TRUE);
 		pCmdUI->SetText("停止");
 	}
 	else
 	{
 		pCmdUI->SetCheck(FALSE);
 		pCmdUI->SetText("播放");
	}			
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//设置背景色函数  
{
	// TODO: Add your message handler code here and/or call default
	CRect Rect;   
	GetClientRect(&Rect);   
	pDC->FillSolidRect(Rect,RGB(0,0,0));
	return TRUE;
}

void CTestView::OnMlred()//设置红色光源 
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(1.0,0.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMlgreen()//设置绿色光源 
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(0.0,1.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}


void CTestView::OnMlblue()//设置蓝色光源 
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(0.0,0.0,1.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMlwhite()//设置白色光源  
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(1.0,1.0,1.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLWHITE,MF_CHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMmred()//设置红宝石材质  
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(1.0,0.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMmgreen()//设置绿宝石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(0.0,1.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMmblue()//设置蓝宝石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(0.0,0.0,1.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_UNCHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMmwhite()//设置钻石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(1.0,1.0,1.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_CHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMMSuiBian() //设置黄色材质
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(1,1,0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_CHECKED);
	InitParameter();
    DoubleBuffer();	
}

void CTestView::OnMMQingTong() //青铜色
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(0.5,0.4,0.3));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMBLUE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMWHITE,MF_CHECKED);
	InitParameter();
    DoubleBuffer();	
}
