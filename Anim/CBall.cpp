#include "pch.h"
#include "CBall.h"

CBall::CBall(int left, int top, int right, int bottom, 
	COLORREF color, int OffX, int OffY, CRect* rectMax)
	: CRect(left, top, right, bottom)
{
	InitObjects(color);
	m_nBallOffX = OffX;
	m_nBallOffY = OffY;
	m_pBoundRect = rectMax;
}

CBall::CBall(CRect* rect, COLORREF color, int OffX, int OffY,
	CRect* rectMax) : CRect(rect)
{
	InitObjects(color);
	m_nBallOffX = OffX;
	m_nBallOffY = OffY;
	m_pBoundRect = rectMax;
}

CBall::CBall(CPoint point, CSize size, COLORREF color, int OffX,
	int OffY, CRect* rectMax) : CRect(point, size)
{
	InitObjects(color);
	m_nBallOffX = OffX;
	m_nBallOffY = OffY;
	m_pBoundRect = rectMax;
}

CBall::~CBall()
{
	DeleteObjects();
	// zwolnienie pamiêci na obiekty graficzne metody public
}

//===============================
// public
//===============================

inline void CBall::SetBallColor(COLORREF color)
{
	DeleteObjects();
	CreateObjects(color);
}

inline void CBall::SetBallSize(CRect* rect)
{
	this->left = rect->left;
	this->top = rect->top;
	this->right = rect->right;
	this->bottom = rect->bottom;
}

void CBall::PaintBall(CDC* DC)
{
	CPen* pOldPen = DC->SelectObject(m_pBallPen);
	CBrush* pOldBrush = DC->SelectObject(m_pBallBrush);

	DC->Ellipse(this);

	DC->SelectObject(pOldPen);
	DC->SelectObject(pOldBrush);
}

void CBall::SetBall(CRect* rect, COLORREF color)
{
	SetBallSize(rect);
	SetBallColor(color);
}

void CBall::SetOffset(int x, int y)
{
	m_nBallOffX = x;
	m_nBallOffY = y;
}

void CBall::SetBoundRect(CRect* rect)
{
	m_pBoundRect->left = rect->left;
	m_pBoundRect->top = rect->top;
	m_pBoundRect->right = rect->right;
	m_pBoundRect->bottom = rect->bottom;
}

void CBall::Move()
{
	this->OffsetRect(m_nBallOffX, m_nBallOffY);

	if (this->left <= m_pBoundRect->left)
		m_nBallOffX = -m_nBallOffX;
	else if (this->right >= m_pBoundRect->right)
		m_nBallOffX = -m_nBallOffX;

	if (this->top <= m_pBoundRect->top)
		m_nBallOffY = -m_nBallOffY;
	else if (this->bottom >= m_pBoundRect->bottom)
		m_nBallOffY = -m_nBallOffY;
}

//===============================
// private
//===============================

inline void CBall::CreateObjects(COLORREF BallColor)
{
	m_pBallPen->CreatePen(PS_SOLID, 1, BallColor);
	m_pBallBrush->CreateSolidBrush(BallColor);
}

inline void CBall::DeleteObjects()
{
	m_pBallPen->DeleteObject();
	m_pBallBrush->DeleteObject();
}

void CBall::InitObjects(COLORREF BallColor)
{
	m_pBallPen = new CPen(PS_SOLID, 1, BallColor);
	m_pBallBrush = new CBrush(BallColor);
}