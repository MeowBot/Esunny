#pragma once

#include "QuantBox.C2Esunny.h"
#include "include\LockFreeQ.h"

class CEsunnyMsgQueue
{
	//��Ӧ�����п��ܳ��ֵ���Ϣ���ͣ�����ĸ����
	enum EnumMsgType
	{
		E_fnOnConnect,	
		E_fnOnDisconnect,
		E_fnOnRspHistoryQuot,
		E_fnOnRspTraceData,
		E_fnOnRspMarketInfo,
		E_fnOnStkQuot,
	};

	struct SMsgItem
	{
		EnumMsgType			type;			//��Ϣ����
		void*				pApi;			//ָ��������ָ��	
		int					err;
		void*				pBuffer;		//ָ��Buf��ָ��	
		char				buffer[1];		//buf�����
	};

public:
	CEsunnyMsgQueue(void)
	{
		m_nSleep = 1;
		m_hThread = NULL;
		m_bRunning = false;

		//�ص�������ַָ��
		m_fnOnConnect = NULL;
		m_fnOnDisconnect = NULL;
		m_fnOnStkQuot = NULL;
		m_fnOnRspHistoryQuot = NULL;
		m_fnOnRspTraceData = NULL;
		m_fnOnRspMarketInfo = NULL;
	}
	virtual ~CEsunnyMsgQueue(void)
	{
		StopThread();
		Clear();
	}

public:
	//��ն���
	void Clear();

	//�������ⲿ����˳������д����ص�����
	bool Process();

	//���ڲ������̣߳��ڲ���������Process�����ص�
	void StartThread();
	void StopThread();

	//���ⲿ�ĺ�����ַע�ᵽ����(����ĸ����)
	void RegisterCallback_OnConnect(fnOnConnectionStatus pCallback){m_fnOnConnect = pCallback;}
	void RegisterCallback_OnDisconnect(fnOnConnectionStatus pCallback){m_fnOnDisconnect = pCallback;}
	void RegisterCallback(fnOnRspHistoryQuot pCallback){m_fnOnRspHistoryQuot = pCallback;}
	void RegisterCallback(fnOnRspMarketInfo pCallback){m_fnOnRspMarketInfo = pCallback;}
	void RegisterCallback(fnOnRspTraceData pCallback){m_fnOnRspTraceData = pCallback;}
	void RegisterCallback(fnOnStkQuot pCallback){m_fnOnStkQuot = pCallback;}

	//��Ӧ�������������(����ĸ����)
	void Input_OnConnect(void* pQuotApi,int err,const char *errtext,ConnectionStatus result);
	void Input_OnDisconnect(void* pQuotApi,int err,const char *errtext,ConnectionStatus step);
	void Input_OnRspHistoryQuot(void* pQuotApi,STKHISDATA *pHisData);
	void Input_OnRspMarketInfo(void* pQuotApi,MarketInfo *pMarketInfo,int bLast);
	void Input_OnRspTraceData(void* pQuotApi,STKTRACEDATA *pTraceData);
	void Input_OnStkQuot(void* pQuotApi,STKDATA *pData);
private:
	friend DWORD WINAPI ProcessThread(LPVOID lpParam);
	void RunInThread();

	//��Ӧ���ֱ�������
	void _Input(SMsgItem* pMsgItem);
	//�����е���Ϣ�жϷַ�
	void _Output(SMsgItem* pMsgItem);

	//��Ӧ���
	void Output_OnConnect(SMsgItem* pItem)
	{
		if(m_fnOnConnect)
			(*m_fnOnConnect)(pItem->pApi,pItem->err,pItem->buffer,(ConnectionStatus)(int)pItem->pBuffer);
	}
	void Output_OnDisconnect(SMsgItem* pItem)
	{
		if(m_fnOnConnect)
			(*m_fnOnConnect)(pItem->pApi,pItem->err,pItem->buffer,(ConnectionStatus)(int)pItem->pBuffer);
	}
	void Output_OnRspHistoryQuot(SMsgItem* pItem)
	{
		if(m_fnOnRspHistoryQuot)
			(*m_fnOnRspHistoryQuot)(pItem->pApi,pItem->pBuffer,(STKHISDATA *)pItem->buffer);
	}
	void Output_OnRspMarketInfo(SMsgItem* pItem)
	{
		if(m_fnOnRspMarketInfo)
			(*m_fnOnRspMarketInfo)(pItem->pApi,pItem->pBuffer,(MarketInfo *)pItem->buffer,pItem->err);
	}
	void Output_OnRspTraceData(SMsgItem* pItem)
	{
		if(m_fnOnRspTraceData)
			(*m_fnOnRspTraceData)(pItem->pApi,pItem->pBuffer,(STKTRACEDATA *)pItem->buffer);
	}
	void Output_OnStkQuot(SMsgItem* pItem)
	{
		if(m_fnOnStkQuot)
			(*m_fnOnStkQuot)(pItem->pApi,pItem->pBuffer,(STKDATA *)pItem->buffer);
	}

private:
	int							m_nSleep;
	volatile bool				m_bRunning;
	HANDLE						m_hThread;

	MSQueue<SMsgItem*>			m_queue;			//��Ӧ����

	//�ص�����ָ�루����ĸ����
	fnOnConnectionStatus		m_fnOnConnect;
	fnOnConnectionStatus		m_fnOnDisconnect;
	fnOnRspHistoryQuot			m_fnOnRspHistoryQuot;
	fnOnRspMarketInfo			m_fnOnRspMarketInfo;
	fnOnRspTraceData			m_fnOnRspTraceData;
	fnOnStkQuot					m_fnOnStkQuot;
};

