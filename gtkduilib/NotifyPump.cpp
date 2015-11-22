#include "stdafx.h"
#include "NotifyPump.h"

CNotifyPump::CNotifyPump(void)
{
}

CNotifyPump::~CNotifyPump(void)
{
}

DUI_BASE_BEGIN_MESSAGE_MAP(CNotifyPump)
DUI_END_MESSAGE_MAP()

static const DUI_MSGMAP_ENTRY* DuiFindMessageEntry(const DUI_MSGMAP_ENTRY* lpEntry,TNotifyUI& msg )
{
    UINT MsgType = msg.Type;
    CUIString strCtrlName = msg.pSender->GetName();
    const DUI_MSGMAP_ENTRY* pMsgTypeEntry = NULL;
    while (lpEntry->nSig != DuiSig_end){
        if(lpEntry->MsgType == MsgType){
            if(!lpEntry->strCtrlName.IsEmpty()){
                if(lpEntry->strCtrlName == strCtrlName){
                    return lpEntry;
                }
            }else{
                pMsgTypeEntry = lpEntry;
            }
        }
        lpEntry++;
    }
    return pMsgTypeEntry;
}

bool CNotifyPump::AddVirtualWnd(CUIString strName, CNotifyPump* pObject)
{
    if(m_VirtualWndMap.Find(strName) == NULL){
        m_VirtualWndMap.Insert(strName.GetData(),(LPVOID)pObject);
        return true;
    }
    return false;
}

bool CNotifyPump::RemoveVirtualWnd(CUIString strName)
{
    if(m_VirtualWndMap.Find(strName) != NULL){
        m_VirtualWndMap.Remove(strName);
        return true;
    }
    return false;
}

bool CNotifyPump::LoopDispatch(TNotifyUI& msg)
{
    const DUI_MSGMAP_ENTRY* lpEntry = NULL;
    const DUI_MSGMAP* pMessageMap = NULL;

#ifndef UILIB_STATIC
    for(pMessageMap = GetMessageMap(); pMessageMap!=NULL; pMessageMap = (*pMessageMap->pfnGetBaseMap)())
#else
    for(pMessageMap = GetMessageMap(); pMessageMap!=NULL; pMessageMap = pMessageMap->pBaseMap)
#endif
    {
#ifndef UILIB_STATIC
        assert(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
#else
        ASSERT(pMessageMap != pMessageMap->pBaseMap);
#endif
        if ((lpEntry = DuiFindMessageEntry(pMessageMap->lpEntries,msg)) != NULL)
        {
            goto LDispatch;
        }
    }
    return false;

LDispatch:
    union DuiMessageMapFunctions mmf;
    mmf.pfn = lpEntry->pfn;

    bool bRet = false;
    int nSig;
    nSig = lpEntry->nSig;
    switch (nSig)
    {
    default:
        assert(FALSE);
        break;
    case DuiSig_lwl:
        (this->*mmf.pfn_Notify_lwl)(msg.wParam,msg.lParam);
        bRet = true;
        break;
    case DuiSig_vn:
        (this->*mmf.pfn_Notify_vn)(msg);
        bRet = true;
        break;
    }
    return bRet;
}

void CNotifyPump::NotifyPump(TNotifyUI& msg)
{
    //
    //遍历虚拟窗口
    //

    /*if( !msg.sVirtualWnd.IsEmpty() ){
        for( int i = 0; i< m_VirtualWndMap.GetSize(); i++ ) {
            if( LPCTSTR key = m_VirtualWndMap.GetAt(i) ) {
                if( _tcsicmp(key, msg.sVirtualWnd.GetData()) == 0 ){
                    CNotifyPump* pObject = static_cast<CNotifyPump*>(m_VirtualWndMap.Find(key, false));
                    if( pObject && pObject->LoopDispatch(msg) )
                        return;
                }
            }
        }
    }*/

    //
    // 遍历主窗口
    //

    LoopDispatch(msg);
}