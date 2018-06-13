// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)//��������
#define PI 3.1415926//Բ����
#include "cmath"//��ѧͷ�ļ�

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
	Play=FALSE;//������ť״̬
	R=800.0,d=1000,Phi=-45,Theta=-50;//�ӵ�λ��������	
	LightNum=3;//��Դ����
	pLight=new CLighting(LightNum);//��Դ��̬����
	pLight->Light[0].SetGlobal(R,Phi,Theta);//��Դλ��������
	pLight->Light[1].SetGlobal(R,Theta,Phi);
	pLight->Light[2].SetGlobal(R/2,2*Theta,Phi/2);
	//���ù�Դ����
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(0.8,0.8,0.8);//��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(0.508,0.508,0.508);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//���Ƿ���
	}	
	//���ò��ʲ���
	pMaterial=new CMaterial;//����ָ��
	pMaterial->M_Ambient=CRGB(0.192,0.192,0.192);//���ʶԻ�����ķ�����
	pMaterial->M_Diffuse=CRGB(0.508,0.508,0.508);//���ʶ��������ķ�����
	pMaterial->M_Specular=CRGB(1.0,1.0,1.0);//���ʶԾ��淴���ķ�����
	pMaterial->M_Exp=5.0;//�߹�ָ��
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

void CTestView::ReadPoint()//���
{
	double a=150;//������߳�
	//�������ά����(x,y,z)
	//������
/*	P[0].x=-a/2;P[0].y=-a/2;P[0].z=-a/2;
	P[1].x=a/2; P[1].y=-a/2;P[1].z=-a/2;
	P[2].x=a/4; P[2].y=a/2; P[2].z=-a/4;
	P[3].x=-a/4;P[3].y=a/2; P[3].z=-a/4;
	P[4].x=-a/2;P[4].y=-a/2;P[4].z=a/2;
	P[5].x=a/2; P[5].y=-a/2;P[5].z=a/2;
	P[6].x=a/4; P[6].y=a/2; P[6].z=a/4;
	P[7].x=-a/4;P[7].y=a/2; P[7].z=a/4;
*/
	//����׶
	P[0].x=0;P[0].y=-a/2;P[0].z=0;
	P[1].x=a;P[1].y=-a/2;P[1].z=0;
	P[2].x=0;P[2].y=-a/2;P[2].z=a;
	P[3].x=a/2;P[3].y= a/2;P[3].z=a/2;
}
void CTestView::ReadFace()//���
{
	//��ı�������Ķ����ź������ɫ
	//������
/*	F[0].En=4;F[0].p[0]=0;F[0].p[1]=3;F[0].p[2]=2;F[0].p[3]=1;//����
	F[1].En=4;F[1].p[0]=4;F[1].p[1]=5;F[1].p[2]=6;F[1].p[3]=7;//ǰ��
	F[2].En=4;F[2].p[0]=0;F[2].p[1]=4;F[2].p[2]=7;F[2].p[3]=3;//����
	F[3].En=4;F[3].p[0]=1;F[3].p[1]=2;F[3].p[2]=6;F[3].p[3]=5;//����
	F[4].En=4;F[4].p[0]=3;F[4].p[1]=7;F[4].p[2]=6;F[4].p[3]=2;//����
	F[5].En=4;F[5].p[0]=0;F[5].p[1]=1;F[5].p[2]=5;F[5].p[3]=4;//����
*/
	//����׶
	F[0].En=3;F[0].p[0]=1;F[0].p[1]=2;F[0].p[2]=3;
	F[1].En=3;F[1].p[0]=2;F[1].p[1]=0;F[1].p[2]=3;
	F[2].En=3;F[2].p[0]=0;F[2].p[1]=1;F[2].p[2]=3;
	F[3].En=3;F[3].p[0]=0;F[3].p[1]=2;F[3].p[2]=1;
}
void CTestView::InitParameter()//͸�ӱ任������ʼ��
{	
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[3]*k[2];
	k[6]=k[1]*k[2];
	k[7]=k[3]*k[4];
	k[8]=k[1]*k[4];
	ViewPoint.x=R*k[5];//�û�����ϵ���ӵ�������
	ViewPoint.y=R*k[6];
	ViewPoint.z=R*k[4];
}
void CTestView::PerProject(CP3 P)//͸�ӱ任
{
	CP3 ViewP;
	ViewP.x=-k[1]*P.x+k[3]*P.y;//�۲�����ϵ��ά����
	ViewP.y=ROUND(-k[7]*P.x-k[8]*P.y+k[2]*P.z);
	ViewP.z=-k[5]*P.x-k[6]*P.y-k[4]*P.z+R;
	ViewP.c=P.c;
	ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ����ϵ��ά����
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
	ScreenP.c=ViewP.c;
}
void CTestView::DoubleBuffer()//˫����
{
	CRect Rect;
	GetClientRect(&Rect);
	CDC *pDC=GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);//�Զ�������ϵ
	pDC->SetWindowExt(Rect.Width(),Rect.Height());
	pDC->SetViewportExt(Rect.Width(),-Rect.Height());//x�ᴹֱ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(Rect.Width()/2,Rect.Height()/2);//��Ļ����ΪԲ��	
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
void CTestView::DrawObject(CDC* pDC)//����������
{
	CPi2  Point[3];
	for(int nFace=0;nFace<4;nFace++)
	{
		CVector VS(P[F[nFace].p[1]],ViewPoint);//�����ʸ��
		CVector V01(P[F[nFace].p[0]],P[F[nFace].p[1]]);//���һ����ʸ��
		CVector V12(P[F[nFace].p[1]],P[F[nFace].p[2]]);//�����һ����ʸ��		
		CVector VN=V01*V12;//��ķ�ʸ��	
		if(Dot(VS,VN)>=0)//�����޳�
		{
			for(int nEdge=0;nEdge<F[nFace].En;nEdge++)
			{
				PerProject(P[F[nFace].p[nEdge]]);
				Point[nEdge].x=ScreenP.x;
				Point[nEdge].y=ROUND(ScreenP.y);
				Point[nEdge].c=pLight->Lighting(ViewPoint,P[F[nFace].p[nEdge]],VN,pMaterial);
			}
			fill.SetPoint(Point,3);//���ö��������������
			fill.CreateBucket();
			fill.CreateEdge();
			fill.Gouraud(pDC);
		}
	}
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//����������
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	DoubleBuffer();
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point)//����Ҽ����� 
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

void CTestView::OnMplay()//������ť���� 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//���ö�����ʱ�䲽
		SetTimer(1,15,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent)//����ʱ�亯�� 
{
	// TODO: Add your message handler code here and/or call default
	Phi-=10;
	Theta-=10;
	pLight->Light[0].SetGlobal(R,Phi,Theta);
	InitParameter();	
	DoubleBuffer();
}

void CTestView::OnUpdateMplay(CCmdUI* pCmdUI)//������ť״̬���� 
{
	// TODO: Add your command update UI handler code here
	if(Play)
 	{
 		pCmdUI->SetCheck(TRUE);
 		pCmdUI->SetText("ֹͣ");
 	}
 	else
 	{
 		pCmdUI->SetCheck(FALSE);
 		pCmdUI->SetText("����");
	}			
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//���ñ���ɫ����  
{
	// TODO: Add your message handler code here and/or call default
	CRect Rect;   
	GetClientRect(&Rect);   
	pDC->FillSolidRect(Rect,RGB(0,0,0));
	return TRUE;
}

void CTestView::OnMlred()//���ú�ɫ��Դ 
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

void CTestView::OnMlgreen()//������ɫ��Դ 
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


void CTestView::OnMlblue()//������ɫ��Դ 
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

void CTestView::OnMlwhite()//���ð�ɫ��Դ  
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

void CTestView::OnMmred()//���ú챦ʯ����  
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

void CTestView::OnMmgreen()//�����̱�ʯ���� 
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

void CTestView::OnMmblue()//��������ʯ���� 
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

void CTestView::OnMmwhite()//������ʯ���� 
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

void CTestView::OnMMSuiBian() //���û�ɫ����
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

void CTestView::OnMMQingTong() //��ͭɫ
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
