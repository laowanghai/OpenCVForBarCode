#ifndef MATIMAGEFUNCTION_H
#define MATIMAGEFUNCTION_H

#include "stdafx.h"
#include<vector>
using namespace std;

void Mat2CImage(Mat & mat, CImage & cimage);
void CImage2Mat(CImage & cimage, Mat & mat);
void SaveMat2File(Mat & mat, char*filePath);
void CheckLegalMatRect(Mat & mat,Rect* bRect);

#endif

