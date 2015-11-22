#ifndef __UI__DEFINE__H__
#define __UI__DEFINE__H__

#ifdef WIN32
#define UILIB_COMDAT __declspec(selectany)
#else
#define UILIB_COMDAT
#endif // WIN32

//
// duilib  notify message type definition
//

#define DUI_MSGTYPE_MENU                   0x40000000
#define DUI_MSGTYPE_LINK                   0x40000001
#define DUI_MSGTYPE_TIMER                  0x40000002
#define DUI_MSGTYPE_CLICK                  0x40000003
#define DUI_MSGTYPE_RETURN                 0x40000004
#define DUI_MSGTYPE_SCROLL                 0x40000005
#define DUI_MSGTYPE_DROPDOWN               0x40000006
#define DUI_MSGTYPE_SETFOCUS               0x40000007
#define DUI_MSGTYPE_KILLFOCUS              0x40000008
#define DUI_MSGTYPE_ITEMCLICK 		   	   0x40000009
#define DUI_MSGTYPE_TABSELECT              0x4000000A
#define DUI_MSGTYPE_ITEMSELECT 		   	   0x4000000B
#define DUI_MSGTYPE_ITEMEXPAND             0x4000000C
#define DUI_MSGTYPE_WINDOWINIT             0x4000000D
#define DUI_MSGTYPE_BUTTONDOWN 		   	   0x4000000E
#define DUI_MSGTYPE_MOUSEENTER			   0x4000000F
#define DUI_MSGTYPE_MOUSELEAVE			   0x40000010
#define DUI_MSGTYPE_TEXTCHANGED            0x40000011
#define DUI_MSGTYPE_HEADERCLICK            0x40000012
#define DUI_MSGTYPE_ITEMDBCLICK            0x40000013
#define DUI_MSGTYPE_SHOWACTIVEX            0x40000014
#define DUI_MSGTYPE_ITEMCOLLAPSE           0x40000015
#define DUI_MSGTYPE_ITEMACTIVATE           0x40000016
#define DUI_MSGTYPE_VALUECHANGED           0x40000017
#define DUI_MSGTYPE_SELECTCHANGED 		   0x40000018
#define DUI_MSGTYPE_INVALID  		       0x4fffffff

enum DuiSig
{
    DuiSig_end = 0, // [marks end of message map]
    DuiSig_lwl,     // LRESULT (WPARAM, LPARAM)
    DuiSig_vn,      // void (TNotifyUI)
};


class CUIControl;
typedef struct tagTNotifyUI 
{
    UINT Type;
    //CUIString sVirtualWnd;
    CUIControl* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    WPARAM wParam;
    LPARAM lParam;
} TNotifyUI;

class CNotifyPump;
typedef void (CNotifyPump::*DUI_PMSG)(TNotifyUI& msg);  //指针类型

union DuiMessageMapFunctions
{
    DUI_PMSG pfn;
    LRESULT (CNotifyPump::*pfn_Notify_lwl)(WPARAM, LPARAM);
    void (CNotifyPump::*pfn_Notify_vn)(TNotifyUI&);
};

struct DUI_MSGMAP_ENTRY;
struct DUI_MSGMAP
{
#ifndef UILIB_STATIC
    const DUI_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
    const DUI_MSGMAP* pBaseMap;
#endif
    const DUI_MSGMAP_ENTRY* lpEntries;
};

//结构定义
struct DUI_MSGMAP_ENTRY //定义一个结构体，来存放消息信息
{
    UINT      MsgType;            // DUI消息类型
    CUIString strCtrlName;        // 控件名称
    UINT       nSig;              // 标记函数指针类型
    DUI_PMSG   pfn;               // 指向函数的指针
};

//定义
#ifndef UILIB_STATIC
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
    static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
    static const DUI_MSGMAP messageMap;                                   \
    static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 \
    virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#else
#define DUI_DECLARE_MESSAGE_MAP()                                         \
private:                                                                  \
    static const DUI_MSGMAP_ENTRY _messageEntries[];                      \
protected:                                                                \
    static  const DUI_MSGMAP messageMap;				                  \
    virtual const DUI_MSGMAP* GetMessageMap() const;                      \

#endif


//基类声明开始
#ifndef UILIB_STATIC
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
    const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
{ return NULL; }                                                          \
    const DUI_MSGMAP* theClass::GetMessageMap() const                     \
{ return &theClass::messageMap; }                                         \
    UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
{  &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };        \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
{                                                                         \

#else
#define DUI_BASE_BEGIN_MESSAGE_MAP(theClass)                              \
    const DUI_MSGMAP* theClass::GetMessageMap() const                     \
{ return &theClass::messageMap; }                                         \
    UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
{  NULL, &theClass::_messageEntries[0] };                                 \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
{                                                                         \

#endif


//子类声明开始
#ifndef UILIB_STATIC
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
    const DUI_MSGMAP* PASCAL theClass::_GetBaseMessageMap()               \
{ return &baseClass::messageMap; }                                        \
    const DUI_MSGMAP* theClass::GetMessageMap() const                     \
{ return &theClass::messageMap; }                                         \
    UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] };         \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
{                                                                         \

#else
#define DUI_BEGIN_MESSAGE_MAP(theClass, baseClass)                        \
    const DUI_MSGMAP* theClass::GetMessageMap() const                     \
{ return &theClass::messageMap; }                                         \
    UILIB_COMDAT const DUI_MSGMAP theClass::messageMap =                  \
{ &baseClass::messageMap, &theClass::_messageEntries[0] };                \
    UILIB_COMDAT const DUI_MSGMAP_ENTRY theClass::_messageEntries[] =     \
{                                                                         \

#endif


//声明结束
#define DUI_END_MESSAGE_MAP()                                             \
{ DUI_MSGTYPE_INVALID, L"", DuiSig_end, (DUI_PMSG)0 }                                  \
};                                                                        \


//定义消息类型--执行函数宏
#define DUI_ON_MSGTYPE(msgtype, memberFxn)                                \
{ msgtype, L"", DuiSig_vn, (DUI_PMSG)&memberFxn},                         \


//定义消息类型--控件名称--执行函数宏
#define DUI_ON_MSGTYPE_CTRNAME(msgtype,ctrname,memberFxn)                 \
{ msgtype, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },                    \


//定义click消息的控件名称--执行函数宏
#define DUI_ON_CLICK_CTRNAME(ctrname,memberFxn)                           \
{ DUI_MSGTYPE_CLICK, ctrname, DuiSig_vn, (DUI_PMSG)&memberFxn },          \


//定义selectchanged消息的控件名称--执行函数宏
#define DUI_ON_SELECTCHANGED_CTRNAME(ctrname,memberFxn)                   \
{ DUI_MSGTYPE_SELECTCHANGED,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },     \


//定义killfocus消息的控件名称--执行函数宏
#define DUI_ON_KILLFOCUS_CTRNAME(ctrname,memberFxn)                       \
{ DUI_MSGTYPE_KILLFOCUS,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },         \


//定义menu消息的控件名称--执行函数宏
#define DUI_ON_MENU_CTRNAME(ctrname,memberFxn)                            \
{ DUI_MSGTYPE_MENU,ctrname,DuiSig_vn,(DUI_PMSG)&memberFxn },              \


//定义与控件名称无关的消息宏

//定义timer消息--执行函数宏
#define DUI_ON_TIMER()                                                    \
{ DUI_MSGTYPE_TIMER, L"", DuiSig_vn,(DUI_PMSG)&OnTimer },                 \


#define  DUI_CTR_GIF                             (L"Gif")
#define  DUI_CTR_EDIT                            (L"Edit")
#define  DUI_CTR_LIST                            (L"List")
#define  DUI_CTR_TEXT                            (L"Text")
#define  DUI_CTR_COMBO                           (L"Combo")
#define  DUI_CTR_LABEL                           (L"Label")
#define  DUI_CTR_FLASH							 (L"Flash")
#define  DUI_CTR_BUTTON                          (L"Button")
#define  DUI_CTR_OPTION                          (L"Option")
#define  DUI_CTR_SLIDER                          (L"Slider")
#define  DUI_CTR_CONTROL                         (L"Control")
#define  DUI_CTR_ACTIVEX                         (L"ActiveX")
#define  DUI_CTR_LISTITEM                        (L"ListItem")
#define  DUI_CTR_PROGRESS                        (L"Progress")
#define  DUI_CTR_RICHEDIT                        (L"RichEdit")
#define  DUI_CTR_CHECKBOX                        (L"CheckBox")
#define  DUI_CTR_COMBOBOX                        (L"ComboBox")
#define  DUI_CTR_DATETIME                        (L"DateTime")
#define  DUI_CTR_TREEVIEW                        (L"TreeView")
#define  DUI_CTR_TREENODE                        (L"TreeNode")
#define  DUI_CTR_CONTAINER                       (L"Container")
#define  DUI_CTR_TABLAYOUT                       (L"TabLayout")
#define  DUI_CTR_SCROLLBAR                       (L"ScrollBar")
#define  DUI_CTR_LISTHEADER                      (L"ListHeader")
#define  DUI_CTR_TILELAYOUT                      (L"TileLayout")
#define  DUI_CTR_WEBBROWSER                      (L"WebBrowser")
#define  DUI_CTR_CHILDLAYOUT                     (L"ChildLayout")
#define  DUI_CTR_LISTELEMENT                     (L"ListElement")
#define  DUI_CTR_DIALOGLAYOUT                    (L"DialogLayout")
#define  DUI_CTR_VERTICALLAYOUT                  (L"VerticalLayout")
#define  DUI_CTR_LISTHEADERITEM                  (L"ListHeaderItem")
#define  DUI_CTR_LISTTEXTELEMENT                 (L"ListTextElement")
#define  DUI_CTR_HORIZONTALLAYOUT                (L"HorizontalLayout")
#define  DUI_CTR_LISTLABELELEMENT                (L"ListLabelElement")
#define  DUI_CTR_LISTCONTAINERELEMENT            (L"ListContainerElement")

///
//////////////END控件名称宏定义//////////////////////////////////////////////////

#endif