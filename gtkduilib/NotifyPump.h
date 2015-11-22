#ifndef __NOTIFYPUMP__H__
#define __NOTIFYPUMP__H__

class CNotifyPump
{
public:
    CNotifyPump(void);
    virtual ~CNotifyPump(void);
public:
    bool AddVirtualWnd(CUIString strName, CNotifyPump* pObject);
    bool RemoveVirtualWnd(CUIString strName);
    void NotifyPump(TNotifyUI& msg);
    bool LoopDispatch(TNotifyUI& msg);
    DUI_DECLARE_MESSAGE_MAP()
private:
    CStdStringPtrMap m_VirtualWndMap;
};

#endif

