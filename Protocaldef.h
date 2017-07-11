#include "AfxTempl.h"

#define WM_DLL_BACK WM_USER + 0x0208

#define  WM_PROTOCOL_BACK     (WM_USER+0x10)//�����ߴ��ڷ�����Ϣ
#define  WM_PROTOCOL_RECEIVE     (WM_USER+0x11)//�յ�����
#define  WM_PROTOCOL_SEND     (WM_USER+0x12)//��������
#define  WM_PROTOCOL_ASKVAL     (WM_USER+0x13)//��Լ�����ȡ����

enum PRO_ASK
{
	Pro_ASKNULL,//��Ч����
	Pro_SendU,
	Pro_SendS,
	//ȫ�ֵ��·�����
	Pro_AskAll,//���ٻ�
	Pro_SendJS,//Уʱ
	Pro_SignRevert,//�źŸ���
	Pro_SendJSAll,//Уʱ
	Pro_SendALLXY,//����
	Pro_SignRevertAll,//�źŸ���
	//һ���ʴ��Լ
	Pro_AskYC,
	Pro_AskYX,
	Pro_AskYM,
	//�ʴ��Լ��չ
	Pro_ResetFCB,
	Pro_ResetCU,
	Pro_Ask1LevelData,//ѯ��һ������
	Pro_Ask2LevelData,//ѯ�ʶ�������
	//��ֵ
	Pro_SendDDZ,//����ֵ
	Pro_SendDMS,//����
	Pro_SendSXX,//������
	Pro_SendDZVAL,//�·���ֵ
	Pro_SendDZFIX,//�̻���ֵ
	Pro_SendDZQH,//�·���ֵ����
	//����
	Pro_SendYK,//ҡ��
	Pro_SendRYB,//��ѹ��
	Pro_AskStatus,//װ��״̬
	Pro_AskSOE,
	Pro_ConfirmSOE,
	//��չ��
	Pro_AskEx1,
	Pro_AskEx2,
	Pro_AskEx3,
	Pro_AskEx4,
	Pro_AskEx5,
	Pro_AskEx6,
	Pro_AskEx7,
	Pro_AskEx8,
	Pro_AskEx9,
	//¼��
	Pro_AskLuboIndex,
	Pro_SendLubo,
	Pro_AskLuboStop,
	Pro_LUBOSTART,
	Pro_LUOQDCMD,
	Pro_LUODATARDY,
	Pro_LUOSTATUSCMD,
	Pro_LOBOSTATUSOK,
	Pro_LUBOMNLRDY,
	Pro_LUBOMNLTRUE,
	Pro_LUBOMNLOK,
	Pro_LUBOEXIT,
	//
	Pro_SendYC,
	Pro_SendYC_A,
	Pro_SendYC_B,
	Pro_SendYC_C,
	Pro_SendYX_D1,
	Pro_SendYM_D2,
	Pro_SendSOE,
	Pro_CDTYKBACK,
	Pro_CDTSOE,
	Pro_ReadGZCurve,
};

enum PRO_RECEIVE
{
	Pro_ReceDevYC,
	Pro_ReceDevYX,
	Pro_ReceDevYM,
	Pro_ReceDevRYB,//��ѹ��
	Pro_ReceDevSOE,
	Pro_ReceDevDzVAL,
	Pro_ReceDevDzNAME,
	Pro_ReceDevDzSXX,
	Pro_ReceDevYKBACK,
	Pro_ReceDevDZQH,
	Pro_ReceDevOnLine,
	Pro_ReceDevStatus,//װ��״̬��������
	Pro_ReceDevOper,//װ�ò���״̬
	Pro_ReceStationStatus,
	Pro_AUTOPACK,
	Pro_MANUPACK,
	Pro_ReceDevGZIndex,//���ϱ�
	Pro_ReceDevGZ_DataReady,//����׼����
	Pro_ReceDevGZ_YXDataReady,//״̬��׼����
	Pro_ReceDevGZ_YXData,//״̬������
	Pro_ReceDevGZ_YXDataEnd,//״̬�����ݽ���
	Pro_ReceDevGZ_YCDataReady,//ģ����׼����
	Pro_ReceDevGZ_YCData,//ģ��������
	Pro_ReceDevGZ_YCDataEnd,//ģ�������ݽ���
	Pro_ReceDevGZ_LUBOEnd,//¼�����̽���

	//����Ϊ��վ�õ�
	Pro_ReceAskYK,//����ң��
	Pro_ReceAskSJ,//��������
	Pro_ReceAskJS,
	Pro_ReceAskFG,
	Pro_ReceGZCurve,
};

struct Circle_Send//ѭ���·��ṹ 
{
	Circle_Send()
	{
		bCircle = FALSE;
		bBroadcast = FALSE;
		nSendFlag = Pro_AskYX;
		nCircleTime = 0;
		nTimerID = 0;
	}
	BOOL bCircle;//�Ƿ�ѭ��
	BOOL bBroadcast;//�Ƿ�㲥
	int nSendFlag;//�·����
	int nCircleTime;//ѭ�����
	UINT nTimerID;
};

struct ykDll
{
	ykDll()
	{
		devno = 0;
		nUnderdevno = 0;
		point = 0;
		status = 0;
		flag = 0;
		num = 0;
		ZeroMemory(val,sizeof(val));
		nYKFlag = Pro_SendYK;
	}
	int devno;
	int nUnderdevno;//����װ��
	int point;
	int status;
	int flag;
	//DZ
	int  num;
	DWORD val[100];
	int nYKFlag;//ң�أ���ֵ���
};

struct cjDll
{
	cjDll()
	{
		type = 0;
		devno = 0;
		nUnderdevno = -1;
		point = 0;
		val = 0;
		fval = 0.f;
		ZeroMemory(cAddr,sizeof(cAddr));
	}
	int type;
	int devno;
	int nUnderdevno;//����װ��
	char cAddr[14];
	int point;
	int val;
	float fval;
};

struct soeDll
{
	soeDll()
	{
		devno = 0;
		nUnderdevno = 0;
		pointNo = 0;
		nIndex = 0;
		nType = 0;
		status = 0;
		ZeroMemory(&time,sizeof(SYSTEMTIME));
		it = 0;
		times = 0;
		ZeroMemory(soeVal,sizeof(soeVal));
		ZeroMemory(cAddr,sizeof(cAddr));
	}
	int devno;
	int nUnderdevno;//����װ��
	char cAddr[14];
	int pointNo;
	int nIndex;//�������
	int nType;//��������
	int status;//ֵ
	long btSOEFlag;////GSM�ò�����1-©��Ԥ����2-©�籨����3-����Ԥ����4-����������5-�豸����
	SYSTEMTIME time;
	long it;
	int times;
	char soeVal[255];//����ֵ
};

struct senddll //�û�����ṹ
{
	senddll()
	{
		btSendFlag = 0xFF;
		ZeroMemory(btData,sizeof(btData));
		nDataLen = 0;
		bNeedAnswer = TRUE;
	}
	BYTE btSendFlag;//���ͱ�ʶ
	BYTE btData[1024];//�������
	int nDataLen; //���ݳ���
	BOOL bNeedAnswer;
};

struct gzIndex
{
	gzIndex()
	{
		nFacNo = 0;
		nDevNo = 0;
		ngzNo = 0;
		GetLocalTime(&gztime);
	}
	int nFacNo;
	int nDevNo;
	int ngzNo;
	SYSTEMTIME gztime;
};

struct sPROTOCOL_DZ //��ֵ���ݽṹ
{
	sPROTOCOL_DZ()
	{
		nZZNo = -1;
		nAreaNo = 1;
		nDzNo = 0;
		fDzVal = 0;
		nDzProp = 0;
		nUnderdevno = -1;
		ZeroMemory(cAddr,sizeof(cAddr));
	}
	int nZZNo; //װ�ú�
	int nUnderdevno;//����װ�ú�
	int nAreaNo;//��ֵ��
	int nDzNo;//��ֵ���
	int nDzProp;//��ֵ����
	float fDzVal;//����
	char cAddr[14];

};

struct sPROTOCOL_103Data
{
	sPROTOCOL_103Data()
	{
		nZZNo = 0;
		btGIN1 = 0;
		btGIN2 = 0;
		btKOD = 0;
		btGDD1 = 0;
		btGDD2 = 0;
		ZeroMemory(btData,sizeof(btData));
	}
	int nZZNo; //װ�ú�
	BYTE btGIN1;//(GIN1)//���
	BYTE btGIN2;//��Ŀ��
	BYTE btKOD;//����
	BYTE btGDD1;//��������
	BYTE btGDD2;//���ݿ��
	BYTE btData[256];//����
};

struct sPROTOCEL_GZCurve
{
	long nZZNo;
	BYTE nPointNo;
	WORD nCurveVal[50];
};

struct sPROTOCOL_DevOperFlag//װ�ò���״̬
{
	sPROTOCOL_DevOperFlag()
	{
		nZZNo = 0;
		nUnderdevno = 0;
		btOperFlag = 0;
		btOperSuccess = 0;
	}
	int nZZNo; //װ�ú�
	int nUnderdevno;//����װ��
	BYTE btOperFlag;//������ʶ
	BYTE btOperSuccess;//�ɹ���ʶ,0-���ɹ�,1-�ɹ�,2-��ʱ
};
struct sPROTOCOL_YK //ң���·�����
{
	sPROTOCOL_YK()
	{
		nZZNo = -1;
		bIsUnderZZ = FALSE;
		nUnderZZNo = -1;
		nYKNo = -1;
		fVal = 0;
		bDLQ = TRUE;
		nYKFlag = Pro_SendYK;
		ZeroMemory(btParam,sizeof(btParam));
		ZeroMemory(cAddr,sizeof(cAddr));
	}

	int nYKFlag;//ң�أ���ֵ���
	int nZZNo; //װ�ú�
	char cAddr[14];
	BOOL bIsUnderZZ;//�Ƕ���װ�ö�ֵ
	int	nUnderZZNo;//����װ�ú�
	int  nYKNo; //���
	float fVal;//���У���������
	//����:����(0-ʧ��,1-�ɹ�)
	BOOL bDLQ;//�Ƿ��·��
	BYTE btParam[10];//��ѡ����
};

struct sInsertCmd //��������ṹ
{
	sInsertCmd()
	{
		nZZNo = 0;
		nInsertCmdFlag = 0x00;
		ZeroMemory(cAddr,sizeof(cAddr));
	}
	BYTE nZZNo;//װ�ú�
	char cAddr[14];

	BYTE nUnderdevno;
	BYTE nInsertCmdFlag;//����֡����
};

#define MAX_LEN 655360
class CMyByteArray : public CByteArray  
{
public:
	int Addx(LPBYTE pNewElement, int len)
	{
		ASSERT(m_nSize+len < MAX_LEN);
		int nIndex=m_nSize;
		SetSize(m_nSize+len);
		memcpy(m_pData+nIndex,pNewElement,len);
		return m_nSize-1;//�µ�������λ��
	}
};

struct sPROTOCOL_FRAME //���ͱ��ĵ�֡�ṹ
{
	sPROTOCOL_FRAME()	{
		nZZNo = 0xFF;
		btSendFlag = 0x00;
		nNum = 0;
		bNeedAnswer = FALSE;
		nOvertime = 0;
		ZeroMemory(btData,sizeof(btData));
	}
	int nZZNo; //װ�ú�
	BYTE btSendFlag;//�·���ʶ
	BOOL bNeedAnswer;//�Ƿ���ҪӦ��
	int nOvertime;//Ӧ��ʱʱ���趨
	int nNum;//�ṹ����
	BYTE btData[1024];//������
};

struct sPROTOCOL_FRAME_RECEIVE //���ձ��ĵ�֡�ṹ
{
	sPROTOCOL_FRAME_RECEIVE()
	{
		btFlag = 0x00;
		nNum = 0;
		ZeroMemory(btData,sizeof(btData));
		memset(cMsg,0,256);
		nZZNo = 0;
	}
	int nZZNo;//װ�ú�
	BYTE btFlag;//֡��ʶ
	char cMsg[256];
	int nNum;//�ṹ����
	BYTE btData[5120];//������
};
//����ṹ���Ա㱻���������
enum E_PROTOCOL_TYPE//����ö��
{
	PROTOCOL_YC,//ң��
	PROTOCOL_YX,//ң��
	PROTOCOL_YM,//ң��
	PROTOCOL_SOE,
	PROTOCOL_DZ,//��ֵ
	PROTOCOL_DevStatus,//װ��(�豸)״̬
	PROTOCOL_YKBACK,//ң�ط���
	PROTOCOL_DZXG_ERROR,//��ֵ�޸Ĵ���
	PROTOCOL_DZ_CURAREA,//��ǰ��ֵ��
	PROTOCOL_DZQH_OK,//��ֵ�л��ɹ�
	PROTOCOL_DZQH_ERROR,//��ֵ�л�����
	PROTOCOL_LB,//¼��
};

enum
{
	GDD1_NULL = 0,
	GDD1_OS8ASCII = 1,
	GDD1_BS1 = 2,
	GDD1_UINT = 3,
	GDD1_INT = 4,
	GDD1_UFLOAT = 5,
	GDD1_FLOAT = 6,
	GDD1_R3223 = 7,
	GDD1_R6453 = 8,
	GDD1_DOUBLEPOINT = 9,
};

enum PRO_YK
{
	YK_SEL0,
	YK_SEL1,
	YK_ZX0,
	YK_ZX1,
	YK_CX0,
	YK_CX1,
	YK_JT0,//��ͣ
	YK_JT1,

};

enum
{
	COT_YF = 1,
	COT_ZX,
	COT_CX,
	COT_READ,
	COT_CHECK,
	COT_WRITEOK = 0x14,
	COT_WRITEERROR,
	COT_READOK,
	COT_READERROR
};

enum
{
	GIN1_YC = 1,
	GIN1_YX,
	GIN1_YK,
	GIN1_DZ,
	GIN1_CAPTION,
	GIN1_DNL//����
};

enum
{
	KOD_NULL = 0,
	KOD_REALVAL,
	KOD_NAME,
	KOD_PRIMARY,
	KOD_SCALE,
	KOD_ADDR,
	KOD_ANGLE
};

//�û��·���������
enum
{
	DEV_CMD_REVERT,//����
	DEV_CMD_DDZ,//����ֵ
	DEV_CMD_DZQH,//��ֵ���л�
	DEV_CMD_DZYF,//��ֵԤ��
	DEV_CMD_DZFIX,//��ֵ�̻�
	DEV_CMD_LUBO,//����¼��
	DEV_CMD_YKYF,//ң��Ԥ����ִ�У�����
	DEV_CMD_YKZX,
	DEV_CMD_YKCX
};

#define MAX_DEV 255
#define MAX_DEV_YC 256
#define MAX_DEV_YX 128
#define MAX_DEV_YK 256
#define MAX_DEV_DZ 256
#define MAX_DEV_YM 4

struct sPROTOCOL_DZQH //��ֵ���л�
{
	sPROTOCOL_DZQH()
	{
		btOld = 0;
		btNew = 0;
		bQHOK = FALSE;
	}
	BYTE btOld;//��ֵ���ɺ�
	BYTE btNew;//��ֵ���º�
	BOOL bQHOK;//�л�OK
};

struct sPROTOCOL_DevStatus
{
	sPROTOCOL_DevStatus()
	{
		nZZNo = 0;
		nUnderZZNo = -1;
		btConnectStatus = 1;
	}
	int nZZNo; //װ�ú�
	int nUnderZZNo;//����װ�ú�
	BYTE btConnectStatus;//����״̬
};
class CProtBase
{
public:
	CProtBase(HWND hWndBack)
	{
		m_hAccessDataArray = CreateMutex(NULL, FALSE, NULL);
	    memset(&m_yk,0,sizeof(ykDll));
	    m_FrameLength = 0;
		m_nPort = 1;
		m_CmdInsertList.RemoveAll();
		m_CJList.RemoveAll();
		m_SOEList.RemoveAll();
		m_DZList.RemoveAll();
		m_103DataList.RemoveAll();
		m_Recv.RemoveAll();
		m_AutoSend.RemoveAll();
		m_ManuSend.RemoveAll();
		m_hPortWnd = hWndBack;
		m_ArrayParam.RemoveAll();
		m_nCurOperZZNo = -1;
		m_bLuboStart = FALSE;
		m_gzIndexList.RemoveAll();
		m_ZZStatusList.RemoveAll();
		m_nGatewayAddr = 1;
		for (int kk = 0;kk < 256;kk ++)
		{
			m_YcDataCur[kk] = 0x00;
			if(kk < 64)
			{
				m_YxDataCur[kk] = 0x00;
				m_YmDataCur[kk] = 0x00;
			}
		}
		m_nZFYCNum = 256;
		m_nZFYXNum = 512;
		m_nZFYMNum = 64;
		m_nOnlineZZNo = -1;
		m_nOnlineUnderZZNo = -1;
		m_bZZLinkStatus = FALSE;
		ZeroMemory(m_btParam,sizeof(m_btParam));
		ZeroMemory(m_sLuboFileName,sizeof(m_sLuboFileName));
		m_StartFalgArray.RemoveAll();
		m_SendFalgArray.RemoveAll();
		ZeroMemory(m_btGlobalAddr,sizeof(m_btGlobalAddr));
		m_bProcessOK = FALSE;

	};

	virtual ~CProtBase()
	{
		CloseHandle(m_hAccessDataArray);
	};

	//���ͱ������
	CByteArray m_StartFalgArray;//�����������
	CByteArray m_SendFalgArray;//װ��ѭ���·�����

	HANDLE m_hAccessDataArray;
	BOOL m_bProcessOK;


	HWND m_hPortWnd;//��Լ������
	CMyByteArray m_Recv;
	CMyByteArray m_AutoSend;
	CMyByteArray m_ManuSend;
	CMyByteArray m_ArrayParam;//�����Լ����

	ykDll m_yk;
	senddll m_SendDll;

	int m_FrameLength;
	int  m_nPort;

	int m_nOnlineZZNo;
	int m_nOnlineUnderZZNo;//����װ��
	BOOL m_bZZLinkStatus;

	CList<sInsertCmd,sInsertCmd> m_CmdInsertList;
	CList<cjDll,cjDll> m_CJList;
	CList<soeDll,soeDll> m_SOEList;
	CList<gzIndex,gzIndex> m_gzIndexList;
	CList<sPROTOCOL_DZ,sPROTOCOL_DZ> m_DZList;
	CList<sPROTOCOL_103Data,sPROTOCOL_103Data> m_103DataList;
	CList<sPROTOCOL_DevStatus,sPROTOCOL_DevStatus> m_ZZStatusList;//װ��״̬

	sPROTOCOL_YK m_YKBack;
	sPROTOCOL_DevOperFlag m_DevOperFlag;//װ�ò���״̬
	sPROTOCEL_GZCurve m_GZCurve;

	virtual BOOL Process(int nDatalen) = 0;
	virtual void Free() = 0;

	//Ϊ����ӿ�
	virtual void SetCurZZ(int nZZNo,int nGateNo,char *ZZType,char * ZZParam) = 0;
	virtual BOOL UnPackData(BYTE* recv,int len) = 0;
    virtual int ManualPackData(sPROTOCOL_YK yk) = 0;
    virtual int AutoPackData() = 0;
    virtual BOOL PackInsertData(int nInsertFlag,BOOL bInsertAll = FALSE) = 0;
	virtual void SetZZVal(BYTE* data,int len,int nZZNo,BYTE btTabNo) = 0;

	void RemoveLeft(int n)
	{
		m_Recv.RemoveAt(0,n);
	}
	void SendMsgToUser(int nSendFlag)
	{
		WaitForSingleObject(m_hAccessDataArray, INFINITE);

		sPROTOCOL_FRAME_RECEIVE receive;
		receive.btFlag = nSendFlag;
//		receive.nZZNo = m_pCurZZ->m_ZZNo;
		if(nSendFlag == Pro_ReceDevYC)
		{
			cjDll cjdll;
			m_ArrayParam.RemoveAll();
			while (!m_CJList.IsEmpty())
			{
				cjdll = m_CJList.RemoveHead();
				m_ArrayParam.Addx((BYTE*)&cjdll,sizeof(cjDll));
				receive.nNum ++;
				if((1024 / sizeof(cjDll)) < receive.nNum)
					break;
			}
			memcpy(receive.btData,m_ArrayParam.GetData(),m_ArrayParam.GetSize());
		}
		else if(nSendFlag == Pro_AUTOPACK)
		{
			memcpy(m_SendDll.btData,m_AutoSend.GetData(),m_AutoSend.GetSize());
			m_SendDll.nDataLen = m_AutoSend.GetSize();
			if(m_SendDll.btSendFlag >= Pro_SendYC && m_SendDll.btSendFlag <= Pro_SendSOE)
				m_SendDll.bNeedAnswer = FALSE;
			BYTE btData[1048];
			memcpy(btData,&m_SendDll,sizeof(senddll));
			receive.nNum = 1;
			memcpy(receive.btData,btData,sizeof(senddll));

			if(m_SendDll.btSendFlag == Pro_AskYC)
				strcpy_s(receive.cMsg,"��YC");
			else if(m_SendDll.btSendFlag == Pro_AskYX)
				strcpy_s(receive.cMsg,"��YX");
			else if(m_SendDll.btSendFlag == Pro_AskYM)
				strcpy_s(receive.cMsg,"��DD");
			else if(m_SendDll.btSendFlag == Pro_SendDDZ)
				strcpy_s(receive.cMsg,"����ֵ");
			else if(m_SendDll.btSendFlag == Pro_SendJS)
				strcpy_s(receive.cMsg,"Уʱ");
			else if(m_SendDll.btSendFlag == Pro_SendJSAll)
				strcpy_s(receive.cMsg,"��Уʱ");
			else if(m_SendDll.btSendFlag == Pro_SignRevert)
				strcpy_s(receive.cMsg,"����");
			else if(m_SendDll.btSendFlag == Pro_SignRevertAll)
				strcpy_s(receive.cMsg,"�ܸ���");
			else if(m_SendDll.btSendFlag == Pro_SendDZFIX)
				strcpy_s(receive.cMsg,"��ֵ�̻�");
			else if(m_SendDll.btSendFlag == Pro_SendDZVAL)
				strcpy_s(receive.cMsg,"��ֵԤ��");
			else if(m_SendDll.btSendFlag == Pro_SendYK)
				strcpy_s(receive.cMsg,"ң��");
			else if(m_SendDll.btSendFlag == Pro_SendRYB)
				strcpy_s(receive.cMsg,"��ѹ��");
			else if(m_SendDll.btSendFlag == Pro_SendLubo)
				strcpy_s(receive.cMsg,"¼��");
			else if(m_SendDll.btSendFlag == Pro_AskLuboIndex)
				strcpy_s(receive.cMsg,"¼��������");
			else if(m_SendDll.btSendFlag == Pro_Ask1LevelData)
				strcpy_s(receive.cMsg,"����1������");
			else if(m_SendDll.btSendFlag == Pro_Ask2LevelData)
				strcpy_s(receive.cMsg,"����2������");
			else if(m_SendDll.btSendFlag == Pro_ResetCU)
				strcpy_s(receive.cMsg,"��λͨ�ŵ�Ԫ");
			else if(m_SendDll.btSendFlag == Pro_ResetFCB)
				strcpy_s(receive.cMsg,"��λ֡����λ");
		}
		else if(nSendFlag == Pro_MANUPACK)
		{
			senddll send;
			memcpy(send.btData,m_ManuSend.GetData(),m_ManuSend.GetSize());
			send.nDataLen = m_ManuSend.GetSize();
			BYTE btData[1048];
			memcpy(btData,&send,sizeof(senddll));
			receive.nNum = 1;
			memcpy(receive.btData,btData,sizeof(senddll));
		}
		else if(nSendFlag == Pro_ReceDevSOE)
		{
			soeDll soe;
			m_ArrayParam.RemoveAll();
			while (!m_SOEList.IsEmpty())
			{
				soe = m_SOEList.RemoveHead();
				m_ArrayParam.Addx((BYTE*)&soe,sizeof(soeDll));
				receive.nNum ++;
			}
			memcpy(receive.btData,m_ArrayParam.GetData(),m_ArrayParam.GetSize());
			strcpy_s(receive.cMsg,"�յ�װ��SOE");

		}
		else if(nSendFlag == Pro_ReceDevOper)
		{
			memcpy(receive.btData,&m_DevOperFlag,sizeof(sPROTOCOL_DevOperFlag));
			strcpy_s(receive.cMsg,"װ�ò�������");
		}
		else if(nSendFlag == Pro_ReceDevYKBACK || nSendFlag == Pro_ReceAskYK)
		{
			memcpy(receive.btData,&m_YKBack,sizeof(sPROTOCOL_YK));
			if(nSendFlag == Pro_ReceDevYKBACK)
				strcpy_s(receive.cMsg,"ң�ط���");
			else if(nSendFlag == Pro_ReceAskYK)
				strcpy_s(receive.cMsg,"��������ң��");
		}
		else if(nSendFlag == Pro_ReceDevDzVAL)//��ֵ����
		{
			sPROTOCOL_DZ dz;
			m_ArrayParam.RemoveAll();
			while (!m_DZList.IsEmpty())
			{
				dz = m_DZList.RemoveHead();
				m_ArrayParam.Addx((BYTE*)&dz,sizeof(sPROTOCOL_DZ));
				receive.nNum ++;
			}
			memcpy(receive.btData,m_ArrayParam.GetData(),m_ArrayParam.GetSize());
			strcpy_s(receive.cMsg,"��ֵ����");

		}
		else if(nSendFlag == Pro_ReceDevStatus)
		{
			sPROTOCOL_DevStatus devstatus;
			m_ArrayParam.RemoveAll();
			while (!m_ZZStatusList.IsEmpty())
			{
				devstatus = m_ZZStatusList.RemoveHead();
				m_ArrayParam.Addx((BYTE*)&devstatus,sizeof(sPROTOCOL_DevStatus));
				receive.nNum ++;
			}
			memcpy(receive.btData,m_ArrayParam.GetData(),m_ArrayParam.GetSize());
			strcpy_s(receive.cMsg,"�յ�װ��״̬");
		}
		else if(nSendFlag == Pro_ReceAskJS || nSendFlag == Pro_ReceAskFG)
		{
			//��������Уʱ
			memcpy(receive.btData,&m_DevOperFlag,sizeof(sPROTOCOL_DevOperFlag));
			if(nSendFlag == Pro_ReceAskJS)
				strcpy_s(receive.cMsg,"��������Уʱ");
			else//�������󸴹�
				strcpy_s(receive.cMsg,"�������󸴹�");
		}
		else if(nSendFlag == Pro_ReceDevGZIndex)
		{
			gzIndex gz;
			m_ArrayParam.RemoveAll();
			while (!m_gzIndexList.IsEmpty())
			{
				gz = m_gzIndexList.RemoveHead();
				m_ArrayParam.Addx((BYTE*)&gz,sizeof(gzIndex));
				receive.nNum ++;
			}
			memcpy(receive.btData,m_ArrayParam.GetData(),m_ArrayParam.GetSize());
			strcpy_s(receive.cMsg,"���ع�������");
		}
		else if(nSendFlag >= Pro_ReceDevGZ_DataReady && nSendFlag < Pro_ReceDevGZ_LUBOEnd)
		{
			m_btParam[1] = nSendFlag;
			memcpy(receive.btData,m_btParam,sizeof(m_btParam));
			strcpy_s(receive.cMsg,"¼������");
		}
		else if(nSendFlag == Pro_ReceDevGZ_LUBOEnd)
		{
			receive.nNum = m_btParam[0];
			memcpy(receive.btData,m_sLuboFileName,receive.nNum);
			strcpy_s(receive.cMsg,"¼���ļ�����");
		}
		else if(nSendFlag == Pro_ReceGZCurve)
		{
			receive.nNum = 1;
			memcpy(receive.btData,&m_GZCurve,sizeof(sPROTOCEL_GZCurve));
			strcpy_s(receive.cMsg,"������������");
		}
		BYTE btTempData[5120+13+256];
		int nLength = sizeof(sPROTOCOL_FRAME_RECEIVE);
		memcpy(btTempData,&receive,nLength);
		if(IsWindow(m_hPortWnd))
			::SendMessage(m_hPortWnd,WM_DLL_BACK,(WPARAM)btTempData,(LPARAM)nLength);
		ReleaseMutex(m_hAccessDataArray);
	}
	Circle_Send m_CircleSend[5];//��ʱ�·�
	int m_nCurOperZZNo;//��ǰ����װ�ú�
	BOOL m_bLuboStart;//¼���������
	int m_nGatewayAddr;

	//CDT��վ�õ�,��Щ������վ�ڷ���ǰ��д������!
	BYTE m_YxDataCur[64];//ת��ң�����512 64*8 = 512
	unsigned short m_YcDataCur[256];//ң��256��
	unsigned int m_YmDataCur[64];//ң��64��
	int m_nZFYCNum;
	int m_nZFYXNum;
	int m_nZFYMNum;
	BYTE m_btParam[2];//��������
	char m_sLuboFileName[256];
	char m_btGlobalAddr[14];
};

typedef CProtBase* (*CREATE_PROTOCOL)(HWND hWndBack);//����������ô�����Լ��һ��ʵ��

//T101  ***************************************
//֡��ʽ
#define   FRM_RESPONSE   0x68
#define   FRM_CONFIRM    0x10
#define   FRM_END        0x16

//֡���Ͷ���
#define   M_SP_NA_1      1
#define   M_SP_TA_1      2
#define   M_ST_NA_1      5
#define   M_BO_NA_1      7
#define   M_ME_NA_1      9
#define   M_IT_NA_1      15
#define   M_EP_TA_1      17
#define   M_EP_TB_1      18
#define   M_EP_TC_1      19
#define   M_PS_NA_1      20
#define   M_ME_ND_1      21

#define   C_DC_NA_1      46
#define   C_RC_NA_1      47
#define   C_SE_NA_1      48

#define   M_EI_NA_1      70

#define   C_IC_NA_1      100
#define   C_CI_NA_1      101
#define   C_CS_NA_1      103
#define   C_TS_NA_1      104

//****************************************************************



inline void FillGIDData(CString sVar,BYTE btGDD1,BYTE btGDD2,BYTE * btGID)
{
	switch(btGDD1)
	{
	case GDD1_OS8ASCII:
		{
			memcpy(btGID,sVar.GetBuffer(0),btGDD2);
		}
		break;
	case GDD1_BS1:
		{
			if(sVar.GetLength() != btGDD2*2)
				return;
			CString sTemp = sVar;
			CString str;
			char crStr[2];
			int nIndex = 0;
			while(sTemp.GetLength() > 0)
			{
				str = sTemp.Left(2);
				strcpy_s(crStr,(char*)str.GetBuffer());
				
				for (int i=0;i<2;i++)
				{
					if(isalpha(crStr[i]))
					{
						crStr[i] = tolower(crStr[i]);
						crStr[i] -= 87;
					}
					else
						crStr[i] -= 48;
				}
				btGID[nIndex] = crStr[0] * 16 + crStr[1];
				nIndex ++;

				sTemp = sTemp.Right(sTemp.GetLength() - 2);
			}
		}
		break;
	case GDD1_UINT:
	case GDD1_INT:
		{
			int nVal = _wtoi(sVar);
			memcpy(btGID,&nVal,btGDD2);
		}
		break;
	case GDD1_UFLOAT:
	case GDD1_FLOAT:
	case GDD1_R3223:
	case GDD1_R6453:
		{
			double fVal = _wtoi(sVar);
			memcpy(btGID,&fVal,btGDD2);
		}
		break;
	case GDD1_DOUBLEPOINT:
		{
			BYTE btVal = (BYTE)_wtoi(sVar);
			btGID[0] = btVal;
		}
		break;
	case GDD1_NULL:
	default:
		break;
	}
}


//DLL�ӿں���ָ�����Ͷ���
typedef void (*PROTOCOL_INIT)(HWND hWndBack);//
typedef void (*PROTOCOL_TOMAIN)();//
typedef void (*PROTOCOL_CLOSE)();//
typedef void (*PROTOCOL_SENDCMD)(sPROTOCOL_FRAME frame);//
typedef void (*PROTOCOL_SHOWMAINDLG)();//
typedef BOOL (*PROTOCOL_GETDEVSTATUS)(int nZZNo);//
typedef void (*PROTOCOL_SETDEV)(BOOL bAddFlag,int nDevAddr,int nFUN);//


inline WORD GetModbusCRC16(BYTE *pb, int n)//Modbus CRCУ����,��������
{
	WORD crc=0xFFFF;
	BYTE loCRC=LOBYTE(crc);
	BYTE hiCRC=HIBYTE(crc);
	for(int i=0; i<n; i++)
	{
		loCRC^=pb[i];
		crc=WORD((hiCRC<<8)+loCRC);
		for(int j=0; j<8; j++)
		{
			if((crc&1) == 1)
				crc=WORD((crc>>1)^0xA001);
			else
				crc=WORD(crc>>1);
		}
		loCRC=LOBYTE(crc);
		hiCRC=HIBYTE(crc);
	}
	return crc;
};

inline WORD GetSumbyByte(BYTE *pb, int n, LPBYTE loW=NULL, LPBYTE hiW=NULL)
{
	WORD wVal=0;
	for(int i=0;i<n;i++)
	{
		wVal=WORD(wVal+pb[i]);
	}
	if(loW!=NULL)
		*loW=LOBYTE(wVal);
	if(hiW!=NULL)
		*hiW=HIBYTE(wVal);
	return wVal;
};

/*����У���,������	loW��hiW�ֱ�Ϊ������ֵĵ��ֽں͸��ֽ�*/
inline WORD GetSumbyWord(WORD *pw, int n, LPBYTE loW=NULL, LPBYTE hiW=NULL)
{
	WORD wVal=0;
	for(int i=0;i<n;i++)
	{
		wVal=WORD(wVal+pw[i]);
	}
	if(loW!=NULL)
		*loW=LOBYTE(wVal);
	if(hiW!=NULL)
		*hiW=HIBYTE(wVal);
	return wVal;
};

inline BYTE BCDToBin(BYTE dByte)//BCD��תΪ�ֽ�
{
	return BYTE((dByte>>4)*10+(dByte&0x0F));
};
inline WORD WBCDToBin(WORD dWord, BYTE& xhByte, BYTE& xlByte)//BCD����ת��
{
	BYTE dhByte=BYTE(dWord>>8);
	BYTE dlByte=BYTE(dWord&0xFF);
	xhByte=BCDToBin(dhByte);
	xlByte=BCDToBin(dlByte);
	return WORD(xhByte*100+xlByte);
};
inline BYTE BinToBCD(BYTE xByte)//�ֽ�תBCD��
{
	return BYTE((xByte/10<<4)+xByte%10);
};
inline BYTE ByteToBCD(BYTE nbyte)//�ֽ�תBCD��
{
	BYTE hibyte,lowbyte;
	//�����ʮλ��
	hibyte=(BYTE)(nbyte/(BYTE)10);
	//�������λ��
	lowbyte=nbyte%10;
	return (BYTE)(hibyte*6+nbyte);
};
inline WORD WBinToBCD(WORD xWord, BYTE& dhByte, BYTE& dlByte)//��תBCD��
{
	BYTE xhByte=BYTE(xWord/100);
	BYTE xlByte=BYTE(xWord%100);
	dhByte=BinToBCD(xhByte);
	dlByte=BinToBCD(xlByte);
	return WORD((dhByte<<8)+dlByte);
};

//���ַ����Ĵ���
#define ADDNULL 1

inline int tszFind(LPCTSTR str,LPCTSTR lpszSub, int nStart=0)
{
	int nLength = _tcslen(str);
	if (nStart > nLength)
		return -1;

	// find first matching substring
	LPCTSTR lpsz = _tcsstr(str + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - str);
}

inline int tszFind(LPCTSTR str,TCHAR ch, int nStart=0)
{
	int nLength = _tcslen(str);
	if (nStart > nLength)
		return -1;

	// find first single character
	LPCTSTR lpsz = _tcschr(str + nStart,  (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - str);
}

inline int tszFindReverse(LPCTSTR str,TCHAR ch)
{
	// find first single character
	LPCTSTR lpsz = _tcsrchr(str,  (_TUCHAR)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - str);
}

inline BOOL StrGetVal(LPCTSTR str,int nIndex,LPTSTR sName,LPTSTR sVal)
{
	int i1=0,i2=0,i=0;
	int nI=0;
	TCHAR sTemp[256];
	memset(sTemp,0,sizeof(sTemp));
	i2=tszFind(str,_T(';'),i1);
	while(i2!=-1)
	{
		if(nI==nIndex)
		{
			int len=i2-i1;
			lstrcpyn(sTemp,str+i1,len+ADDNULL);
			i=tszFindReverse(sTemp,_T('='));
			if(i!=-1)
			{
				lstrcpyn(sName,sTemp,i+ADDNULL);
				sName[i]=0;
				lstrcpyn(sVal,sTemp+i+1,len-i-1+ADDNULL);
				sVal[len-i-1]=0;
				return TRUE;
			}
			else
				return FALSE;
		}
		i1=i2+1;
		i2=tszFind(str,_T(';'),i1);
		nI++;
	}
	return FALSE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,LPTSTR sVal)
{
	int i1=0,i2=0;
	i1=tszFind(str,sName);
	while(1)
	{
		if(i1==-1)
			return FALSE;
		i2=tszFind(str,_T('='),i1);
		if(i2==-1)//�쳣
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//��֤sName�������
			break;
		i1=tszFind(str,sName,i2);
	}
	i1=i2+1;
	i2=tszFind(str,_T(';'),i1);
	if(i2==-1)
		return FALSE;
	lstrcpyn(sVal,str+i1,i2-i1+ADDNULL);
	sVal[i2-i1]=0;
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,int& nVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	nVal=_ttoi(sVal);
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,BYTE& nVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	nVal=BYTE(_ttoi(sVal));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,UINT& nVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	nVal=UINT(_ttoi(sVal));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,DWORD& nVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	nVal=DWORD(_ttoi(sVal));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,USHORT& nVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	nVal=USHORT(_ttoi(sVal));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,FLOAT& fVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	LPTSTR* stopstr=NULL;
	fVal=FLOAT(_tcstod(sVal,stopstr));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,double& fVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	LPTSTR* stopstr=NULL;
	fVal=double(_tcstod(sVal,stopstr));
	return TRUE;
}

inline BOOL StrGetVal(LPCTSTR str,LPCTSTR sName,CString& strVal)
{
	TCHAR sVal[128];
	if(!StrGetVal(str,sName,sVal))
		return FALSE;
	strVal=sVal;
	return TRUE;
}
inline BOOL StrSetVal(LPTSTR str,LPCTSTR sName,LPCTSTR sVal,TCHAR cPart=_T(';'))
{
	int i1=0,i2=0;
	i1=tszFind(str,sName);
	while(1)
	{
		if(i1==-1)//������,������
		{
			TCHAR sAdd[256];
			wsprintf(sAdd,_T("%s=%s%c"),sName,sVal,cPart);
			lstrcat(str,sAdd);
			return TRUE;
		}
		i2=tszFind(str,_T('='),i1);
		if(i2==-1)//�쳣
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//��֤sName�������
			break;
		i1=tszFind(str,sName,i2);
	}
	i1=i2+1;
	i2=tszFind(str,cPart,i1);
	if(i2==-1)
		return FALSE;
	TCHAR sz[2048];
	lstrcpy(sz,sVal);
	lstrcat(sz,str+i2);
	lstrcpy(str+i1,sz);
	return TRUE;
}

inline BOOL StrSetVal(CString& str,LPCTSTR sName,LPCTSTR sVal,TCHAR cPart=_T(';'))
{
	int i1=0,i2=0;
	i1=str.Find(sName,0);
	while(1)
	{
		if(i1==-1)//������,������
		{
			TCHAR sAdd[256];
			wsprintf(sAdd,_T("%s=%s%c"),sName,sVal,cPart);
			str+=sAdd;
			return TRUE;
		}
		i2=str.Find(_T('='),i1);
		if(i2==-1)//�쳣
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//��֤sName�������
			break;
		i1=str.Find(sName,i2);
	}
	i1=i2+1;
	i2=str.Find(cPart,i1);
	if(i2==-1)
		return FALSE;
	CString stemp(str.Mid(0,i1));
	stemp+=sVal;
	stemp+=str.Mid(i2);
	str=stemp;
	return TRUE;
}

inline BOOL StrSetVal(LPTSTR str,LPCTSTR sName,int nVal,TCHAR cPart=_T(';'))
{
	TCHAR sVal[128];
	wsprintf(sVal,_T("%ld"),nVal);
	if(!StrSetVal(str,sName,sVal,cPart))
		return FALSE;
	return TRUE;
}

inline BOOL StrSetVal(CString& str,LPCTSTR sName,int nVal,TCHAR cPart=_T(';'))
{
	TCHAR sVal[128];
	wsprintf(sVal,_T("%ld"),nVal);
	if(!StrSetVal(str,sName,sVal,cPart))
		return FALSE;
	return TRUE;
}
inline void GetExListStr(CListCtrl* pList,CString& str,TCHAR cPart=_T(';'))
{
	str.Empty();
	CString stemp;
	for(int i=0;i<pList->GetItemCount();i++)
	{
		stemp=pList->GetItemText(i,0);
		if(stemp.IsEmpty())//�����������Ƶ�����
			continue;
		stemp.Remove(cPart);
		str+=stemp;
		str+=_T('=');
		stemp=pList->GetItemText(i,1);
		stemp.Remove(cPart);
		str+=stemp;
		str+=cPart;
	}
}

inline void SetExListStr(CListCtrl* pList,LPCTSTR str)
{
	pList->SetRedraw(FALSE);
	pList->DeleteAllItems();
	int ii=0;
	TCHAR sName[128],sVal[128];
	memset(sName,0,sizeof(sName));
	memset(sVal,0,sizeof(sVal));
	while(StrGetVal(str,ii,sName,sVal))
	{
		pList->InsertItem(ii,_T(""));
		pList->SetItemText(ii,0,sName);
		pList->SetItemText(ii,1,sVal);
		memset(sName,0,sizeof(sName));
		memset(sVal,0,sizeof(sVal));
		ii++;
	}
	pList->SetRedraw(TRUE);
	for(int Columncount=0;Columncount<pList->GetHeaderCtrl()->GetItemCount();Columncount++)
		pList->SetColumnWidth(Columncount,LVSCW_AUTOSIZE_USEHEADER);
	pList->Invalidate();
}