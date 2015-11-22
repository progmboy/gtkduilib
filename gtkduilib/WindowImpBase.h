#pragma once

enum UILIB_RESOURCETYPE
{
    UILIB_FILE=1,				// 来自磁盘文件
    UILIB_ZIP,						// 来自磁盘zip压缩包
    UILIB_RESOURCE,			// 来自资源
    UILIB_ZIPRESOURCE,	// 来自资源的zip压缩包
};

class CWindowImpBase : public CUIWindow, public CNotifyPump, public INotifyUI
{
public:
    CWindowImpBase(void);
    virtual ~CWindowImpBase(void);

    virtual void InitWindow(){};
   // virtual void OnFinalMessage( HWND hWnd );
    virtual void Notify(TNotifyUI& msg);
    DUI_DECLARE_MESSAGE_MAP()
    virtual void OnClick(TNotifyUI& msg);

protected:
    virtual CUIString GetSkinFolder() = 0;
    virtual CUIString GetSkinFile() = 0;
    virtual CUIString GetWindowClassName(void) const = 0 ;
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

public:
    virtual UINT GetClassStyle() const;
    virtual UILIB_RESOURCETYPE GetResourceType() const;
    virtual CUIString GetZIPFileName() const;
    virtual CUIControl* CreateControl(const CUIString& strClass);
    virtual bool MessageHandler(int uMsg, WPARAM wParam, LPARAM lParam); 
    virtual bool OnClose(int Msg, WPARAM wParam, LPARAM lParam);
    virtual void OnDestroy(int /*Msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

    virtual bool OnGetMinMaxInfo(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnMouseWheel(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnMouseHover(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnSize(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnChar(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnSysCommand(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnCreate(int uMsg, WPARAM wParam, LPARAM lParam);
    virtual bool OnKeyDown(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnKillFocus(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnSetFocus(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnLButtonDown(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnLButtonUp(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool OnMouseMove(int /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);
    virtual bool HandleCustomMessage(int Msg, WPARAM wParam, LPARAM lParam);
    virtual LONG GetStyle();
};
