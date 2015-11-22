#include "stdafx.h"
#include "UITabLayout.h"

CUITabLayout::CUITabLayout() : m_iCurSel(-1)
{
}

CUIString CUITabLayout::GetClass() const
{
	return L"TabLayoutUI";
}

LPVOID CUITabLayout::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_TABLAYOUT) 
        return static_cast<CUITabLayout*>(this);
	return CUIContainer::GetInterface(strName);
}

bool CUITabLayout::Add(CUIControl* pControl)
{
	bool ret = CUIContainer::Add(pControl);
	if( !ret ) return ret;

	if(m_iCurSel == -1 && pControl->IsVisible())
	{
		m_iCurSel = GetItemIndex(pControl);
	}
	else
	{
		pControl->SetVisible(false);
	}

	return ret;
}

bool CUITabLayout::AddAt(CUIControl* pControl, int iIndex)
{
	bool ret = CUIContainer::AddAt(pControl, iIndex);
	if( !ret ) return ret;

	if(m_iCurSel == -1 && pControl->IsVisible())
	{
		m_iCurSel = GetItemIndex(pControl);
	}
	else if( m_iCurSel != -1 && iIndex <= m_iCurSel )
	{
		m_iCurSel += 1;
	}
	else
	{
		pControl->SetVisible(false);
	}

	return ret;
}

bool CUITabLayout::Remove(CUIControl* pControl)
{
	if( pControl == NULL) return false;

	int index = GetItemIndex(pControl);
	bool ret = CUIContainer::Remove(pControl);
	if( !ret ) return false;

	if( m_iCurSel == index)
	{
		if( GetCount() > 0 )
		{
			m_iCurSel=0;
			GetItemAt(m_iCurSel)->SetVisible(true);
		}
		else
			m_iCurSel=-1;
		NeedParentUpdate();
	}
	else if( m_iCurSel > index )
	{
		m_iCurSel -= 1;
	}

	return ret;
}

void CUITabLayout::RemoveAll()
{
	m_iCurSel = -1;
	CUIContainer::RemoveAll();
	NeedParentUpdate();
}

int CUITabLayout::GetCurSel() const
{
	return m_iCurSel;
}

bool CUITabLayout::SelectItem(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_items.GetSize()) 
        return false;
	if(iIndex == m_iCurSel) 
        return true;

	int iOldSel = m_iCurSel;
	m_iCurSel = iIndex;
	for(int it = 0; it < m_items.GetSize(); it++){
		if(it == iIndex){
			GetItemAt(it)->SetVisible(true);
            
            //
            // TODO SetFocus
            //
            
			//GetItemAt(it)->SetFocus();
			SetPos(m_rcItem);
		}else 
            GetItemAt(it)->SetVisible(false);
	}
	NeedParentUpdate();

	if(m_Window){
        
        //
        // TODO: SetNextTabControl
        //
        
		//m_Window->SetNextTabControl();
		m_Window->SendNotify(this, DUI_MSGTYPE_TABSELECT, (WPARAM)m_iCurSel, (LPARAM)iOldSel);
	}
	return true;
}

bool CUITabLayout::SelectItem(CUIControl* pControl)
{
	int iIndex=GetItemIndex(pControl);
	if (iIndex==-1)
		return false;
	else
		return SelectItem(iIndex);
}

void CUITabLayout::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if(wcscmp(pstrName, L"selectedid") == 0)
        SelectItem(/*_wtoi(pstrValue)*/ wcstol(pstrValue, NULL, 10));
	return
        CUIContainer::SetAttribute(pstrName, pstrValue);
}

void CUITabLayout::SetPos(const CUIRect& rc)
{
    CUIRect rcTemp = rc;
	CUIControl::SetPos(rc);
	rcTemp = m_rcItem;
    
    //
	// Adjust for inset
	//

    rcTemp.left += m_rcInset.left;
	rcTemp.top += m_rcInset.top;
	rcTemp.right -= m_rcInset.right;
	rcTemp.bottom -= m_rcInset.bottom;

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it);
			continue;
		}

		if( it != m_iCurSel ) 
			continue;

		RECT rcPadding = pControl->GetPadding();
		rcTemp.left += rcPadding.left;
		rcTemp.top += rcPadding.top;
		rcTemp.right -= rcPadding.right;
		rcTemp.bottom -= rcPadding.bottom;

		SIZE szAvailable = {rcTemp.GetWidth(), rcTemp.GetHeight()};

		SIZE sz = pControl->EstimateSize(szAvailable);
		if( sz.cx == 0 ) {
			sz.cx = MAX(0, szAvailable.cx);
		}
		if( sz.cx < pControl->GetMinWidth() ) 
			sz.cx = pControl->GetMinWidth();
		if( sz.cx > pControl->GetMaxWidth() ) 
			sz.cx = pControl->GetMaxWidth();

		if(sz.cy == 0) {
			sz.cy = MAX(0, szAvailable.cy);
		}
		if( sz.cy < pControl->GetMinHeight() ) 
			sz.cy = pControl->GetMinHeight();
		if( sz.cy > pControl->GetMaxHeight() ) 
			sz.cy = pControl->GetMaxHeight();

		CUIRect rcCtrl(rcTemp.left, rcTemp.top, rcTemp.left + sz.cx, rcTemp.top + sz.cy);
		pControl->SetPos(rcCtrl);
	}
}
