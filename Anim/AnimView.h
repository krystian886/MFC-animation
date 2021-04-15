
// AnimView.h : interface of the CAnimView class
//

#pragma once

#include <vector>
#include <random>
#include "CBall.h"


class CAnimView : public CView
{
protected: // create from serialization only
	CAnimView() noexcept;
	DECLARE_DYNCREATE(CAnimView)

// Attributes
public:
	CAnimDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CAnimView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStart();
private:
	UINT_PTR m_nTimerID;
	CRect*   m_pBall;
	CPen*    m_pBallPen;
	CBrush*  m_pBallBrush;
	int m_nBallOffX;
	int m_nBallOffY;
	BOOL m_bStart;
	CRect* m_pClientRect;
	std::vector<CBall*> m_pCBallVect;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnBallAdd();
	afx_msg void OnBallDelete();

private:
	inline int dice(int from, int to);
	void createNewBall();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateBallAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBallDelete(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in AnimView.cpp
inline CAnimDoc* CAnimView::GetDocument() const
   { return reinterpret_cast<CAnimDoc*>(m_pDocument); }
#endif

