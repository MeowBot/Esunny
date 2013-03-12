#include "StdAfx.h"
#include "QuotApi.h"
#include "EsunnyMsgQueue.h"

#include <iostream>
using namespace std;

CQuotApi::CQuotApi(void)
{
	m_msgQueue = NULL;
	m_status = E_uninit;
}

CQuotApi::~CQuotApi(void)
{
	Disconnect();
}

void CQuotApi::RegisterMsgQueue(CEsunnyMsgQueue* pMsgQueue)
{
	m_msgQueue = pMsgQueue;
}

void CQuotApi::Connect(
		const string& szIP,
		int nPort,
		const string& szInvestorId,
		const string& szPassword)
{
	m_szIP = szIP;
	m_nPort = nPort;
	m_szInvestorId = szInvestorId;
	m_szPassword = szPassword;

	m_pApi = CreateEsunnyQuotClient(this);

	m_status = E_inited;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,0,"",m_status);
	
	if (m_pApi)
	{
		int i = 0;
		do
		{
			++i;
			char buf[1024] = {0};
			sprintf(buf,"���ڳ��Ե� %d ������......",i);

			m_status = E_connecting;
			if(m_msgQueue)
				m_msgQueue->Input_OnConnect(this,NULL,buf,m_status);

			int nRet = m_pApi->Connect(m_szIP.c_str(),m_nPort);
			if(0 == nRet)
			{
				Login();
				return;
			}
			
		} while (i<5);
		
		m_status = E_connecting;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,NULL,"���Ӷ��ʧ��",m_status);
	}
}

void CQuotApi::Login()
{
	if (NULL == m_pApi)
		return;
	
	int nRet = m_pApi->Login(m_szInvestorId.c_str(),m_szPassword.c_str());
	m_status = E_logining;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,"",m_status);
}

void CQuotApi::Disconnect()
{
	m_status = E_unconnected;
	if(m_pApi)
	{
		m_pApi->DisConnect();
		DelEsunnyQuotClient(m_pApi);
		m_pApi = NULL;

		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,NULL,"",m_status);
	}
}

int CQuotApi::RequestHistory(const char *market,const char *stk,int period)
{
	if(m_pApi)
	{
		return m_pApi->RequestHistory(market,stk,period);
	}
	return -2;
}

int CQuotApi::RequestTrace(const char *market,const char *stk,const char *date)
{
	if(m_pApi)
	{
		return m_pApi->RequestTrace(market,stk,date);
	}
	return -2;
}

int CQuotApi::OnRspLogin(int err,const char *errtext)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,err,errtext,err==0?E_logined:E_connected);
	return 0;
}

int CQuotApi::OnChannelLost(int err,const char *errtext)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnDisconnect(this,err,errtext,E_unconnected);
	return 0;
}

int CQuotApi::OnStkQuot(struct STKDATA *pData)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnStkQuot(this,pData);
	return 0;
}

int CQuotApi::OnRspHistoryQuot(struct STKHISDATA *pHisData)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspHistoryQuot(this,pHisData);
	return 0;
}

int CQuotApi::OnRspTraceData(struct STKTRACEDATA *pTraceData)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspTraceData(this,pTraceData);
	return 0;
}

int CQuotApi::OnRspMarketInfo(struct MarketInfo *pMarketInfo,int bLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspMarketInfo(this,pMarketInfo,bLast);
	return 0;
}

//void CQuotApi::OnFrontConnected()
//{
//	m_status =  E_connected;
//	if(m_msgQueue)
//		m_msgQueue->Input_OnConnect(this,NULL,m_status);
//
//	//���ӳɹ����Զ������¼
//	ReqUserLogin();
//}
//
//void CMdUserApi::OnFrontDisconnected(int nReason)
//{
//	m_status = E_unconnected;
//	CThostFtdcRspInfoField RspInfo;
//	//����ʧ�ܷ��ص���Ϣ��ƴ�Ӷ��ɣ���Ҫ��Ϊ��ͳһ���
//	RspInfo.ErrorID = nReason;
//	GetOnFrontDisconnectedMsg(&RspInfo);
//	
//	if(m_msgQueue)
//		m_msgQueue->Input_OnDisconnect(this,&RspInfo,m_status);
//}
//
//void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	if (!IsErrorRspInfo(pRspInfo)
//		&&pRspUserLogin)
//	{
//		m_status = E_logined;
//		if(m_msgQueue)
//			m_msgQueue->Input_OnConnect(this,pRspUserLogin,m_status);
//		
//		//�п��ܶ����ˣ������Ƕ������������¶���
//		set<string> mapOld = m_setInstrumentIDs;//�����ϴζ��ĵĺ�Լ
//		//Unsubscribe(mapOld);//�����Ѿ������ˣ�û�б�Ҫ��ȡ������
//		Subscribe(mapOld);//����
//	}
//	else
//	{
//		m_status = E_authed;
//		if(m_msgQueue)
//			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
//	}
//}
//
//void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	if(m_msgQueue)
//		m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
//}
//
//void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	//��ģ��ƽ̨��������������ᴥ��������Ҫ�Լ�ά��һ���Ѿ����ĵĺ�Լ�б�
//	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
//		&&pSpecificInstrument)
//	{
//		m_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
//	}
//}
//
//void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//	//ģ��ƽ̨��������������ᴥ��
//	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
//		&&pSpecificInstrument)
//	{
//		m_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
//	}
//}
//
////����ص����ñ�֤�˺������췵��
//void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
//{
//	if(m_msgQueue)
//		m_msgQueue->Input_OnRtnDepthMarketData(this,pDepthMarketData);
//}