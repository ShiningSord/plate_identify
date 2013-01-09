#ifndef __CIMG_H__
#define __CIMG_H__
#include <afxwin.h>
#define WIDTHBYTES(x) ((((x)+31)>>5)<<2)	//make 4 as the factor of the width bytes
class CImg
{
public:
    //data member
    BITMAPINFOHEADER* m_pBMIH;
    LPBYTE* m_lpData;
    LPBYTE m_lpvColorTable;
    UINT m_nColorTableEntries;
    //member function
public:
    CImg():m_lpData(NULL),m_lpvColorTable(NULL),m_nColorTableEntries(0),
        m_pBMIH(NULL) {
    };
    CImg(const CImg& gray);
    void Construtor(const CImg& gray);
    ~CImg();

public:
    CImg& operator =(const CImg& gray);
    CImg& operator +=(const CImg& gray);
    CImg operator +(const CImg& gray) const;
    CImg& operator -=(const CImg& gray);
    CImg operator -(const CImg& gray) const;

public:

    BOOL IsValidate() const;

    BOOL AttachFromFile(LPCTSTR lpFileName);
    BOOL AttachFromFile(CFile& file);

    BOOL SaveToFile(LPCTSTR lpFileName) const;
    BOOL SaveToFile(CFile& file) const;

    UINT GetHeight() const;
    UINT GetWidthPixel() const;
    UINT GetWidthBytes() const;
    UINT GetGray(int x, int y) const;			//��ȡ��������λ�õĻҶ�ֵ
    COLORREF GetPixel(int x, int y) const;			//��ȡ��������λ�õ���ɫ
    void SetPixel(int x, int y, COLORREF color);	//����ָ��λ�õ����ص���ɫ

    BOOL IsBinaryImg() const;					//�ж��Ƿ��Ƕ�ֵ��ͼ��
    BOOL IsIndexImg() const;					//�ж��Ƿ�����������ͼ��

public:
    BOOL OnDraw(CDC* pDC) const;
private:
    void CleanUp();

private:
};
inline BOOL CImg::IsValidate() const
{
    return (m_pBMIH) ? TRUE : FALSE;
}
inline UINT CImg::GetHeight() const
{
    return m_pBMIH->biHeight;
}
inline UINT CImg::GetWidthPixel() const
{
    return m_pBMIH->biWidth;
}
inline UINT CImg::GetWidthBytes() const
{
    return WIDTHBYTES(m_pBMIH->biWidth * m_pBMIH->biBitCount);
}
inline COLORREF CImg::GetPixel(int x, int y) const
{
    switch (m_pBMIH->biBitCount) {
    case 1:
        //��ɫͼ
    {
        int index = ((1 << (7 - x % 8)) & m_lpData[m_pBMIH->biHeight - 1 - y][x / 8]);
        if (((RGBQUAD *) m_lpvColorTable)->rgbBlue != 0)			//����Ϊ0�ĵط�ҪΪ��ɫ������0��0��0��
            index = !index;
        if (index)
            return RGB(255, 255, 255);
        else
            return RGB(0, 0, 0);
    }
    case 8:
        //�Ҷ�ͼ
    {
        int col = m_lpData[m_pBMIH->biHeight - 1 - y][x];		//ע���ļ������ǵ��Ŵ�ŵ�
        return RGB(col, col, col);
    }
    case 24:
        //���ͼ
    {
        BYTE bValue = m_lpData[m_pBMIH->biHeight - 1 - y][3 * x];		//ע����С�˵Ĵ洢��ģʽ
        BYTE gValue = m_lpData[m_pBMIH->biHeight - 1 - y][3 * x + 1];
        BYTE rValue = m_lpData[m_pBMIH->biHeight - 1 - y][3 * x + 2];
        return RGB(rValue, gValue, bValue);
    }
    default: {
        throw CString("Not Support Format\n");
    }
    }
}

inline	UINT CImg::GetGray(int x, int y) const
{
    COLORREF ref = GetPixel(x, y);
    UINT rValue, gValue, bValue;
    rValue = GetRValue(ref);
    gValue = GetGValue(ref);
    bValue = GetBValue(ref);

    if (rValue == gValue && gValue == bValue)
        return rValue;

    double gray = 0.3 * rValue + 0.59 * gValue + 0.11 * bValue;
    return (int) gray;
}

inline BOOL CImg::IsBinaryImg() const
{
    return (m_pBMIH->biBitCount == 1);
}
inline BOOL CImg::IsIndexImg() const
{
    return (m_nColorTableEntries != 0 && m_lpvColorTable != NULL);
}
#endif