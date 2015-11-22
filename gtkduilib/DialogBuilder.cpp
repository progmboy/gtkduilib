#include "stdafx.h"

#include "DialogBuilder.h"

CDialogBuilder::CDialogBuilder(void):m_callBack(NULL)
{
}

CDialogBuilder::CDialogBuilder(IDialogBuilderCallback *callback)
{
    m_callBack = callback;
}

CDialogBuilder::~CDialogBuilder(void)
{
}

CUIControl* CDialogBuilder::Create(const CUIString& strXmlPath, 
        CUIWindow* pRootDialog, CUIControl* pParent)
{
    //
    // generate the full path of xml file
    //

    CUIString strXmlFullPath = CResourceMgr::GetInstance().GetResourePath() + strXmlPath;
    
    //
    // load the xml file.
    //

    pugi::xml_parse_result result = m_doc.load_file(strXmlFullPath);
    if (!result){
        
        //
        // out error message
        //
        
        g_print("load xml file error\n");
        return NULL;
    }
    
    //
    // here xml load successfully
    //
    
    return Create(pRootDialog, pParent);
}

CUIControl* CDialogBuilder::Create(CUIWindow* pRootDialog, CUIControl* pParent)
{
    //
    // get root node.
    //

    pugi::xml_node node_win = m_doc.child(L"Window");
    if (!node_win){
        return NULL;
    }

    if (pRootDialog){

        //
        // parse window attribute
        //

        for (pugi::xml_attribute_iterator at = node_win.attributes_begin();
            at != node_win.attributes_end(); 
            at ++){

                //
                // check attribute by name.
                // 注意解析的时候 size --> sizebox --> mininfo --> maxinfo的顺序不能变
                // 因为gtk在设置不可resize的时候如果先设置了minifo和maxinfo则不会生效
                //

                LPCWSTR pszValue;
                CUIString strName;

                strName = at->name();
                pszValue = at->value();

                //
                // size - 窗口的初始化大小
                //

                if (strName == L"size"){
                    LPWSTR pstr = NULL;

                    int cx = wcstol(pszValue, &pstr, 10);    assert(pstr);    
                    int cy = wcstol(pstr + 1, &pstr, 10);    assert(pstr); 
                    pRootDialog->SetInitSize(CUISize(cx, cy));

                }

                //
                // sizebox - 窗口可拖动改变窗口大小的边距
                //

                else if (strName == L"sizebox"){
                    CUIRect rcSizeBox;
                    assert(ParseRectString(pszValue, rcSizeBox));

                    pRootDialog->SetSizeBox(rcSizeBox);
                }

                //
                // caption - 窗口可拖动的标题栏大小的边距
                // 最后一个参数是指离上边框的距离
                //

                else if (strName == L"caption"){
                    CUIRect rcCaption;
                    assert(ParseRectString(pszValue, rcCaption));
                    pRootDialog->SetCaptionRect(rcCaption);
                }

                //
                // roundcorner - 窗口圆角大小
                //

                else if (strName == L"roundcorner"){

                    LPWSTR pstr = NULL;
                    int cx = wcstol(pszValue, &pstr, 10);    assert(pstr);    
                    int cy = wcstol(pstr + 1, &pstr, 10);    assert(pstr); 

                    //
                    // TODO:  
                    //

                    //pRootDialog->SetRoundCorner(cx, cy);

                }

                //
                // mininfo - 窗口最小大小
                //

                else if (strName == L"mininfo"){

                    CUISize size;
                    assert(ParseSizeString(pszValue, size));
                    pRootDialog->SetMinSize(size);
                }

                //
                // maxinfo - 窗口最大大小
                //

                else if (strName == L"maxinfo"){
                    CUISize size;
                    assert(ParseSizeString(pszValue, size));
                    pRootDialog->SetMaxSize(size);
                }

                //
                // showdirty - 显示要绘制区域
                //

                else if (strName == L"showdirty"){
                    bool bSet = (wcscmp(pszValue, L"true") == 0);

                    //
                    // TODO:SetShowUpdateRect
                    //

                    //pRootDialog->SetShowUpdateRect(bSet)
                }

                //
                // alpha - 窗口的alpha值
                //

                else if (strName == L"alpha"){

                    //
                    // TODO:SetTransparent
                    //

                    //pRootDialog->SetTransparent(_wtoi(pstrValue));
                }

                else if (strName == L"bktrans"){

                    //
                    // TODO: SetBackgroundTransparent
                    //

                    bool bSet = (wcscmp(pszValue, L"true") == 0);
                    //pRootDialog->SetBackgroundTransparent(bSet);
                }

                else if (strName == L"disabledfontcolor"){
                    DWORD dwColor;
                    assert(ParseColorString(pszValue, dwColor));

                    //
                    // TODO: SetDefaultDisabledColor
                    //

                    //pRootDialog->SetDefaultDisabledColor(dwColor);
                }

                else if (strName == L"defaultfontcolor"){

                    DWORD dwColor;
                    assert(ParseColorString(pszValue, dwColor));

                    //
                    // TODO:SetDefaultFontColor
                    //

                    //pRootDialog->SetDefaultFontColor(dwColor);
                }

                else if (strName == L"linkfontcolor"){

                    DWORD dwColor;
                    assert(ParseColorString(pszValue, dwColor));

                    //
                    // TODO:SetDefaultLinkFontColor
                    //

                    //pRootDialog->SetDefaultLinkFontColor(dwColor);
                }

                else if (strName == L"linkhoverfontcolor"){

                    DWORD dwColor;
                    assert(ParseColorString(pszValue, dwColor));

                    //
                    // TODO:SetDefaultLinkFontColor
                    //

                    //pRootDialog->SetDefaultLinkHoverFontColor(dwColor);
                }

                else if (strName == L"selectedcolor"){

                    DWORD dwColor;
                    assert(ParseColorString(pszValue, dwColor));

                    //
                    // TODO:SetDefaultSelectedBkColor
                    //

                    //pRootDialog->SetDefaultSelectedBkColor(dwColor);
                }
        }

        //
        // parse font/image/default attribute
        //

        for (pugi::xml_node_iterator nt = node_win.begin();
            nt != node_win.end();
            nt ++){

                //
                // parse image node
                //

                CUIString strNodeName = nt->name();

                if (strNodeName == L"Image"){

                    //
                    // ignore that
                    //

                    NOTHING;

                }

                //
                // parse font.
                //

                else if (strNodeName == L"Font"){

                    LPWSTR pstr;
                    CUIString strFontName;
                    CUIString strFontFaceName;
                    int nSize = 12;
                    BOOL bBold = FALSE;
                    BOOL bUnderLine = FALSE;
                    BOOL bItalic = FALSE;
                    BOOL bDefault = FALSE;

                    //
                    // loop the attribute
                    //

                    for (pugi::xml_attribute_iterator at = nt->attributes_begin();
                        at != nt->attributes_end();
                        at ++){

                            //
                            // get name attribute
                            //

                            CUIString strAttribName = at->name();
                            LPCWSTR pszValue = at->value();

                            if (strAttribName == L"name"){
                                strFontName = pszValue;
                            }else if (strAttribName == L"facename"){
                                strFontFaceName = pszValue;
                            }else if (strAttribName == L"size"){
                                nSize = wcstol(pszValue, &pstr, 10);
                            }else if (strAttribName == L"bold"){
                                bBold = (wcscmp(pszValue, L"true") == 0);
                            }else if (strAttribName == L"underline"){
                                bUnderLine = (wcscmp(pszValue, L"true") == 0);
                            }else if (strAttribName == L"italic"){
                                bItalic = (wcscmp(pszValue, L"true") == 0);
                            }else if (strAttribName == L"default"){
                                bDefault = (wcscmp(pszValue, L"true") == 0);
                            }
                    }

                    CResourceMgr::GetInstance().AddFont(strFontName, strFontFaceName, 
                        bDefault, nSize, bBold, bUnderLine, bItalic);

                }

                //
                // parse default setting
                //

                else if (strNodeName == L"Default"){

                    CUIString strControlName;
                    CUIString strControlValue;

                    for (pugi::xml_attribute_iterator at = nt->attributes_begin();
                        at != nt->attributes_end();
                        at ++){

                            //
                            // get name attribute
                            //

                            CUIString strAttribName = at->name();
                            LPCWSTR pszValue = at->value();

                            if (strAttribName == L"name"){
                                strControlName = pszValue;
                            }

                            if (strAttribName == L"value"){
                                strControlValue = pszValue;
                            }
                    }

                    //
                    // save the default name to the window who own this control
                    //

                    pRootDialog->AddDefaultAttributeList(strControlName, strControlValue);

                }
        }
    }
    return ParseControl(node_win, pRootDialog, pParent);
}

CUIControl* CDialogBuilder::ParseControl(pugi::xml_node node, 
           CUIWindow* pRootDialog, CUIControl* pParent)
{
    CUIControl *pControl = NULL;
    for (pugi::xml_node nt = node.first_child(); nt; nt = nt.next_sibling()){

        CUIString strNodeName = nt.name();
        
        //
        // init pControl
        //
        
        pControl = NULL;

        //
        // skip the node we has parsed
        //
        
        if (strNodeName == L"Image" ||
            strNodeName == L"Font"  ||
            strNodeName == L"Default"){
            continue;
        }

        //
        // parse include node.
        //
        
        if (strNodeName == L"Include"){
            
            //
            // not implement
            //
            
            NOTHING;
        }
        
        //
        // create control by type
        //
        
        if (strNodeName == DUI_CTR_HORIZONTALLAYOUT){
            pControl = new CUIHorizontalLayout;
        }else if (strNodeName == DUI_CTR_VERTICALLAYOUT){
            pControl = new CUIVerticalLayout;
        }else if (strNodeName == DUI_CTR_TILELAYOUT){
            pControl = new CUITileLayout;
        }else if (strNodeName == DUI_CTR_TABLAYOUT){
            pControl = new CUITabLayout;
        }else if (strNodeName == DUI_CTR_CHILDLAYOUT){
            pControl = new CUIChildLayout;
        }else if (strNodeName == DUI_CTR_CONTAINER){
            pControl = new CUIContainer;
        }else if (strNodeName == DUI_CTR_CONTROL){
            pControl = new CUIControl;
        }else if (strNodeName == DUI_CTR_BUTTON){
            pControl = new CUIButton;
        }else if (strNodeName == DUI_CTR_LABEL){
            pControl = new CUILabel;
        }else if (strNodeName == DUI_CTR_TEXT){
            pControl = new CUIText;
        }else if (strNodeName == DUI_CTR_OPTION){
            pControl = new CUIOption;
        }else if (strNodeName == DUI_CTR_PROGRESS){
            pControl = new CUIProgress;
        }else if (strNodeName == DUI_CTR_SLIDER){
            pControl = new CUISlider;
        }else if (strNodeName == DUI_CTR_SCROLLBAR){
            pControl = new CUIScrollBar;
        }else if (strNodeName == DUI_CTR_LIST){
            pControl = new CListUI;
        }else if (strNodeName == DUI_CTR_LISTHEADER){
            pControl = new CListHeaderUI;
        }else if (strNodeName == DUI_CTR_LISTHEADERITEM){
            pControl = new CListHeaderItemUI;
        }else if (strNodeName == DUI_CTR_LISTCONTAINERELEMENT){
            pControl = new CListContainerElementUI;
        }
        
        //
        // create user self defined control
        //
        
        if (pControl == NULL && m_callBack){
            pControl = m_callBack->CreateControl(strNodeName);
        }

        if (strNodeName == DUI_CTR_CHILDLAYOUT){
            
            //
            // set callback
            //
            
            static_cast<CUIChildLayout*>(pControl)->SetBuilderCallback(m_callBack);
        }

        if (pControl == NULL){
            
            //
            // output err message
            //
            
            continue;
        }

        //
        // is control have child?
        //
        
        if (nt.first_child()){
            
            //
            // yes it has child so parse it
            //
            
            ParseControl(nt, pRootDialog, pControl);
        }

        //
        // attach child to parent
        //
        
        if (pParent){
            IContainerUI* pContainer = static_cast<IContainerUI*>(pParent->GetInterface(L"IContainer"));
            assert(pContainer);
            
            //
            // parent must be container
            //
            
            if (pContainer){
                if(!pContainer->Add(pControl)){
                    delete pControl;
                    continue;
                }
            }
        }


        if (pRootDialog){
            
            //
            // init default attributes.
            //

            LPCWSTR lpszDefaultAttribute;
            pControl->SetOwnWindow(pRootDialog, NULL, false);
            lpszDefaultAttribute = pRootDialog->GetDefaultAttributeList(strNodeName);
            if (lpszDefaultAttribute){
                pControl->ApplyAttributeList(lpszDefaultAttribute);
            }
        }
        
        for (pugi::xml_attribute_iterator at = nt.attributes_begin();
             at != nt.attributes_end();
             at ++){
            
            //
            // parse control's attributes
            //
            
            LPCWSTR szAttribName = at->name();
            LPCWSTR szAttribValue = at->value();

            pControl->SetAttribute(szAttribName, szAttribValue);
        }
        
        //
        // reset own window
        //
        
        if (pRootDialog){
            pControl->SetOwnWindow(NULL, NULL, false);
            NOTHING;
        }
    }

    return pControl;
}

void CDialogBuilder::SetCallback(IDialogBuilderCallback* callback)
{
    m_callBack = callback;
}