#pragma once

#include <gtk/gtk.h>

#define DUI_WM_START            0x10000000


//
// WPARAM is cairo context cr.
// LPARAM is NULL
//

#define DUI_WM_PAINT            0x10000000


//
// WPARAM is Window new SIZE
// LPARAM is NULL
//

#define DUI_WM_SIZE             0x10000001

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSEMOVE        0x10000002

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSEPRESS       0x10000003

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSERELEASE     0x10000004

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSEENTER       0x10000005

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSELEAVE       0x10000006

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_KEYPRESS         0x10000007

//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_KEYPRELEASE      0x10000008


//
// WPARAM is pointer to GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_CLOSE           0x10000009

//
// WPARAM is sender control.
// LPARAM is timer id
//

#define DUI_WM_TIMER           0x1000000A

//
// WPARAM is GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_MOUSEWHEEL      0x1000000B

//
// WPARAM is GtkEvent.
// LPARAM is NULL
//

#define DUI_WM_DESTROY         0x1000000C

//
// WPARAM is Window pointer.
// LPARAM is NULL
//

#define DUI_WM_CREATE          0x1000000D

#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000


// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};


class CUIControl;
class CUIWindow;

typedef struct tagFINDTABINFO
{
    CUIControl* pFocus;
    CUIControl* pLast;
    bool bForward;
    bool bNextIsIt;
}FINDTABINFO, *PFINDTABINFO;

typedef struct tagTIMERINFO
{
    CUIControl* pSender;
    UINT uTimerId;
    bool bKilled;
    CUIWindow* pWindow;
} TIMERINFO;

class CUIWindow
{
public:
    CUIWindow(void);
    virtual ~CUIWindow(void);
    void CenterWindow();
    void Minimize();
    void Restore();
    void Maximize();
    void FullScreen();
    bool IsMinimize();
    bool IsMaximize();
    bool IsFullScreen();
    bool Create(int x, int y, int nWidth, int nHeight);
    void Invalidate(const CUIRect &Rect);
    void NeedUpdate();
    CUIControl* GetRoot() const;
    CStdPtrArray* GetSubControlsByClass();
    CUIControl* FindSubControlByPoint(CUIControl* pParent, POINT pt) const;
    CUIControl* FindControl(LPCWSTR pstrName) const;
    CUIControl* FindControl(POINT pt) const;
    CUIControl* FindSubControlByName(CUIControl* pParent, LPCWSTR pstrName) const;
    bool SetTimer(CUIControl* pControl, UINT uElapse);
    bool KillTimer(CUIControl* pControl, UINT nTimerID);
    void KillTimer(CUIControl* pControl);
    void RemoveAllTimers();
    void Show();
    void MainLoop();
    void AttachDlg(CUIControl* Root);
    UINT MapKeyState(UINT State);
    bool AddOptionGroup(LPCWSTR pStrGroupName, CUIControl* pControl);
    CStdPtrArray* GetOptionGroup(LPCWSTR pStrGroupName);
    void RemoveOptionGroup(LPCWSTR pStrGroupName, CUIControl* pControl);
    void RemoveAllOptionGroups();
    GtkWidget* GetWidget();
    bool AddNotifier(INotifyUI* pNotifier);
    bool RemoveNotifier(INotifyUI* pNotifier);
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
    void SendNotify(CUIControl* pControl, UINT uMsgType, WPARAM wParam = NULL,
        LPARAM lParam = NULL, bool bAsync = false);
    void Close();
    void ReSize(const CUISize& size);
    void SetInitSize(const CUISize& size);
    void GetInitSize(CUISize& size);
    void GetSize(CUISize& size);
    void SetMinSize(const CUISize& size);
    void SetMaxSize(const CUISize& size);
    void SetCaptionRect(const CUIRect& rc);
    CUIRect GetCaptionRect() const;
    void SetSizeBox(const CUIRect& size);

    bool InitControls(CUIControl* Root, CUIControl* pParent = NULL);
    cairo_t *GetPaintContext();

    //
    // for default attribute manager
    //

    void AddDefaultAttributeList(LPCWSTR pStrControlName, LPCWSTR pStrControlAttrList);
    LPCWSTR GetDefaultAttributeList(LPCWSTR pStrControlName) const;
    bool RemoveDefaultAttributeList(LPCWSTR pStrControlName);
    const CStdStringPtrMap& GetDefaultAttribultes() const;
    void RemoveAllDefaultAttributeList();

public:
    virtual bool MessageHandler(int Msg, WPARAM wParam, LPARAM lParam);

private:
    static CUIControl* CALLBACK __FindControlFromName(CUIControl* pThis, LPVOID pData);
    static CUIControl* CALLBACK __FindControlFromUpdate(CUIControl* pThis, LPVOID pData);
    static CUIControl* CALLBACK __FindControlFromPoint(CUIControl* pThis, LPVOID pData);
    static CUIControl* CALLBACK __FindControlFromCount(CUIControl* /*pThis*/, LPVOID pData);
    static CUIControl* CALLBACK __FindControlFromNameHash(CUIControl* pThis, LPVOID pData);

private:
    GtkWidget *m_Widget;
    CUIControl *m_Root;
    CStdStringPtrMap m_mNameHash;
    CStdPtrArray m_aFoundControls;
    CStdPtrArray m_aTimers;
    CStdPtrArray m_aNotifiers;
    UINT m_uTimerID;
    CUIControl *m_pEventClick;
    CUIControl *m_pEventHover;
    CStdStringPtrMap m_mOptionGroup;
    POINT m_ptLastMousePos;
    CStdStringPtrMap m_DefaultAttrHash;


    /** 窗口标题栏(可拖动部分)大小*/
    CUIRect m_rcCaption;

    bool m_bNeedUpdate;

    /** 这个变量在调用了Show函数之后才设置成True, 然后再窗口Invalid的时候判断一下.避免gtk出现错误信息*/
    bool m_bWindowShowed;

    /** cairo conext used for get font info*/
    cairo_t *m_crPaintContext;

    CUISize m_InitSize;

    CUISize m_MaxSizeInfo;
    CUISize m_MinSizeInfo;

    CUIRect m_rcSizeBox;
};
