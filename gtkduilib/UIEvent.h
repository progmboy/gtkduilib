#pragma once

enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT__LAST,
};

#define IS_MOUSE_EVENT(_type) (_type > UIEVENT__MOUSEBEGIN && _type < UIEVENT__MOUSEEND)

class CUIControl;

typedef struct tagTEventUI
{
    int Type;
    CUIControl* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

class CUIEvent
{
public:
    CUIEvent(void);
    CUIEvent(PVOID Sender, DWORD dwType, DWORD dwTime);
    CUIEvent(GdkEvent *Event);
    virtual ~CUIEvent(void);

private:
    DWORD m_Type;
    PVOID m_Sender;
    DWORD m_Time;
    
};
