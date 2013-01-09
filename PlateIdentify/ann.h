#ifndef _ANN__H_
#define _ANN__H_

#define NEED_MOMENTUM				//�Ƿ���Ҫ����

#include <afx.h>
#include <cmath>
#include <vector>
#include <fstream>
typedef double WEIGHT_TYPE;
typedef std::vector<double> DVECTOR;

const DWORD MY_NEURALNET_VERSION = 12345;		//�����Լ��İ汾ͷ��Ϣ
const double BIAS = 1.0;		//ƫ����ϵ��
#ifdef NEED_MOMENTUM
const double MOMENTUM = 0.6;	//����ϵ�����������ӳ�����(#define NEED_MOMENTUM)��ʱ��������
#endif

//��Ԫ��Ԫ�ṹ
class CNeuron
{
public:
    int m_nInputNum;			//�������ݵĸ���
    WEIGHT_TYPE* m_pWeights;	//Ȩֵ����w
#ifdef NEED_MOMENTUM
    WEIGHT_TYPE* m_pLastUpdates; //������һ�εĸ�������������ʾ����
#endif
    double m_dResult;			//���ļ���ֵ
    double m_dError;			//��������ֵ

public:
    CNeuron() : m_pLastUpdates(NULL), m_pWeights(NULL) {
    }	//һ��Ҫ��Ĭ�Ϲ����ʹ��Initialize
    CNeuron(int nInputNum) {
        Initialize(nInputNum);
    }
    void Initialize(int nInputNum) {
        m_nInputNum = nInputNum + 1;		//��1��Ϊƫ����Ҳ����һ���ռ�
        m_pWeights = new WEIGHT_TYPE[m_nInputNum];
        m_pLastUpdates = new WEIGHT_TYPE[m_nInputNum];
        m_dResult = m_dError = 0.0;
    }
    ~CNeuron() {
        if (m_pWeights != NULL) {
            delete[]m_pWeights;
            m_pWeights = NULL;
        }
        if (m_pLastUpdates != NULL) {
            delete[]m_pLastUpdates;
            m_pLastUpdates = NULL;
        }
    }
};

//��Ԫ��Ľṹ
class CNeuronLayer
{
public:
    int m_nNeuronNum;			//��Ԫ����Ŀ
    CNeuron* m_pNeurons;		//������Ԫ�Ķ�̬����

public:
    CNeuronLayer() : m_pNeurons(NULL) {
        //һ��Ҫ��Ĭ�Ϲ����ʹ��Initialize
    }
    CNeuronLayer(int nNeuronNum, int nInputNum) {
        //nInputNumΪÿ����Ԫ����ĸ���
        Initialize(nNeuronNum, nInputNum);
    }
    void Initialize(int nNeuronNum, int nInputNum) {
        m_nNeuronNum = nNeuronNum;
        m_pNeurons = new CNeuron[m_nNeuronNum];
        for (int i = 0; i < m_nNeuronNum; ++i) {
            m_pNeurons[i].Initialize(nInputNum);
        }
    }
    ~CNeuronLayer() {
        if (m_pNeurons != NULL)
            delete[]m_pNeurons;
    }
};

struct NEURALNET_HEADER {				//�����ļ�����Ϣͷ
    DWORD dwVersion;				//�汾��Ϣ

    int m_nInputNum;			//������Ϣ�ĸ���
    int m_nOutputNum;			//�����������
    int m_nHNeuronNum;			//���ز����Ԫ����

    //ѵ��������Ϣ
    double m_dLearningRate;		//ѧϰ��
    unsigned int m_nMaxEpoch;	//������ʱ����
    double m_dMinErr;			//��С�����

    //�м���Ϣ����
    double m_dErrSum;			//һ��ʱ�����ۻ����
    double m_dMeanErr;			//һ��ʱ����ƽ����ÿ��ѵ������������
    unsigned int m_nEposhNum;	//���������Ѿ�Epoch�Ĵ���
};

//��������
class CNeuralNet
{
    //���ݳ�Ա
public:
    //����һ�����ز��һ�������
    CNeuronLayer* m_pHiddenL;
    CNeuronLayer* m_pOutputL;

    //����������Ϣ
    int m_nInputNum;			//������Ϣ�ĸ���
    int m_nOutputNum;			//�����������
    int m_nHNeuronNum;			//���ز����Ԫ����

    //ѵ��������Ϣ
    double m_dLearningRate;		//ѧϰ��
    unsigned int m_nMaxEpoch;	//������ʱ����
    double m_dMinErr;			//��С�����

    //�м���Ϣ����
    double m_dErrSum;			//һ��ʱ�����ۻ����
    double m_dMeanErr;			//һ��ʱ����ƽ����ÿ��ѵ������������
    unsigned int m_nEposhNum;	//���������Ѿ�Epoch�Ĵ���
    bool m_bStop;				//��������ѵ��ѵ��;ֹͣ

    //�������������
public:
    CNeuralNet() {
        //һ��Ҫ��Ĭ�Ϲ����ʹ��Initialize
        m_pHiddenL = NULL;
        m_pOutputL = NULL;
    }
    CNeuralNet(int nInputNum, int nOutputNum, int nHNeuronNum) {
        m_pHiddenL = NULL;
        m_pOutputL = NULL;
        Initialize(nInputNum, nOutputNum, nHNeuronNum);
    }
    CNeuralNet(int nInputNum, int nOutputNum, int nHNeuronNum,
               double dLearningRate, unsigned int unMaxEpoch, double dMinErr) {
        m_pHiddenL = NULL;
        m_pOutputL = NULL;
        Initialize(nInputNum, nOutputNum, nHNeuronNum, dLearningRate,
                   unMaxEpoch, dMinErr);
    }
    void Initialize(int nInputNum, int nOutputNum, int nHNeuronNum, bool bCreate = true) {
        //ʹ��Ĭ�ϵ�����ֵ
        m_dErrSum = m_dMeanErr = 0.0;
        m_nEposhNum = 0;
        m_nInputNum = nInputNum;
        m_nOutputNum = nOutputNum;
        m_nHNeuronNum = nHNeuronNum;
        m_dLearningRate = 0.1;
        m_nMaxEpoch = 8000;
        m_dMinErr = 0.001;
        m_bStop = false;

        if (m_pOutputL != NULL)						//Ϊ�����µ�ֵ����׼��
            delete m_pOutputL;
        if (m_pHiddenL != NULL)
            delete m_pHiddenL;
        m_pHiddenL = new CNeuronLayer(m_nHNeuronNum, m_nInputNum);
        m_pOutputL = new CNeuronLayer(m_nOutputNum, m_nHNeuronNum);

        if(bCreate)
            InitializeNetwork();
#ifdef _CONSOLETEST
        int i,j;
        for (i = 0; i < m_nHNeuronNum; ++i) {
            for (j = 0; j < m_nInputNum + 1; ++j) {
                cout<<m_pHiddenL->m_pNeurons[i].m_pWeights[j]<<"  ";
            }
            cout<<endl<<"*****************************************"<<endl;
        }
        cout<<"-----------------------------------------------"<<endl;
        for (i = 0; i < m_nOutputNum; ++i) {
            for (j = 0; j < m_nHNeuronNum + 1; ++j) {
                cout<<m_pOutputL->m_pNeurons[i].m_pWeights[j]<<"  ";
            }
            cout<<endl;
        }
        cout<<"-------------------------------------------------"<<endl;
#endif
    }
    void Initialize(int nInputNum, int nOutputNum, int nHNeuronNum,
                    double dLearningRate, unsigned int unMaxEpoch, double dMinErr, bool bCreate = true) {
        //ʹ�õ��ƶ�������ֵ
        m_dErrSum = m_dMeanErr = 0.0;
        m_nEposhNum = 0;
        m_nInputNum = nInputNum;
        m_nOutputNum = nOutputNum;
        m_nHNeuronNum = nHNeuronNum;
        m_dLearningRate = dLearningRate;
        m_nMaxEpoch = unMaxEpoch;
        m_dMinErr = dMinErr;
        m_bStop = false;

        if (m_pOutputL != NULL)						//Ϊ�����µ�ֵ����׼��
            delete m_pOutputL;
        if (m_pHiddenL != NULL)
            delete m_pHiddenL;
        m_pHiddenL = new CNeuronLayer(m_nHNeuronNum, m_nInputNum);
        m_pOutputL = new CNeuronLayer(m_nOutputNum, m_nHNeuronNum);

        if(bCreate)
            InitializeNetwork();
    }
    virtual ~CNeuralNet() {
        if (m_pOutputL != NULL) {
            delete m_pOutputL;
            m_pOutputL = NULL;
        }
        if (m_pHiddenL != NULL) {
            delete m_pHiddenL;
            m_pHiddenL = NULL;
        }
    }

    //get and set ����
public:
    int GetBStop() const {
        return m_bStop;
    }
    void SetBStop(bool stop) {
        m_bStop = stop;
    }
    unsigned int GetMaxEpoch() const {
        return m_nMaxEpoch;
    }
    void SetMaxEpoch(unsigned int nMaxEpoch) {
        m_nMaxEpoch = nMaxEpoch;
    }
    //�ڲ��ķ���
protected:
    double Sigmoid(double netinput) {			//sigmoid��������
        return (1 / (1 + exp(-netinput)));
    }
    void InitializeNetwork();					//Ϊ��������ó�ʼ��ֵ�����ʼ��Ȩֵ�����ز�ĳ���ֵ)
    double WeightRand();						//������ʼ��-0.05-0.05�������Ȩֵ
    //����Ľӿں���
public:
    bool CalculateOutput(const DVECTOR &eachInput, DVECTOR &eachOutput);
    void UpdateWeight(const DVECTOR& eachInput, const DVECTOR &eachOutput);	//�������ݶ��½�������Ȩֵ
    bool TrainingEpoch(const std::vector<DVECTOR>& input, const std::vector<DVECTOR> &output);	//ѵ��һ��Epoch
    bool Train(const std::vector<DVECTOR>& input, const std::vector<DVECTOR> &output);		//����ѵ���ĺ���
    bool SaveToFile(const char *lpszFileName, bool bCreate = true);			//�����������浽�ļ���
    bool LoadFromFile(const char *lpszFileName);							//�������ȡ�ᵽ������
    BOOL Recognize(const DVECTOR &eachInput, int &nType, double &dConfidence);	//ʶ�����
};
#endif