#include "AfxTempl.h"

#define WM_DLL_BACK WM_USER + 0x0208

#define  WM_PROTOCOL_BACK     (WM_USER+0x10)//调用者窗口返回消息
#define  WM_PROTOCOL_RECEIVE     (WM_USER+0x11)//收到数据
#define  WM_PROTOCOL_SEND     (WM_USER+0x12)//发送数据
#define  WM_PROTOCOL_ASKVAL     (WM_USER+0x13)//规约请求读取数据

enum PRO_ASK
{
	Pro_ASKNULL,//无效命令
	Pro_SendU,
	Pro_SendS,
	//全局的下发命令
	Pro_AskAll,//总召唤
	Pro_SendJS,//校时
	Pro_SignRevert,//信号复归
	Pro_SendJSAll,//校时
	Pro_SendALLXY,//消音
	Pro_SignRevertAll,//信号复归
	//一般问答规约
	Pro_AskYC,
	Pro_AskYX,
	Pro_AskYM,
	//问答规约扩展
	Pro_ResetFCB,
	Pro_ResetCU,
	Pro_Ask1LevelData,//询问一级数据
	Pro_Ask2LevelData,//询问二级数据
	//定值
	Pro_SendDDZ,//调定值
	Pro_SendDMS,//描述
	Pro_SendSXX,//上下限
	Pro_SendDZVAL,//下发定值
	Pro_SendDZFIX,//固化定值
	Pro_SendDZQH,//下发定值区号
	//其他
	Pro_SendYK,//摇控
	Pro_SendRYB,//软压板
	Pro_AskStatus,//装置状态
	Pro_AskSOE,
	Pro_ConfirmSOE,
	//扩展的
	Pro_AskEx1,
	Pro_AskEx2,
	Pro_AskEx3,
	Pro_AskEx4,
	Pro_AskEx5,
	Pro_AskEx6,
	Pro_AskEx7,
	Pro_AskEx8,
	Pro_AskEx9,
	//录波
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
	Pro_ReceDevRYB,//软压板
	Pro_ReceDevSOE,
	Pro_ReceDevDzVAL,
	Pro_ReceDevDzNAME,
	Pro_ReceDevDzSXX,
	Pro_ReceDevYKBACK,
	Pro_ReceDevDZQH,
	Pro_ReceDevOnLine,
	Pro_ReceDevStatus,//装置状态，上下线
	Pro_ReceDevOper,//装置操作状态
	Pro_ReceStationStatus,
	Pro_AUTOPACK,
	Pro_MANUPACK,
	Pro_ReceDevGZIndex,//故障表
	Pro_ReceDevGZ_DataReady,//数据准备好
	Pro_ReceDevGZ_YXDataReady,//状态量准备好
	Pro_ReceDevGZ_YXData,//状态量数据
	Pro_ReceDevGZ_YXDataEnd,//状态量数据结束
	Pro_ReceDevGZ_YCDataReady,//模拟量准备好
	Pro_ReceDevGZ_YCData,//模拟量数据
	Pro_ReceDevGZ_YCDataEnd,//模拟量数据结束
	Pro_ReceDevGZ_LUBOEnd,//录波过程结束

	//以下为从站用到
	Pro_ReceAskYK,//请求遥控
	Pro_ReceAskSJ,//请求升降
	Pro_ReceAskJS,
	Pro_ReceAskFG,
	Pro_ReceGZCurve,
};

struct Circle_Send//循环下发结构 
{
	Circle_Send()
	{
		bCircle = FALSE;
		bBroadcast = FALSE;
		nSendFlag = Pro_AskYX;
		nCircleTime = 0;
		nTimerID = 0;
	}
	BOOL bCircle;//是否循环
	BOOL bBroadcast;//是否广播
	int nSendFlag;//下发标记
	int nCircleTime;//循环间隔
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
	int nUnderdevno;//二级装置
	int point;
	int status;
	int flag;
	//DZ
	int  num;
	DWORD val[100];
	int nYKFlag;//遥控，定值标记
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
	int nUnderdevno;//二级装置
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
	int nUnderdevno;//二级装置
	char cAddr[14];
	int pointNo;
	int nIndex;//故障序号
	int nType;//故障类型
	int status;//值
	long btSOEFlag;////GSM用参数，1-漏电预警，2-漏电报警，3-温升预警，4-温升报警，5-设备故障
	SYSTEMTIME time;
	long it;
	int times;
	char soeVal[255];//故障值
};

struct senddll //用户命令结构
{
	senddll()
	{
		btSendFlag = 0xFF;
		ZeroMemory(btData,sizeof(btData));
		nDataLen = 0;
		bNeedAnswer = TRUE;
	}
	BYTE btSendFlag;//发送标识
	BYTE btData[1024];//打包数据
	int nDataLen; //数据长度
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

struct sPROTOCOL_DZ //定值数据结构
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
	int nZZNo; //装置号
	int nUnderdevno;//二级装置号
	int nAreaNo;//定值区
	int nDzNo;//定值序号
	int nDzProp;//定值属性
	float fDzVal;//数据
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
	int nZZNo; //装置号
	BYTE btGIN1;//(GIN1)//组号
	BYTE btGIN2;//条目号
	BYTE btKOD;//描述
	BYTE btGDD1;//数据类型
	BYTE btGDD2;//数据宽度
	BYTE btData[256];//数据
};

struct sPROTOCEL_GZCurve
{
	long nZZNo;
	BYTE nPointNo;
	WORD nCurveVal[50];
};

struct sPROTOCOL_DevOperFlag//装置操作状态
{
	sPROTOCOL_DevOperFlag()
	{
		nZZNo = 0;
		nUnderdevno = 0;
		btOperFlag = 0;
		btOperSuccess = 0;
	}
	int nZZNo; //装置号
	int nUnderdevno;//二级装置
	BYTE btOperFlag;//操作标识
	BYTE btOperSuccess;//成功标识,0-不成功,1-成功,2-超时
};
struct sPROTOCOL_YK //遥控下发参数
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

	int nYKFlag;//遥控，定值标记
	int nZZNo; //装置号
	char cAddr[14];
	BOOL bIsUnderZZ;//是二级装置定值
	int	nUnderZZNo;//二级装置号
	int  nYKNo; //点号
	float fVal;//下行：控制数据
	//上行:返回(0-失败,1-成功)
	BOOL bDLQ;//是否断路器
	BYTE btParam[10];//备选参数
};

struct sInsertCmd //插入命令结构
{
	sInsertCmd()
	{
		nZZNo = 0;
		nInsertCmdFlag = 0x00;
		ZeroMemory(cAddr,sizeof(cAddr));
	}
	BYTE nZZNo;//装置号
	char cAddr[14];

	BYTE nUnderdevno;
	BYTE nInsertCmdFlag;//命令帧类型
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
		return m_nSize-1;//新的索引的位置
	}
};

struct sPROTOCOL_FRAME //发送报文的帧结构
{
	sPROTOCOL_FRAME()	{
		nZZNo = 0xFF;
		btSendFlag = 0x00;
		nNum = 0;
		bNeedAnswer = FALSE;
		nOvertime = 0;
		ZeroMemory(btData,sizeof(btData));
	}
	int nZZNo; //装置号
	BYTE btSendFlag;//下发标识
	BOOL bNeedAnswer;//是否需要应答
	int nOvertime;//应答超时时间设定
	int nNum;//结构个数
	BYTE btData[1024];//数据体
};

struct sPROTOCOL_FRAME_RECEIVE //接收报文的帧结构
{
	sPROTOCOL_FRAME_RECEIVE()
	{
		btFlag = 0x00;
		nNum = 0;
		ZeroMemory(btData,sizeof(btData));
		memset(cMsg,0,256);
		nZZNo = 0;
	}
	int nZZNo;//装置号
	BYTE btFlag;//帧标识
	char cMsg[256];
	int nNum;//结构个数
	BYTE btData[5120];//数据体
};
//定义结构，以便被主程序调用
enum E_PROTOCOL_TYPE//类型枚举
{
	PROTOCOL_YC,//遥测
	PROTOCOL_YX,//遥信
	PROTOCOL_YM,//遥脉
	PROTOCOL_SOE,
	PROTOCOL_DZ,//定值
	PROTOCOL_DevStatus,//装置(设备)状态
	PROTOCOL_YKBACK,//遥控返回
	PROTOCOL_DZXG_ERROR,//定值修改错误
	PROTOCOL_DZ_CURAREA,//当前定值区
	PROTOCOL_DZQH_OK,//定值切换成功
	PROTOCOL_DZQH_ERROR,//定值切换错误
	PROTOCOL_LB,//录波
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
	YK_JT0,//急停
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
	GIN1_DNL//电能
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

//用户下发命令类型
enum
{
	DEV_CMD_REVERT,//复归
	DEV_CMD_DDZ,//调定值
	DEV_CMD_DZQH,//定值区切换
	DEV_CMD_DZYF,//定值预发
	DEV_CMD_DZFIX,//定值固化
	DEV_CMD_LUBO,//触发录波
	DEV_CMD_YKYF,//遥控预发，执行，撤销
	DEV_CMD_YKZX,
	DEV_CMD_YKCX
};

#define MAX_DEV 255
#define MAX_DEV_YC 256
#define MAX_DEV_YX 128
#define MAX_DEV_YK 256
#define MAX_DEV_DZ 256
#define MAX_DEV_YM 4

struct sPROTOCOL_DZQH //定值区切换
{
	sPROTOCOL_DZQH()
	{
		btOld = 0;
		btNew = 0;
		bQHOK = FALSE;
	}
	BYTE btOld;//定值区旧号
	BYTE btNew;//定值区新号
	BOOL bQHOK;//切换OK
};

struct sPROTOCOL_DevStatus
{
	sPROTOCOL_DevStatus()
	{
		nZZNo = 0;
		nUnderZZNo = -1;
		btConnectStatus = 1;
	}
	int nZZNo; //装置号
	int nUnderZZNo;//二级装置号
	BYTE btConnectStatus;//连接状态
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

	//发送标记数组
	CByteArray m_StartFalgArray;//启动命令队列
	CByteArray m_SendFalgArray;//装置循环下发队列

	HANDLE m_hAccessDataArray;
	BOOL m_bProcessOK;


	HWND m_hPortWnd;//规约主窗口
	CMyByteArray m_Recv;
	CMyByteArray m_AutoSend;
	CMyByteArray m_ManuSend;
	CMyByteArray m_ArrayParam;//数组规约参数

	ykDll m_yk;
	senddll m_SendDll;

	int m_FrameLength;
	int  m_nPort;

	int m_nOnlineZZNo;
	int m_nOnlineUnderZZNo;//二级装置
	BOOL m_bZZLinkStatus;

	CList<sInsertCmd,sInsertCmd> m_CmdInsertList;
	CList<cjDll,cjDll> m_CJList;
	CList<soeDll,soeDll> m_SOEList;
	CList<gzIndex,gzIndex> m_gzIndexList;
	CList<sPROTOCOL_DZ,sPROTOCOL_DZ> m_DZList;
	CList<sPROTOCOL_103Data,sPROTOCOL_103Data> m_103DataList;
	CList<sPROTOCOL_DevStatus,sPROTOCOL_DevStatus> m_ZZStatusList;//装置状态

	sPROTOCOL_YK m_YKBack;
	sPROTOCOL_DevOperFlag m_DevOperFlag;//装置操作状态
	sPROTOCEL_GZCurve m_GZCurve;

	virtual BOOL Process(int nDatalen) = 0;
	virtual void Free() = 0;

	//为对外接口
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
				strcpy_s(receive.cMsg,"读YC");
			else if(m_SendDll.btSendFlag == Pro_AskYX)
				strcpy_s(receive.cMsg,"读YX");
			else if(m_SendDll.btSendFlag == Pro_AskYM)
				strcpy_s(receive.cMsg,"读DD");
			else if(m_SendDll.btSendFlag == Pro_SendDDZ)
				strcpy_s(receive.cMsg,"调定值");
			else if(m_SendDll.btSendFlag == Pro_SendJS)
				strcpy_s(receive.cMsg,"校时");
			else if(m_SendDll.btSendFlag == Pro_SendJSAll)
				strcpy_s(receive.cMsg,"总校时");
			else if(m_SendDll.btSendFlag == Pro_SignRevert)
				strcpy_s(receive.cMsg,"复归");
			else if(m_SendDll.btSendFlag == Pro_SignRevertAll)
				strcpy_s(receive.cMsg,"总复归");
			else if(m_SendDll.btSendFlag == Pro_SendDZFIX)
				strcpy_s(receive.cMsg,"定值固化");
			else if(m_SendDll.btSendFlag == Pro_SendDZVAL)
				strcpy_s(receive.cMsg,"定值预置");
			else if(m_SendDll.btSendFlag == Pro_SendYK)
				strcpy_s(receive.cMsg,"遥控");
			else if(m_SendDll.btSendFlag == Pro_SendRYB)
				strcpy_s(receive.cMsg,"软压板");
			else if(m_SendDll.btSendFlag == Pro_SendLubo)
				strcpy_s(receive.cMsg,"录波");
			else if(m_SendDll.btSendFlag == Pro_AskLuboIndex)
				strcpy_s(receive.cMsg,"录波索引表");
			else if(m_SendDll.btSendFlag == Pro_Ask1LevelData)
				strcpy_s(receive.cMsg,"请求1级数据");
			else if(m_SendDll.btSendFlag == Pro_Ask2LevelData)
				strcpy_s(receive.cMsg,"请求2级数据");
			else if(m_SendDll.btSendFlag == Pro_ResetCU)
				strcpy_s(receive.cMsg,"复位通信单元");
			else if(m_SendDll.btSendFlag == Pro_ResetFCB)
				strcpy_s(receive.cMsg,"复位帧计数位");
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
			strcpy_s(receive.cMsg,"收到装置SOE");

		}
		else if(nSendFlag == Pro_ReceDevOper)
		{
			memcpy(receive.btData,&m_DevOperFlag,sizeof(sPROTOCOL_DevOperFlag));
			strcpy_s(receive.cMsg,"装置操作返回");
		}
		else if(nSendFlag == Pro_ReceDevYKBACK || nSendFlag == Pro_ReceAskYK)
		{
			memcpy(receive.btData,&m_YKBack,sizeof(sPROTOCOL_YK));
			if(nSendFlag == Pro_ReceDevYKBACK)
				strcpy_s(receive.cMsg,"遥控返回");
			else if(nSendFlag == Pro_ReceAskYK)
				strcpy_s(receive.cMsg,"调度请求遥控");
		}
		else if(nSendFlag == Pro_ReceDevDzVAL)//定值返回
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
			strcpy_s(receive.cMsg,"定值返回");

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
			strcpy_s(receive.cMsg,"收到装置状态");
		}
		else if(nSendFlag == Pro_ReceAskJS || nSendFlag == Pro_ReceAskFG)
		{
			//调度请求校时
			memcpy(receive.btData,&m_DevOperFlag,sizeof(sPROTOCOL_DevOperFlag));
			if(nSendFlag == Pro_ReceAskJS)
				strcpy_s(receive.cMsg,"调度请求校时");
			else//调度请求复归
				strcpy_s(receive.cMsg,"调度请求复归");
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
			strcpy_s(receive.cMsg,"返回故障索引");
		}
		else if(nSendFlag >= Pro_ReceDevGZ_DataReady && nSendFlag < Pro_ReceDevGZ_LUBOEnd)
		{
			m_btParam[1] = nSendFlag;
			memcpy(receive.btData,m_btParam,sizeof(m_btParam));
			strcpy_s(receive.cMsg,"录波数据");
		}
		else if(nSendFlag == Pro_ReceDevGZ_LUBOEnd)
		{
			receive.nNum = m_btParam[0];
			memcpy(receive.btData,m_sLuboFileName,receive.nNum);
			strcpy_s(receive.cMsg,"录波文件名称");
		}
		else if(nSendFlag == Pro_ReceGZCurve)
		{
			receive.nNum = 1;
			memcpy(receive.btData,&m_GZCurve,sizeof(sPROTOCEL_GZCurve));
			strcpy_s(receive.cMsg,"故障曲线数据");
		}
		BYTE btTempData[5120+13+256];
		int nLength = sizeof(sPROTOCOL_FRAME_RECEIVE);
		memcpy(btTempData,&receive,nLength);
		if(IsWindow(m_hPortWnd))
			::SendMessage(m_hPortWnd,WM_DLL_BACK,(WPARAM)btTempData,(LPARAM)nLength);
		ReleaseMutex(m_hAccessDataArray);
	}
	Circle_Send m_CircleSend[5];//定时下发
	int m_nCurOperZZNo;//当前操作装置号
	BOOL m_bLuboStart;//录波启动标记
	int m_nGatewayAddr;

	//CDT从站用到,这些数据主站在发送前，写入数据!
	BYTE m_YxDataCur[64];//转发遥信最多512 64*8 = 512
	unsigned short m_YcDataCur[256];//遥测256个
	unsigned int m_YmDataCur[64];//遥脉64个
	int m_nZFYCNum;
	int m_nZFYXNum;
	int m_nZFYMNum;
	BYTE m_btParam[2];//两个参数
	char m_sLuboFileName[256];
	char m_btGlobalAddr[14];
};

typedef CProtBase* (*CREATE_PROTOCOL)(HWND hWndBack);//由主程序调用创建规约的一个实例

//T101  ***************************************
//帧格式
#define   FRM_RESPONSE   0x68
#define   FRM_CONFIRM    0x10
#define   FRM_END        0x16

//帧类型定义
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


//DLL接口函数指针类型定义
typedef void (*PROTOCOL_INIT)(HWND hWndBack);//
typedef void (*PROTOCOL_TOMAIN)();//
typedef void (*PROTOCOL_CLOSE)();//
typedef void (*PROTOCOL_SENDCMD)(sPROTOCOL_FRAME frame);//
typedef void (*PROTOCOL_SHOWMAINDLG)();//
typedef BOOL (*PROTOCOL_GETDEVSTATUS)(int nZZNo);//
typedef void (*PROTOCOL_SETDEV)(BOOL bAddFlag,int nDevAddr,int nFUN);//


inline WORD GetModbusCRC16(BYTE *pb, int n)//Modbus CRC校验码,返回整数
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

/*按字校验和,返回字	loW和hiW分别为检验和字的低字节和高字节*/
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

inline BYTE BCDToBin(BYTE dByte)//BCD码转为字节
{
	return BYTE((dByte>>4)*10+(dByte&0x0F));
};
inline WORD WBCDToBin(WORD dWord, BYTE& xhByte, BYTE& xlByte)//BCD字码转字
{
	BYTE dhByte=BYTE(dWord>>8);
	BYTE dlByte=BYTE(dWord&0xFF);
	xhByte=BCDToBin(dhByte);
	xlByte=BCDToBin(dlByte);
	return WORD(xhByte*100+xlByte);
};
inline BYTE BinToBCD(BYTE xByte)//字节转BCD码
{
	return BYTE((xByte/10<<4)+xByte%10);
};
inline BYTE ByteToBCD(BYTE nbyte)//字节转BCD码
{
	BYTE hibyte,lowbyte;
	//分离出十位数
	hibyte=(BYTE)(nbyte/(BYTE)10);
	//分离出个位数
	lowbyte=nbyte%10;
	return (BYTE)(hibyte*6+nbyte);
};
inline WORD WBinToBCD(WORD xWord, BYTE& dhByte, BYTE& dlByte)//字转BCD码
{
	BYTE xhByte=BYTE(xWord/100);
	BYTE xlByte=BYTE(xWord%100);
	dhByte=BinToBCD(xhByte);
	dlByte=BinToBCD(xlByte);
	return WORD((dhByte<<8)+dlByte);
};

//对字符串的处理
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
		if(i2==-1)//异常
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//保证sName绝对相等
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
		if(i1==-1)//不存在,则增加
		{
			TCHAR sAdd[256];
			wsprintf(sAdd,_T("%s=%s%c"),sName,sVal,cPart);
			lstrcat(str,sAdd);
			return TRUE;
		}
		i2=tszFind(str,_T('='),i1);
		if(i2==-1)//异常
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//保证sName绝对相等
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
		if(i1==-1)//不存在,则增加
		{
			TCHAR sAdd[256];
			wsprintf(sAdd,_T("%s=%s%c"),sName,sVal,cPart);
			str+=sAdd;
			return TRUE;
		}
		i2=str.Find(_T('='),i1);
		if(i2==-1)//异常
			return FALSE;
		int len=lstrlen(sName);
		if(i2-i1 == len)//保证sName绝对相等
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
		if(stemp.IsEmpty())//不允许无名称的属性
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