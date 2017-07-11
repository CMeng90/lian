#if !defined(AFX_SENDDEFINE_H__INCLUDED_)
#define AFX_SENDDEFINE_H__INCLUDED_

//����������
#define SN_YC		1
#define SN_YX		2
#define TN_WFYX		3			
#define SN_YM		4
#define SN_SOE		5
#define SN_FAC		7
#define SN_ZZ		8
typedef struct
{
	int   s_ZZID;
	int   s_PntNo;
	float s_Val;
}TYC,TYM;
typedef struct
{
	int   s_ZZID;
	int   s_PntNo;
	int   s_Val;
}TYX;
//Event
typedef struct 
{
	int   nZZID;
    int   nPointNo;
	char  cEntInfo[128];
	char  cLZInfo[64];
	long  nType;
	long  nSource;
	long  btSOEFlag;//GSM�ò�����1-©��Ԥ����2-©�籨����3-����Ԥ����4-����������5-�豸����
	SYSTEMTIME t;
}TSOE;
typedef struct
{
	int   s_FacNo;
	int   s_Status;
}TFAC;
typedef struct
{
	int   s_ZZID;
	int   s_Status;
}TZZ;

//˫�򴫵�
#define SN_TIME						6
#define SN_YXFG						9
#define SN_XHFG						10
#define SN_ZZFG						11
#define SN_READ_RYB					12
#define SN_SET_RYB					13
#define SN_READ_DZ					14
#define SN_SET_DZ					15
#define SN_YK_YF					16
#define SN_YK_ZX					17
#define SN_YK_CX					18
#define SN_READ_GZINDEX				19
#define SN_READ_LUBO				20
#define SN_LUBONAME					21
#define SN_LUBOCFGFILE				22
#define SN_LUBODATFILE				23
#define SN_READ_GZCURVE				24
#define SN_GZCURVE_BACK				53

//ң����Ϣ
struct stYKCmd
{
	int nZZID;
	int nPntNo;
	int nType;//0:��բ��1����բ��2����ͣ
	int flag;//0��ʧ��1���ɹ�
};

//����ֵ������Ϣ
struct stBackDZCmd
{
	stBackDZCmd()
	{
		nPointNo = 0;
		fVal = 0;
		ZeroMemory(cName,sizeof(cName));
		nStyle = 0;
		fMin = 0;
		fMax = 0;
		fFoot = 0;
		ZeroMemory(cUnit,sizeof(cUnit));
		ZeroMemory(cCode0,sizeof(cCode0));
		ZeroMemory(cCode1,sizeof(cCode1));
	}
	long  nPointNo;
	float fVal;
	char  cName[64];
	long  nStyle;//0:float,1:BOOL;2:int
	float fMin;
	float fMax;
	float fFoot;
	char  cUnit[32];
	char  cCode0[32];
	char  cCode1[32];
};
//����ѹ�巵����Ϣ
struct stBackRYBCmd
{
	long  nPointNo;
	long  nVal;//0:tui ,1:tou
	char  cName[64];
};

//д��ֵ/��ѹ�弰������Ϣ
struct stSetDZRYBCmd
{
	int   nZZID;
	long  nPointNo;
	float fVal;
};

#endif