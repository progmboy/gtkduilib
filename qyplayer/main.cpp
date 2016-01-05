// gtkduilib_test.cpp : Defines the entry point for the console application.
//

#include <vector>
#include "../gtkduilib/UILib.h"

const wchar_t * gszMsg[] = {
    L"DUI_MSGTYPE_MENU",
    L"DUI_MSGTYPE_LINK",
    L"DUI_MSGTYPE_TIMER",
    L"DUI_MSGTYPE_CLICK",
    L"DUI_MSGTYPE_RETURN",
    L"DUI_MSGTYPE_SCROLL",
    L"DUI_MSGTYPE_DROPDOWN",
    L"DUI_MSGTYPE_SETFOCUS",
    L"DUI_MSGTYPE_KILLFOCUS",
    L"DUI_MSGTYPE_ITEMCLICK",
    L"DUI_MSGTYPE_TABSELECT",
    L"DUI_MSGTYPE_ITEMSELECT",
    L"DUI_MSGTYPE_ITEMEXPAND",
    L"DUI_MSGTYPE_WINDOWINIT",
    L"DUI_MSGTYPE_BUTTONDOWN",
    L"DUI_MSGTYPE_MOUSEENTER",
    L"DUI_MSGTYPE_MOUSELEAVE",
    L"DUI_MSGTYPE_TEXTCHANGED",
    L"DUI_MSGTYPE_HEADERCLICK",
    L"DUI_MSGTYPE_ITEMDBCLICK",
    L"DUI_MSGTYPE_SHOWACTIVEX",
    L"DUI_MSGTYPE_ITEMCOLLAPSE",
    L"DUI_MSGTYPE_ITEMACTIVATE",
    L"DUI_MSGTYPE_VALUECHANGED",
    L"DUI_MSGTYPE_SELECTCHANGED"
};


class CSkinWnd : public CWindowImpBase
{
public:
    CSkinWnd(){};
    virtual ~CSkinWnd(){};

public:
    CUIString GetWindowClassName(void) const
    {
        return L"skinwnd";
    }
    CUIString GetSkinFolder()
    {
        return L"";
    }
    CUIString GetSkinFile()
    {
        return L"";
    }

    bool OnClose(int Msg, WPARAM wParam, LPARAM lParam)
    {
        g_print("on close()");
        return false;
    }

    void OnDestroy(int, WPARAM, LPARAM)
    {
        gtk_main_quit();
    }

    void InitWindow()
    {
        CDialogBuilder builer;
        CUIControl *pRoot = builer.Create(L"PlayerMain2.xml", this, NULL);
        //CDialogBuilderCallbackEx cb;
        //builer.SetCallback(&cb);
        //CUIControl *pRoot = builer.Create(L"skin2.xml", this, NULL);
        if (pRoot){
            AttachDlg(pRoot);
        }

        CDialogBuilder builer2;
        CUILabel *pTempLabel;
        CListContainerElementUI *pListElement = static_cast<CListContainerElementUI*>(builer2.Create(L"music_list.xml"));
        CListUI* pControl = static_cast<CListUI*>(FindControl(L"fuck_list"));

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"music_title"));
        assert(pTempLabel);
        pTempLabel->SetText(L"fuck");

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"auth_name"));
        assert(pTempLabel);
        pTempLabel->SetText(L"ssss");

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"song_time"));
        assert(pTempLabel);
        pTempLabel->SetText(L"--:--");
        pControl->Add(pListElement);

        CDialogBuilder builer3;
        pListElement = static_cast<CListContainerElementUI*>(builer3.Create(L"music_list.xml"));
        pControl = static_cast<CListUI*>(FindControl(L"fuck_list"));

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"music_title"));
        assert(pTempLabel);
        pTempLabel->SetText(L"fuck2");

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"auth_name"));
        assert(pTempLabel);
        pTempLabel->SetText(L"aaa");

        pTempLabel = static_cast<CUILabel*>(pListElement->FindSubControl(L"song_time"));
        assert(pTempLabel);
        pTempLabel->SetText(L"20:20");
        pControl->Add(pListElement);
    }

    void Notify(TNotifyUI& msg)
    {
        if (msg.Type <= (sizeof(gszMsg) / sizeof(gszMsg[0]) + 0x40000000)){
            wprintf(L"msg %s sender %s\n", (LPCWSTR)gszMsg[msg.Type-0x40000000], (LPCWSTR)msg.pSender->GetName());
        }

        if (msg.Type == DUI_MSGTYPE_CLICK && msg.pSender->GetName() == L"button"){
            Close();
        }

        CWindowImpBase::Notify(msg);
    }

    void OnClick(TNotifyUI& msg)
    {
        g_print("click event\n");
        if (msg.Type == DUI_MSGTYPE_CLICK && msg.pSender->GetName() == L"button"){
            Close();
        }
    }

private:
    CUIButton button;
    CUIProgress Progress;
    CUISlider Slider;
    CUILabel Label;
    CUIContainer Container;
public:
    DUI_DECLARE_MESSAGE_MAP()
};

DUI_BEGIN_MESSAGE_MAP(CSkinWnd, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, CSkinWnd::OnClick)
DUI_END_MESSAGE_MAP()

#ifdef WIN32
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    wprintf(L"%s\n", "ÖÐ¹ú");
    //CResourceMgr ss;

    gtk_init(NULL, NULL);

    CSkinWnd Window;
    //CResourceMgr::GetInstance().SetResourePath(L"/home/pgboy/qyplayer");
    CResourceMgr::GetInstance().SetResourePath(L"../skin/qyplayer");

    Window.Create(0, 0, /*400*/800, /*500*/570);

    Window.CenterWindow();
    Window.Show();
    Window.MainLoop();

    //gtk_main_quit();

	return 0;
}

