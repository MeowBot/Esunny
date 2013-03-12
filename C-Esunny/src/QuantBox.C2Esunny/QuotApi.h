#pragma once

#include "QuantBox.C2Esunny.h"
#include "include\EsunnyQuotAPI\EsunnyQuot.h"

#include <set>
#include <string>

using namespace std;

class CEsunnyMsgQueue;

class CQuotApi :
	public IEsunnyQuotNotify
{
public:
	CQuotApi(void);
	virtual ~CQuotApi(void);

	void RegisterMsgQueue(CEsunnyMsgQueue* pMsgQueue);

	void Connect(
		const string& szIP,
		int szPort,
		const string& szInvestorId,
		const string& szPassword);
	void Disconnect();

	int RequestHistory(const char *market,const char *stk,int period);
	int RequestTrace(const char *market,const char *stk,const char *date);

private:
	////��������
	//void Subscribe(const set<string>& instrumentIDs);
	//��¼����
	void Login();


	int __cdecl OnRspLogin(int err,const char *errtext);
	int __cdecl OnChannelLost(int err,const char *errtext);
	int __cdecl OnStkQuot(struct STKDATA *pData);
	int __cdecl OnRspHistoryQuot(struct STKHISDATA *pHisData);
	int __cdecl OnRspTraceData(struct STKTRACEDATA *pTraceData);
	int __cdecl OnRspMarketInfo(struct MarketInfo *pMarketInfo,int bLast);

private:
	ConnectionStatus			m_status;				//����״̬
	
	set<string>					m_setInstrumentIDs;		//���ڶ��ĵĺ�Լ
	IEsunnyQuotClient*			m_pApi;					//����API
	CEsunnyMsgQueue*			m_msgQueue;				//��Ϣ����ָ��

	string						m_szIP;				//���������ļ���·��
	int							m_nPort;
	string						m_szInvestorId;			//Ͷ����ID
	string						m_szPassword;			//����
};

