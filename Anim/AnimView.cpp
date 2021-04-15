
// AnimView.cpp : implementation of the CAnimView class
//

#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Anim.h"
#endif

#include "AnimDoc.h"
#include "AnimView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimView

IMPLEMENT_DYNCREATE(CAnimView, CView)

BEGIN_MESSAGE_MAP(CAnimView, CView)
	ON_COMMAND(ID_START_STOP, &CAnimView::OnStart)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BALL_ADD, &CAnimView::OnBallAdd)
	ON_COMMAND(ID_BALL_DELETE, &CAnimView::OnBallDelete)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(ID_BALL_ADD, &CAnimView::OnUpdateBallAdd)
	ON_UPDATE_COMMAND_UI(ID_BALL_DELETE, &CAnimView::OnUpdateBallDelete)
END_MESSAGE_MAP()

// CAnimView construction/destruction

#define BALLSIZE 60
#define BLUE RGB( 0, 0, 255 )

//=============================
// callback
void CALLBACK ZfxTimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime)
{
	::SendMessage( hWnd, WM_TIMER, 0, 0 );
}
//=============================
// rand

//=============================

CAnimView::CAnimView() noexcept
{
	m_nTimerID = 0;
	m_pBall = new CRect(20, 20, 20 + BALLSIZE, 20 + BALLSIZE);
	m_pBallPen = new CPen(PS_SOLID, 1, BLUE);
	m_pBallBrush = new CBrush(BLUE);
	m_nBallOffX = 3;
	m_nBallOffY = 1;
	m_bStart = FALSE;
	m_pClientRect = new CRect(0,0,0,0);

	srand(time(NULL));
	createNewBall();
}

CAnimView::~CAnimView()
{
	delete m_pBall;
	delete m_pBallPen;
	delete m_pBallBrush;
	delete m_pClientRect;
}

BOOL CAnimView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAnimView drawing

void CAnimView::OnDraw(CDC* pDC )
{
	CAnimDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*CPen* pOldPen = pDC->SelectObject( m_pBallPen );
	CBrush* pOldBrush = pDC->SelectObject( m_pBallBrush );
	pDC->Ellipse( m_pBall );
	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );*/

	CDC memDC;	//pamieciowy kontekst urzadzenia
	BOOL b = memDC.CreateCompatibleDC(pDC);
	ASSERT(b);

	CBitmap bmp;
	b = bmp.CreateCompatibleBitmap(pDC, m_pClientRect->Width(), m_pClientRect->Height());
	ASSERT(b);
	CBitmap* pOldBitmap = memDC.SelectObject( &bmp );
	memDC.FillSolidRect(m_pClientRect, RGB(230,230,200));

	//CPen* pOldPen = memDC.SelectObject(m_pBallPen);
	//CBrush* pOldBrush = memDC.SelectObject(m_pBallBrush);

	//memDC.Ellipse(m_pBall);

	//memDC.SelectObject(pOldPen);
	//memDC.SelectObject(pOldBrush);


	for (size_t i = 0; i < m_pCBallVect.size(); i++)
		m_pCBallVect.at(i)->PaintBall(&memDC);

	b = pDC->BitBlt(0, 0, m_pClientRect->Width(), m_pClientRect->Height(), &memDC, 0, 0, SRCCOPY);
	ASSERT(b);

	// cleaning
	memDC.SelectObject(pOldBitmap);
	bmp.DeleteObject();
	memDC.DeleteDC();
}


// CAnimView diagnostics

#ifdef _DEBUG
void CAnimView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimDoc* CAnimView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimDoc)));
	return (CAnimDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimView message handlers


void CAnimView::OnStart()
{
	m_bStart = !m_bStart;

	CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
	pFrame->ResetButton(m_bStart);
}


void CAnimView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetClientRect(m_pClientRect);

	//m_nTimerID = SetTimer( WM_USER + 1, 20, NULL);
	// add callback function
	m_nTimerID = SetTimer( WM_USER + 1, 20, ZfxTimerProc );
}


void CAnimView::OnDestroy()
{
	KillTimer( m_nTimerID );
	CView::OnDestroy();
}


void CAnimView::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bStart)
	{
		for (size_t i = 0; i < m_pCBallVect.size(); i++)
			m_pCBallVect.at(i)->Move();

		Invalidate();
	}

	CView::OnTimer(nIDEvent);
}


BOOL CAnimView::OnEraseBkgnd(CDC* pDC)
{
	// block background erasing
	//return CView::OnEraseBkgnd(pDC);
	return 1;
}


void CAnimView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// read client window size
	GetClientRect(m_pClientRect);

	for (size_t i = 0; i < m_pCBallVect.size(); i++)
		m_pCBallVect.at(i)->SetBoundRect(m_pClientRect);

	CView::OnPrepareDC(pDC, pInfo);
}


void CAnimView::OnBallAdd()
{
	if (m_pCBallVect.size() < 10)
		createNewBall();

	Invalidate();
}


void CAnimView::OnBallDelete()
{	
	if (m_pCBallVect.size() > 1)
	{
		CBall* val = m_pCBallVect.back();
		delete val;
		m_pCBallVect.pop_back();
	}
		

	Invalidate();
}

int CAnimView::dice(int from, int to)
{
	return std::rand() % (to - from + 1) + from;
}

void CAnimView::createNewBall()
{
	int l = 1;
	int moveBallX = dice(0, m_pClientRect->right - 61);
	int r = dice(l + 30, l + 60);
	int moveBallY = dice(0, m_pClientRect->bottom - 61);
	int R = dice(0, 255);
	int G = dice(0, 255);
	int B = dice(0, 255);
	int X = dice(1, 10);
	int Y = dice(1, 10);

	m_pCBallVect.push_back(new CBall(l + moveBallX, l + moveBallY, r + moveBallX, r + moveBallY,
		RGB(R, G, B), X, Y, m_pClientRect));
}


void CAnimView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	for (size_t i = 0; i < m_pCBallVect.size(); i++)
	{
		if(m_pCBallVect.at(i)->right > cx)
			m_pCBallVect.at(i)->SetBallSize(&CRect(m_pCBallVect.at(i)->left-6, m_pCBallVect.at(i)->top, m_pCBallVect.at(i)->right-6, m_pCBallVect.at(i)->bottom));

		if (m_pCBallVect.at(i)->bottom > cy)
			m_pCBallVect.at(i)->SetBallSize(&CRect(m_pCBallVect.at(i)->left, m_pCBallVect.at(i)->top - 6, m_pCBallVect.at(i)->right, m_pCBallVect.at(i)->bottom - 6));
	}
}

void CAnimView::OnUpdateBallAdd(CCmdUI* pCmdUI)
{
	bool enabled = m_pCBallVect.size() == 10 ? false : true;
	pCmdUI->Enable(enabled);
}

void CAnimView::OnUpdateBallDelete(CCmdUI* pCmdUI)
{
	bool enabled = m_pCBallVect.size() == 1 ? false : true;
	pCmdUI->Enable(enabled);
}
