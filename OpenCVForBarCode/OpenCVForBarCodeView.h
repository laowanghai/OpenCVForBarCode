
// OpenCVForBarCodeView.h : COpenCVForBarCodeView ��Ľӿ�
//

#pragma once

#include<vector>
using namespace std;
#include "zbar.h"
using namespace zbar;
#include<core_c.h>
#include "MatImageFunction.h"

class COpenCVForBarCodeView : public CView
{
protected: // �������л�����
	COpenCVForBarCodeView();
	DECLARE_DYNCREATE(COpenCVForBarCodeView)

// ����
public:
	COpenCVForBarCodeDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~COpenCVForBarCodeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CImage  imageDraw;
	CImage  imageDrawForComputer;
	string  ZbarDecoder(Mat image, int grayFlag);
	string  ZbarDecoderLeanAdjust(Mat image);
	string  BarcodeForMat(Mat image);
	CString m_strBarcode;
	CFont*  m_font;
	int     m_iFontHeight;
	int CalculateTitleSize(CDC* pDC, CRect rcClient);
	CPoint m_ptDown; 
	CPoint m_ptLast; 
	BOOL   m_bDown;
	CRect  m_rectSelect;
	BOOL   m_bCapture;
	ImageScanner* scanner;
	Image *imageZbar;
	Image::SymbolIterator symbol;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBarcodeArea();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBarcodeVideo();
	afx_msg void OnBarcodeStopVideo();
	afx_msg void OnBarcodeSnagit();
	afx_msg void OnBarcodeVideoFile();
};

#ifndef _DEBUG  // OpenCVForBarCodeView.cpp �еĵ��԰汾
inline COpenCVForBarCodeDoc* COpenCVForBarCodeView::GetDocument() const
   { return reinterpret_cast<COpenCVForBarCodeDoc*>(m_pDocument); }
#endif

