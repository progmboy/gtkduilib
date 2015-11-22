#include "stdafx.h"
#include "WindowImpBase.h"

DUI_BEGIN_MESSAGE_MAP(CWindowImpBase, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, CWindowImpBase::OnClick)
DUI_END_MESSAGE_MAP()

CWindowImpBase::CWindowImpBase(void)/*:CUIWindow()*/
{
}

CWindowImpBase::~CWindowImpBase(void)
{
}

//void CWindowImpBase::OnFinalMessage(HWND hWnd)
//{
    //m_PaintManager.RemovePreMessageFilter(this);
    //m_PaintManager.RemoveNotifier(this);
    //m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
//}

LRESULT CWindowImpBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == (WPARAM)VK_RETURN)
    {
        return FALSE;
    }
    else if (wParam == (WPARAM)VK_ESCAPE)
    {
        //Close();
        return TRUE;
    }

    return FALSE;
}

UINT CWindowImpBase::GetClassStyle() const
{
    return CS_DBLCLKS;
}

UILIB_RESOURCETYPE CWindowImpBase::GetResourceType() const
{
    return UILIB_FILE;
}

CUIString CWindowImpBase::GetZIPFileName() const
{
    return L"";
}

CUIControl* CWindowImpBase::CreateControl(const CUIString& strClass)
{
    return NULL;
}

bool CWindowImpBase::OnClose(int Msg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

void CWindowImpBase::OnDestroy(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    
}


bool CWindowImpBase::OnGetMinMaxInfo(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CWindowImpBase::OnMouseWheel(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool CWindowImpBase::OnMouseHover(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CWindowImpBase::OnSize(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CWindowImpBase::OnChar(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CWindowImpBase::OnSysCommand(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

bool CWindowImpBase::OnCreate(int uMsg, WPARAM wParam, LPARAM lParam)
{
#if 0
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    RECT rcClient;
    ::GetClientRect(*this, &rcClient);
    ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
        rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddPreMessageFilter(this);

    CDialogBuilder builder;
    if (m_PaintManager.GetResourcePath().IsEmpty())
    {	// 允许更灵活的资源路径定义
        CDuiString strResourcePath=m_PaintManager.GetInstancePath();
        strResourcePath+=GetSkinFolder().GetData();
        m_PaintManager.SetResourcePath(strResourcePath.GetData());
    }

    switch(GetResourceType())
    {
    case UILIB_ZIP:
        m_PaintManager.SetResourceZip(GetZIPFileName().GetData(), true);
        break;
    case UILIB_ZIPRESOURCE:
        {
            HRSRC hResource = ::FindResource(m_PaintManager.GetResourceDll(), GetResourceID(), _T("ZIPRES"));
            if( hResource == NULL )
                return 0L;
            DWORD dwSize = 0;
            HGLOBAL hGlobal = ::LoadResource(m_PaintManager.GetResourceDll(), hResource);
            if( hGlobal == NULL ) 
            {
#if defined(WIN32) && !defined(UNDER_CE)
                ::FreeResource(hResource);
#endif
                return 0L;
            }
            dwSize = ::SizeofResource(m_PaintManager.GetResourceDll(), hResource);
            if( dwSize == 0 )
                return 0L;
            m_lpResourceZIPBuffer = new BYTE[ dwSize ];
            if (m_lpResourceZIPBuffer != NULL)
            {
                ::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
            }
#if defined(WIN32) && !defined(UNDER_CE)
            ::FreeResource(hResource);
#endif
            m_PaintManager.SetResourceZip(m_lpResourceZIPBuffer, dwSize);
        }
        break;
    }

    CControlUI* pRoot=NULL;
    if (GetResourceType()==UILIB_RESOURCE)
    {
        STRINGorID xml(_ttoi(GetSkinFile().GetData()));
        pRoot = builder.Create(xml, _T("xml"), this, &m_PaintManager);
    }
    else
        pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
    ASSERT(pRoot);
    if (pRoot==NULL)
    {
        MessageBox(NULL,_T("加载资源文件失败"),_T("Duilib"),MB_OK|MB_ICONERROR);
        ExitProcess(1);
        return 0;
    }
    m_PaintManager.AttachDialog(pRoot);
    m_PaintManager.AddNotifier(this);
#endif
    
    //
    // set window has no default caption area
    //
    
    GtkWidget* widget = GetWidget();
    gtk_window_set_decorated(GTK_WINDOW(widget), FALSE); 
    
    
    //
    // once remove caption area we should handled the mouse 
    // drag/double click event to response the window draging 
    // max/min request
    //
    
    InitWindow();
    AddNotifier(this);
    return 0;
}

bool CWindowImpBase::OnKeyDown(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool CWindowImpBase::OnKillFocus(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool CWindowImpBase::OnSetFocus(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool CWindowImpBase::OnLButtonDown(int Msg, WPARAM wParam, LPARAM /*lParam*/)
{
    GdkEventButton *Event = (GdkEventButton*)wParam;
    CUIRect rcCaption;

    rcCaption = GetCaptionRect();
    if (Event->y < rcCaption.bottom){
        
        //
        // 判断是否在button类的控件内,如果在这里面则不进行拖拽动作
        //
        
        
        POINT pt = {(LONG)Event->x, (LONG)Event->y};
        CUIControl *pControl = FindControl(pt);
        if (pControl && pControl->GetInterface(L"Button") != NULL){
            return false;
        }else{
            gtk_window_begin_move_drag(GTK_WINDOW(GetWidget()), Event->button, 
                (gint)Event->x_root, (gint)Event->y_root, Event->time);
        }
    }
    return false;
}

bool CWindowImpBase::OnLButtonUp(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool CWindowImpBase::OnMouseMove(int Msg, WPARAM wParam, LPARAM /*lParam*/)
{
    return 0;
}

bool CWindowImpBase::MessageHandler(int Msg, WPARAM wParam, LPARAM lParam)
{
    bool bHandled = false;
    switch (Msg)
    {
    case DUI_WM_CREATE:			
        bHandled = OnCreate(Msg, wParam, lParam); 
        break;
    case DUI_WM_CLOSE:
        bHandled = OnClose(Msg, wParam, lParam);
        break;
    case DUI_WM_DESTROY:
        OnDestroy(Msg, wParam, lParam);
        break;
    case DUI_WM_MOUSEPRESS:
        {
            GdkEventButton *Event = (GdkEventButton*)wParam;
            if (Event->type == GDK_BUTTON_PRESS){
                if (Event->button == 1){
                    bHandled = OnLButtonDown(Msg, wParam, lParam);
                }
            }
        }
        break;
    case DUI_WM_MOUSERELEASE:
        {
            GdkEventButton *Event = (GdkEventButton*)wParam;
            if (Event->button == 1){
                bHandled = OnLButtonUp(Msg, wParam, lParam);
            }
        }
        break;
    default:
        bHandled = false; 
        break;
    }
    if (bHandled) 
        return true;

    bHandled = HandleCustomMessage(Msg, wParam, lParam);
    if (bHandled) 
        return true;

    return CUIWindow::MessageHandler(Msg, wParam, lParam);
}

bool CWindowImpBase::HandleCustomMessage(int uMsg, WPARAM wParam, LPARAM lParam)
{
    return false;
}

LONG CWindowImpBase::GetStyle()
{
    return 0;
}

void CWindowImpBase::OnClick(TNotifyUI& msg)
{
    CUIString sCtrlName = msg.pSender->GetName();
    if(sCtrlName == L"closebtn")
    {
        Close();
        return; 
    }
    else if( sCtrlName == L"minbtn")
    { 
        Minimize();
        return; 
    }
    else if( sCtrlName == L"maxbtn")
    { 
        Maximize();
        return; 
    }
    else if(sCtrlName == L"restorebtn")
    { 
        Restore();
        return; 
    }
    return;
}

void CWindowImpBase::Notify(TNotifyUI& msg)
{
    return CNotifyPump::NotifyPump(msg);
}

