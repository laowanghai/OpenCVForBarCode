
// OpenCVForBarCodeView.cpp : COpenCVForBarCodeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenCVForBarCode.h"
#endif

#include "OpenCVForBarCodeDoc.h"
#include "OpenCVForBarCodeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCVForBarCodeView

IMPLEMENT_DYNCREATE(COpenCVForBarCodeView, CView)

BEGIN_MESSAGE_MAP(COpenCVForBarCodeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_BARCODE_AREA, &COpenCVForBarCodeView::OnBarcodeArea)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_BARCODE_VIDEO, &COpenCVForBarCodeView::OnBarcodeVideo)
	ON_COMMAND(IDM_BARCODE_STOPVIDEO, &COpenCVForBarCodeView::OnBarcodeStopVideo)
	ON_COMMAND(IDM_BARCODE_SNAGIT, &COpenCVForBarCodeView::OnBarcodeSnagit)
	ON_COMMAND(IDM_BARCODE_VIDEOFILE, &COpenCVForBarCodeView::OnBarcodeVideoFile)
END_MESSAGE_MAP()

// COpenCVForBarCodeView 构造/析构

COpenCVForBarCodeView::COpenCVForBarCodeView()
{
	// TODO: 在此处添加构造代码
	m_strBarcode = "";
	m_bDown = FALSE;
	m_bCapture = FALSE;
}

COpenCVForBarCodeView::~COpenCVForBarCodeView()
{
}

BOOL COpenCVForBarCodeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COpenCVForBarCodeView 绘制
int COpenCVForBarCodeView::CalculateTitleSize(CDC* pDC, CRect rcClient)
{
	int fontHeight;
	int le = strlen("原始图像   计算图像");
	int la = m_strBarcode.GetLength();
	CRect textRect;
	CFont* pOldFont;
	CString title;
	int length = le;

	if (le<la)
		length = la;
	if (le<la)
		title=m_strBarcode;
	else
		title="原始图像   计算图像";
	fontHeight = rcClient.Height() / 20;
	do
	{
		VERIFY(m_font->CreateFont(
			fontHeight,             // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			GB2312_CHARSET,            // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("楷体_GB2312")));          // lpszFacename

		pOldFont = pDC->SelectObject(m_font);
		pDC->DrawText(title.GetBuffer(0), title.GetLength(), &textRect, DT_CALCRECT);
		m_font->DeleteObject();
		pDC->SelectObject(pOldFont);
		fontHeight--;
	} while (rcClient.Width()<textRect.Width());
	
	return fontHeight + 1;
}


void COpenCVForBarCodeView::OnDraw(CDC* pDC)
{
	COpenCVForBarCodeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rcClient;
	GetClientRect(&rcClient);
	CFont* pOldFont;
	m_font = new CFont;

	int space = 0;
	int titleHeight=0;

	// TODO: 在此处为本机数据添加绘制代码
	if (!imageDraw.IsNull())
	{
		CRect textRect;
		m_iFontHeight = CalculateTitleSize(pDC, rcClient);
		VERIFY(m_font->CreateFont(
			m_iFontHeight,             // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			GB2312_CHARSET,            // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("楷体_GB2312")));          // lpszFacename
		pDC->SetTextColor(RGB(0, 0, 0));
		pOldFont = pDC->SelectObject(m_font);
		imageDraw.Draw(pDC->GetSafeHdc(), 0, 0);
		CString Tip = _T("原始图像");
		pDC->DrawText(Tip, Tip.GetLength(), &textRect, DT_CALCRECT);
		titleHeight = textRect.Height();
		int left = 0;
		if ((imageDraw.GetWidth() - textRect.Width() / 2) > 0)
			left = (imageDraw.GetWidth() - textRect.Width()) / 2;		
		pDC->TextOut(left, imageDraw.GetHeight() + space , Tip);


		if (!imageDrawForComputer.IsNull())
		{ 
			left = imageDraw.GetWidth() + 10;
			imageDrawForComputer.Draw(pDC->GetSafeHdc(), left, 0);
			Tip = _T("计算图像");
			pDC->DrawText(Tip, Tip.GetLength(), &textRect, DT_CALCRECT);
			if ((imageDrawForComputer.GetWidth() - textRect.Width() / 2) > 0)
				left += (imageDrawForComputer.GetWidth() - textRect.Width()) / 2;
			pDC->TextOut(left, imageDraw.GetHeight() + space, Tip);
		}
		pDC->SetTextColor(RGB(255, 0, 0));
		if (m_strBarcode.GetLength() != 0)
		{
			pDC->DrawText(m_strBarcode, m_strBarcode.GetLength(), &textRect, DT_CALCRECT);
			space += 10;
			pDC->TextOut((rcClient.Width() - textRect.Width()) / 2, imageDraw.GetHeight() + space + titleHeight, m_strBarcode);
			
		}
		pDC->SelectObject(pOldFont);
		m_font->DeleteObject();
	}
}

// COpenCVForBarCodeView 打印

BOOL COpenCVForBarCodeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenCVForBarCodeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COpenCVForBarCodeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}
// COpenCVForBarCodeView 诊断

#ifdef _DEBUG
void COpenCVForBarCodeView::AssertValid() const
{
	CView::AssertValid();
}

void COpenCVForBarCodeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenCVForBarCodeDoc* COpenCVForBarCodeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenCVForBarCodeDoc)));
	return (COpenCVForBarCodeDoc*)m_pDocument;
}
#endif //_DEBUG

string ISBN10_13(string isbn)
{
	isbn = isbn.substr(0, isbn.length() - 1);
	isbn = "978" + isbn;	
	int a = 0;		
	int b = 0;		
	int c = 0;		
	int d = 0;	
	for (int i = 0; i < isbn.length(); i++) 
	{ 
		int x = isbn[i] - 0x30;
		if (i % 2 == 0)
		{ 
			a += x;
		}
		else 
		{ 
			b += x;
		} 
	}		
	c = a + 3 * b;	
	d = 10 - c % 10;
	d = d % 10;
	char temp[2];
	temp[0] = d + 0x30;
	temp[1] = '\0';
	isbn.append(temp);
	return isbn;
}

//按轮廓面积排序：
static bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2)
{
	return (cv::contourArea(contour1) > cv::contourArea(contour2));
}

string COpenCVForBarCodeView::BarcodeForMat(Mat image)
{
	// TODO: 在此添加命令处理程序代码
	string str = "";
	str = ZbarDecoder(image, 0);
	//UpdateWindow();
	if (str.length() != 0)
		return str;

	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	Mat gradX, gradY, gradient, blurred, thresh, kernel, closed, closedCopy;
	// compute the Scharr gradient magnitude representation of the images
	// in both the x and y direction
	Sobel(gray, gradX, CV_32F, 1, 0, -1);
	Sobel(gray, gradY, CV_32F, 0, 1, -1);
	// subtract the y-gradient from the x-gradient
	subtract(gradX, gradY, gradient);
	convertScaleAbs(gradient, gradient);
	// blur and threshold the image
	blur(gradient, blurred, Size(9, 9));
	threshold(blurred, thresh, 225, 255, THRESH_BINARY);
	// construct a closing kernel and apply it to the thresholded image
	kernel = getStructuringElement(MORPH_RECT, Size(21, 7));

	morphologyEx(thresh, closed, MORPH_CLOSE, kernel);
	// perform a series of erosions and dilations
	erode(closed, closed, Mat(), Point(-1, -1), 4);
	dilate(closed, closed, Mat(), Point(-1, -1), 4);
	// find the contours in the thresholded image, then sort the contours
	// by their area, keeping only the largest one
	closed.copyTo(closedCopy);
	std::vector<std::vector<cv::Point>> cnts;
	int contours_num;
	findContours(closedCopy, cnts, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	contours_num = cnts.size();
	//sorted
	std::sort(cnts.begin(), cnts.end(), ContoursSortFun);
	// compute the rotated bounding box of the largest contour
	if (cnts.size() == 0)
		return str;
	RotatedRect minRect = minAreaRect(cnts[0]);
	Rect bRect = minRect.boundingRect();  //
	CheckLegalMatRect(image, &bRect);
	Mat result1 = image(bRect);
	str = ZbarDecoder(result1, 0);
	if (str.length() != 0)
	{
		Mat2CImage(result1, imageDrawForComputer);
		return str;
	}
	str = ZbarDecoderLeanAdjust(image);
	if (str.length() != 0)
	{
		Mat2CImage(result1, imageDrawForComputer);
		return str;
	}
	Point2f rect_points[4];
	minRect.points(rect_points);
	Point2f center = minRect.center;//外接矩形中心点坐标	
	Mat rot_mat = getRotationMatrix2D(center, minRect.angle, 1.0);//求旋转矩阵	
	Mat rot_image;
	Size dst_sz(image.size());
	warpAffine(image, rot_image, rot_mat, dst_sz);//原图像旋转	
	bRect = Rect(center.x - (minRect.size.width / 2), center.y - (minRect.size.height / 2), minRect.size.width, minRect.size.height);
	CheckLegalMatRect(rot_image, &bRect);
	result1 = rot_image(bRect);//提取ROI	
	str = ZbarDecoder(result1, 0);
	if (str.length() != 0)
	{
		Mat2CImage(result1, imageDrawForComputer);
		return str;
	}
	return str;
}

string COpenCVForBarCodeView::ZbarDecoder(Mat image, int grayFlag)
{
	string result = "";
	scanner = new ImageScanner;
	
	scanner->set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat imageGray;
	if (grayFlag)
		imageGray = image;
	else
		cvtColor(image, imageGray, COLOR_BGR2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	//Image imageZbar(width, height, "Y800", raw, width * height);
	imageZbar=new Image(width, height, "Y800", raw, width * height);

	scanner->scan(*imageZbar); //扫描条码    
	if (imageZbar->symbol_begin() == imageZbar->symbol_end())
	{
		return result;
	}
	symbol = imageZbar->symbol_begin();
	//扫描结果打印	
	for (; symbol != imageZbar->symbol_end(); ++symbol)
	{
		result += symbol->get_type_name();
		result += " : ";
		result += symbol->get_data();
		if (symbol->get_type_name() == "ISBN-10")
		{
			result += "   ISBN - 13";
			result += " : ";
			result += ISBN10_13(symbol->get_data());;
		}
	}
	Mat2CImage(imageGray, imageDrawForComputer);
	imageZbar->set_data(NULL, 0);//清除缓存
	free(imageZbar);
	m_strBarcode = result.c_str();
	free(scanner);

	return result;
}

string COpenCVForBarCodeView::ZbarDecoderLeanAdjust(Mat image)
{
	string result = "";
	Mat imageGray, imageGuussian;
	Mat imageSobelX, imageSobelY, imageSobelOut;
	cvtColor(image, imageGray, COLOR_BGR2GRAY);
	GaussianBlur(imageGray, imageGuussian, Size(3, 3), 0);
	//水平和垂直方向灰度图像的梯度和,使用Sobel算子    
	Mat imageX16S, imageY16S;
	Sobel(imageGuussian, imageX16S, CV_16S, 1, 0, 3, 1, 0, 4);
	Sobel(imageGuussian, imageY16S, CV_16S, 0, 1, 3, 1, 0, 4);
	convertScaleAbs(imageX16S, imageSobelX, 1, 0);
	convertScaleAbs(imageY16S, imageSobelY, 1, 0);
	imageSobelOut = imageSobelX + imageSobelY;
	Mat srcImg = imageSobelOut;
	//宽高扩充，非必须，特定的宽高可以提高傅里叶运算效率  
	Mat padded;
	int opWidth = getOptimalDFTSize(srcImg.rows);
	int opHeight = getOptimalDFTSize(srcImg.cols);
	copyMakeBorder(srcImg, padded, 0, opWidth - srcImg.rows, 0, opHeight - srcImg.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat comImg;
	//通道融合，融合成一个2通道的图像  
	merge(planes, 2, comImg);
	dft(comImg, comImg);
	split(comImg, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat magMat = planes[0];
	magMat += Scalar::all(1);
	log(magMat, magMat);
	//对数变换，方便显示 	
	magMat = magMat(Rect(0, 0, magMat.cols & -2, magMat.rows & -2));
	//以下把傅里叶频谱图的四个角落移动到图像中心	
	int cx = magMat.cols / 2;
	int cy = magMat.rows / 2;
	Mat q0(magMat, Rect(0, 0, cx, cy));
	Mat q1(magMat, Rect(0, cy, cx, cy));
	Mat q2(magMat, Rect(cx, cy, cx, cy));
	Mat q3(magMat, Rect(cx, 0, cx, cy));
	Mat tmp;
	q0.copyTo(tmp);
	q2.copyTo(q0);
	tmp.copyTo(q2);
	q1.copyTo(tmp);
	q3.copyTo(q1);
	tmp.copyTo(q3);
	normalize(magMat, magMat, 0, 1, CV_MINMAX);
	Mat magImg(magMat.size(), CV_8UC1);
	magMat.convertTo(magImg, CV_8UC1, 255, 0);
	//HoughLines查找傅里叶频谱的直线，该直线跟原图的一维码方向相互垂直	
	threshold(magImg, magImg, 180, 255, THRESH_BINARY);
	vector<Vec2f> lines;
	float pi180 = (float)CV_PI / 180;
	Mat linImg(magImg.size(), CV_8UC3);
	HoughLines(magImg, lines, 1, pi180, 100, 0, 0);
	int numLines = lines.size();
	float theta;
	for (int l = 0; l<numLines; l++)
	{
		float rho = lines[l][0];
		theta = lines[l][1];
		float aa = (theta / CV_PI) * 180;
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(linImg, pt1, pt2, Scalar(255, 0, 0), 3, 8, 0);
	}
	//校正角度计算
	float angelD = 180 * theta / CV_PI - 90;
	Point center(imageGray.cols / 2, imageGray.rows / 2);
	Mat rotMat = getRotationMatrix2D(center, angelD, 1.0);
	Mat imageSource = Mat::ones(imageGray.size(), CV_8UC3);
	warpAffine(imageGray, imageSource, rotMat, imageGray.size(), 1, 0, Scalar(255, 255, 255));//仿射变换校正图像 
	return ZbarDecoder(imageSource, 1);
}


void COpenCVForBarCodeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ptDown = point;
	m_ptLast = point;
	m_bDown = TRUE;
	CView::OnLButtonDown(nFlags, point);
}


void COpenCVForBarCodeView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDown)
	{
		CClientDC dc(this);
		CRect rect(m_ptDown.x, m_ptDown.y, m_ptLast.x, m_ptLast.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		rect.SetRect(m_ptDown.x, m_ptDown.y, point.x, point.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		m_ptLast = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void COpenCVForBarCodeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bDown)
	{
		m_bDown = FALSE;
		CClientDC dc(this);
		CRect rect(m_ptDown.x, m_ptDown.y, m_ptLast.x, m_ptLast.y);
		rect.NormalizeRect();
		dc.DrawFocusRect(rect);
		m_rectSelect = rect;
		if (!rect.IsRectEmpty())
		{
			if (!imageDraw.IsNull())
			{
				Mat imageDrawMat;
				CImage2Mat(imageDraw, imageDrawMat);
				if (rect.bottom > imageDraw.GetHeight())
					rect.bottom = imageDraw.GetHeight();
				if (rect.right > imageDraw.GetWidth())
					rect.right = imageDraw.GetWidth();
				Mat roi_img = imageDrawMat(Range(rect.top, rect.bottom),Range(rect.left, rect.right) );
				SaveMat2File(roi_img, "d:\\rect.jpg");
				string result= BarcodeForMat(roi_img);
				if (result.length() != 0)
				{
					imageDraw.Destroy();
					Mat2CImage(roi_img, imageDraw);
					Invalidate();
					UpdateWindow();
				}
			}
		}
	}
	CView::OnLButtonUp(nFlags, point);
}

// COpenCVForBarCodeView 消息处理程序
void COpenCVForBarCodeView::OnBarcodeArea()
{
	// TODO: 在此添加命令处理程序代码
	string str = "";
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult = imageDraw.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (IDOK != dlg.DoModal())
		return;
	if (!imageDraw.IsNull())
	{
		imageDraw.Destroy();
	}
	if (!imageDrawForComputer.IsNull())
	{
		imageDrawForComputer.Destroy();
	}
	m_strBarcode.Empty();
	CString szFileName = dlg.GetPathName();
	imageDraw.Load(szFileName);
	Invalidate();
	USES_CONVERSION;
	cv::String cvStrFileName = W2A(szFileName);
	Mat image = imread(cvStrFileName); //载入图像到Mat
	BarcodeForMat(image);
	return;
}

void COpenCVForBarCodeView::OnBarcodeVideo()
{
	Mat frame;
	VideoCapture capture(0);
	m_bCapture = TRUE;
	int rr;
	while (m_bCapture)
	{
		capture >> frame;
		if (!imageDraw.IsNull())
			imageDraw.Destroy();
		if (!frame.empty()) 
		{
			Mat2CImage(frame, imageDraw);
			string result = BarcodeForMat(frame);
			Invalidate();
			UpdateWindow();
		}
		if (waitKey(20) == 27)   //延时30  
			break;
	}
	return;
}

void COpenCVForBarCodeView::OnBarcodeStopVideo()
{
	// TODO: 在此添加命令处理程序代码
	m_bCapture = FALSE;
}

void COpenCVForBarCodeView::OnBarcodeSnagit()
{
	//m_bCapture = FALSE;
	if (!imageDraw.IsNull())
	{
		Mat imageDrawMat;
		CImage2Mat(imageDraw, imageDrawMat);
		SaveMat2File(imageDrawMat, "d:\\snagit.jpg");
	}
}

void COpenCVForBarCodeView::OnBarcodeVideoFile()
{
	// TODO: 在此添加命令处理程序代码
	CString strFilter;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("视频文件(*.mp4)|*.mp4|"));

	if (IDOK != dlg.DoModal())
		return;
	CString szFileName = dlg.GetPathName();
	USES_CONVERSION;
	cv::String cvStrFileName = W2A(szFileName);
	VideoCapture cap;
	cap.open(cvStrFileName);
	if (!cap.isOpened())//如果视频不能正常打开则返回
		return;
	m_bCapture = TRUE;
	Mat frame;
	cap >> frame;
	while (!frame.empty() && m_bCapture)
	{
		if (!imageDraw.IsNull())
			imageDraw.Destroy();
		{
			Mat2CImage(frame, imageDraw);
			string result = BarcodeForMat(frame);
			Invalidate();
			UpdateWindow();
		}
		if (waitKey(5) == 27)   //延时30  
			break;
		cap >> frame;
	}
}
