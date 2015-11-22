#include "stdafx.h"
#include "UIChildLayout.h"

CUIChildLayout::CUIChildLayout():m_Callback(NULL)
{

}

void CUIChildLayout::Init()
{
	if (!m_pstrXMLFile.IsEmpty()){
        
        //
        // TODO CDialogBuilder
        //
        
		CDialogBuilder builder;
		CUIContainer* pChildWindow = static_cast<CUIContainer*>(builder.Create(m_pstrXMLFile.GetData(), m_Window, NULL));
		if (pChildWindow){
			this->Add(pChildWindow);
		}else{
			this->RemoveAll();
		}
		
		//
		// set callback to NULL
		//
		
		if (!m_Callback){
			m_Callback = NULL;
		}
	}
}

void CUIChildLayout::SetAttribute( LPCWSTR pstrName, LPCWSTR pstrValue )
{
	if(wcscmp(pstrName, L"xmlfile") == 0)
		SetChildLayoutXML(pstrValue);
	else
		CUIContainer::SetAttribute(pstrName,pstrValue);
}

void CUIChildLayout::SetChildLayoutXML(const CUIString& strXML)
{
	m_pstrXMLFile = strXML;
}

CUIString CUIChildLayout::GetChildLayoutXML()
{
	return m_pstrXMLFile;
}

LPVOID CUIChildLayout::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_CHILDLAYOUT) 
		return static_cast<CUIChildLayout*>(this);
	return CUIControl::GetInterface(strName);
}

CUIString CUIChildLayout::GetClass() const
{
	return L"ChildLayoutUI";
}


VOID CUIChildLayout::SetBuilderCallback(IDialogBuilderCallback* pCallback)
{
	m_Callback = pCallback;
}

