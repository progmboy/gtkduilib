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


class ComputerExamineUI : public CUIContainer
{
public:
    ComputerExamineUI()
    {
        CDialogBuilder builder;
        CUIContainer* pComputerExamine = static_cast<CUIContainer*>(builder.Create(L"ComputerExamine2.xml"));
        if( pComputerExamine ) {
            this->Add(pComputerExamine);
        }
        else {
            this->RemoveAll();
            return;
        }
    }
};

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
    CUIControl* CreateControl(const CUIString& strClass)
    {
        if(strClass == L"ComputerExamine")
            return new ComputerExamineUI;
        return NULL;
    }
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
        //AddNotifier(this);
        CDialogBuilder builer;
        CDialogBuilderCallbackEx cb;
        builer.SetCallback(&cb);
        CUIControl *pRoot = builer.Create(L"skin2.xml", this, NULL);
        if (pRoot){
            AttachDlg(pRoot);
            AddNotifier(this);
        }
    }

    void Notify(TNotifyUI& msg)
    {
        if (msg.Type <= (sizeof(gszMsg) / sizeof(gszMsg[0]) + 0x40000000)){
            wprintf(L"msg %s sender %s\n", gszMsg[msg.Type-0x40000000], (LPCWSTR)msg.pSender->GetName());
        }

        if (msg.Type == DUI_MSGTYPE_CLICK && msg.pSender->GetName() == L"button"){
            Close();
        }

        else if(msg.Type == DUI_MSGTYPE_SELECTCHANGED)
        {
            CUIString name = msg.pSender->GetName();
            CUITabLayout* pControl = static_cast<CUITabLayout*>(FindControl(L"switch"));
            if(name==L"examine")
                pControl->SelectItem(0);
            else if(name==L"trojan")
                pControl->SelectItem(1);
            else if(name==L"plugins")
                pControl->SelectItem(2);
            else if(name==L"vulnerability")
                pControl->SelectItem(3);
            else if(name==L"rubbish")
                pControl->SelectItem(4);
            else if(name==L"cleanup")
                pControl->SelectItem(5);
            else if(name==L"fix")
                pControl->SelectItem(6);
            else if(name==L"tool")
                pControl->SelectItem(7);
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
    //CResourceMgr ss;

    gtk_init(NULL, NULL);

    CSkinWnd Window;
    //CResourceMgr::GetInstance().SetResourePath(L"/home/pgboy/qyplayer");
    CResourceMgr::GetInstance().SetResourePath(L"../skin/360SafeRes");

    Window.Create(0, 0, /*400*/800, /*500*/570);

    Window.CenterWindow();
    Window.Show();
    Window.MainLoop();

    //gtk_main_quit();

	return 0;
}


