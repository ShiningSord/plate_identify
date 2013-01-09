// PlateIdenfifyView.cpp : implementation of the CPlateIdenfifyView class
//

#include "stdafx.h"

#include "PlateIdenfify.h"
#include "PlateIdenfifyDoc.h"
#include "PlateIdenfifyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlateIdenfifyView

IMPLEMENT_DYNCREATE(CPlateIdenfifyView, CView)

BEGIN_MESSAGE_MAP(CPlateIdenfifyView, CView)
//{{AFX_MSG_MAP(CPlateIdenfifyView)
    ON_WM_CREATE()
    ON_COMMAND(ID_CLOSE, OnClose)
    ON_COMMAND(ID_FINDPLATE, OnFindPlate)
    ON_COMMAND(ID_SPLIETECHARA, OnSpliteChara)
    ON_COMMAND(ID_NET_IDENTIFY, OnNetIdentify)
    ON_COMMAND(ID_NET_TRAIN, OnNetTrain)
    ON_COMMAND(ID_TRAIN_NUM, OnTrainNum)
    ON_COMMAND(ID_TRAIN_LET, OnTrainLet)
    ON_COMMAND(ID_TRAIN_NUMLET, OnTrainNumlet)
    ON_COMMAND(ID_TRAIN_CHINESE, OnTrainChinese)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlateIdenfifyView construction/destruction

CPlateIdenfifyView::CPlateIdenfifyView()
{
    // TODO: add construction code here
    m_pOpenImg = NULL;
    m_pGrayImg = NULL;
    m_pCurrentImg = NULL;
    m_pTwoValueImg = NULL;
    m_pCanndy = NULL;
    m_pPlateArea = NULL;
    m_strFileName = "";
}

CPlateIdenfifyView::~CPlateIdenfifyView()
{
    CleanUp();
}

BOOL CPlateIdenfifyView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPlateIdenfifyView drawing

int nDrawx = 0, nDrawy = 0;
void CPlateIdenfifyView::OnDraw(CDC* pDC)
{
    CPlateIdenfifyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (m_pCurrentImg != NULL) {
        m_Showimg.SetImg(m_pCurrentImg);
        CRect rec;
        GetClientRect(&rec);
        rec.left = nDrawx;
        rec.top = nDrawy;
        rec.right = nDrawx + m_Showimg.Width();
        rec.bottom = nDrawy + m_Showimg.Height();
        m_Showimg.DrawToHDC(pDC->GetSafeHdc(), &rec);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CPlateIdenfifyView diagnostics

#ifdef _DEBUG
void CPlateIdenfifyView::AssertValid() const
{
    CView::AssertValid();
}

void CPlateIdenfifyView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CPlateIdenfifyDoc* CPlateIdenfifyView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlateIdenfifyDoc)));
    return (CPlateIdenfifyDoc *) m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlateIdenfifyView message handlers

int CPlateIdenfifyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here
    CPlateIdenfifyDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    pDoc->m_pView = (CWnd *)this;

    return 0;
}

void CPlateIdenfifyView::GetGray()
{
    // TODO: Add your command handler code here
    if (m_pCurrentImg == NULL)
        return;
    //ת��Ϊ�Ҷ�ͼ��
    m_pGrayImg = cvCreateImage(cvGetSize(m_pCurrentImg), 8, 1);
    cvCvtColor(m_pCurrentImg, m_pGrayImg, CV_BGR2GRAY);

    //����С��Ϊ800*600��С
    // 	IplImage *pTemp = cvCreateImage(cvSize(800,600),8,1);
    // 	cvResize(m_pGrayImg,pTemp,CV_INTER_LINEAR);
    // 	cvReleaseImage(&m_pGrayImg);
    // 	m_pGrayImg = pTemp;
    m_pCurrentImg = m_pGrayImg;
}

void CPlateIdenfifyView::AdaptiveThreshold()
{
    // TODO: Add your command handler code here

    m_pTwoValueImg = cvCreateImage(cvGetSize(m_pCurrentImg), 8, 1);
    cvThreshold(m_pCurrentImg, m_pTwoValueImg, 0, 255, CV_THRESH_OTSU);
    //cvAdaptiveThreshold(m_pCurrentImg, m_pTwoValueImg, 255,
    //CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3, 5);
    m_pCurrentImg = m_pTwoValueImg;
    int i,j;
    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;
    for(i = 0; i < height; ++i) {
        for(j = 0; j < width; ++j) {
            if(VALUE(m_pCurrentImg,j,i) != 0 && VALUE(m_pCurrentImg,j,i) != 255) {
                VALUE(m_pCurrentImg,j,i) = 0;
            }
        }
    }
}

void CPlateIdenfifyView::OnClose()
{
    // TODO: Add your command handler code here
    CleanUp();
    Invalidate(TRUE);
}

void CPlateIdenfifyView::CleanUp()
{
    if (m_pTwoValueImg != NULL)
        cvReleaseImage(&m_pTwoValueImg);
    if (m_pOpenImg != NULL)
        cvReleaseImage(&m_pOpenImg);
    if (m_pGrayImg != NULL)
        cvReleaseImage(&m_pGrayImg);
    if (m_pCanndy != NULL)
        cvReleaseImage(&m_pCanndy);
    if (m_pPlateArea != NULL)
        cvReleaseImage(&m_pPlateArea);
    m_pTwoValueImg = m_pOpenImg = m_pGrayImg = m_pCurrentImg = m_pCanndy = m_pPlateArea = NULL;
    m_characterName.clear();
}

// void CPlateIdenfifyView::OnHough()
// {
// 	// TODO: Add your command handler code here
// 	IplImage* src;
// 	src = m_pCurrentImg;
// 	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
// 	IplImage* color_dst = cvCreateImage(cvGetSize(src), 8, 3);
// 	CvMemStorage* storage = cvCreateMemStorage(0);
// 	CvSeq* lines = 0;
// 	int i;
// 	IplImage* src1 = cvCreateImage(cvSize(src->width, src->height),
// 						IPL_DEPTH_8U, 1);
//
// 	cvCvtColor(src, src1, CV_BGR2GRAY);
// 	cvCanny(src1, dst, 50, 200, 3);
//
// 	cvCvtColor(dst, color_dst, CV_GRAY2BGR);
//
// 	lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1,
// 				CV_PI / 180, 90, 30, 10);
// 	for (i = 0; i < lines->total; i++)
// 	{
// 		CvPoint* line = (CvPoint*) cvGetSeqElem(lines, i);
// 		cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, 8);
// 	}
//
// 	// 	cvNamedWindow("Source", 1);
// 	// 	cvShowImage("Source", src);
// 	//
// 	// 	cvNamedWindow("Hough", 1);
// 	// 	cvShowImage("Hough", color_dst);
// 	m_pCurrentImg = color_dst;
// 	Invalidate(FALSE);
// }

void CPlateIdenfifyView::GrayChange()
{
    // TODO: Add your command handler code here
    double hist[256] = {
        0
    };
    int i, j;
    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;
    UINT gray;
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            gray = ((uchar *) (m_pCurrentImg->imageData + i * width))[j];
            hist[gray]++;
        }
    }

    for (i = 0; i < 256; ++i)
        hist[i] /= (width * height);
    for (i = 1; i < 256; ++i)
        hist[i] += hist[i - 1];

    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            gray = ((uchar *) (m_pCurrentImg->imageData + i * width))[j];
            ((uchar *) (m_pCurrentImg->imageData + i * width))[j] = (UCHAR)
                    (hist[gray] * 255 + 0.5);
        }
    }
}

void SlopeAdjust(IplImage* pImg);

void CPlateIdenfifyView::OnPlatelocate()
{
    // TODO: Add your command handler code here
    if (m_pOpenImg == NULL)
        return;
    GetGray();
    GrayChange();
    //OnAdaptiveThreshold();
    CPoint start(0, 0);
    CPoint end(0, 0);

    //������������ɨ��
    int i, j;
    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;
    int length = (width > height) ? width : height;

    int* ptNum = new int [length];
    memset(ptNum, 0, sizeof(int) * length);
    int gray = 0;
    bool firstFind = false;

    for (i = height - 1; i >= 0; --i) {
        for (j = 0; j < width; ++j) {
            gray = ((UCHAR *) (m_pCurrentImg->imageData + i * width))[j];
            if (gray == 255)
                ptNum[i]++;
        }
        if (!firstFind && ptNum[i] > 30 && ptNum[i] < (width >> 1)) {
            firstFind = true;
            end.y = i;
        }
        if (firstFind) {
            if (ptNum[i] > 30)
                start.y = i;
            else {
                if ((end.y - start.y) > 20)
                    break;
                else
                    firstFind = false;
            }
            if ((end.y - start.y) > 40)
                break;
        }
    }
    start.y -= 2;
    end.y -= 4;

    memset(ptNum, 0, sizeof(int) * length);
    for (i = (width >> 2) ; i < width; ++i) {
        for (j = start.y; j <= end.y; ++j) {	//�ڳ��Ƹ߶ȷ�Χ�ĵ�ͳ��
            gray = ((UCHAR *) (m_pCurrentImg->imageData + j * width))[i];
            if (gray == 255)
                ptNum[i]++;
        }

        if ((ptNum[i] >= 10) || ptNum[i] > ptNum[i - 1] + 10) {
            start.x = i;
            break;
        }
    }
    end.x = 3 * (end.y - start.y) + start.x;
    //start.x -= 2;
    //	end.x += 2;

    IplImage* pTemp = cvCloneImage(m_pOpenImg);

    CvRect rec;
    rec.x = start.x;
    rec.y = start.y;
    rec.width = end.x - start.x;
    rec.height = end.y -
                 start.y;
    cvSetImageROI(pTemp, rec);
    CleanUp();

    m_pOpenImg = cvCreateImage(cvSize(pTemp->width, pTemp->height), 8, 1);
    m_pOpenImg = cvCloneImage(pTemp);
    m_pCurrentImg = m_pOpenImg;
    //OnGray();
    //	OnGraychange();
    //OnAdaptiveThreshold();

    // 	for (i = 0; i < height; ++i)
    // 	{
    // 		if (i <start.y || i> end.y)
    // 		{
    // 			for (j = 0; j < width; ++j)
    // 			{
    // 				((uchar *) (pTemp->imageData + i * width))[j] = 255;
    // 			}
    // 		}
    // 		else
    // 		{
    // 			for (j = 0; j < width; ++j)
    // 			{
    // 				if (j <start.x || j> end.x)
    // 					((uchar *) (pTemp->imageData + i * width))[j] = 255;
    // 			}
    // 		}
    // 	}

    delete[]ptNum;

    //SlopeAdjust(m_pCurrentImg);
    //Invalidate(TRUE);
}

void CPlateIdenfifyView::SlopeAdjust(IplImage* pImg)
{
    int i, j;
    double dAvgLHeight = 0; //ͼ����벿��ǰ�������ƽ���߶�
    double dAvgRHeight = 0; //ͼ���Ұ벿��ǰ�������ƽ���߶�
    int nLCount = 0, nRCount = 0;
    //ȡ��ͼ��ĸߺͿ�
    int nHeight = pImg->height;
    int nWidth = pImg->width;

    int nGray;

    //����ɨ����벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth / 2; j++) {
            nGray = VALUE(pImg, j, i);
            if (nGray == 255) { //��ǰ�����壨�ڣ�
                //����߶ȵļ�Ȩ�ͣ��������ߵ����ط�������Ȩ��
                dAvgLHeight += i;
                nLCount++;
            }
        }//for j
    }//for i

    dAvgLHeight /= nLCount; //ƽ����Ȩ�߶�

    //����ɨ���Ұ벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
    for (i = 0; i < nHeight; i++) {
        for (j = nWidth / 2; j < nWidth; j++) {
            nGray = VALUE(pImg, j, i);

            if (nGray == 255) { //��ǰ�����壨�ڣ�
                //����߶ȵļ�Ȩ�ͣ��������ߵ����ط�������Ȩ��
                dAvgRHeight += i;
                nRCount++;
            }
        }//for j
    }//for i

    dAvgRHeight /= nRCount;

    //����б��
    double dSlope = (dAvgLHeight - dAvgRHeight) / (nWidth / 2) ;
    dSlope = 0.01;
    int nYSrc; //y��Դ����

    IplImage* pTemp = cvCloneImage(pImg);
    //ɨ����ͼ�񣬸���б�ʵõ����¾�ͼ���ӳ���ϵΪÿһ�����ظ�ֵ
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            //�ҵ�����ͼ��ĵ�ǰ���Ӧ�ľ�ͼ����ˮƽ����(��ˮƽ�����е�Ϊ����)
            nYSrc = int(i - (j - nWidth / 2) * dSlope);
            if (nYSrc < 0 || nYSrc >= nHeight) //��Ӧ���ڲ���ͼ������֮��
                nGray = 0;
            else
                nGray = VALUE(pTemp, j, nYSrc);
            VALUE(pImg, j, i) = nGray; //����Դͼ���Ӧ������ֵΪ��ͼ�����ظ�ֵ
        }//for j
    }//for i
}

// void CPlateIdenfifyView::SlopeAdjust(IplImage *pImg )
// {
// 	int width = pImg->width;
// 	int height = pImg->height;
// 	int i,j,gray;
// 	double leftHeight = 0, rightHeight = 0;
// 	double leftWeightSum = 0, rightWeightSum = 0;
// 	double leftCount = 0, rightCount = 0;
// 	for(i = height/4; i <= height * 3/4; ++i)
// 	{
// 		for(j = width/4; j < (width >> 1); ++j)
// 		{
// 			gray = ((UCHAR*)(pImg->imageData + i * width))[j];
// 			if(gray == 255)
// 			{
// // 				leftWeightSum += ((width >> 1 - 1) - j);			//���ߵĵ��Ȩ��
// // 				leftHeight += ((width >> 1) - j) * i;			//����Ǽ�Ȩ��
// 				leftHeight += i;
// 				leftCount++;
// 			}
// 		}
// 		for(j = ((width >> 1) + 1); j < width * 3/4; ++j)
// 		{
// 			gray = ((UCHAR*)(pImg->imageData + i * width))[j];
// 			if(gray == 255)
// 			{
// //  				rightWeightSum += (j - (width >> 1 + 1));
// //  				rightHeight += (j - (width >> 1)) * i;
// 				rightHeight += i;
// 				rightCount++;
// 			}
// 		}
// 	}
// // 	double slope = (leftHeight / leftWeightSum / leftCount - rightHeight / rightWeightSum / rightCount)
// // 		/ (width >> 1);
// 	double slope = (leftHeight / leftCount - rightHeight / rightCount)
// 		/ (width >> 2);
// 	slope = 4;
// 	int y;
// 	IplImage *pTemp = cvCloneImage(pImg);
// 	for(i = 0; i < height; ++i)
// 	{
// 		for(j = 0; j < width; ++j)
// 		{
// 			y = (int)(i - (j - (width >> 1)) * slope);
// 			if(y < 0 || y >= height)
// 				((UCHAR*)(pImg->imageData + i * height))[j] = 0;
// 			else
// 			{
// 				gray = ((UCHAR*)(pTemp->imageData + y * height))[j];
// 				((UCHAR*)(pImg->imageData + i * height))[j] = gray;
// 			}
// 		}
// 	}
// 	cvReleaseImage(&pTemp);
// 	//m_pCurrentImg = pImg;
// 		Invalidate(TRUE);
// }

void CPlateIdenfifyView::OnCanny()
{
    using namespace std;
    if (m_pCurrentImg == NULL)
        return;

    GetGray();
    //	GrayChange();

    AdaptiveThreshold();
    // 	//----------------------
    // 	Invalidate(TRUE);
    // 	return;
    // 	//----------------------
    m_pCanndy = cvCreateImage(cvSize(m_pCurrentImg->width,
                                     m_pCurrentImg->height),	8, 1);
    cvCanny(m_pTwoValueImg, m_pCanndy, -1, -1, 3);
    m_pCurrentImg = m_pCanndy;
    Invalidate(TRUE);
}

void CPlateIdenfifyView::FindConnectArea(std::vector<std::vector<CPoint> >& ptVec)
{
    using namespace std;
    IplImage* pTemp = cvCloneImage(m_pCurrentImg);
    queue<CPoint> q;
    int x, y;
    CPoint start;
    int dir[8][2] = {
        { - 1, -1}, {0, -1}, {1, -1}, {1,0}, {1,1}, {0,1}, { - 1,1}, { - 1,0}
    };
    while (1 == FindFirstPt(pTemp, start)) {
        q.push(start);
        VALUE(pTemp, start.x, start.y) = 0;
        CPoint pt;
        vector<CPoint> v;
        v.push_back(CPoint(start.x, start.y));
        while (!q.empty()) {
            pt = q.front();
            q.pop();
            for (int i = 0; i < 8; ++i) {
                x = pt.x + dir[i][0];
                y = pt.y + dir[i][1];
                if (x < 0 || y < 0 || x >= pTemp->width || y >= pTemp->height)
                    continue;
                if (255 == VALUE(pTemp, x, y)) { // && 255 == VALUE(m_pCurrentImg,x,y))
                    q.push(CPoint(x, y));
                    v.push_back(CPoint(x, y));
                    VALUE(pTemp, x, y) = 0;
                }
            }
        }
        ptVec.push_back(v);
        v.clear();
    }
    cvReleaseImage(&pTemp);
}

bool CPlateIdenfifyView::FindFirstPt(IplImage* pImg, CPoint& pt)
{
    for (int i = 0; i < pImg->height; ++i) {
        for (int j = 0; j < pImg->width; ++j) {
            if (255 == VALUE(pImg, j, i)) {
                pt.x = j;
                pt.y = i;
                return 1;
            }
        }
    }
    return 0;
}

void CPlateIdenfifyView::OnPlateArea()
{
    using namespace std;
    // TODO: Add your command handler code here
    OnCanny();
    IplImage* pTemp = cvCloneImage(m_pCanndy);
    m_pCurrentImg = pTemp;
    cvDilate(m_pCurrentImg, m_pCurrentImg, NULL, 2);
    cvMorphologyEx(m_pCurrentImg, m_pCurrentImg, NULL, NULL, CV_MOP_CLOSE, 1);
    cvErode(m_pCurrentImg, m_pCurrentImg, NULL, 3);

    vector<vector<CPoint> > ptVec;
    vector<vector<CPoint> > ptUseVec;
    vector<MyRect> recVec;
    vector<MyRect> recUseVec;
    FindConnectArea(ptVec);

    //IplImage* pTemp = cvCreateImage(cvSize(m_pCurrentImg->width, m_pCurrentImg->height), 8, 1);
    //memset(m_pCurrentImg->imageData,0,m_pCurrentImg->height * m_pCurrentImg->width);
    //ɾѡ����1�����������ڵĿ�߱��ڡ�2��5��֮�䣬�ҳ���������ܶȴ���0.25
    int i, j, k;

    for (i = 0; i < ptVec.size(); ++i) {
        if (ptVec[i].size() < 50)					//̫�ٵ�����ֱ�Ӻ���
            continue;
        MyRect rec;
        rec.xMin = 0xffff, rec.xMax = -1;
        rec.yMin = 0xffff, rec.yMax = -1;
        for (j = 0; j < ptVec[i].size(); ++j) {
            if (ptVec[i][j].x < rec.xMin)
                rec.xMin = ptVec[i][j].x;
            if (ptVec[i][j].y < rec.yMin)
                rec.yMin = ptVec[i][j].y;
            if (ptVec[i][j].x > rec.xMax)
                rec.xMax = ptVec[i][j].x;
            if (ptVec[i][j].y > rec.yMax)
                rec.yMax = ptVec[i][j].y;
        }
        double ratio;
        ratio = (rec.xMax - rec.xMin + 1) / (rec.yMax - rec.yMin + 1);
        bool flag1 = (ratio >= 2 && ratio <= 5);
        ratio = ((double) (ptVec[i].size())) /
                ((rec.xMax - rec.xMin + 1) * (rec.yMax - rec.yMin + 1));
        bool flag2 = (ratio > 0.25);
        if (flag1 && flag2) {
            ptUseVec.push_back(ptVec[i]);
            recUseVec.push_back(rec);
        }
    }
    if (0 == ptUseVec.size()) {
        MessageBox("ʶ��������ʧ��");
        return;
    }
    if (1 == ptUseVec.size()) {
        GotTheArea(recUseVec[0]);
        return;
    }

    //����2�����������ڵĻҶ���������ڡ�5��15��֮��
    ptVec = ptUseVec;
    ptUseVec.clear();
    recVec = recUseVec;
    recUseVec.clear();

    double* gray = new double [m_pCurrentImg->width];
    int count;
    for (i = 0; i < ptVec.size(); ++i) {
        memset(gray, 0, m_pCurrentImg->width * sizeof(double));
        count = 0;

        int mid = (recVec[i].yMin + recVec[i].yMax) / 2;
        for (j = -1; j < 2; ++j) {				//����������м�����
            for (k = recVec[i].xMin; k <= recVec[i].xMax; ++k) {
                gray[k] += VALUE(m_pTwoValueImg, k, mid + j);
            }
        }
        const double DELTA = 255 * 2 / 3 - 1;
        gray[(int) (recVec[i].xMin)] /= 3;
        for (k = recVec[i].xMin + 1; k <= recVec[i].xMax; ++k) {
            gray[k] /= 3;						//ȡ�м�ֵ
            if (gray[k] - gray[k - 1] > DELTA)
                count++;
        }
        if (count >= 5 && count <= 15) {
            ptUseVec.push_back(ptVec[i]);
            recUseVec.push_back(recVec[i]);
        }
    }
    delete[]gray;

    if (0 == ptUseVec.size()) {
        MessageBox("ʶ��������ʧ��");
        return;
    }
    if (1 == ptUseVec.size()) {
        GotTheArea(recUseVec[0]);
        return;
    }

    //����3�����������ڵķ�����С
    int loc = 0;
    double min = 1.79769e+300;
    for (i = 0; i < ptUseVec.size(); ++i) {
        double xMean = 0.0, yMean = 0.0;
        double variance = 0.0;
        for (j = 0; j < ptUseVec[i].size(); ++j) {
            xMean += ptUseVec[i][j].x;
            yMean += ptUseVec[i][j].y;
        }
        xMean /= ptUseVec[i].size();
        yMean /= ptUseVec[i].size();

        for (j = 0; j < ptUseVec[i].size(); ++j) {
            variance += pow(ptUseVec[i][j].x - xMean, 2) +
                        pow(ptUseVec[i][j].y - yMean, 2);
        }
        if (variance < min) {
            min = variance;
            loc = i;
        }
    }
    GotTheArea(recUseVec[loc]);
    return;
}

void CPlateIdenfifyView::GotTheArea(MyRect& rec)
{
    m_r.x = rec.xMin;
    m_r.y = rec.yMin;
    m_r.width = rec.xMax - rec.xMin + 1;
    m_r.height = rec.yMax - rec.yMin + 1;

    cvSetImageROI(m_pOpenImg,m_r);
    cvReleaseImage(&m_pCurrentImg);
    m_pCurrentImg = m_pOpenImg;
    Invalidate(TRUE);
}

// bool CPlateIdenfifyView::SpliteCharacter()
// {
// 	//ȷ�����±߽�
// 	int i,j;
// 	int width = m_pCurrentImg->width;
// 	int height = m_pCurrentImg->height;
// 	CvRect rec;
// 	int count;
// 	for(i = 0; i < height; ++i)
// 	{
// 		count = 0;
// 		for(j = 0; j < width; ++j)
// 		{
// 			if(255 == VALUE(m_pCurrentImg, j, i))
// 			{
// 				count ++;
// 			}
// 		}
// 		if(count == 0 )// || count >= (width/2))			//�������ȫ�ڱߺͱ߿��߶�ȥ��
// 			continue;
// 		else
// 			break;
// 	}
// 	rec.y = i;
// 	for(i = height - 1; i >= 0; --i)
// 	{
// 		count = 0;
// 		for(j = 0; j < width; ++j)
// 		{
// 			if(255 == VALUE(m_pCurrentImg, j, i))
// 			{
// 				count++;
// 			}
// 		}
// 		if(count == 0 )//|| count >= (width/2))
// 			continue;
// 		else
// 			break;
// 	}
// 	rec.height = i - rec.y + 1;
// 	rec.x = 0; rec.width = m_pCurrentImg->width;
// 	cvSetImageROI(m_pCurrentImg, rec);
//
// 	//�ָ��ַ�
// 	width = rec.width;
// 	height = rec.height;
// 	//�ָ������ʼ��
// 	for(i = 0; i < 7; ++i)
// 	{
// 		m_recCharacter[i].y = rec.y;
// 		m_recCharacter[i].height = height;
// 	}
// 	int num = 0;				//�ѷָ���ַ��±�
// 	int start,end;				//ȷ����������ı߽�
// 	bool leftBoundry = false;	//�Ƿ��ҵ�����ߵı߽�
// 	bool inWhite;				//�Ƿ��ڰ�ɫ�����ڲ�
// 	for(i = 0; i < width; ++i)	//	�Կ�ִ�������ѭ��
// 	{
//
// 		for(j = 0; j < height; ++j)
// 		{
// 			if(0 == VALUE(m_pCurrentImg, i, j ))	//�ҵ���һ���ڵ㣨Ϊ���ų��߽�ĸ��ţ�
// 			{
// 				start = j;
// 				break;
// 			}
// 		}
// 		if(start == height )
// 			return false;
// 		for(j = height - 1; j >= 0; --j)
// 		{
// 			if(0 == VALUE(m_pCurrentImg, i, j ))	//�ҵ�����ĵ�һ���ڵ㣨Ϊ���ų��߽�ĸ��ţ�
// 			{
// 				end = j;
// 				break;
// 			}
// 		}
// 		if(end < 0)
// 			return false;
//
// 		//ͳ��һ�е��������
// 		count = 0;inWhite = false;
// 		for(j = start; j <= end; ++j)
// 		{
// 			if(255 == VALUE(m_pCurrentImg, i, j ) && !inWhite)	//��ɫͼ������һ�������ں�ɫ����
// 			{
// 				inWhite = true;
// 				count++;
// 			}
// 			else if(0 == VALUE(m_pCurrentImg, i, j ) && inWhite)	//�Ǻڵ���Ҫ����һ���ڰ�ɫ����
// 			{
// 				inWhite = false;
// 				count++;
// 			}
// 		}
// 		if(!leftBoundry && count >= 3)			//�����û��ȷ����߽�
// 		{
// 			leftBoundry = true;
// 			m_recCharacter[num].x = i - 1;
// 		}
//
// 		if(leftBoundry && count < 3 && (i - m_recCharacter[num].x + 1) > 8)
// 		//������߽�������С��2�Ҿ������8 �ſ���ȷ��һ���ַ��ָ���ɣ����ִ������ӵ����壩
// 		{
// 			leftBoundry = false;
// 			m_recCharacter[num].width = i - m_recCharacter[num].x + 2;
// 			num++;
// 		}
// 		if(7 == num)			//�Ѿ����ҵ���
// 			return true;
// 	}
// 	if(num != 7)				//û���ҵ�7���ַ�
// 		return false;
// 	return true;
// }

bool CPlateIdenfifyView::SpliteCharacter()
{
    //ȷ�����±߽�
    int i,j;
    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;
    CvRect rec;
    int count = 0;
    int changeCount = 0;
    for(i = 0; i < height; ++i) {
        count = 0;
        changeCount = 0;
        for(j = 1; j < width; ++j) {
            if(255 == VALUE(m_pCurrentImg, j, i)) {
                if(0 == VALUE(m_pCurrentImg, j - 1, i))
                    changeCount++;
                count++;
            }
        }
        //��ȥ������ȫ�ĺ��ߺͱ߽���ߺ�����<5������>18����
        if(count > 0 && count <= (width/2))
            break;
        if(changeCount >= 5 && changeCount <= 18)
            break;
    }
    rec.y = i - 1;
    for(i = height - 1; i >= 0; --i) {
        count = 0;
        changeCount = 0;
        for(j = 0; j < width; ++j) {
            if(255 == VALUE(m_pCurrentImg, j, i)) {
                if(0 == VALUE(m_pCurrentImg, j - 1, i))
                    changeCount++;
                count++;
            }
        }
        if(count > 0 && count <= (width/2))
            break;
        if(changeCount >= 5 && changeCount <= 18)
            break;
    }
    rec.height = i - rec.y + 1;
    rec.x = 0;
    rec.width = m_pCurrentImg->width;

    // 	cvSetImageROI(m_pCurrentImg,rec);
    // 	Invalidate(TRUE);
    // 	return false;
    //�ָ��ַ�
    width = rec.width;
    height = rec.height;
    //�ָ������ʼ��
    for(i = 0; i < 7; ++i) {
        m_recCharacter[i].y = rec.y;
        m_recCharacter[i].height = height;
    }

    int start,end;				//ȷ����������ı߽�
    int *grayMean = new int [width + 1];
    memset(grayMean,0,sizeof(int) * (width + 1));
    grayMean[width] = 1000;		//������Ϊ�˼��밲ȫ��ϵ��(�߽�������

    for(i = 0; i < width; ++i) {	//	�Կ�ִ�������ѭ��
        for(j = rec.y; j < rec.y + rec.height; ++j) {
            if(0 == VALUE(m_pCurrentImg, i, j )) {	//�ҵ���һ���ڵ㣨Ϊ���ų��߽�ĸ��ţ�
                start = j;
                break;
            }
        }
        if(start == height )
            return false;
        for(j = rec.y + rec.height - 1; j >= rec.y; --j) {
            if(0 == VALUE(m_pCurrentImg, i, j )) {	//�ҵ�����ĵ�һ���ڵ㣨Ϊ���ų��߽�ĸ��ţ�
                end = j;
                break;
            }
        }
        if(end < 0)
            return false;

        for(j = start; j <= end; ++j) {				//���㷶Χ�ڵ�ƽ��ֵ
            grayMean[i] += VALUE(m_pCurrentImg, i, j);
        }
        grayMean[i] /= (end - start + 1);
    }

    bool leftBoundry = false;	//�Ƿ��ҵ�����ߵı߽�
    int num = 0;				//�ҵ��ľ�����������
    for(i = 1; i < width; ++i) {
        if(2 == num && leftBoundry && 0 == grayMean[i]
                && i - m_recCharacter[num].x + 1 < width / 10) {		//���������е��Ӱ��
            leftBoundry = false;
            --i;
            continue;
        }
        if(!leftBoundry && grayMean[i] <= grayMean[i - 1] && grayMean[i] < grayMean[i + 1]) {
            leftBoundry = true;
            m_recCharacter[num].x = i;
        }
        if(leftBoundry && grayMean[i] < grayMean[i - 1] && grayMean[i] <= grayMean[i + 1]
                && i - m_recCharacter[num].x + 1 >= width / 10) {			//����8������
            leftBoundry = false;
            m_recCharacter[num].width = i - m_recCharacter[num].x + 1;
            num++;
        }
        if(7 == num) {
            delete []grayMean;
            return true;
        }
    }
    delete []grayMean;
    if(num != 7)
        return false;
    return true;
}

void CPlateIdenfifyView::OnSplite()
{
    using namespace std;
    int i;

    OnPlateArea();
    m_pCurrentImg = m_pGrayImg;
    GrayChange();
    AdaptiveThreshold();

    m_pPlateArea = cvCreateImage(cvSize(m_r.width,m_r.height),8,1);
    CopyRect(m_pTwoValueImg,m_pPlateArea,m_r);
    m_pCurrentImg = m_pPlateArea;
    Rotate();

    if(!SpliteCharacter()) {
        MessageBox("�ָ��ַ�ʧ��");
        return;
    }
    m_pCurrentImg = m_pPlateArea;

    CvPoint start,end;
    cvResetImageROI(m_pOpenImg);
    for(i = 0; i < 7; ++i) {
        start.x = m_recCharacter[i].x + m_r.x;
        start.y = m_recCharacter[i].y + m_r.y;
        end.x = start.x + m_recCharacter[i].width - 1;
        end.y = start.y + m_recCharacter[i].height - 1;
        cvRectangle(m_pOpenImg,start,end,CV_RGB(255,0,0));
    }
    m_pCurrentImg = m_pOpenImg;
    CString fileName = m_strFileName;
    fileName = m_strFileName.Left(m_strFileName.ReverseFind('\\'));
    fileName += "\\character";
    cvResetImageROI(m_pPlateArea);
    for(i = 0; i < 7; ++i) {
        char id = i + '0';
        CString strTemp = fileName + id + ".bmp";
        IplImage *pTemp = cvCreateImage(cvSize(m_recCharacter[i].width, m_recCharacter[i].height),
                                        m_pPlateArea->depth, 1);
        //		CopyRect(m_pPlateArea, pTemp, m_recCharacter[i]);
        cvSetImageROI(m_pPlateArea,m_recCharacter[i]);
        cvCopy(m_pPlateArea,pTemp,NULL);
        cvResetImageROI(m_pPlateArea);
        //������ʵ�����ŵĹ���

        IplImage *dst = cvCreateImage(cvSize(32,64),8,1);
        cvResize(pTemp,dst,CV_INTER_LINEAR);
        cvReleaseImage(&pTemp);
        m_pCurrentImg = pTemp = dst;
        //	ReverseGray();
        // 		int *count = new int[pTemp->height];
        // 		for(u = 0; u < pTemp->height; ++u)
        // 		{
        // 			for(v = 0; v < pTemp->width; ++v)
        // 			{
        // 				if(0 == VALUE(pTemp,v,u))
        // 					count[u]++;
        // 			}
        // 		}
        // 		for(u = 1; u < pTemp->height - 1; ++u)
        // 		{
        // 			if(count[u] != 0 && count[u-1] == 0 && count[u+1] == 0)
        // 			{
        // 				for(v = 0; v < pTemp->width; ++v)
        // 				{
        // 					VALUE(pTemp,v,u) = 255;
        // 				}
        // 			}
        // 		}
        // 		delete []count;
        cvSaveImage(strTemp, pTemp);
        cvReleaseImage(&pTemp);
    }
    m_pCurrentImg = m_pOpenImg;
    Invalidate(true);
}

/*********************************************************
//����ͼ���ѡ��������ת��ͼ������Ƕ�ֵ��ͼ��
*********************************************************/
void CPlateIdenfifyView::Rotate()
{
    if(m_pCurrentImg == NULL)
        return;

    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;

    int i, j;
    CvScalar s;
    double dAvgLHeight = 0; //ͼ����벿��ǰ�������ƽ���߶�
    double dAvgRHeight = 0; //ͼ���Ұ벿��ǰ�������ƽ���߶�
    int nLCount = 0, nRCount = 0;
    //ȡ��ͼ��ĸߺͿ�

    int nGray;
    //����ɨ����벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
    for (i = 0; i < height; i++) {
        for (j = 0; j < width / 2; j++) {
            s = cvGet2D(m_pCurrentImg,i,j);
            nGray = (int)s.val[0];
            if (nGray == 255) {
                dAvgLHeight += i;
                nLCount++;
            }
        }
    }

    dAvgLHeight /= nLCount; //���ƽ���߶�
    //����ɨ���Ұ벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
    for (i = 0; i < height; i++) {
        for (j = width / 2 ; j < width; j++) {
            s = cvGet2D(m_pCurrentImg,i,j);
            nGray = (int)s.val[0];
            if (nGray == 255) {
                dAvgRHeight += i;
                nRCount++;
            }
        }
    }

    dAvgRHeight /= nRCount;				//�ұ�ƽ���߶�
    double slope = (dAvgLHeight - dAvgRHeight) / (width / 2) ;	//��ת�Ƕȵ�б��

    IplImage *pTemp = cvCreateImage(cvGetSize(m_pCurrentImg),8,1);
    int u,v;
    double dsin,dcos;
    dsin = sin(atan(slope));
    dcos = cos(atan(slope));
    for(j = 0; j < height; ++j) {	//for y
        for(i = 0; i < width; ++i) { //for x
            u = (int)(i * dcos + j *dsin - 0.5 * (width * dcos + height * dsin - width));
            v = (int)(j * dcos - i * dsin + 0.5 * (width * dsin - height * dcos + height));
            s = cvGet2D(pTemp,j,i);
            if(u < 0 || v < 0 || u >= width || v >= height)
                s.val[0] = 0;
            else
                s.val[0] = cvGet2D(m_pCurrentImg,v,u).val[0];//VALUE(m_pCurrentImg,u,v);
            cvSet2D(pTemp,j,i,s);
        }
    }

    cvReleaseImage(&m_pCurrentImg);
    m_pCurrentImg = cvCloneImage(pTemp);
    cvReleaseImage(&pTemp);
}

void CPlateIdenfifyView::CopyRect( IplImage *src, IplImage *dst, CvRect rec )
{
    int i,j;
    for(i = rec.y; i < rec.y + rec.height; ++i) {
        for(j = rec.x; j < rec.x + rec.width; ++j) {
            VALUE(dst, j - rec.x, i - rec.y) = VALUE(src, j, i);
        }
    }
}

void CPlateIdenfifyView::ReverseGray(IplImage *img)
{
    CvScalar s;
    int i,j;
    int width = img->width;
    int height = img->height;
    for(i = 0; i < height; ++i) {
        for(j = 0; j < width; ++j) {
            s = cvGet2D(img,i,j);
            s.val[0] = (s.val[0] == 255) ? 0:255;
            cvSet2D(img,i,j,s);
            //VALUE(img, j, i) = (VALUE(img, j, i) == 255) ? 0 : 255;
        }
    }
}

/////////////////////////////////////new method//////////////

//Ѱ��ͼ���е������ı���
CvSeq* CPlateIdenfifyView:: findSquares4( IplImage* img, CvMemStorage* storage )
{
    CvSeq* contours;
    int i, c, l, N = 11;
    CvSize sz = cvSize( img->width & -2, img->height & -2 );
    IplImage* timg = cvCloneImage( img ); // make a copy of input image
    IplImage* gray = cvCreateImage( sz, 8, 1 );
    IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );
    IplImage* tgray;
    CvSeq* result;
    double s, t;

    //���������ı�������4���㱣��һ������
    CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );

    // select the maximum ROI in the image
    // with the width and height divisible by 2
    cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
    cvPyrDown( timg, pyr, 7 );
    cvPyrUp( pyr, timg, 7 );
    tgray = cvCreateImage( sz, 8, 1 );

    // find squares in every color plane of the image
    for( c = 0; c < 3; c++ ) {
        // extract the c-th color plane
        cvSetImageCOI( timg, c+1 );
        cvCopy( timg, tgray, 0 );

        // try several threshold levels
        for( l = 0; l < N; l++ ) {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 ) {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                cvCanny( tgray, gray,60, 180, 3 );
                // dilate canny output to remove potential
                // holes between edge segments
                cvDilate( gray, gray, 0, 1 );
            } else {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                //cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
                cvThreshold( tgray, gray, 50, 255, CV_THRESH_BINARY );
            }

            // find contours and store them all as a list
            cvFindContours( gray, storage, &contours, sizeof(CvContour),
                            CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

            // test each contour
            while( contours ) {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                result = cvApproxPoly( contours, sizeof(CvContour), storage,
                                       CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( result->total == 4 &&
                        fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&
                        cvCheckContourConvexity(result) ) {
                    s = 0;

                    for( i = 0; i < 5; i++ ) {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 ) {
                            t = fabs(angle(
                                         (CvPoint*)cvGetSeqElem( result, i ),
                                         (CvPoint*)cvGetSeqElem( result, i-2 ),
                                         (CvPoint*)cvGetSeqElem( result, i-1 )));
                            s = s > t ? s : t;
                        }
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.3 )
                        for( i = 0; i < 4; i++ )
                            cvSeqPush( squares,
                                       (CvPoint*)cvGetSeqElem( result, i ));
                }

                // take the next contour
                contours = contours->h_next;
            }
        }
    }

    // release all the temporary images
    cvReleaseImage( &gray );
    cvReleaseImage( &pyr );
    cvReleaseImage( &tgray );
    cvReleaseImage( &timg );

    return squares;
}

// the function draws all the squares in the image
void CPlateIdenfifyView::drawSquares( IplImage* img, CvSeq* squares )
{
    using namespace std;
    CvSeqReader reader;
    int i,j;
    CvPoint *rect = new CvPoint[4];
    int count = 4;
    // initialize reader of the sequence

    cvStartReadSeq( squares, &reader, 0 );
    CvRect r,lastRect;
    MyRect CompareRec;
    memset(&r,0,sizeof(CvRect));
    memset(&lastRect,0,sizeof(CvRect));

    // read 4 sequence elements at a time (all vertices of a square)
    for( i = 0; i < squares->total; i += 4 ) {
        // read 4 vertices
        memcpy( rect, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( rect + 1, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( rect + 2, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );
        memcpy( rect + 3, reader.ptr, squares->elem_size );
        CV_NEXT_SEQ_ELEM( squares->elem_size, reader );

        // draw the square as a closed polyline
        if(rect[0].x != 1 && rect[0].y != 1) {		//ȷ�����Ǳ�ʾͼƬ����ľ���
            lastRect = r;									//����������һ�����ε�����
            CompareRec.xMin = 0xffff, CompareRec.xMax = -1;	//�ҵ���Χ�Ŀ��Ƶ�
            CompareRec.yMin = 0xffff, CompareRec.yMax = -1;
            for (j = 0; j < 4; ++j) {
                if (rect[j].x < CompareRec.xMin)
                    CompareRec.xMin = rect[j].x;
                if (rect[j].y < CompareRec.yMin)
                    CompareRec.yMin = rect[j].y;
                if (rect[j].x > CompareRec.xMax)
                    CompareRec.xMax = rect[j].x;
                if (rect[j].y > CompareRec.yMax)
                    CompareRec.yMax = rect[j].y;
            }
            r.x = CompareRec.xMin;
            r.y = CompareRec.yMin;
            r.width = CompareRec.xMax - CompareRec.xMin + 1;
            r.height = CompareRec.yMax - CompareRec.yMin + 1;
            if(lastRect.x != r.x || lastRect.y != r.y)
                m_recArea.push_back(r);
            //cvPolyLine( img, &rect, &count, 1, 1, CV_RGB(0,255,0), 1, CV_AA, 0 );
        }
    }
    delete []rect;
}

/******************************************************
//Ѱ�ҳ�������ĺ���
//����findSquares4�ҳ�ͼ���е������ı���
//�������򱣴���m_bestRec��
******************************************************/
void CPlateIdenfifyView::OnFindPlate()
{
    if(m_pOpenImg == NULL) {
        MessageBox("û�д�ͼƬ");
        return;
    }

    CvMemStorage* storage = cvCreateMemStorage(0);
    drawSquares( m_pCurrentImg, findSquares4( m_pCurrentImg, storage ) );

    if(m_recArea.size() == 0) {	//m_recArea�б�����ǿ��ܵĳ�������
        MessageBox("��������λʧ��");
        return;
    }
    if(m_recArea.size() == 1) {	//һ����λ�Ķ�λ
        std::list<CvRect>::iterator i = m_recArea.begin();
        double ratio = (i->width) / (double)(i->height);
        if(ratio >= 2 && ratio <= 5)			//������Ҫ�����������
            m_bestRec = *m_recArea.begin();
        else {
            MessageBox("��������λʧ��");
            return;
        }
    } else {
        int count = 0;
        int maxCount = -1;
        int u,v;

        for(std::list<CvRect>::iterator i = m_recArea.begin(); i != m_recArea.end(); ++i) {
            //�ڶ����ѡ�������ҵ���ɫ�������Ǹ�
            double ratio = (i->width) / (double)(i->height);
            if(ratio >= 2 && ratio <= 5) {		//������Ҫ�����������
                count = 0;
                CvRect r = *i;
                for(u = r.y; u < r.y + r.height ; ++u) {	//��������ͳ����ɫ��ĸ���
                    for(v = r.x; v < r.x + r.width; ++v)
                        if(BeBlue(m_pOpenImg,v,u))
                            count++;
                }
                if(count > maxCount) {					//��Ѱ���ֵ
                    maxCount = count;
                    m_bestRec = r;
                }
            }
        }

        if(maxCount == -1) {	//������û�з���Ҫ���
            MessageBox("��������λʧ��");
            return;
        }
    }

    //����Χ�ſ�һ������
    if(m_bestRec.x > 0)
        m_bestRec.x --;
    if(m_bestRec.y > 0)
        m_bestRec.y --;
    if(m_bestRec.y + m_bestRec.height - 1 + 2 >= m_pOpenImg->height - 1)
        m_bestRec.height += 2;
    if(m_bestRec.x + m_bestRec.width - 1 + 2>= m_pOpenImg->width - 1)
        m_bestRec.width += 2;

    m_pPlateArea = cvCreateImage(cvSize(m_bestRec.width,m_bestRec.height),
                                 m_pOpenImg->depth,m_pOpenImg->nChannels);

    cvSetImageROI(m_pOpenImg,m_bestRec);
    cvCopy(m_pOpenImg,m_pPlateArea);

    m_recArea.clear();
    m_pCurrentImg = m_pPlateArea;
    Invalidate(true);

    return;
}

/******************************************************
//����ָ��ַ��ĺ���
//�ȵ���OnFindPlate�ҵ���������
//��Ԥ����1.��Ϊ�Ҷ�ͼ�� 2.��Ϊ��ֵͼ�� 3��ת
//��������������(1:�������ƶ���ı߿� 2:�ָ������ַ�
******************************************************/
void CPlateIdenfifyView::OnSpliteChara()
{
    if(m_pOpenImg == NULL) {
        MessageBox("û�д�ͼƬ");
        return;
    }

    if(m_pPlateArea == NULL) {
        MessageBox("����Ѱ�ҳ���");
        return;
    }

    //Ԥ����
    m_pGrayImg = cvCreateImage(cvGetSize(m_pCurrentImg), 8, 1);		//�Ҷ�ͼ��
    cvCvtColor(m_pCurrentImg, m_pGrayImg, CV_BGR2GRAY);
    m_pTwoValueImg = cvCreateImage(cvGetSize(m_pGrayImg), 8, 1);	//��ֵͼ��
    cvThreshold(m_pGrayImg, m_pTwoValueImg, 0, 255.0, CV_THRESH_OTSU);
    m_pCurrentImg = m_pTwoValueImg;
    Rotate();					//��תͼ��

// 	Invalidate(true);
// 	return;

    int i,j;
    int start,end;
    int width = m_pCurrentImg->width;
    int height = m_pCurrentImg->height;
    CvScalar s;
//-------------------������ɫ�����߿�(Ч������)--------------------------
// 	m_pCurrentImg = m_pTwoValueImg;

// 	MyRect myRect;
// 	bool find = false;
// 	for(i = 0; i < bestRec.height; ++i)
// 	{
// 		for(j = 0; j < bestRec.width; ++j)
// 		{
// 			if(BeBlue(pTemp,j,i))
// 			{
// 				myRect.xMin = j;
// 				myRect.yMin = i;
// 				find = true;
// 				break;
// 			}
// 		}
// 		if(find)
// 			break;
// 	}
// 	if(!find || myRect.yMin > (bestRec.height/4))
// 	{
// 		MessageBox("�ָ��ַ�ʧ��");
// 		return;
// 	}
// 	i = myRect.yMin;
// 	for(j = bestRec.width - 1; j > myRect.xMin; --j)
// 	{
// 		if(BeBlue(pTemp,j,i))
// 		{
// 			myRect.xMax = j;
// 			break;
// 		}
// 	}
// 	if(j == myRect.xMin)
// 	{
// 		MessageBox("�ָ��ַ�ʧ��");
// 		return;
// 	}
// 	j = myRect.xMax;
// 	for(i = bestRec.height - 1; i > myRect.yMin; --i)
// 	{
// 		if(BeBlue(pTemp,j,i))
// 		{
// 			myRect.yMax = i;
// 			break;
// 		}
// 	}
// 	if(i == myRect.yMin)
// 	{
// 		MessageBox("�ָ��ַ�ʧ��");
// 		return;
// 	}
// 	CvRect r;
// 	r.x = myRect.xMin;r.y = myRect.yMin;
// 	r.width = myRect.xMax - myRect.xMin + 1;
// 	r.height = myRect.yMax - myRect.yMin + 1;
// 	cvSetImageROI(m_pTwoValueImg,r);
// 	m_pCurrentImg = m_pTwoValueImg;

////////////////////������ɫ�����߿�---------------------------------------

//-------------------------������ɫ������ȡ�����±߿�----------------------
//-----------------------(�����ϵĹ̶����Ӳ�������)------------------------

// 	int count = 0;
// 	int changeCount = 0;
// 	//ȥ������ı߽�
// 	for(i = 0; i < height; ++i)
// 	{
// 		count = 0;changeCount = 0;
// 		for(j = 1; j < width; ++j)
// 		{
// 			if(255 == VALUE(m_pCurrentImg, j, i))
// 			{
// 				if(0 == VALUE(m_pCurrentImg, j - 1, i))
// 					changeCount++;
// 				count++;
// 			}
// 		}
// 		//��ɫ��������Ǻܶࣨ�ַ���������<5������>18���߾�ֹͣ������Ķ��Ǳ߿��ߣ�
// 		if(count <= (width/2) && changeCount >= 6 && changeCount <= 18)
// 			break;
// 	}
// 	m_bestRec.y = i - 1;
//
// 	//ȥ������ı߽�
// 	for(i = height - 1; i >= 0; --i)
// 	{
// 		count = 0;changeCount = 0;
// 		for(j = 0; j < width; ++j)
// 		{
// 			if(255 == VALUE(m_pCurrentImg, j, i))
// 			{
// 				if(0 == VALUE(m_pCurrentImg, j - 1, i))
// 					changeCount++;
// 				count++;
// 			}
// 		}
// 		if(count <= (width/2) && changeCount >= 6 && changeCount <= 18)
// 			break;
// 	}
// 	m_bestRec.height = i - m_bestRec.y + 1;
// 	m_bestRec.x = 0; m_bestRec.width = width;

///////////////////////������ɫ������ȡ�����±߿�--------------------------

//----------------------------����ͶӰ�ĳ��ƶ�λ---------------------------

    //hCountͳ��ˮƽ����ͶӰ�İ�ɫ�����
    int *hCount = new int[height];
    memset(hCount, 0, sizeof(int) * height);
    for(i = 0; i < height; ++i) {
        for(j = 0; j < width; ++j) {
            s = cvGet2D(m_pCurrentImg,i,j);
            if(255 == s.val[0])
                hCount[i]++;
        }
    }

    //ȥ�����Ϻ�������ĺڵ㣨�Է���Ϊ���ڲ���������
    for(i = 0; i < height; ++i) {
        if(hCount[i] != 0)
            break;
    }
    start = (0 == i) ? 1 : i;
    for(i = height - 1; i >=0; --i) {
        if(hCount[i] != 0)
            break;
    }
    end = (i == (height - 1)) ? (height - 2) : i;

    //Ѱ������ı߿�
    for(i = start; i <= end; ++i) {
        if(hCount[i] <= (width / 2) && hCount[i] <= hCount[i-1]
                && hCount[i] < hCount[i+1]) {
            break;
        }
    }
    m_bestRec.y = i;
    if(m_bestRec.y >= (height / 2)) {
        MessageBox("�ָ��ַ�ʧ��");
        return;
    }
    //�������϶��ӵ�Ӱ��
    if(hCount[i] <= (width/8)) {	//�׵�̫�ٲ����ַ����Ƕ���
        for(; i <= end; ++i) {
            if(hCount[i] > (width /8))	//ֱ���ҵ��϶�׵���ߣ�ȥ����ɫԲȦѰ�ҵ�һ���ַ���
                break;
        }
    }
    m_bestRec.y = i;
    if(i >= (height / 2)) {
        MessageBox("�ָ��ַ�ʧ��");
        return;
    }

    //Ѱ������ı߿�
    for(i = end; i >= start; --i) {
        if(hCount[i] <= (width / 2) && hCount[i] < hCount[i-1]
                && hCount[i] <= hCount[i+1]) {
            m_bestRec.height = i - m_bestRec.y + 1;
            break;
        }
    }
    if(i < (height / 2)) {
        MessageBox("�ָ��ַ�ʧ��");
        return;
    }
    delete []hCount;
    m_bestRec.x = 0;
    m_bestRec.width = width;

    //���û�б߿��ߣ���ô�ͻ���ֱȽ����صĴ��󣬾��������ַ��ĵ�һ���ļ�ֵ��
    if(m_bestRec.y > (m_pCurrentImg->height / 4))
        m_bestRec.y = start;

    //��Χ������һ��
    if(m_bestRec.y > 0)
        m_bestRec.y --;
    if(m_bestRec.y + m_bestRec.height - 1 + 2 <= m_pOpenImg->height - 1)
        m_bestRec.height += 2;

// 	cvSetImageROI(m_pCurrentImg,m_bestRec);
// 	Invalidate(true);
// 	return;

    //ȥ�����ҵı߿���
    //vCount����ͳ�ƴ�ֱͶӰ��ɫ��ĸ���
    int *vCount = new int[width + 1];
    memset(vCount,0,sizeof(int) * (width + 1));
    vCount[width] = 10000;

    for(j = 0; j < width; ++j) {
        for(i = m_bestRec.y; i < m_bestRec.y + m_bestRec.height; ++i) {
            s = cvGet2D(m_pCurrentImg,i,j);
            if(255 == (int)s.val[0]) {
                vCount[j]++;
            }
        }
    }

    //ȥ������ߺ����ұߵĺڵ㣨�Է���Ϊ���ڲ���������
    for(j = 0; j < width; ++j) {
        if(vCount[j] != 0)
            break;
    }
    start = (0 == j) ? 1 : j;
    for(j = width - 1; j >=0; --j) {
        if(vCount[j] != 0)
            break;
    }
    end = (j == (width - 1)) ? (width - 2) : j;

    //Ѱ����ߵı߽磬��ȥ�߿��Ӱ��
    for(j = start; j <= end; ++j) {
        if(vCount[j] <= (m_bestRec.height / 2) && vCount[j] <= vCount[j-1] //��������׵㣬��Ȼ�ǰ��ߵ�λ��
                && vCount[j] < vCount[j+1] ) {	//�ֲ���С�ĵ�
            break;
        }
    }
    if(j <= (width/8))
        m_bestRec.x = j;
    else
        m_bestRec.x = 0;				//��������Ǳ߿��ߺ���������һ��Ľ��

    //Ѱ���ұߵı߽磬��ȥ�߿��Ӱ��
    for(j = end; j >= start ; --j) {
        if(vCount[j] <= (m_bestRec.height / 2) && vCount[j] <= vCount[j+1]
                && vCount[j] < vCount[j-1] ) {
            break;
        }
    }
    if(j >=(width*7/8))
        m_bestRec.width = j - m_bestRec.x + 1;
    else
        m_bestRec.width = width - m_bestRec.x + 1;

    if(m_bestRec.x > 0)
        m_bestRec.x--;
    if(m_bestRec.x + m_bestRec.width - 1 + 2 <= m_pCurrentImg->width - 1)
        m_bestRec.width += 2;
// 	cvSetImageROI(m_pCurrentImg,m_bestRec);
// 	Invalidate(true);
// 	return;

////////////////����ͶӰ�ĳ��ƶ�λ-----------------------------------------

//----------------------------�ָ��ַ����㷨-------------------------------

//----------------ֱ�ӻ���ͶӰ���������ָ�---------------------------------
//----------------(ȱ���ǲ��������Լ�����ģ���������ϴ�ʶ�𲻺�)---------

    //m_bestRec�ľ���ָ��֮��,����������ڷָ��ַ�
    bool leftBoundry = false;	//�Ƿ��ҵ�����ߵı߽�
    int num = 0;				//�ҵ��ľ�����������

    //�ָ������ʼ��
    for(i = 0; i < 7; ++i) {
        m_recCharacter[i].y = m_bestRec.y;
        m_recCharacter[i].height = m_bestRec.height;
    }
    vCount[m_bestRec.x + m_bestRec.width] = 0;			//�߽����
    bool addFlag = false;

// 	using namespace std;
// 	fstream fout;
// 	fout.open("vCount.txt",ios::out);
// 	for(i = 0; i < width; ++i)
// 		fout<<vCount[i]<<" ";
// 	fout.close();

// 	int *temp = new int[m_bestRec.width];
// 	for(i = m_bestRec.x; i < m_bestRec.x + m_bestRec.width; ++i)
// 	{
// 		temp[i - m_bestRec.x] = vCount[i];
// 	}
// 	for(int k = 0; k < 3; ++k)
// 	{
// 		for(i = k + 1; i < m_bestRec.width; ++i)
// 		{
// 			if(temp[i] < temp[k])
// 			{
// 				int t = temp[i];
// 				temp[i] = temp[k];
// 				temp[k] = t;
// 			}
// 		}
// 	}
// 	int meanMin = (temp[0] + temp[1] + temp[2]) / 3;
// 	delete []temp;

    for(i = m_bestRec.x; i <= m_bestRec.x + m_bestRec.width; ++i) {
        if(!leftBoundry && vCount[i] > (m_bestRec.height / 12) && vCount[i+1] > (m_bestRec.height / 12)
                && vCount[i+2] > (m_bestRec.height / 12)) {
            leftBoundry = true;
            m_recCharacter[num].x = i;
            continue;
        }
        if(leftBoundry && vCount[i] <= (m_bestRec.height / 12) + 1) {
            //����д����������м���ֽ��������(���ֵ��ʵ��ó��ĽϺõ�ֵ��
            if(i - m_recCharacter[num].x + 1 >= m_bestRec.width / 8 - 4) {
                leftBoundry = false;
                m_recCharacter[num].width = i - m_recCharacter[num].x + 1;
                num++;
                continue;
            } else if(i - m_recCharacter[num].x + 1 >= (m_bestRec.width / 36)) { //����һ��������ǿ���Ϊ����'1'
                //��Сһ���ľ���
                start = m_recCharacter[num].x + 1;
                end = i-1;
                //��һ����С��Χ���ҵ�����1��׼ȷ����)
                for(j = start; j <=end; ++j) {
                    if(vCount[j] > (m_bestRec.height / 12))
                        break;
                }
                start = j;
                for(j = end; j >=start; --j) {
                    if(vCount[j] > (m_bestRec.height / 12))
                        break;
                }
                end = j;
                //���ϴ��³���Ҫ��ʱ��
                if(end  - start + 1 >= (m_bestRec.width / 36)) {
                    int count = 0;
                    for(j = start; j <= end; ++j) {			//ͳ��ÿһ�еĸ���
                        if(vCount[j] >= (m_bestRec.height * 2/3))
                            count++;
                    }
                    if(count >= ((end - start + 1) * 2/3)) {	//�����1�����
                        leftBoundry = false;
                        m_recCharacter[num].width = i - m_recCharacter[num].x + 1;
                        num++;
                        continue;
                    }
                }
            }
            if(!addFlag && 2 == num) {						//�����м�׵��Ӱ�죨���ҵ��ڶ���֮����У�
                i += (m_bestRec.width / 24);		//���׵���м�հ׵����򲻿���
                addFlag = true;						//��������ֻ���Լ���һ��
                leftBoundry = false;				//������㿪ʼ������ߵı߽�
                --i;
            }
        }
        if(7 == num)
            break;
    }
    if(6 == num && leftBoundry) {
        m_recCharacter[num].width = i - 1 - m_recCharacter[num].x;
        num++;
    }

////////////////////ֱ�ӻ���ͶӰ���������ָ�-------------------------------

//---------����Ѱ�Ҿֲ���С��ķָ���ڵ����������������1������T�ַ���ʱ�򲻺÷ָ---
//	vCount[m_bestRec.x + m_bestRec.width] = 100000;			//�߽����
// 	for(i = m_bestRec.x; i < m_bestRec.x + m_bestRec.width; ++i)
// 	{
// 		TRACE("%d ",vCount[i]);
// 		if(!leftBoundry && vCount[i] < (m_bestRec.height / 8)
// 			&& vCount[i] <= vCount[i - 1] && vCount[i] < vCount[i + 1])
// 		{
// 			leftBoundry = true;
// 			m_recCharacter[num].x = i;
// 			continue;
// 		}
// 		if(leftBoundry && vCount[i] < (m_bestRec.height / 8) &&
// 			vCount[i] < vCount[i - 1] && vCount[i] <= vCount[i + 1] )
// 		{
// 			//����д����������м���ֽ��������(���ֵ��ʵ��ó��ĽϺõ�ֵ��
// 			if(i - m_recCharacter[num].x + 1 >= m_bestRec.width / 8 - 2)
// 			{
// 				leftBoundry = false;
// 				m_recCharacter[num].width = i - m_recCharacter[num].x + 1;
// 				num++;
// 			}
// 			else
// 			{
//
// 			}
// 			if(!addFlag && 2 == num)		//�����м�׵��Ӱ�죨���ҵ��ڶ���֮����У�
// 			{
// 				i += (m_bestRec.width / 24);//���׵���м�հ׵����򲻿���
// 				--i;
// 				addFlag = true;				//��������ֻ���Լ���һ��
// 				leftBoundry = false;		//������㿪ʼ������ߵı߽�
// 			}
// 		}
// 		if(7 == num)
// 			break;
// 	}

////////////////////����Ѱ�Ҿֲ���С��ķָ�-------------------------------

    delete []vCount;
    if(num != 7) {
        MessageBox("�ָ��ַ�ʧ��");
        return;
    }
    CvPoint startPt,endPt;
    for(i = 0; i < 7; ++i) {
        startPt.x = m_recCharacter[i].x;
        startPt.y = m_recCharacter[i].y;
        endPt.x = startPt.x + m_recCharacter[i].width - 1;
        endPt.y = startPt.y + m_recCharacter[i].height - 1;
        cvRectangle(m_pPlateArea,startPt,endPt,CV_RGB(255,0,0));
    }

////////////////////�ָ��ַ����㷨-----------------------------------------
    SaveCharacter();			//�ָ��ַ�����
    m_pCurrentImg = m_pPlateArea;
    Invalidate(true);
    return;
}

void CPlateIdenfifyView::SaveCharacter()
{
    //���������Ŀ¼
    CFileFind finder;
    const unsigned int MAX_LENGTH = 200;
    CString strDirc;
    GetCurrentDirectory(MAX_LENGTH,strDirc.GetBuffer(MAX_LENGTH));
    strDirc.ReleaseBuffer();

    strDirc += "\\Pic";
    if(!finder.FindFile(strDirc))
        CreateDirectory(strDirc,NULL);
    finder.Close();
    strDirc += "\\SplitePics";
    if(!finder.FindFile(strDirc))
        CreateDirectory(strDirc,NULL);
    finder.Close();

    //�������ļ����Ϻʹ��ļ�һ����������Ϊǰ׺
    CString fileName = m_strFileName;
    fileName = m_strFileName.Right(m_strFileName.GetLength() - m_strFileName.ReverseFind('\\') - 1);
    fileName = fileName.Left(fileName.Find('.'));
    fileName += "_character";
    strDirc = strDirc + "\\" + fileName;
    if(!finder.FindFile(strDirc))
        CreateDirectory(strDirc,NULL);
    finder.Close();
    strDirc += "\\" + fileName;

    for(int i = 0; i < 7; ++i) {
        char id = i + '0';
        CString strTemp = strDirc + '_' + id + ".bmp";
        IplImage *pSrc = cvCreateImage(cvSize(m_recCharacter[i].width, m_recCharacter[i].height), 8, 1);

        m_pCurrentImg = m_pTwoValueImg;
        cvSetImageROI(m_pCurrentImg,m_recCharacter[i]);
        cvCopyImage(m_pCurrentImg, pSrc);
        cvResetImageROI(m_pCurrentImg);

        //���ַ�����
        TightCharacter(pSrc);
        ReverseGray(pSrc);

        //��һ����С
        IplImage *sizeDst = cvCreateImage(cvSize(32,64),8,1);
        cvResize(pSrc,sizeDst,CV_INTER_NN);
        cvReleaseImage(&pSrc);
        pSrc = sizeDst;
        cvSaveImage(strTemp,pSrc);
        m_characterName.push_back(strTemp);

        //���ַ�ϸ��
        strTemp = strDirc + '_' + "thin" + '_' + id + ".bmp";
        IplImage *pThin = cvCreateImage(cvGetSize(pSrc), 8, 1);
        Thining(pSrc,pThin);
        cvReleaseImage(&pSrc);
        pSrc = pThin;
        cvSaveImage(strTemp,pSrc);

        cvReleaseImage(&pSrc);
    }
}

void CPlateIdenfifyView::Thining(IplImage *src,IplImage *dst)
{
    ASSERT(src->height == dst->height && src->width == dst->width);

    int width = src->width;
    int height = src->height;

    //����
    BOOL bModified;

    //ѭ������
    int i, j, n, m;

    //�ĸ�����
    BOOL bCondition1, bCondition2, bCondition3, bCondition4;

    //������
    unsigned char nCount;

    //����ֵ
    unsigned char pixel;
    CvScalar s;

    //5��5������������ֵ
    unsigned char neighbour[5][5];

    // pTemp�����м����ݣ���srcBackup���������㣬���ı�src��ֵ
    IplImage *pTemp = cvCreateImage(cvSize(width,height),8,1);
    IplImage *srcBackup = cvCloneImage(src);

    //�Ƿ�����޸�����
    bModified=TRUE;

    while(bModified) {
        bModified = FALSE;
        memset(pTemp->imageData, (BYTE)255, width * height);

        //����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������
        for(i = 2; i < height - 2; i++) {
            for(j = 2; j < width - 2; j++) {
                bCondition1 = bCondition2 = bCondition3 = bCondition4 = FALSE;

                //ȡ�õ�ǰָ�봦������ֵ
                s = cvGet2D(srcBackup, i, j);
                pixel = s.val[0];

                //Ŀ��ͼ���к���0��255��������Ҷ�ֵ
                ASSERT(pixel == 255 || pixel == 0);

                //���Դͼ���е�ǰ��Ϊ��ɫ��������
                if(pixel == 255)
                    continue;

                //��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
                for (m = 0; m < 5; m++ ) {
                    for (n = 0; n < 5; n++) {
                        neighbour[m][n] = (VALUE(srcBackup, j+n-2, i+m-2) == 255) ? 0 : 1;
                    }
                }

                //����ж�������
                //�ж�2<=NZ(P1)<=6
                nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
                          + neighbour[2][1] + neighbour[2][3] + \
                          + neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
                if ( nCount >= 2 && nCount <= 6)
                    bCondition1 = TRUE;

                //�ж�Z0(P1)=1
                nCount = 0;
                if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
                    nCount++;
                if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
                    nCount++;
                if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
                    nCount++;
                if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
                    nCount++;
                if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
                    nCount++;
                if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
                    nCount++;
                if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
                    nCount++;
                if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
                    nCount++;
                if (nCount == 1)
                    bCondition2 = TRUE;

                //�ж�P2*P4*P8=0 or Z0(p2)!=1
                if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
                    bCondition3 = TRUE;
                else {
                    nCount = 0;
                    if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
                        nCount++;
                    if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
                        nCount++;
                    if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
                        nCount++;
                    if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
                        nCount++;
                    if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
                        nCount++;
                    if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
                        nCount++;
                    if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
                        nCount++;
                    if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
                        nCount++;
                    if (nCount != 1)
                        bCondition3 = TRUE;
                }

                //�ж�P2*P4*P6=0 or Z0(p4)!=1
                if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
                    bCondition4 = TRUE;
                else {
                    nCount = 0;
                    if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
                        nCount++;
                    if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
                        nCount++;
                    if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
                        nCount++;
                    if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
                        nCount++;
                    if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
                        nCount++;
                    if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
                        nCount++;
                    if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
                        nCount++;
                    if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
                        nCount++;
                    if (nCount != 1)
                        bCondition4 = TRUE;
                }
                if(bCondition1 && bCondition2 && bCondition3 && bCondition4) {
                    s.val[0] = 255;
                    cvSet2D(pTemp,i,j,s);
                    bModified = TRUE;
                } else {
                    s.val[0] = 0;
                    cvSet2D(pTemp,i,j,s);
                }
            }// for j
        }// for i
        cvCopyImage(pTemp,srcBackup);
    }// while
    cvCopyImage(srcBackup,dst);

    cvReleaseImage(&pTemp);
    cvReleaseImage(&srcBackup);
}

void CPlateIdenfifyView::TightCharacter( IplImage *&img )
{
    int i,j;
    CvScalar s;
    bool find = false;
    CvRect tightArea;

    for(i = 0; i <img->height; ++i) {
        for(j = 0; j < img->width; ++j) {
            s = cvGet2D(img,i,j);
            if(255 == s.val[0]) {
                find = true;
                break;
            }
        }
        if(find)
            break;
    }
    tightArea.y = i;
    find = false;

    for(i = img->height - 1; i >=0; --i) {
        for(j = 0; j < img->width; ++j) {
            s = cvGet2D(img,i,j);
            if(255 == s.val[0]) {
                find = true;
                break;
            }
        }
        if(find)
            break;
    }
    tightArea.height = i - tightArea.y + 1;
    find = false;

    for(j = 0; j < img->width; ++j) {
        for(i = tightArea.y; i < tightArea.y + tightArea.height; ++i) {
            s = cvGet2D(img,i,j);
            if(255 == s.val[0]) {
                find = true;
                break;
            }
        }
        if(find)
            break;
    }
    tightArea.x = j;
    find = false;

    for(j = img->width - 1; j >= 0; --j) {
        for(i = tightArea.y; i < tightArea.y + tightArea.height; ++i) {
            s = cvGet2D(img,i,j);
            if(255 == s.val[0]) {
                find = true;
                break;
            }
        }
        if(find)
            break;
    }
    tightArea.width = j - tightArea.x + 1;

    IplImage *tightImage = cvCreateImage(cvSize(tightArea.width,tightArea.height),8,1);
    cvSetImageROI(img,tightArea);
    cvCopyImage(img,tightImage);

    cvReleaseImage(&img);
    img = tightImage;
    return;
}

//����ʶ��һ���ַ�
bool CPlateIdenfifyView::NetCalculate(CNeuralNet &net, char *fileName, int &nType, double &dConfidence)
{
    DVECTOR eachInput;
    CPicData testPic;

    if(!testPic.Initialize(fileName)) {
        MessageBox("��ʶ���ַ�ʧ��");
        return false;
    }
    testPic.GetResampleData(eachInput);
    if(net.Recognize(eachInput, nType, dConfidence))
        return true;
    return false;
}

//�����ַ���ʶ�����
void CPlateIdenfifyView::OnNetIdentify()
{
    if(m_pOpenImg == NULL) {
        MessageBox("û�д�ͼƬ");
        return;
    }

    if(m_pPlateArea == NULL) {
        MessageBox("���ȷָ��ַ�");
        return;
    }

    CNeuralNet netNum, netLetter, netLetNum, netChinese;
    char dircBuff[MAX_BUFF_LENGTH];
    GetCurrentDirectory(MAX_BUFF_LENGTH, dircBuff);

    char targetBuff[MAX_BUFF_LENGTH];
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetNumber.net");
    if(!netNum.LoadFromFile(targetBuff)) {
        MessageBox("��ѵ����������ʧ��");
        return;
    }

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetLetter.net");
    if(!netLetter.LoadFromFile(targetBuff)) {
        MessageBox("��ѵ����������ʧ��");
        return;
    }

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetLetNum.net");
    if(!netLetNum.LoadFromFile(targetBuff)) {
        MessageBox("��ѵ����������ʧ��");
        return;
    }

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetChinese.net");
    if(!netChinese.LoadFromFile(targetBuff)) {
        MessageBox("��ѵ����������ʧ��");
        return;
    }

    int nType = 0;
    double dConfidence = 0.0;
    CString result;
    DVECTOR eachInput;

    //�ȶ�ʶ������ĺ��ִ����
    char chineseName[33][4] = {{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},
        {"ԥ"},{"³"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"ǭ"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"},{"��"}
    };

    bool reviseFlag = false;
    CString reviseLoc = "";
    //��ʼ�ַ���ʶ��
    for(int i = 0; i < 7; ++i) {
        CString temp = m_characterName[i];
        CPicData testPic;

        if(!testPic.Initialize(temp)) {
            MessageBox("���ַ�����ʧ��");
            return;
        }
        eachInput.clear();
        testPic.GetResampleData(eachInput);

        switch (i) {
        case 0: {
            nType = 0;
            dConfidence = 0.0;
            netChinese.Recognize(eachInput,nType,dConfidence);
            char name[3];
            strcpy(name, chineseName[nType]);
            temp.Format("��%d���ַ�Ϊ: %s  ���Ŷ�Ϊ: %%%d \n",i+1, name, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "1 ";
            }
            break;
        }
        case 1: {
            nType = 0;
            dConfidence = 0.0;
            netLetter.Recognize(eachInput, nType, dConfidence);
            char c;
            if(nType < 14)
                c = 'A' + nType;
            else
                c = 'A' + nType + 1;
            temp.Format("��%d���ַ�Ϊ: %c   ���Ŷ�Ϊ: %%%d \n",i+1, c, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "2 ";
            }
            break;
        }
        case 2: {
            nType = 0;
            dConfidence = 0.0;
            netLetNum.Recognize(eachInput, nType, dConfidence);
            char c;
            if(nType < 10)
                c = '0' + nType;
            else if(nType < 24)
                c = 'A' + nType - 10;
            else
                c = 'A' + nType - 9;
            temp.Format("��%d���ַ�Ϊ: %c   ���Ŷ�Ϊ: %%%d \n",i+1, c, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "3 ";
            }
            break;
        }
        case 3: {
            nType = 0;
            dConfidence = 0.0;
            netLetNum.Recognize(eachInput, nType, dConfidence);
            char c;
            if(nType < 10)
                c = '0' + nType;
            else if(nType < 24)
                c = 'A' + nType - 10;
            else
                c = 'A' + nType - 9;
            temp.Format("��%d���ַ�Ϊ: %c   ���Ŷ�Ϊ: %%%d \n",i+1, c, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.5) {
                reviseFlag = true;
                reviseLoc += "4 ";
            }
            break;
        }
        case 4: {
            nType = 0;
            dConfidence = 0.0;
            netLetNum.Recognize(eachInput, nType, dConfidence);
            char c;
            if(nType < 10)
                c = '0' + nType;
            else if(nType < 24)
                c = 'A' + nType - 10;
            else
                c = 'A' + nType - 9;
            temp.Format("��%d���ַ�Ϊ: %c   ���Ŷ�Ϊ: %%%d \n",i+1, c, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "5 ";
            }
            break;
        }
        case 5: {
            nType = 0;
            dConfidence = 0.0;
            netNum.Recognize(eachInput, nType, dConfidence);
            temp.Format("��%d���ַ�Ϊ: %d   ���Ŷ�Ϊ: %%%d \n",i+1, nType, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "6 ";
            }
            break;
        }
        case 6: {
            nType = 0;
            dConfidence = 0.0;
            netNum.Recognize(eachInput, nType, dConfidence);
            temp.Format("��%d���ַ�Ϊ: %d   ���Ŷ�Ϊ: %%%d \n",i+1, nType, (int)(dConfidence * 100));
            result += temp;
            if(dConfidence < 0.4) {
                reviseFlag = true;
                reviseLoc += "7 ";
            }
            break;
        }
        default:
            break;
        }
    }
    //�Ƿ���Ҫ�˹����޸�
    if(reviseFlag) {
        CString temp = "���ŶȽϵͣ���" + reviseLoc + "���ַ���Ҫ�˹�����";
        MessageBox(temp);
    }
    MessageBox(result);
}

//����ѵ������
//�ڵ�һ��ѵ��ʶ���ʱ��Ҫ���ã����µ����ݷ��뵽TestData�ļ��е�ʱ��ҲҪѵ��
void CPlateIdenfifyView::OnNetTrain()
{
    DWORD dwEsplise = ::GetTickCount();
    if(!TrainNumImpl()) {
        MessageBox("ѵ������ʧ��");
        return;
    }
    if(!TrainLetImpl()) {
        MessageBox("ѵ����ĸʧ��");
        return;
    }
    if(!TrainNumLetImpl()) {
        MessageBox("ѵ��������ĸʧ��");
        return;
    }
    if(!TrainChineseImpl()) {
        MessageBox("ѵ������ʧ��");
        return;
    }

    dwEsplise = ::GetTickCount() - dwEsplise;
    CString outInfo;
    outInfo.Format("ѵ������ɹ�! \n һ����ʱ:%d��", dwEsplise /1000);
    MessageBox(outInfo);
}

void CPlateIdenfifyView::OnTrainNum()
{
    TrainNumImpl();
}

void CPlateIdenfifyView::OnTrainLet()
{
    TrainLetImpl();
}

void CPlateIdenfifyView::OnTrainNumlet()
{
    TrainNumLetImpl();
}

void CPlateIdenfifyView::OnTrainChinese()
{
    TrainChineseImpl();
}

bool CPlateIdenfifyView::TrainNumImpl()
{
    //------------------First Get Train Data-------------
    char dircBuff[MAX_BUFF_LENGTH] = "";
    GetCurrentDirectory(MAX_BUFF_LENGTH, dircBuff);

    char testNumberBuff[] = "\\NetData\\TrainData\\Numbers";
    char targetBuff[MAX_BUFF_LENGTH];
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, testNumberBuff);

    std::vector<DVECTOR> input, output;

    CTrainData dataNum(targetBuff, 10);
    if(!dataNum.GetTrainData(input, output)) {
        MessageBox("��ȡѵ������ʧ��");
        return false;
    }

    //------------  Initialize NeuralNet------------------
    CNeuralNet netNum;
    int dataSize = input[0].size();
    netNum.Initialize(dataSize, 10, 4);
    DWORD dwEsplise = ::GetTickCount();
    if (!netNum.Train(input, output)) {
        MessageBox("����ѵ������ʧ��");
        return false;
    };
    //------------- Save Net-------------------------------
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetNumber.net");
    netNum.SaveToFile(targetBuff);

    dwEsplise = ::GetTickCount() - dwEsplise;
    CString outInfo;
    outInfo.Format("ѵ����������ɹ��� \nһ����ʱ:%d��", dwEsplise / 1000);
    MessageBox(outInfo);
    return true;
}

bool CPlateIdenfifyView::TrainLetImpl()
{
    //--------------------For Letter Part------------------------
    char dircBuff[MAX_BUFF_LENGTH] = "";
    GetCurrentDirectory(MAX_BUFF_LENGTH, dircBuff);

    char testLetterBuff[] = "\\NetData\\TrainData\\Letter";
    char targetBuff[MAX_BUFF_LENGTH];
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, testLetterBuff);

    std::vector<DVECTOR> input, output;
    CTrainData dataLetter(targetBuff, 25);			//û����ĸ'O'
    if(!dataLetter.GetTrainData(input, output)) {
        MessageBox("��ȡѵ������ʧ��");
        return false;
    }

    CNeuralNet netLetter;
    int dataSize = input[0].size();
    netLetter.Initialize(dataSize, 25, 4);
    DWORD dwEsplise = ::GetTickCount();
    if (!netLetter.Train(input, output)) {
        MessageBox("����ѵ������ʧ��");
        return false;
    };

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetLetter.net");
    netLetter.SaveToFile(targetBuff);

    dwEsplise = ::GetTickCount() - dwEsplise;
    CString outInfo;
    outInfo.Format("ѵ����ĸ����ɹ��� \nһ����ʱ:%d��", dwEsplise / 1000);
    MessageBox(outInfo);
    return true;
}

bool CPlateIdenfifyView::TrainNumLetImpl()
{
    //----------------For Number And Letter Part-------------------
    char dircBuff[MAX_BUFF_LENGTH] = "";
    GetCurrentDirectory(MAX_BUFF_LENGTH, dircBuff);

    char testLetterNumberBuff[] = "\\NetData\\TrainData\\Letter and Numbers";
    char targetBuff[MAX_BUFF_LENGTH];
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, testLetterNumberBuff);

    std::vector<DVECTOR> input, output;
    CTrainData dataLetNum(targetBuff, 35);
    if(!dataLetNum.GetTrainData(input, output)) {
        MessageBox("��ȡѵ������ʧ��");
        return false;
    }

    CNeuralNet netLetNum;
    int dataSize = input[0].size();
    netLetNum.Initialize(dataSize, 35, 4);
    DWORD dwEsplise = ::GetTickCount();
    if (!netLetNum.Train(input, output)) {
        MessageBox("����ѵ������ʧ��");
        return false;
    };

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetLetNum.net");
    netLetNum.SaveToFile(targetBuff);

    dwEsplise = ::GetTickCount() - dwEsplise;
    CString outInfo;
    outInfo.Format("ѵ��������ĸ����ɹ��� \nһ����ʱ:%d��", dwEsplise / 1000);
    MessageBox(outInfo);
    return true;
}

bool CPlateIdenfifyView::TrainChineseImpl()
{
    //---------------------------For Chinese----------------
    char dircBuff[MAX_BUFF_LENGTH] = "";
    GetCurrentDirectory(MAX_BUFF_LENGTH, dircBuff);

    char testChinese[] = "\\NetData\\TrainData\\Chinese";
    char targetBuff[MAX_BUFF_LENGTH];
    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, testChinese);

    CTrainData dataChinese(targetBuff, 33);
    std::vector<DVECTOR> input, output;
    if(!dataChinese.GetTrainData(input, output)) {
        MessageBox("��ȡѵ������ʧ��");
        return false;
    }

    CNeuralNet netChinese;
    int dataSize = input[0].size();
    netChinese.Initialize(dataSize, 33, 4);
    DWORD dwEsplise = ::GetTickCount();
    if (!netChinese.Train(input, output)) {
        MessageBox("����ѵ������ʧ��");
        return false;
    };

    strcpy(targetBuff, dircBuff);
    strcat(targetBuff, "\\NetData\\MyNetChinese.net");
    netChinese.SaveToFile(targetBuff);

    dwEsplise = ::GetTickCount() - dwEsplise;
    CString outInfo;
    outInfo.Format("ѵ����������ɹ�!  \nһ����ʱ:%d��", dwEsplise / 1000);
    MessageBox(outInfo);
    return true;
}