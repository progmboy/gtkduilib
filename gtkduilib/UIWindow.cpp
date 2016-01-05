#include "stdafx.h"
#include "UIWindow.h"

gboolean wrap_draw(GtkWidget *widget, cairo_t *cr, CUIWindow *pWindow)
{
    assert(pWindow);

    //
    // return true will stop call the old handler.
    //

    g_print ("draw event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_PAINT, (LPARAM)cr, NULL);
}

void wrap_size(GtkWidget *widget, GdkRectangle *allocation,
                CUIWindow *pWindow)
{

    //
    // this event is emitted when window size if changed
    //

    //g_print ("size event\n");
    SIZE Size = {allocation->width, allocation->height};
    pWindow->MessageHandler(DUI_WM_SIZE, (WPARAM)&Size, NULL);
}

gboolean wrap_motion_notify(GtkWidget *widget, GdkEventMotion  *event, CUIWindow *pWindow)
{
    //
    // this even is emitted when mouse move,
    // just like WM_MOUSEMOVE message
    // note:
    // in GTK the event handler return true will stop call the old handler
    //

    //g_print ("motion_notify event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSEMOVE, (WPARAM)event, NULL);
}

gboolean wrap_button_press(GtkWidget *widget, GdkEventButton *Event, CUIWindow *pWindow)
{

    //
    // this event is send when mouse button is pressed
    // In window this event is combine with
    // WM_LBUTTONDOWN WM_RBUTTONDOWN WM_RBUTTON WM_MBUTTONDOWN
    // WM_LBUTTONDBLCLK WM_RBUTTONDBLCLK WM_MBUTTONDBLCLK
    //


    if (Event->type == GDK_BUTTON_PRESS){
        g_print ("WM_BUTTONDOWN event %d button\n", Event->button);
    }else if (Event->type == GDK_BUTTON_RELEASE){
        g_print ("WM_BUTTONUP event %d button\n", Event->button);
    }else if (Event->type == GDK_DOUBLE_BUTTON_PRESS){
        g_print ("WM_BUTTONDBLCLK event %d button\n", Event->button);
    }



    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSEPRESS, (WPARAM)Event, NULL);
}

gboolean wrap_button_release(GtkWidget *widget, GdkEventButton *event, CUIWindow *pWindow)
{

    //
    // this event is send when mouse button is released
    // In window this event is combine with
    // WM_LBUTTONDOWN WM_RBUTTONDOWN WM_RBUTTON WM_MBUTTONDOWN
    // WM_LBUTTONDBLCLK WM_RBUTTONDBLCLK WM_MBUTTONDBLCLK
    //

    g_print ("button_release event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSERELEASE, (WPARAM)event, NULL);
}

gboolean wrap_enter_notify(GtkWidget *widget, GdkEventCrossing *event, CUIWindow *pWindow)
{

    //
    // this event is send when mouse enter the window
    //

    //g_print ("enter_notify event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSEENTER, (WPARAM)event, NULL);
}

gboolean wrap_leave_notify(GtkWidget *widget, GdkEventCrossing *event, CUIWindow *pWindow)
{

    //
    // this event is send when mouse leave the window
    // In window this event is like WM_MOUSELEAVE
    //

    //g_print ("leave_notify event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSELEAVE, (WPARAM)event, NULL);
}

gboolean wrap_key_press(GtkWidget *widget, GdkEvent *event, CUIWindow *pWindow)
{

    //
    // this event is send when key press
    // In window this event is like WM_KEYDOWN
    //

    //g_print ("key_press event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_KEYPRESS, (WPARAM)event, NULL);
}

gboolean wrap_key_release(GtkWidget *widget, GdkEvent *Event, CUIWindow *pWindow)
{

    //
    // this event is send when key press
    // In window this event is like WM_KEYDOWN
    //

    //g_print ("key_release event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_KEYPRELEASE, (WPARAM)Event, NULL);
}

gboolean wrap_timer_event(gpointer userdata)
{
    TIMERINFO *pTimer = (TIMERINFO *)userdata;
    g_print ("timer event\n");
    pTimer->pWindow->MessageHandler(DUI_WM_TIMER, (WPARAM)pTimer->pSender,
        (LPARAM)pTimer->uTimerId);
    return true;
}

gboolean wrap_delete_event( GtkWidget *widget, GdkEvent  *Event, CUIWindow *pWindow)
{
    //
    // this event is send when window close
    // In window this event is like WM_CLOSE
    //

    g_print ("delete event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_CLOSE, (WPARAM)Event, NULL);
}

gboolean wrap_scroll_event (GtkWidget *widget, GdkEventScroll *Event, CUIWindow *pWindow)
{
    //g_print("catch scroll event\n");
    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSEWHEEL, (WPARAM)Event, NULL);
}

//gboolean wrap_destroy_event (GtkWidget *widget, GdkEvent *Event, CUIWindow *pWindow)
//{
//    return (gboolean)pWindow->MessageHandler(DUI_WM_MOUSEWHEEL, (WPARAM)Event, NULL);
//}

void wrap_destroy(GtkWidget *widget, CUIWindow *pWindow)
{
    pWindow->MessageHandler(DUI_WM_DESTROY, NULL, NULL);
}

CUIWindow::CUIWindow(void):m_Widget(NULL),m_uTimerID(0), m_bNeedUpdate(false), m_pEventClick(NULL),
m_pEventHover(NULL), m_bWindowShowed(false), m_crPaintContext(NULL)
{

}

CUIWindow::~CUIWindow(void)
{
    if (m_crPaintContext){
        cairo_destroy(m_crPaintContext);
    }
}

// void CUIWindow::Init()
// {
//     gtk_init(NULL, NULL);
// }

void CUIWindow::CenterWindow()
{
    if (GTK_IS_WINDOW(m_Widget)){

        //
        // use GTK_WIN_POS_CENTER_ALWAYS, GTK_WIN_POS_CENTER do not work.
        //

        gtk_window_set_position(GTK_WINDOW(m_Widget), GTK_WIN_POS_CENTER_ALWAYS);

    }
}

UINT CUIWindow::MapKeyState(UINT State)
{
    UINT retState = 0;
    if(State | GDK_CONTROL_MASK)
        retState |= MK_CONTROL;
    if(State | GDK_BUTTON1_MASK)
        retState |= MK_LBUTTON;
    if(State | GDK_BUTTON3_MASK)
        retState |= MK_RBUTTON;
    if(State | GDK_SHIFT_MASK)
        retState |= MK_SHIFT;
    /*if(State | GDK_MOD1_MASK)
        retState |= MK_ALT;*/
    return retState;
}

bool CUIWindow::MessageHandler(int Msg, WPARAM wParam, LPARAM lParam)
{
    bool bHandled = true;
    switch(Msg)
    {
    case DUI_WM_CLOSE:
        bHandled = false;
        break;
    case DUI_WM_PAINT:
        {
            cairo_t *cr = (cairo_t*)wParam;

            if (!m_crPaintContext){
                GdkWindow *gdkWindow;
                gdkWindow = gtk_widget_get_window(m_Widget);
                if (gdkWindow){
                    m_crPaintContext = gdk_cairo_create(gdkWindow);
                }
            }

            //
            // first check is anyone want change the pos
            //

            if (m_bNeedUpdate){

                GtkAllocation Allocation;
                gtk_widget_get_allocation(m_Widget, &Allocation);
                if (Allocation.width <= 0 || Allocation.height <= 0){
                    g_critical("allocation is empty");
                }else{

                    //
                    // check is root control need update pos
                    //

                    CUIRect ClientRect(Allocation);
                    if (m_Root->IsUpdateNeeded()){
                        m_Root->SetPos(ClientRect);
                    }else{

                        //
                        // check if child control need update
                        //

                        CUIControl* pControl = NULL;
                        while(pControl = m_Root->FindControl(__FindControlFromUpdate,
                            NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST)){
                                pControl->SetPos(pControl->GetPos());
                        }
                    }

                    //
                    // generate windows init notify
                    //

                    static bool bInit = false;
                    if (!bInit){
                        bInit = true;
                        SendNotify(m_Root, DUI_MSGTYPE_WINDOWINIT,  0, 0, false);
                    }

                }
            }

            //
            // Is here has a rect for draw.
            //


            GdkRectangle Rect;
            if(gdk_cairo_get_clip_rectangle(cr, &Rect)){

                //
                // render control
                //

                CUIRect paintRect(Rect);
                m_Root->DoPaint(cr, paintRect);
            }
        }
        break;
    case DUI_WM_TIMER:
        {
            TEventUI SendEvent;
            ZeroMemory(&SendEvent, sizeof(SendEvent));
            SendEvent.Type = UIEVENT_TIMER;
            SendEvent.pSender = (CUIControl*)wParam;
            SendEvent.wParam = (LPARAM)lParam;
            SendEvent.pSender->DoEvent(SendEvent);
        }
        break;

    case DUI_WM_MOUSEPRESS:
        {
            GdkEventButton *Event = (GdkEventButton*)wParam;
            int Type;

            if (Event->type == GDK_BUTTON_PRESS){
                if (Event->button == 1){
                    Type = UIEVENT_BUTTONDOWN;
                }else if (Event->button == 3){
                    Type = UIEVENT_RBUTTONDOWN;
                }
            }else if (Event->type == GDK_2BUTTON_PRESS && Event->button == 1){
                Type = UIEVENT_DBLCLICK;
            }else{
                break;
            }

            POINT pt = {(LONG)Event->x, (LONG)Event->y};
            m_ptLastMousePos = pt;
            CUIControl *pControl = FindControl(pt);
            if (pControl){
                m_pEventClick = pControl;

                //
                // TODO: SetFocus
                //

                //pControl->SetFocus();
                TEventUI SendEvent;
                ZeroMemory(&SendEvent, sizeof(SendEvent));

                SendEvent.Type = Type;
                SendEvent.pSender = pControl;
                SendEvent.wParam = wParam;
                SendEvent.lParam = lParam;
                SendEvent.ptMouse = pt;
                SendEvent.dwTimestamp = Event->time;
                pControl->DoEvent(SendEvent);
            }
            bHandled = false;
        }
        break;
    case DUI_WM_MOUSERELEASE:
        {
            GdkEventButton *Event = (GdkEventButton*)wParam;
            POINT pt = {(LONG)Event->x, (LONG)Event->y};

            //
            // set last mouse position
            //

            m_ptLastMousePos = pt;

            if (Event->type == GDK_BUTTON_RELEASE && Event->button == 1){
                CUIControl *pControl = FindControl(pt);
                if (pControl){
                    m_pEventClick = pControl;

                    //
                    // TODO: SetFocus
                    //

                    //pControl->SetFocus();

                    TEventUI SendEvent;
                    ZeroMemory(&SendEvent, sizeof(SendEvent));

                    SendEvent.Type = UIEVENT_BUTTONUP;
                    SendEvent.pSender = pControl;
                    SendEvent.wParam = wParam;
                    SendEvent.lParam = lParam;
                    SendEvent.ptMouse = pt;
                    SendEvent.dwTimestamp = Event->time;
                    pControl->DoEvent(SendEvent);
                }
            }
            bHandled = false;
        }
        break;

    //
    // TODO:tooltip WM_MOUSEHOVER
    //

    case DUI_WM_MOUSEMOVE:
        {
            GdkEventMotion *Event = (GdkEventMotion *)wParam;
            POINT pt = {(LONG)Event->x, (LONG)Event->y};

            m_ptLastMousePos = pt;
            CUIControl* pNewHover = FindControl(pt);
            if(pNewHover == NULL)
                break;

            TEventUI SendEvent;
            ZeroMemory(&SendEvent, sizeof(SendEvent));
            SendEvent.ptMouse = pt;
            SendEvent.dwTimestamp = Event->time;

            if(pNewHover != m_pEventHover && m_pEventHover != NULL){
                SendEvent.Type = UIEVENT_MOUSELEAVE;
                SendEvent.pSender = m_pEventHover;
                m_pEventHover->DoEvent(SendEvent);
                m_pEventHover = NULL;
            }
            if(pNewHover != m_pEventHover && pNewHover != NULL){
                SendEvent.Type = UIEVENT_MOUSEENTER;
                SendEvent.pSender = pNewHover;
                pNewHover->DoEvent(SendEvent);
                m_pEventHover = pNewHover;
            }
            if(m_pEventClick != NULL){
                SendEvent.Type = UIEVENT_MOUSEMOVE;
                SendEvent.pSender = m_pEventClick;
                m_pEventClick->DoEvent(SendEvent);
            }else if(pNewHover != NULL){
                SendEvent.Type = UIEVENT_MOUSEMOVE;
                SendEvent.pSender = pNewHover;
                pNewHover->DoEvent(SendEvent);
            }
            bHandled = false;
        }
        break;
    case DUI_WM_MOUSELEAVE:
        {
            if(m_pEventHover){
                GdkEventCrossing *Event = (GdkEventCrossing *)wParam;
                TEventUI SendEvent;
                ZeroMemory(&SendEvent, sizeof(SendEvent));

                POINT pt = {(LONG)Event->x, (LONG)Event->y};
                SendEvent.ptMouse = pt;
                SendEvent.dwTimestamp = Event->time;
                SendEvent.Type = UIEVENT_MOUSELEAVE;
                SendEvent.pSender = m_pEventHover;
                m_pEventHover->DoEvent(SendEvent);
                m_pEventHover = NULL;
            }
            bHandled = false;
        }
        break;
    case DUI_WM_MOUSEWHEEL:
        {
            GdkEventScroll *Event = (GdkEventScroll *)wParam;
            POINT pt = {(LONG)Event->x, (LONG)Event->y};
            m_ptLastMousePos = pt;
            CUIControl* pControl = FindControl(pt);
            if(pControl){
                TEventUI SendEvent;
                ZeroMemory(&SendEvent, sizeof(SendEvent));

                SendEvent.Type = UIEVENT_SCROLLWHEEL;
                SendEvent.pSender = pControl;
                SendEvent.wParam = MAKELPARAM(Event->direction == GDK_SCROLL_DOWN ? SB_LINEDOWN : SB_LINEUP, 0);
                SendEvent.lParam = lParam;
                SendEvent.wKeyState = MapKeyState(Event->state);
                SendEvent.dwTimestamp = Event->time;
                pControl->DoEvent(SendEvent);
            }
            bHandled = false;
        }
        break;
    default:
        bHandled = false;
        break;
    }
    return bHandled;
}

void CUIWindow::Invalidate(const CUIRect &Rect)
{
    if (!m_bWindowShowed){
        return;
    }
    GdkWindow *gdkWindow;
    GdkRectangle gdkRect;
    gdkWindow = gtk_widget_get_window(m_Widget);
    gdkRect.x = Rect.left;
    gdkRect.y = Rect.top;
    gdkRect.width = Rect.GetWidth();
    gdkRect.height = Rect.GetHeight();
    gdk_window_invalidate_rect(gdkWindow, &gdkRect, FALSE);
}

void CUIWindow::Minimize()
{
    if (GTK_IS_WINDOW(m_Widget)){
        gtk_window_iconify(GTK_WINDOW(m_Widget));
    }
}

void CUIWindow::Show()
{
    gtk_widget_show(m_Widget);
    m_bWindowShowed = true;
}

void CUIWindow::MainLoop()
{
    gtk_main();
}

void CUIWindow::Close()
{
    if (m_Widget){

        //
        // we should emit a delete-event
        // since gtk 3.10. export a funtion gtk_window_close.
        // but unfortunately. in windows the last release version is 3.0x
        // but we can do it. this code copy from gtk's source code.
        //

        /*

        //
        // this code can be worked..umm..too ugly
        // official is always best. is it?
        //

        gboolean bHandled=FALSE;
        g_signal_emit_by_name(G_OBJECT(m_Widget), "delete-event", this, &bHandled);
        if (!bHandled){
            gtk_widget_hide(m_Widget);
            gtk_widget_destroy(m_Widget);
        }*/

        GdkEvent *Event;

        Event = gdk_event_new (GDK_DELETE);
        Event->any.window = GDK_WINDOW(g_object_ref(G_OBJECT(gtk_widget_get_window(m_Widget))));
        Event->any.send_event = TRUE;

        gtk_main_do_event (Event);
        gdk_event_free (Event);
    }
}

void CUIWindow::Restore()
{
    if (GTK_IS_WINDOW(m_Widget)){

        GdkWindow* gdkWindow;
        GdkWindowState State;

        gdkWindow = gtk_widget_get_window(m_Widget);
        State = gdk_window_get_state(gdkWindow);

        if (State == GDK_WINDOW_STATE_ICONIFIED){
            gtk_window_deiconify(GTK_WINDOW(m_Widget));
        }

        if (State == GDK_WINDOW_STATE_MAXIMIZED){
            gtk_window_unmaximize(GTK_WINDOW(m_Widget));
        }

        if (State == GDK_WINDOW_STATE_FULLSCREEN){
            gtk_window_unfullscreen(GTK_WINDOW(m_Widget));
        }
    }
}

void CUIWindow::Maximize()
{
    if (GTK_IS_WINDOW(m_Widget)){
        gtk_window_maximize(GTK_WINDOW(m_Widget));
    }
}

void CUIWindow::FullScreen()
{
    if (GTK_IS_WINDOW(m_Widget)){
        gtk_window_fullscreen(GTK_WINDOW(m_Widget));
    }
}

bool CUIWindow::IsMinimize()
{
    if (GTK_IS_WINDOW(m_Widget)){
        GdkWindow* gdkWindow;

        gdkWindow = gtk_widget_get_window(m_Widget);
        GdkWindowState State = gdk_window_get_state(gdkWindow);
        return State == GDK_WINDOW_STATE_ICONIFIED;
    }

    return false;
}

bool CUIWindow::IsMaximize()
{
    if (GTK_IS_WINDOW(m_Widget)){
        GdkWindow* gdkWindow;

        gdkWindow = gtk_widget_get_window(m_Widget);
        GdkWindowState State = gdk_window_get_state(gdkWindow);
        return State == GDK_WINDOW_STATE_MAXIMIZED;
    }

    return false;
}

bool CUIWindow::IsFullScreen()
{
    if (GTK_IS_WINDOW(m_Widget)){
        GdkWindow* gdkWindow;

        gdkWindow = gtk_widget_get_window(m_Widget);
        GdkWindowState State = gdk_window_get_state(gdkWindow);
        return State == GDK_WINDOW_STATE_FULLSCREEN;
    }

    return false;
}

void CUIWindow::NeedUpdate()
{
    m_bNeedUpdate = true;
}

bool CUIWindow::Create(int x, int y, int nWidth, int nHeight)
{
    bool bHandled;

    GtkWidget *Widget;

    //
    // create a new gtk window
    //

    Widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    assert(Widget);

    //
    // save handle.
    //

    m_Widget = Widget;

    //
    // set window need intend to handle 'draw' event
    //

    gtk_widget_set_app_paintable(Widget, FALSE);

    //
    // set window size.
    // gtk_widget_set_size_request is set the minimum size of the windows
    // Setting the size request will force them to leave the window at least as large as the size request
    //

    gtk_window_set_default_size(GTK_WINDOW(Widget), nWidth, nHeight);

    //
    // set window position. 0x80000000 for default.
    //

    if (x != (int)0x80000000 && y != (int)0x80000000 ){
        gtk_window_move(GTK_WINDOW(Widget), x, y);
    }

    //
    // add the event we interest
    //

    gtk_widget_add_events(Widget,
        GDK_EXPOSURE_MASK               |
        GDK_SCROLL_MASK                 |
        GDK_POINTER_MOTION_MASK         |
        GDK_POINTER_MOTION_HINT_MASK    |
        GDK_BUTTON_MOTION_MASK          |
        GDK_BUTTON1_MOTION_MASK         |
        GDK_BUTTON2_MOTION_MASK         |
        GDK_BUTTON3_MOTION_MASK         |
        GDK_BUTTON_PRESS_MASK           |
        GDK_BUTTON_RELEASE_MASK         |
        GDK_KEY_PRESS_MASK              |
        GDK_KEY_RELEASE_MASK            |
        GDK_ENTER_NOTIFY_MASK           |
        GDK_LEAVE_NOTIFY_MASK           |
        GDK_FOCUS_CHANGE_MASK);

    //
    // set window event handler.
    //

    g_signal_connect(G_OBJECT(Widget), "size-allocate", G_CALLBACK(wrap_size), this);
    g_signal_connect(G_OBJECT(Widget), "motion-notify-event", G_CALLBACK(wrap_motion_notify), this);
    g_signal_connect(G_OBJECT(Widget), "button-press-event", G_CALLBACK(wrap_button_press), this);
    g_signal_connect(G_OBJECT(Widget), "button-release-event", G_CALLBACK(wrap_button_release), this);
    g_signal_connect(G_OBJECT(Widget), "scroll-event", G_CALLBACK(wrap_scroll_event), this);
    g_signal_connect(G_OBJECT(Widget), "enter-notify-event", G_CALLBACK(wrap_enter_notify), this);
    g_signal_connect(G_OBJECT(Widget), "leave-notify-event", G_CALLBACK(wrap_leave_notify), this);
    g_signal_connect(G_OBJECT(Widget), "key-press-event", G_CALLBACK(wrap_key_press), this);
    g_signal_connect(G_OBJECT(Widget), "key-release-event", G_CALLBACK(wrap_key_release), this);
    g_signal_connect(G_OBJECT(Widget), "delete-event", G_CALLBACK(wrap_delete_event), this);
    //g_signal_connect(G_OBJECT(Widget), "destroy-event", G_CALLBACK(wrap_destroy_event), this);
    g_signal_connect(G_OBJECT(Widget), "destroy", G_CALLBACK(wrap_destroy), this);
    g_signal_connect(G_OBJECT(Widget), "draw", G_CALLBACK(wrap_draw), this);

    //
    // generate DUI_WM_CREATE Message
    //

    bHandled = MessageHandler(DUI_WM_CREATE, (LPARAM)Widget, NULL);
    if (bHandled){

        //
        // TODO:some one handle it.
        //

        NOTHING;

    }

    //
    // 这里统一设下窗口属性
    //

    if (m_MinSizeInfo.IsNull()){
        m_MinSizeInfo = m_InitSize;
    }

    if (m_MaxSizeInfo.IsNull()){
        m_MaxSizeInfo.cx = -1;
        m_MaxSizeInfo.cy = -1;
    }
    if (m_rcSizeBox.IsNull()){

        //
        // can not be resize
        //

        GdkGeometry geometry;
        geometry.min_width = m_MinSizeInfo.cx;
        geometry.min_height = m_MinSizeInfo.cy;
        geometry.max_width = m_MinSizeInfo.cx;
        geometry.max_height = m_MinSizeInfo.cy;

        gtk_window_set_geometry_hints(GTK_WINDOW(m_Widget), m_Widget,
            &geometry, (GdkWindowHints)(GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE));
    }else{
        GdkGeometry geometry;
        geometry.min_width = m_MinSizeInfo.cx;
        geometry.min_height = m_MinSizeInfo.cy;
        geometry.max_width = m_MaxSizeInfo.cx;
        geometry.max_height = m_MaxSizeInfo.cy;

        gtk_window_set_geometry_hints(GTK_WINDOW(m_Widget), m_Widget,
            &geometry, (GdkWindowHints)(GDK_HINT_MIN_SIZE | GDK_HINT_MAX_SIZE));
    }

    return true;
}

bool CUIWindow::InitControls(CUIControl* Root, CUIControl* pParent)
{

    //
    // 将窗口实例设置到每个控件中去
    //

    Root->SetOwnWindow(this, pParent, true);

    //
    // 收集所有控件的名称,并加入到hash列表中
    //

    Root->FindControl(__FindControlFromNameHash, this, UIFIND_ALL);
    return true;
}

void CUIWindow::AttachDlg(CUIControl* Root)
{
    m_Root = Root;
    m_bNeedUpdate = true;
    InitControls(Root, NULL);
}

CUIControl* CUIWindow::FindControl(POINT pt) const
{
    return m_Root->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

CUIControl* CUIWindow::FindControl(LPCWSTR pstrName) const
{
    return static_cast<CUIControl*>(m_mNameHash.Find(pstrName));
}

CUIControl* CUIWindow::FindSubControlByPoint(CUIControl* pParent, POINT pt) const
{
    if(pParent == NULL) pParent = GetRoot();
    return pParent->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
}

CUIControl* CUIWindow::FindSubControlByName(CUIControl* pParent, LPCWSTR pstrName) const
{
    if(pParent == NULL) pParent = GetRoot();
    return pParent->FindControl(__FindControlFromName, (LPVOID)pstrName, UIFIND_ALL);
}

// CUIControl* CUIWindow::FindSubControlByClass(CUIControl* pParent, LPCTSTR pstrClass, int iIndex)
// {
//     if( pParent == NULL ) pParent = GetRoot();
//     ASSERT(pParent);
//     m_aFoundControls.Resize(iIndex + 1);
//     return pParent->FindControl(__FindControlFromClass, (LPVOID)pstrClass, UIFIND_ALL);
// }
//
// CStdPtrArray* CUIWindow::FindSubControlsByClass(CUIControl* pParent, LPCTSTR pstrClass)
// {
//     if( pParent == NULL ) pParent = GetRoot();
//     ASSERT(pParent);
//     m_aFoundControls.Empty();
//     pParent->FindControl(__FindControlsFromClass, (LPVOID)pstrClass, UIFIND_ALL);
//     return &m_aFoundControls;
// }

CStdPtrArray* CUIWindow::GetSubControlsByClass()
{
    return &m_aFoundControls;
}

CUIControl* CALLBACK CUIWindow::__FindControlFromNameHash(CUIControl* pThis, LPVOID pData)
{
    CUIWindow* pManager = static_cast<CUIWindow*>(pData);
    const CUIString& sName = pThis->GetName();
    if(!sName.IsEmpty()){

        //
        // Add this control to the hash list
        //

        pManager->m_mNameHash.Set(sName, pThis);
    }

    return NULL;
}

CUIControl* CALLBACK CUIWindow::__FindControlFromCount(CUIControl* /*pThis*/, LPVOID pData)
{
    int* pnCount = static_cast<int*>(pData);
    (*pnCount)++;
    return NULL;
}

CUIControl* CALLBACK CUIWindow::__FindControlFromPoint(CUIControl* pThis, LPVOID pData)
{
    CUIRect posTemp;
    LPPOINT pPoint = static_cast<LPPOINT>(pData);
    posTemp = pThis->GetPos();
    return posTemp.IsPtIn(*pPoint) ? pThis : NULL;
}

// CUIControl* CALLBACK CUIWindow::__FindControlFromTab(CUIControl* pThis, LPVOID pData)
// {
//     FINDTABINFO* pInfo = static_cast<FINDTABINFO*>(pData);
//     if( pInfo->pFocus == pThis ) {
//         if( pInfo->bForward ) pInfo->bNextIsIt = true;
//         return pInfo->bForward ? NULL : pInfo->pLast;
//     }
//     if( (pThis->GetControlFlags() & UIFLAG_TABSTOP) == 0 ) return NULL;
//     pInfo->pLast = pThis;
//     if( pInfo->bNextIsIt ) return pThis;
//     if( pInfo->pFocus == NULL ) return pThis;
//     return NULL;  // Examine all controls
// }

// CUIControl* CALLBACK CUIWindow::__FindControlFromShortcut(CUIControl* pThis, LPVOID pData)
// {
//     if( !pThis->IsVisible() ) return NULL;
//     FINDSHORTCUT* pFS = static_cast<FINDSHORTCUT*>(pData);
//     if( pFS->ch == toupper(pThis->GetShortcut()) ) pFS->bPickNext = true;
//     if( _tcsstr(pThis->GetClass(), _T("LabelUI")) != NULL ) return NULL;   // Labels never get focus!
//     return pFS->bPickNext ? pThis : NULL;
// }

CUIControl* CALLBACK CUIWindow::__FindControlFromUpdate(CUIControl* pThis, LPVOID pData)
{
    return pThis->IsUpdateNeeded() ? pThis : NULL;
}

CUIControl* CALLBACK CUIWindow::__FindControlFromName(CUIControl* pThis, LPVOID pData)
{
    LPCWSTR pstrName = static_cast<LPCWSTR>(pData);
    const CUIString& sName = pThis->GetName();
    return (sName == pstrName) ? pThis : NULL;
}

// CUIControl* CALLBACK CUIWindow::__FindControlFromClass(CUIControl* pThis, LPVOID pData)
// {
//     LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
//     LPCTSTR pType = pThis->GetClass();
//     CStdPtrArray* pFoundControls = pThis->GetManager()->GetSubControlsByClass();
//     if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 ) {
//         int iIndex = -1;
//         while( pFoundControls->GetAt(++iIndex) != NULL ) ;
//         if( iIndex < pFoundControls->GetSize() ) pFoundControls->SetAt(iIndex, pThis);
//     }
//     if( pFoundControls->GetAt(pFoundControls->GetSize() - 1) != NULL ) return pThis;
//     return NULL;
// }

// CUIControl* CALLBACK CUIWindow::__FindControlsFromClass(CUIControl* pThis, LPVOID pData)
// {
//     LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
//     LPCTSTR pType = pThis->GetClass();
//     if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 )
//         pThis->GetManager()->GetSubControlsByClass()->Add((LPVOID)pThis);
//     return NULL;
// }

CUIControl* CUIWindow::GetRoot() const
{
    return m_Root;
}

bool CUIWindow::SetTimer(CUIControl* pControl, UINT uElapse)
{
    assert(pControl != NULL);
    assert(uElapse > 0);

    TIMERINFO* pTimer = new TIMERINFO;
    if(pTimer == NULL)
        return FALSE;

    memset(pTimer, 0, sizeof(*pTimer));

    pTimer->pWindow = this;
    pTimer->bKilled = false;
    pTimer->pSender = pControl;
    pTimer->uTimerId = g_timeout_add((guint)uElapse, wrap_timer_event, pTimer);

    return m_aTimers.Add(pTimer);
}

bool CUIWindow::KillTimer(CUIControl* pControl, UINT nTimerID)
{
    assert(pControl != NULL);
    for(int i = 0; i< m_aTimers.GetSize(); i++){
        TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
        if(pTimer->pSender == pControl && pTimer->uTimerId == nTimerID){
            g_source_remove(pTimer->uTimerId);
            pTimer->uTimerId = 0;
            pTimer->bKilled = true;
            break;
        }
    }
    return false;
}

void CUIWindow::KillTimer(CUIControl* pControl)
{
    assert(pControl != NULL);
    int count = m_aTimers.GetSize();
    for(int i = 0, j = 0; i < count; i++){
        TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i - j]);
        if(pTimer->pSender == pControl){
            if(pTimer->bKilled == false)
                g_source_remove(pTimer->uTimerId);
            delete pTimer;
            m_aTimers.Remove(i - j);
            j++;
        }
    }
}

void CUIWindow::RemoveAllTimers()
{
    for(int i = 0; i < m_aTimers.GetSize(); i++){
        TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
        if( pTimer->bKilled == false ) {
            g_source_remove(pTimer->uTimerId);
        }
        delete pTimer;
    }
    m_aTimers.Empty();
}

bool CUIWindow::AddOptionGroup(LPCWSTR pStrGroupName, CUIControl* pControl)
{
    LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
    if(lp){
        CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
        for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
            if( static_cast<CUIControl*>(aOptionGroup->GetAt(i)) == pControl ) {
                return false;
            }
        }
        aOptionGroup->Add(pControl);
    }else{
        CStdPtrArray* aOptionGroup = new CStdPtrArray(6);
        aOptionGroup->Add(pControl);
        m_mOptionGroup.Insert(pStrGroupName, aOptionGroup);
    }
    return true;
}

CStdPtrArray* CUIWindow::GetOptionGroup(LPCWSTR pStrGroupName)
{
    LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
    if(lp)
        return static_cast<CStdPtrArray*>(lp);
    return NULL;
}

void CUIWindow::RemoveOptionGroup(LPCWSTR pStrGroupName, CUIControl* pControl)
{
    LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
    if( lp ) {
        CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
        if( aOptionGroup == NULL )
            return;
        for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
            if( static_cast<CUIControl*>(aOptionGroup->GetAt(i)) == pControl ) {
                aOptionGroup->Remove(i);
                break;
            }
        }
        if(aOptionGroup->IsEmpty()){
            delete aOptionGroup;
            m_mOptionGroup.Remove(pStrGroupName);
        }
    }
}

void CUIWindow::RemoveAllOptionGroups()
{
    CStdPtrArray* aOptionGroup;
    for( int i = 0; i< m_mOptionGroup.GetSize(); i++ ) {
        if(LPCWSTR key = m_mOptionGroup.GetAt(i)) {
            aOptionGroup = static_cast<CStdPtrArray*>(m_mOptionGroup.Find(key));
            delete aOptionGroup;
        }
    }
    m_mOptionGroup.RemoveAll();
}

GtkWidget* CUIWindow::GetWidget()
{
    return m_Widget;
}

void CUIWindow::SendNotify(CUIControl* pControl, UINT uMsgType, WPARAM wParam /*= 0*/,
                           LPARAM lParam /*= 0*/, bool bAsync /*= false*/)
{
    TNotifyUI Msg;
    Msg.pSender = pControl;
    Msg.Type = uMsgType;
    Msg.wParam = wParam;
    Msg.lParam = lParam;
    SendNotify(Msg, bAsync);
}

void CUIWindow::SendNotify(TNotifyUI& Msg, bool bAsync)
{
    Msg.ptMouse = m_ptLastMousePos;
    Msg.dwTimestamp = UIGetTickCount();

    //
    // TODO:VirtualWnd
    //

    //if(m_bUsedVirtualWnd){
    //    Msg.sVirtualWnd = Msg.pSender->GetVirtualWnd();
    //}

    if(!bAsync){

        //
        // Send to all listeners
        //

        /*if(Msg.pSender != NULL){
            if(Msg.pSender->OnNotify)
                Msg.pSender->OnNotify(&Msg);
        }*/

        for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
            static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(Msg);
        }
    }else{

        //
        // TODO: Asynchronous
        //

    }
}

bool CUIWindow::AddNotifier(INotifyUI* pNotifier)
{
    //assert(m_aNotifiers.Find(pNotifier)<0);
    return m_aNotifiers.Add(pNotifier);
}

bool CUIWindow::RemoveNotifier(INotifyUI* pNotifier)
{
    for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
        if( static_cast<INotifyUI*>(m_aNotifiers[i]) == pNotifier ) {
            return m_aNotifiers.Remove(i);
        }
    }
    return false;
}

void CUIWindow::AddDefaultAttributeList(LPCWSTR pStrControlName, LPCWSTR pStrControlAttrList)
{
    CUIString* pDefaultAttr = new CUIString(pStrControlAttrList);
    if (pDefaultAttr != NULL){
        if (m_DefaultAttrHash.Find(pStrControlName) == NULL)
            m_DefaultAttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr);
    }
}

LPCWSTR CUIWindow::GetDefaultAttributeList(LPCWSTR pStrControlName) const
{
    CUIString* pDefaultAttr = static_cast<CUIString*>(m_DefaultAttrHash.Find(pStrControlName));
    if(pDefaultAttr)
        return pDefaultAttr->GetData();
    else
        return NULL;
}

bool CUIWindow::RemoveDefaultAttributeList(LPCWSTR pStrControlName)
{
    CUIString* pDefaultAttr = static_cast<CUIString*>(m_DefaultAttrHash.Find(pStrControlName));
    if(!pDefaultAttr)
        return false;

    delete pDefaultAttr;
    return m_DefaultAttrHash.Remove(pStrControlName);
}

const CStdStringPtrMap& CUIWindow::GetDefaultAttribultes() const
{
    return m_DefaultAttrHash;
}

void CUIWindow::RemoveAllDefaultAttributeList()
{
    CUIString* pDefaultAttr;
    for( int i = 0; i< m_DefaultAttrHash.GetSize(); i++ ) {
        if(LPCWSTR key = m_DefaultAttrHash.GetAt(i)) {
            pDefaultAttr = static_cast<CUIString*>(m_DefaultAttrHash.Find(key));
            delete pDefaultAttr;
        }
    }
    m_DefaultAttrHash.RemoveAll();
}

void CUIWindow::SetCaptionRect(const CUIRect& rc)
{
    m_rcCaption = rc;
}

CUIRect CUIWindow::GetCaptionRect() const
{
    return m_rcCaption;
}

void CUIWindow::ReSize(const CUISize& size)
{
    if (m_Widget){
        gtk_window_resize(GTK_WINDOW(m_Widget), (gint)size.cx, (gint)size.cy);
    }
}

void CUIWindow::GetSize(CUISize& size)
{
    if (m_Widget){
        gint width, height;

        gtk_window_get_size(GTK_WINDOW(m_Widget), &width, &height);

        size.cx = width;
        size.cy = height;
    }
}

void CUIWindow::SetInitSize(const CUISize& size)
{
    m_InitSize = size;
    ReSize(size);
}

void CUIWindow::GetInitSize(CUISize& size)
{
    size = m_InitSize;
}

void CUIWindow::SetMaxSize(const CUISize& size)
{
    m_MaxSizeInfo = size;
    /*GdkGeometry geometry;

    //
    // 这里如果设置了sizebox.就不能设置max box
    //

    if (m_rcSizeBox.IsNull()){
        return;
    }

    geometry.max_height = size.cx;
    geometry.max_width = size.cy;

    m_MaxSizeInfo = size;

    //
    // 注意这个函数在windows下是不好使的.
    // 因为官方windows下最新的版本是3.6这个版本里
    // 的这个函数是有bug的.即使设置了max size.窗口照样可以拖动
    // 在linux下就没问题
    //

    gtk_window_set_geometry_hints(GTK_WINDOW(m_Widget), m_Widget,
        &geometry, GDK_HINT_MAX_SIZE);*/
}

void CUIWindow::SetMinSize(const CUISize& size)
{
    m_MinSizeInfo = size;
    /*GdkGeometry geometry;

    geometry.min_width = size.cx;
    geometry.min_height = size.cy;

    m_MinSizeInfo = size;

    //
    // GDK_HINT_MIN_SIZE同上.见上面SetMaxSize的注释
    //

    gtk_window_set_geometry_hints(GTK_WINDOW(m_Widget), m_Widget,
        &geometry, GDK_HINT_MIN_SIZE);*/
}

void CUIWindow::SetSizeBox(const CUIRect& size)
{
    m_rcSizeBox = size;
}

cairo_t *CUIWindow::GetPaintContext()
{
    return m_crPaintContext;
}
