// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� QUANTBOXC2ESUNNY_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// QUANTBOXC2ESUNNY_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef _QUANTBOXC2ESUNNY_H_
#define _QUANTBOXC2ESUNNY_H_

#ifdef QUANTBOXC2ESUNNY_EXPORTS
#define QUANTBOXC2ESUNNY_API __declspec(dllexport)
#else
#define QUANTBOXC2ESUNNY_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "include\EsunnyQuotAPI\EsunnyQuot.h"

//����״̬ö��
enum ConnectionStatus
{
	E_uninit,		//δ��ʼ��
	E_inited,		//�Ѿ���ʼ��
	E_unconnected,	//�����Ѿ��Ͽ�
	E_connecting,	//������
	E_connected,	//���ӳɹ�
	E_authing,		//��Ȩ��
	E_authed,		//��Ȩ�ɹ�
	E_logining,		//��¼��
	E_logined,		//��¼�ɹ�
	E_confirming,	//���㵥ȷ����
	E_confirmed,	//�Ѿ�ȷ��
	E_conn_max		//���ֵ
};

enum MarketType
{
	CZCE = 1,
	DCE = 2,
	SHFE = 3,
	CFFEX = 5,
	MAX_MARKET_TYPE,
};

enum PeriodType
{
	min1 = 1,
	min5 = 2,
	min60 = 3,
	daily = 4,
	MAX_PERIOD_TYPE
};

//�ص��������Ͷ��壨Ϊ��д���㣬����ĸ����
typedef void (__stdcall *fnOnConnectionStatus)(void* pApi,int err,const char *errtext,ConnectionStatus result);//���Ӻ�Ľ��״̬
typedef void (__stdcall *fnOnRspHistoryQuot)(void* pQuotApi,void* pBuffer,STKHISDATA *pHisData);
typedef void (__stdcall *fnOnRspMarketInfo)(void* pQuotApi,void* pBuffer,MarketInfo *pMarketInfo,int bLast);
typedef void (__stdcall *fnOnRspTraceData)(void* pQuotApi,void* pBuffer,STKTRACEDATA *pTraceData);
typedef void (__stdcall *fnOnStkQuot)(void* pQuotApi,void* pBuffer,STKDATA *pData);
//����������Ϣ���У�֧����Ӧ����ͽ���
QUANTBOXC2ESUNNY_API MarketType __stdcall ES_ConvetString2MarketType(const char *market);
QUANTBOXC2ESUNNY_API const char * __stdcall ES_ConvetMarketType2String(MarketType market);

QUANTBOXC2ESUNNY_API void* __stdcall ES_CreateMsgQueue();

//����Ϣ����ע��ص�����
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnConnect(void* pMsgQueue,fnOnConnectionStatus pCallback);
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnDisconnect(void* pMsgQueue,fnOnConnectionStatus pCallback);
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnRspHistoryQuot(void* pMsgQueue,fnOnRspHistoryQuot pCallback);
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnRspMarketInfo(void* pMsgQueue,fnOnRspMarketInfo pCallback);
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnRspTraceData(void* pMsgQueue,fnOnRspTraceData pCallback);
QUANTBOXC2ESUNNY_API void __stdcall ES_RegOnStkQuot(void* pMsgQueue,fnOnStkQuot pCallback);

//�ͷ���Ϣ����
QUANTBOXC2ESUNNY_API void __stdcall ES_ReleaseMsgQueue(void* pMsgQueue);
//�����Ϣ��������
QUANTBOXC2ESUNNY_API void __stdcall ES_ClearMsgQueue(void* pMsgQueue);

//������Ϣ���е�һ����¼
QUANTBOXC2ESUNNY_API bool __stdcall ES_ProcessMsgQueue(void* pMsgQueue);
//�������лص����������߳�
QUANTBOXC2ESUNNY_API void __stdcall ES_StartMsgQueue(void* pMsgQueue);
//ֹͣ���лص����������߳�
QUANTBOXC2ESUNNY_API void __stdcall ES_StopMsgQueue(void* pMsgQueue);

//����ӿ�=======================================

//��������ӿ�
QUANTBOXC2ESUNNY_API void* __stdcall QT_CreateQuotApi();
//����Ϣ����ע�ᵽ����ӿ���
QUANTBOXC2ESUNNY_API void __stdcall QT_RegMsgQueue2QuotApi(void* pQuotApi,void* pMsgQueue);
//����
QUANTBOXC2ESUNNY_API void __stdcall QT_Connect(
	void* pQuotApi,
	const char* szIP,
	int nPort,
	const char* szInvestorId,
	const char* szPassword);

//���ĺ�Լ
QUANTBOXC2ESUNNY_API void __stdcall QT_RequestQuot(void* pQuotApi,const char * market,const char *stk,int need);
QUANTBOXC2ESUNNY_API int __stdcall QT_RequestHistory(void* pQuotApi,const char * market,const char *stk,PeriodType period);
QUANTBOXC2ESUNNY_API int __stdcall QT_RequestTrace(void* pQuotApi,const char * market,const char *stk,const char *date);
QUANTBOXC2ESUNNY_API void __stdcall QT_AddReqStk(void* pQuotApi,const char * market,const char *stk,int need);
QUANTBOXC2ESUNNY_API void __stdcall QT_SendReqStk(void* pQuotApi);
QUANTBOXC2ESUNNY_API void __stdcall QT_GetStkData(void* pQuotApi,const char * market,const char *stk,struct STKDATA *pData);
//�Ͽ�����
QUANTBOXC2ESUNNY_API void __stdcall QT_Disconnect(void* pQuotApi);
//�ͷ�����ӿ�
QUANTBOXC2ESUNNY_API void __stdcall QT_ReleaseQuotApi(void* pQuotApi);

#ifdef __cplusplus
}
#endif


#endif//end of _QUANTBOXC2ESUNNY_H_