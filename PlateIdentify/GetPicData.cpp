#include "StdAfx.h"
#include "GetPicData.h"
bool CPicData::GetResampleData(DVECTOR& eachInput)
{
    if (m_pImg == NULL)
        return false;
    int i, j, m, n;
    int sum;
    for (i = 0; i < m_pImg->GetHeight(); i += RESAMPLE_LENTH) {
        for (j = 0; j < m_pImg->GetWidthPixel(); j += RESAMPLE_LENTH) {
            sum = 0;
            for (m = i; m < i + RESAMPLE_LENTH; ++m)
                for (n = j; n < j + RESAMPLE_LENTH; ++n)
                    sum += m_pImg->GetGray(n, m);
            eachInput.push_back(sum / (RESAMPLE_LENTH * RESAMPLE_LENTH) / 255.0);
        }
    }
    return true;
}

bool CTrainData::GetTrainData(std::vector<DVECTOR>& input,
                              std::vector<DVECTOR>& output)
{
    if (m_lpszDir == NULL)
        return false;

    int type = -1;
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    CString strDir = m_lpszDir;
    hFind = ::FindFirstFile(CString(strDir + "\\*.bmp"), &findData); 	//ֻ֧��λͼ����
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            if(findData.cFileName[0] == '.')
                continue;
            if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                CPicData p;
                CString strPicName = strDir + '\\' + findData.cFileName;
                p.Initialize(strPicName);				//��ͼ�����Ϣ���������
                DVECTOR inTemp;
                p.GetResampleData(inTemp);				//ȡ������ֵ������
                input.push_back(inTemp);

                //��ͼƬ��ǰ��λ������ͼ�����Ϣ
                type = 10 * (findData.cFileName[0] - '0') + (findData.cFileName[1] - '0');
                ASSERT(type >= 0 && type <= 50);
                if(type < 0 || type > 50)
                    return false;

                DVECTOR v(m_nTypeNum,0.1);
                v[type] = 0.9;					//ֻ������������ֵ����Ϊ0.9
                output.push_back(v);
            }
        } while (::FindNextFile(hFind, &findData));
    }
    return true;
}