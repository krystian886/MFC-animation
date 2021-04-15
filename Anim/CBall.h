#pragma once
#include <atltypes.h>

class CBall : public CRect
{
private:
	CPen* m_pBallPen;
	CBrush* m_pBallBrush;
	CRect* m_pBoundRect;
	int m_nBallOffX;
	int m_nBallOffY;

public:
	CBall(int left = 0, int top = 0, int right = 0, int bottom = 0, COLORREF color = RGB(0, 0, 0),
		int OffX = 0, int OffY = 0, CRect* rectMax = NULL);
	CBall(CRect* rect = NULL, COLORREF color = RGB(0, 0, 0), int OffX = 0, int OffY = 0,
		CRect* rectMax = NULL);
	CBall(CPoint point, CSize size, COLORREF color = RGB(0, 0, 0), int OffX = 0,
		int OffY = 0, CRect* rectMax = NULL);

	~CBall();

public:
	inline void SetBallColor(COLORREF color);
	inline void SetBallSize(CRect* rect);
	void PaintBall(CDC* DC);
	void SetBall(CRect* rect, COLORREF color);
	void SetOffset(int x, int y);
	void SetBoundRect(CRect* rect);
	void Move();

private:
	inline void CreateObjects(COLORREF BallColor);
	inline void DeleteObjects();
	void InitObjects(COLORREF BallColor);
};

