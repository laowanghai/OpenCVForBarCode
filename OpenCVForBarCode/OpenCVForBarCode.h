
// OpenCVForBarCode.h : OpenCVForBarCode Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// COpenCVForBarCodeApp:
// �йش����ʵ�֣������ OpenCVForBarCode.cpp
//

class COpenCVForBarCodeApp : public CWinApp
{
public:
	COpenCVForBarCodeApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenCVForBarCodeApp theApp;
