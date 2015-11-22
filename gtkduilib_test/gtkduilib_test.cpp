// gtkduilib_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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
        /*Container.SetBkColor(0xFFD1E8F5);
        button.SetFloat(true);
        button.SetName(L"button");
        button.SetText(L"Fucking Button");
        button.SetTextColor(0xFF00FF00);
        button.SetNormalImage(L"file='D:\\my_program\\skin\\default\\dialog-button-normal.png' corner='5,5,5,5'");
        button.SetPushedImage(L"file='D:\\my_program\\skin\\default\\dialog-button-pressed.png' corner='5,5,5,5'");
        button.SetHotImage(L"file='D:\\my_program\\skin\\default\\dialog-button-prelight.png' corner='5,5,5,5'");


        Label.SetFloat(true);
        Label.SetName(L"Label");
        Label.SetBkColor(0xFF0000FF);
        Label.SetTextColor(0xFFFF0000);
        Label.SetText(L"Fucking Label");
        Label.SetTextStyle(DT_CENTER | DT_VCENTER);


        Progress.SetFloat(true);
        Progress.SetName(L"Progress");
        Progress.SetValue(40);
        Progress.SetBkColor(0xFFC7C7C7);
        Progress.SetDisabledTextColor(0xFFA7A6AA);
        Progress.SetForeImage(L"D:\\my_program\\skin\\test\\fg.png");
        Progress.SetTextStyle(DT_CENTER | DT_VCENTER);


        Slider.SetFloat(true);
        Slider.SetName(L"Slider");
        Slider.SetValue(40);
        Slider.SetBkColor(0xFFC7C7C7);
        Slider.SetDisabledTextColor(0xFFA7A6AA);
        Slider.SetForeImage(L"D:\\my_program\\skin\\test\\fg.png");
        Slider.SetThumbImage(L"D:\\my_program\\skin\\test\\point_normal.png");
        Slider.SetThumbSize(CUISize(18, 8));
        Slider.SetTextStyle(DT_CENTER | DT_VCENTER);
        Slider.SetSendMove(true);

        Container.Add(&button);
        Container.SetName(L"Container");
        Container.Add(&Label);
        Container.Add(&Progress);
        Container.Add(&Slider);

        AttachDlg(&Container);

        button.SetPos(CUIRect(22, 17, 145, 64));
        Slider.SetPos(CUIRect(20, 90, 220, 98));
        Progress.SetPos(CUIRect(20, 70, 220, 78));
        Label.SetPos(CUIRect(175, 17, 290, 64));*/
        //AddNotifier(this);*/

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
    /*CUIString strTest;

    strTest = L"test";

    wprintf(L"%s\n", strTest);
    strTest += L"1234";

    wprintf(L"%s\n", strTest);
    strTest.Format(L"%dwocao %s %d %d", 1111, L"what the fuck", 2222, 3333);
    wprintf(L"%s\n", strTest);
    wchar_t szTest[520] = {0};

    for (int i = 0; i < 520; i++){
        szTest[i] = L'A';
    }
    szTest[520-1] = L'\0';
    //memset(szTest, 'A', 520);

    szTest[512] = L'%';
    szTest[513] = L's';
    szTest[520-1] = L'\0';

    strTest.Format(szTest, L"1234567");

    wprintf(L"%s\n", strTest);
    strTest.Empty();

    wprintf(L"%s\n", strTest);
    strTest = L"test1234";
    strTest.Replace(L"23", L"45");

    wprintf(L"%s\n", strTest);*/


    /*CUIControl Control1;
    Control1.SetFloat(true);
    //Control1.SetBkColor(0xFFFFFF00);
    Control1.SetPos(CUIRect(22, 17, 141, 64));
    Control1.SetBkImage(L"file='D:\\test\\btn.png' corner='5,5,5,5'");*/

    //CResourceMgr ss;

    gtk_init(NULL, NULL);

    //ss.AddImage(L"D:\\my_program\\skin\\default\\dialog-button-normal.png");
    //ss.AddImage(L"D:\\my_program\\skin\\default\\dialog-button-pressed.png");
    //ss.AddImage(L"D:\\my_program\\skin\\default\\dialog-button-prelight.png");
    //CResourceMgr::GetInstance().AddImage(L"D:\\my_program\\skin\\default\\dialog-button-normal.png");
    //CResourceMgr::GetInstance().AddImage(L"D:\\my_program\\skin\\default\\dialog-button-pressed.png");
    //CResourceMgr::GetInstance().AddImage(L"D:\\my_program\\skin\\default\\dialog-button-prelight.png");

    CSkinWnd Window;
    //CResourceMgr::GetInstance().SetResourePath(L"/home/pgboy/qyplayer");
    CResourceMgr::GetInstance().SetResourePath(L"D:\\my_program\\skin\\qyplayer");

    Window.Create(0, 0, /*400*/800, /*500*/570);

    Window.CenterWindow();
    Window.Show();
    Window.MainLoop();

    //gtk_main_quit();

	return 0;
}

