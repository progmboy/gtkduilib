#ifndef __DIALOGBUILDER__H__
#define __DIALOGBUILDER__H__

#include "pugixml.hpp"

class IDialogBuilderCallback
{
public:
    virtual CUIControl* CreateControl(const CUIString& strClass) = 0;
};

class CDialogBuilder
{
public:
    CDialogBuilder(void);
    CDialogBuilder(IDialogBuilderCallback *callback);
    virtual ~CDialogBuilder(void);

public:
    
    CUIControl* Create(const CUIString& strXmlPath, CUIWindow* pRootDialog = NULL, CUIControl* pParent = NULL);
    void SetCallback(IDialogBuilderCallback* callback);

private:
    CUIControl* Create(CUIWindow* pRootDialog = NULL, CUIControl* pParent = NULL);
    CUIControl* ParseControl(pugi::xml_node node, CUIWindow* pRootDialog, CUIControl* pParent);

private:
    IDialogBuilderCallback *m_callBack;
    pugi::xml_document m_doc;
};

#endif