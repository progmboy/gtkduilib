#include "stdafx.h"

CListUI::CListUI() : m_pCallback(NULL), m_bScrollSelect(false), m_iCurSel(-1), m_iExpandedItem(-1)
{
    m_pList = new CListBodyUI(this);
    m_pHeader = new CListHeaderUI;

    Add(m_pHeader);
    CUIVerticalLayout::Add(m_pList);

    m_ListInfo.nColumns = 0;
    m_ListInfo.strFontName.Empty();
    m_ListInfo.uTextStyle = DT_VCENTER; // m_uTextStyle(DT_VCENTER | DT_END_ELLIPSIS)
    m_ListInfo.dwTextColor = 0xFF000000;
    m_ListInfo.dwBkColor = 0;
    m_ListInfo.bAlternateBk = false;
    m_ListInfo.dwSelectedTextColor = 0xFF000000;
    m_ListInfo.dwSelectedBkColor = 0xFFC1E3FF;
    m_ListInfo.dwHotTextColor = 0xFF000000;
    m_ListInfo.dwHotBkColor = 0xFFE9F5FF;
    m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
    m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
    m_ListInfo.dwLineColor = 0;
    m_ListInfo.bShowHtml = false;
    m_ListInfo.bMultiExpandable = false;
}

CUIString CListUI::GetClass() const
{
    return L"ListUI";
}

UINT CListUI::GetControlFlags() const
{
    return UIFLAG_TABSTOP;
}

LPVOID CListUI::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_LIST)
		return static_cast<CListUI*>(this);

    if(strName == L"IList")
		return static_cast<IListUI*>(this);

    if(strName == L"IListOwner")
		return static_cast<IListOwnerUI*>(this);

    return CUIVerticalLayout::GetInterface(strName);
}

CUIControl* CListUI::GetItemAt(int iIndex) const
{
    return m_pList->GetItemAt(iIndex);
}

int CListUI::GetItemIndex(CUIControl* pControl) const
{
    if( pControl->GetInterface(L"ListHeader") != NULL )
		return CUIVerticalLayout::GetItemIndex(pControl);

	//
    // We also need to recognize header sub-items
    //

	if(pControl->GetClass().Find(L"ListHeaderItemUI") != -1)
		return m_pHeader->GetItemIndex(pControl);

    return m_pList->GetItemIndex(pControl);
}

bool CListUI::SetItemIndex(CUIControl* pControl, int iIndex)
{
    if( pControl->GetInterface(L"ListHeader") != NULL )
		return CUIVerticalLayout::SetItemIndex(pControl, iIndex);

	//
    // We also need to recognize header sub-items
    //

    if(pControl->GetClass().Find(L"ListHeaderItemUI") != -1)
		return m_pHeader->SetItemIndex(pControl, iIndex);

    int iOrginIndex = m_pList->GetItemIndex(pControl);
    if(iOrginIndex == -1)
		return false;
    if(iOrginIndex == iIndex)
		return true;

    IListItemUI* pSelectedListItem = NULL;
    if( m_iCurSel >= 0 )
		pSelectedListItem = static_cast<IListItemUI*>(GetItemAt(m_iCurSel)->GetInterface(L"ListItem"));
    if( !m_pList->SetItemIndex(pControl, iIndex) )
		return false;

    int iMinIndex = MIN(iOrginIndex, iIndex);
    int iMaxIndex = MAX(iOrginIndex, iIndex);
    for(int i = iMinIndex; i < iMaxIndex + 1; ++i) {
        CUIControl* p = m_pList->GetItemAt(i);
        IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(L"ListItem"));
        if( pListItem != NULL ) {
            pListItem->SetIndex(i);
        }
    }
    if( m_iCurSel >= 0 && pSelectedListItem != NULL )
		m_iCurSel = pSelectedListItem->GetIndex();

    return true;
}

int CListUI::GetCount() const
{
    return m_pList->GetCount();
}

bool CListUI::Add(CUIControl* pControl)
{
	//
    // Override the Add() method so we can add items specifically to
    // the intended widgets. Headers are assumed to be
    // answer the correct interface so we can add multiple list headers.
    //

	if( pControl->GetInterface(L"ListHeader") != NULL ) {
        if( m_pHeader != pControl && m_pHeader->GetCount() == 0 ) {
            CUIVerticalLayout::Remove(m_pHeader);
            m_pHeader = static_cast<CListHeaderUI*>(pControl);
        }
        m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
        return CUIVerticalLayout::AddAt(pControl, 0);
    }

	//
    // We also need to recognize header sub-items
    //

	if(pControl->GetClass().Find(L"ListHeaderItemUI") != -1) {
        bool ret = m_pHeader->Add(pControl);
        m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
        return ret;
    }

	//
    // The list items should know about us
    //

	IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
    if( pListItem != NULL ) {
        pListItem->SetOwner(this);
        pListItem->SetIndex(GetCount());
    }
    return m_pList->Add(pControl);
}

bool CListUI::AddAt(CUIControl* pControl, int iIndex)
{
	//
    // Override the AddAt() method so we can add items specifically to
    // the intended widgets. Headers and are assumed to be
    // answer the correct interface so we can add multiple list headers.
    //

	if( pControl->GetInterface(L"ListHeader") != NULL ) {
        if( m_pHeader != pControl && m_pHeader->GetCount() == 0 ) {
            CUIVerticalLayout::Remove(m_pHeader);
            m_pHeader = static_cast<CListHeaderUI*>(pControl);
        }
        m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
        return CUIVerticalLayout::AddAt(pControl, 0);
    }

	//
    // We also need to recognize header sub-items
    //

	if(pControl->GetClass().Find(L"ListHeaderItemUI") != -1){
        bool ret = m_pHeader->AddAt(pControl, iIndex);
        m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
        return ret;
    }
    if (!m_pList->AddAt(pControl, iIndex))
		return false;

	//
    // The list items should know about us
    //

	IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
    if( pListItem != NULL ) {
        pListItem->SetOwner(this);
        pListItem->SetIndex(iIndex);
    }

    for(int i = iIndex + 1; i < m_pList->GetCount(); ++i) {
        CUIControl* p = m_pList->GetItemAt(i);
        pListItem = static_cast<IListItemUI*>(p->GetInterface(L"ListItem"));
        if( pListItem != NULL ) {
            pListItem->SetIndex(i);
        }
    }
    if( m_iCurSel >= iIndex )
		m_iCurSel += 1;
    return true;
}

bool CListUI::Remove(CUIControl* pControl)
{
    if( pControl->GetInterface(L"ListHeader") != NULL )
		return CUIVerticalLayout::Remove(pControl);

	//
    // We also need to recognize header sub-items
    //

	if(pControl->GetClass().Find(L"ListHeaderItemUI") != -1)
		return m_pHeader->Remove(pControl);

    int iIndex = m_pList->GetItemIndex(pControl);
    if (iIndex == -1)
		return false;

    if (!m_pList->RemoveAt(iIndex))
		return false;

    for(int i = iIndex; i < m_pList->GetCount(); ++i) {
        CUIControl* p = m_pList->GetItemAt(i);
        IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(L"ListItem"));
        if( pListItem != NULL ) {
            pListItem->SetIndex(i);
        }
    }

    if( iIndex == m_iCurSel && m_iCurSel >= 0 ) {
        int iSel = m_iCurSel;
        m_iCurSel = -1;
        SelectItem(FindSelectable(iSel, false));
    }else if( iIndex < m_iCurSel )
		m_iCurSel -= 1;
    return true;
}

bool CListUI::RemoveAt(int iIndex)
{
    if (!m_pList->RemoveAt(iIndex)) return false;

    for(int i = iIndex; i < m_pList->GetCount(); ++i) {
        CUIControl* p = m_pList->GetItemAt(i);
        IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(L"ListItem"));
        if( pListItem != NULL )
			pListItem->SetIndex(i);
    }

    if( iIndex == m_iCurSel && m_iCurSel >= 0 ) {
        int iSel = m_iCurSel;
        m_iCurSel = -1;
        SelectItem(FindSelectable(iSel, false));
    }
    else if( iIndex < m_iCurSel )
		m_iCurSel -= 1;
    return true;
}

void CListUI::RemoveAll()
{
    m_iCurSel = -1;
    m_iExpandedItem = -1;
    m_pList->RemoveAll();
}

void CListUI::SetPos(RECT rc)
{
    CUIVerticalLayout::SetPos(rc);
    if( m_pHeader == NULL )
		return;

	//
    // Determine general list information and the size of header columns
    //

	m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);

	//
	// The header/columns may or may not be visible at runtime. In either case
    // we should determine the correct dimensions...
	//

    if( !m_pHeader->IsVisible() ) {
        for( int it = 0; it < m_pHeader->GetCount(); it++ ) {

            //
            // TODO SetInternVisible
            //

            //static_cast<CUIControl*>(m_pHeader->GetItemAt(it))->SetInternVisible(true);
        }
        m_pHeader->SetPos(CUIRect(rc.left, 0, rc.right, 0));
    }
    int iOffset = m_pList->GetScrollPos().cx;
    for( int i = 0; i < m_ListInfo.nColumns; i++ ) {
        CUIControl* pControl = static_cast<CUIControl*>(m_pHeader->GetItemAt(i));
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) continue;

        RECT rcPos = pControl->GetPos();
        if( iOffset > 0 ) {
            rcPos.left -= iOffset;
            rcPos.right -= iOffset;
            pControl->SetPos(rcPos);
        }
        m_ListInfo.rcColumn[i] = pControl->GetPos();
    }
    if( !m_pHeader->IsVisible() ) {
        for( int it = 0; it < m_pHeader->GetCount(); it++ ) {

            //
            // TODO: SetInternVisible
            //

            //static_cast<CUIControl*>(m_pHeader->GetItemAt(it))->SetInternVisible(false);
        }
    }
}

void CListUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_Parent != NULL ) m_Parent->DoEvent(event);
        else CUIVerticalLayout::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ){
        m_bFocused = true;
        return;
    }

    if( event.Type == UIEVENT_KILLFOCUS ){
        m_bFocused = false;
        return;
    }

    switch( event.Type ) {
    case UIEVENT_KEYDOWN:
        switch( event.chKey ) {
        case VK_UP:
            SelectItem(FindSelectable(m_iCurSel - 1, false), true);
            return;
        case VK_DOWN:
            SelectItem(FindSelectable(m_iCurSel + 1, true), true);
            return;
        case VK_PRIOR:
            PageUp();
            return;
        case VK_NEXT:
            PageDown();
            return;
        case VK_HOME:
            SelectItem(FindSelectable(0, false), true);
            return;
        case VK_END:
            SelectItem(FindSelectable(GetCount() - 1, true), true);
            return;
        case VK_RETURN:
            if( m_iCurSel != -1 ) GetItemAt(m_iCurSel)->Activate();
            return;
            }
        break;
    case UIEVENT_SCROLLWHEEL:
        {
            switch( LOWORD(event.wParam) ) {
            case SB_LINEUP:
                if( m_bScrollSelect )
					SelectItem(FindSelectable(m_iCurSel - 1, false), true);
                else
					LineUp();
                return;
            case SB_LINEDOWN:
                if( m_bScrollSelect )
					SelectItem(FindSelectable(m_iCurSel + 1, true), true);
                else
					LineDown();
                return;
            }
        }
        break;
    }
    CUIVerticalLayout::DoEvent(event);
}

CListHeaderUI* CListUI::GetHeader() const
{
    return m_pHeader;
}

CUIContainer* CListUI::GetList() const
{
    return m_pList;
}

bool CListUI::GetScrollSelect()
{
    return m_bScrollSelect;
}

void CListUI::SetScrollSelect(bool bScrollSelect)
{
    m_bScrollSelect = bScrollSelect;
}

int CListUI::GetCurSel() const
{
    return m_iCurSel;
}

bool CListUI::SelectItem(int iIndex, bool bTakeFocus)
{
    if( iIndex == m_iCurSel ) return true;

    int iOldSel = m_iCurSel;

	//
    // We should first unselect the currently selected item
    //

	if( m_iCurSel >= 0 ) {
        CUIControl* pControl = GetItemAt(m_iCurSel);
        if( pControl != NULL) {
            IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
            if( pListItem != NULL )
				pListItem->Select(false);
        }

        m_iCurSel = -1;
    }
    if( iIndex < 0 )
		return false;

    CUIControl* pControl = GetItemAt(iIndex);
    if( pControl == NULL )
		return false;
    if( !pControl->IsVisible() )
		return false;
    if( !pControl->IsEnabled() )
		return false;

    IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
    if( pListItem == NULL )
		return false;
    m_iCurSel = iIndex;
    if( !pListItem->Select(true) ) {
        m_iCurSel = -1;
        return false;
    }

    EnsureVisible(m_iCurSel);

    //
    // TODO SetFocus
    //

    //if(bTakeFocus)
	//	pControl->SetFocus();

    if(m_Window != NULL){
        m_Window->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, (WPARAM)m_iCurSel, (LPARAM)iOldSel);
    }

    return true;
}

TListInfoUI* CListUI::GetListInfo()
{
    return &m_ListInfo;
}

int CListUI::GetChildPadding() const
{
    return m_pList->GetChildPadding();
}

void CListUI::SetChildPadding(int iPadding)
{
    m_pList->SetChildPadding(iPadding);
}

void CListUI::SetItemFont(const CUIString& strFontName)
{
    m_ListInfo.strFontName = strFontName;
    NeedUpdate();
}

void CListUI::SetItemTextStyle(UINT uStyle)
{
    m_ListInfo.uTextStyle = uStyle;
    NeedUpdate();
}

void CListUI::SetItemTextPadding(RECT rc)
{
    m_ListInfo.rcTextPadding = rc;
    NeedUpdate();
}

RECT CListUI::GetItemTextPadding() const
{
	return m_ListInfo.rcTextPadding;
}

void CListUI::SetItemTextColor(DWORD dwTextColor)
{
    m_ListInfo.dwTextColor = dwTextColor;
    Invalidate();
}

void CListUI::SetItemBkColor(DWORD dwBkColor)
{
    m_ListInfo.dwBkColor = dwBkColor;
    Invalidate();
}

void CListUI::SetItemBkImage(const CUIString& strImage)
{
    m_ListInfo.sBkImage = strImage;
    Invalidate();
}

void CListUI::SetAlternateBk(bool bAlternateBk)
{
    m_ListInfo.bAlternateBk = bAlternateBk;
    Invalidate();
}

DWORD CListUI::GetItemTextColor() const
{
	return m_ListInfo.dwTextColor;
}

DWORD CListUI::GetItemBkColor() const
{
	return m_ListInfo.dwBkColor;
}

CUIString CListUI::GetItemBkImage() const
{
	return m_ListInfo.sBkImage;
}

bool CListUI::IsAlternateBk() const
{
    return m_ListInfo.bAlternateBk;
}

void CListUI::SetSelectedItemTextColor(DWORD dwTextColor)
{
    m_ListInfo.dwSelectedTextColor = dwTextColor;
    Invalidate();
}

void CListUI::SetSelectedItemBkColor(DWORD dwBkColor)
{
    m_ListInfo.dwSelectedBkColor = dwBkColor;
    Invalidate();
}

void CListUI::SetSelectedItemImage(const CUIString& strImage)
{
    m_ListInfo.sSelectedImage = strImage;
    Invalidate();
}

DWORD CListUI::GetSelectedItemTextColor() const
{
	return m_ListInfo.dwSelectedTextColor;
}

DWORD CListUI::GetSelectedItemBkColor() const
{
	return m_ListInfo.dwSelectedBkColor;
}

CUIString CListUI::GetSelectedItemImage() const
{
	return m_ListInfo.sSelectedImage;
}

void CListUI::SetHotItemTextColor(DWORD dwTextColor)
{
    m_ListInfo.dwHotTextColor = dwTextColor;
    Invalidate();
}

void CListUI::SetHotItemBkColor(DWORD dwBkColor)
{
    m_ListInfo.dwHotBkColor = dwBkColor;
    Invalidate();
}

void CListUI::SetHotItemImage(const CUIString& strImage)
{
    m_ListInfo.sHotImage = strImage;
    Invalidate();
}

DWORD CListUI::GetHotItemTextColor() const
{
	return m_ListInfo.dwHotTextColor;
}
DWORD CListUI::GetHotItemBkColor() const
{
	return m_ListInfo.dwHotBkColor;
}

CUIString CListUI::GetHotItemImage() const
{
	return m_ListInfo.sHotImage;
}

void CListUI::SetDisabledItemTextColor(DWORD dwTextColor)
{
    m_ListInfo.dwDisabledTextColor = dwTextColor;
    Invalidate();
}

void CListUI::SetDisabledItemBkColor(DWORD dwBkColor)
{
    m_ListInfo.dwDisabledBkColor = dwBkColor;
    Invalidate();
}

void CListUI::SetDisabledItemImage(const CUIString& strImage)
{
    m_ListInfo.sDisabledImage = strImage;
    Invalidate();
}

DWORD CListUI::GetDisabledItemTextColor() const
{
	return m_ListInfo.dwDisabledTextColor;
}

DWORD CListUI::GetDisabledItemBkColor() const
{
	return m_ListInfo.dwDisabledBkColor;
}

CUIString CListUI::GetDisabledItemImage() const
{
	return m_ListInfo.sDisabledImage;
}

DWORD CListUI::GetItemLineColor() const
{
	return m_ListInfo.dwLineColor;
}

void CListUI::SetItemLineColor(DWORD dwLineColor)
{
    m_ListInfo.dwLineColor = dwLineColor;
    Invalidate();
}

bool CListUI::IsItemShowHtml()
{
    return m_ListInfo.bShowHtml;
}

void CListUI::SetItemShowHtml(bool bShowHtml)
{
    if( m_ListInfo.bShowHtml == bShowHtml ) return;

    m_ListInfo.bShowHtml = bShowHtml;
    NeedUpdate();
}

void CListUI::SetMultiExpanding(bool bMultiExpandable)
{
    m_ListInfo.bMultiExpandable = bMultiExpandable;
}

bool CListUI::ExpandItem(int iIndex, bool bExpand /*= true*/)
{
    if( m_iExpandedItem >= 0 && !m_ListInfo.bMultiExpandable) {
        CUIControl* pControl = GetItemAt(m_iExpandedItem);
        if( pControl != NULL ) {
            IListItemUI* pItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
            if( pItem != NULL ) pItem->Expand(false);
        }
        m_iExpandedItem = -1;
    }
    if( bExpand ) {
        CUIControl* pControl = GetItemAt(iIndex);
        if( pControl == NULL ) return false;
        if( !pControl->IsVisible() ) return false;
        IListItemUI* pItem = static_cast<IListItemUI*>(pControl->GetInterface(L"ListItem"));
        if( pItem == NULL ) return false;
        m_iExpandedItem = iIndex;
        if( !pItem->Expand(true) ) {
            m_iExpandedItem = -1;
            return false;
        }
    }
    NeedUpdate();
    return true;
}

int CListUI::GetExpandedItem() const
{
    return m_iExpandedItem;
}

void CListUI::EnsureVisible(int iIndex)
{
    if( m_iCurSel < 0 ) return;
    RECT rcItem = m_pList->GetItemAt(iIndex)->GetPos();
    RECT rcList = m_pList->GetPos();
    RECT rcListInset = m_pList->GetInset();

    rcList.left += rcListInset.left;
    rcList.top += rcListInset.top;
    rcList.right -= rcListInset.right;
    rcList.bottom -= rcListInset.bottom;

    CUIScrollBar* pHorizontalScrollBar = m_pList->GetHorizontalScrollBar();
    if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();

    int iPos = m_pList->GetScrollPos().cy;
    if( rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom ) return;
    int dx = 0;
    if( rcItem.top < rcList.top ) dx = rcItem.top - rcList.top;
    if( rcItem.bottom > rcList.bottom ) dx = rcItem.bottom - rcList.bottom;
    Scroll(0, dx);
}

void CListUI::Scroll(int dx, int dy)
{
    if( dx == 0 && dy == 0 ) return;
    SIZE sz = m_pList->GetScrollPos();
    m_pList->SetScrollPos(CUISize(sz.cx + dx, sz.cy + dy));
}

void CListUI::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
    CUIString strName = pstrName;
    if(strName == L"header"){
        GetHeader()->SetVisible(wcscmp(pstrValue, L"hidden") != 0);
    }else if(strName == L"headerbkimage"){
        GetHeader()->SetBkImage(pstrValue);
    }else if(strName == L"scrollselect"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetScrollSelect(bSet);
    }else if(strName == L"multiexpanding"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetMultiExpanding(bSet);
    }else if(strName == L"itemfont"){
        m_ListInfo.strFontName = pstrValue;
    }else if(strName == L"itemalign"){
        if(wcsstr(pstrValue, L"left") != NULL){
            m_ListInfo.uTextStyle &= ~(DT_CENTER | DT_RIGHT);
            m_ListInfo.uTextStyle |= DT_LEFT;
        }
        if(wcsstr(pstrValue, L"center") != NULL){
            m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_RIGHT);
            m_ListInfo.uTextStyle |= DT_CENTER;
        }
        if(wcsstr(pstrValue, L"right") != NULL){
            m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_CENTER);
            m_ListInfo.uTextStyle |= DT_RIGHT;
        }
    }
    else if(strName == L"itemendellipsis"){
        if(wcscmp(pstrValue, L"true") == 0)
            m_ListInfo.uTextStyle |= DT_END_ELLIPSIS;
        else
            m_ListInfo.uTextStyle &= ~DT_END_ELLIPSIS;
    }
    if(strName == L"itemtextpadding"){
        CUIRect rcTextPadding;
        assert(ParseRectString(pstrValue, rcTextPadding));
        SetItemTextPadding(rcTextPadding);

    }
    else if(strName == L"itemtextcolor") {
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetItemTextColor(dwColor);
    }
    else if(strName == L"itembkcolor"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetItemBkColor(dwColor);
    }
    else if(strName == L"itembkimage"){
        SetItemBkImage(pstrValue);
    }
    else if(strName == L"itemaltbk"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetAlternateBk(bSet);
    }
    else if(strName == L"itemselectedtextcolor" ) {
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetSelectedItemTextColor(dwColor);
    }
    else if(strName == L"itemselectedbkcolor") {
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetSelectedItemBkColor(dwColor);
    }
    else if(strName == L"itemselectedimage"){
        SetSelectedItemImage(pstrValue);
    }
    else if(strName == L"itemhottextcolor"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetHotItemTextColor(dwColor);
    }
    else if( strName == L"itemhotbkcolor" ){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetHotItemBkColor(dwColor);

    }
    else if( strName == L"itemhotimage" ){
        SetHotItemImage(pstrValue);
    }
    else if( strName == L"itemdisabledtextcolor" ) {
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetDisabledItemTextColor(dwColor);
    }
    else if( strName == L"itemdisabledbkcolor" ) {
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetDisabledItemBkColor(dwColor);
    }
    else if( strName == L"itemdisabledimage" ){
        SetDisabledItemImage(pstrValue);
    }
    else if( strName == L"itemlinecolor" ){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetItemLineColor(dwColor);
    }
    else if(strName == L"itemshowhtml"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetItemShowHtml(bSet);
    }
    else
        CUIVerticalLayout::SetAttribute(pstrName, pstrValue);
}

IListCallbackUI* CListUI::GetTextCallback() const
{
    return m_pCallback;
}

void CListUI::SetTextCallback(IListCallbackUI* pCallback)
{
    m_pCallback = pCallback;
}

SIZE CListUI::GetScrollPos() const
{
    return m_pList->GetScrollPos();
}

SIZE CListUI::GetScrollRange() const
{
    return m_pList->GetScrollRange();
}

void CListUI::SetScrollPos(SIZE szPos)
{
    m_pList->SetScrollPos(szPos);
}

void CListUI::LineUp()
{
    m_pList->LineUp();
}

void CListUI::LineDown()
{
    m_pList->LineDown();
}

void CListUI::PageUp()
{
    m_pList->PageUp();
}

void CListUI::PageDown()
{
    m_pList->PageDown();
}

void CListUI::HomeUp()
{
    m_pList->HomeUp();
}

void CListUI::EndDown()
{
    m_pList->EndDown();
}

void CListUI::LineLeft()
{
    m_pList->LineLeft();
}

void CListUI::LineRight()
{
    m_pList->LineRight();
}

void CListUI::PageLeft()
{
    m_pList->PageLeft();
}

void CListUI::PageRight()
{
    m_pList->PageRight();
}

void CListUI::HomeLeft()
{
    m_pList->HomeLeft();
}

void CListUI::EndRight()
{
    m_pList->EndRight();
}

void CListUI::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
{
    m_pList->EnableScrollBar(bEnableVertical, bEnableHorizontal);
}

CUIScrollBar* CListUI::GetVerticalScrollBar() const
{
    return m_pList->GetVerticalScrollBar();
}

CUIScrollBar* CListUI::GetHorizontalScrollBar() const
{
    return m_pList->GetHorizontalScrollBar();
}

BOOL CListUI::SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
{
	if (!m_pList)
		return FALSE;
	return m_pList->SortItems(pfnCompare, dwData);
}
/////////////////////////////////////////////////////////////////////////////////////
//
//


CListBodyUI::CListBodyUI(CListUI* pOwner) : m_pOwner(pOwner)
{
    assert(m_pOwner);
}

BOOL CListBodyUI::SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
{
	if (!pfnCompare)
		return FALSE;
	m_pCompareFunc = pfnCompare;
	CUIControl **pData = (CUIControl **)m_items.GetData();
	#ifdef WIN32
	qsort_s(m_items.GetData(), m_items.GetSize(), sizeof(CUIControl*), CListBodyUI::ItemComareFunc, this);
	#else
	qsort_r(m_items.GetData(), m_items.GetSize(), sizeof(CUIControl*), CListBodyUI::ItemComareFunc, this);
	#endif
	IListItemUI *pItem = NULL;
	for (int i = 0; i < m_items.GetSize(); ++i){
		pItem = (IListItemUI*)(static_cast<CUIControl*>(m_items[i])->GetInterface(L"ListItem"));
		if (pItem)
		{
			pItem->SetIndex(i);
			pItem->Select(false);
		}
	}
	m_pOwner->SelectItem(-1);
	if (m_Window){
		SetPos(GetPos());
		Invalidate();
	}

	return TRUE;
}

int __cdecl CListBodyUI::ItemComareFunc(void *pvlocale, const void *item1, const void *item2)
{
	CListBodyUI *pThis = (CListBodyUI*)pvlocale;
	if (!pThis || !item1 || !item2)
		return 0;
	return pThis->ItemComareFunc(item1, item2);
}

int __cdecl CListBodyUI::ItemComareFunc(const void *item1, const void *item2)
{
	CUIControl *pControl1 = *(CUIControl**)item1;
	CUIControl *pControl2 = *(CUIControl**)item2;
	return m_pCompareFunc((UINT_PTR)pControl1, (UINT_PTR)pControl2, m_compareData);
}

void CListBodyUI::SetScrollPos(SIZE szPos)
{
    int cx = 0;
    int cy = 0;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
        m_pVerticalScrollBar->SetScrollPos(szPos.cy);
        cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
    }

    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
        m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
        cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
    }

    if( cx == 0 && cy == 0 ) return;

    RECT rcPos;
    for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
        CUIControl* pControl = static_cast<CUIControl*>(m_items[it2]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) continue;

        rcPos = pControl->GetPos();
        rcPos.left -= cx;
        rcPos.right -= cx;
        rcPos.top -= cy;
        rcPos.bottom -= cy;
        pControl->SetPos(rcPos);
    }

    Invalidate();

    if( cx != 0 && m_pOwner ) {
        CListHeaderUI* pHeader = m_pOwner->GetHeader();
        if( pHeader == NULL ) return;
        TListInfoUI* pInfo = m_pOwner->GetListInfo();
        pInfo->nColumns = MIN(pHeader->GetCount(), UILIST_MAX_COLUMNS);

        if( !pHeader->IsVisible() ) {
            for( int it = 0; it < pHeader->GetCount(); it++ ) {

                //
                // TODO SetInternVisible
                //

                //static_cast<CUIControl*>(pHeader->GetItemAt(it))->SetInternVisible(true);
            }
        }
        for( int i = 0; i < pInfo->nColumns; i++ ) {
            CUIControl* pControl = static_cast<CUIControl*>(pHeader->GetItemAt(i));
            if( !pControl->IsVisible() ) continue;
            if( pControl->IsFloat() ) continue;

            RECT rcPos = pControl->GetPos();
            rcPos.left -= cx;
            rcPos.right -= cx;
            pControl->SetPos(rcPos);
            pInfo->rcColumn[i] = pControl->GetPos();
        }
        if( !pHeader->IsVisible() ) {
            for( int it = 0; it < pHeader->GetCount(); it++ ) {

                //
                // TODO SetInternVisible
                //

                //static_cast<CUIControl*>(pHeader->GetItemAt(it))->SetInternVisible(false);
            }
        }
    }
}

void CListBodyUI::SetPos(RECT rc)
{
    CUIControl::SetPos(rc);
    rc = m_rcItem;

	//
    // Adjust for inset
    //

	rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() )
		rc.right -= m_pVerticalScrollBar->GetFixedWidth();
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() )
		rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	//
    // Determine the minimum size
    //

	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() )
        szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

    int cxNeeded = 0;
    int nAdjustables = 0;
    int cyFixed = 0;
    int nEstimateNum = 0;
    for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
        CUIControl* pControl = static_cast<CUIControl*>(m_items[it1]);
        if( !pControl->IsVisible() )
			continue;
        if( pControl->IsFloat() )
			continue;

        SIZE sz = pControl->EstimateSize(szAvailable);
        if( sz.cy == 0 ) {
            nAdjustables++;
        }else{
            if( sz.cy < pControl->GetMinHeight() )
				sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() )
				sz.cy = pControl->GetMaxHeight();
        }
        cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;

        RECT rcPadding = pControl->GetPadding();
        sz.cx = MAX(sz.cx, 0);
        if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
        if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
        cxNeeded = MAX(cxNeeded, sz.cx);
        nEstimateNum++;
    }
    cyFixed += (nEstimateNum - 1) * m_iChildPadding;

    if( m_pOwner ) {
        CListHeaderUI* pHeader = m_pOwner->GetHeader();
        if( pHeader != NULL && pHeader->GetCount() > 0 ) {
            cxNeeded = MAX(0, pHeader->EstimateSize(CUISize(rc.right - rc.left, rc.bottom - rc.top)).cx);
        }
    }

	//
    // Place elements
    //

	int cyNeeded = 0;
    int cyExpand = 0;
    if( nAdjustables > 0 )
		cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);

	//
	// Position the elements
    //

	SIZE szRemaining = szAvailable;
    int iPosY = rc.top;
    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
        iPosY -= m_pVerticalScrollBar->GetScrollPos();
    }
    int iPosX = rc.left;
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
        iPosX -= m_pHorizontalScrollBar->GetScrollPos();
    }
    int iAdjustable = 0;
    int cyFixedRemaining = cyFixed;
    for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
        CUIControl* pControl = static_cast<CUIControl*>(m_items[it2]);
        if( !pControl->IsVisible() ) continue;
        if( pControl->IsFloat() ) {
            SetFloatPos(it2);
            continue;
        }

        RECT rcPadding = pControl->GetPadding();
        szRemaining.cy -= rcPadding.top;
        SIZE sz = pControl->EstimateSize(szRemaining);
        if( sz.cy == 0 ) {
            iAdjustable++;
            sz.cy = cyExpand;

			//
            // Distribute remaining to last element (usually round-off left-overs)
            //

			if( iAdjustable == nAdjustables ) {
                sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
            }
            if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
        }
        else {
            if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
            if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
            cyFixedRemaining -= sz.cy;
        }

        sz.cx = MAX(cxNeeded, szAvailable.cx - rcPadding.left - rcPadding.right);

        if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
        if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

        RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top + rcPadding.bottom };
        pControl->SetPos(rcCtrl);

        iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
        cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
        szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
    }
    cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

    if( m_pHorizontalScrollBar != NULL ) {
        if( cxNeeded > rc.right - rc.left ) {
            if( m_pHorizontalScrollBar->IsVisible() ) {
                m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
            }
            else {
                m_pHorizontalScrollBar->SetVisible(true);
                m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
                m_pHorizontalScrollBar->SetScrollPos(0);
                rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
            }
        }
        else {
            if( m_pHorizontalScrollBar->IsVisible() ) {
                m_pHorizontalScrollBar->SetVisible(false);
                m_pHorizontalScrollBar->SetScrollRange(0);
                m_pHorizontalScrollBar->SetScrollPos(0);
                rc.bottom += m_pHorizontalScrollBar->GetFixedHeight();
            }
        }
    }

	//
    // Process the scrollbar
    //

	ProcessScrollBar(rc, cxNeeded, cyNeeded);
}

void CListBodyUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL )
			m_pOwner->DoEvent(event);
        else
			CUIControl::DoEvent(event);
        return;
    }

    if( m_pOwner != NULL )
		m_pOwner->DoEvent(event);
	else
		CUIControl::DoEvent(event);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CListHeaderUI::CListHeaderUI()
{
}

CUIString CListHeaderUI::GetClass() const
{
    return L"ListHeaderUI";
}

LPVOID CListHeaderUI::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LISTHEADER)
		return this;
    return CUIHorizontalLayout::GetInterface(strName);
}

SIZE CListHeaderUI::EstimateSize(SIZE szAvailable)
{
    SIZE cXY = {0, m_cxyFixed.cy};
	if( cXY.cy == 0 && m_Window != NULL) {
		for(int it = 0; it < m_items.GetSize(); it++ ) {
			cXY.cy = MAX(cXY.cy,static_cast<CUIControl*>(m_items[it])->EstimateSize(szAvailable).cy);
		}
		int nMin = CResourceMgr::GetInstance().GetDefaultFontHeight() + 6;
		cXY.cy = MAX(cXY.cy,nMin);
	}

    for( int it = 0; it < m_items.GetSize(); it++ ) {
        cXY.cx +=  static_cast<CUIControl*>(m_items[it])->EstimateSize(szAvailable).cx;
    }

    return cXY;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CListHeaderItemUI::CListHeaderItemUI() : m_bDragable(true), m_uButtonState(0), m_iSepWidth(4),
m_uTextStyle(DT_VCENTER | DT_CENTER | DT_SINGLELINE), m_dwTextColor(0), m_bShowHtml(false)
{
	SetTextPadding(CUIRect(2, 0, 2, 0));
    ptLastMouse.x = ptLastMouse.y = 0;
    SetMinWidth(16);
}

CUIString CListHeaderItemUI::GetClass() const
{
    return L"ListHeaderItemUI";
}

LPVOID CListHeaderItemUI::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LISTHEADERITEM)
		return this;
    return CUIControl::GetInterface(strName);
}

UINT CListHeaderItemUI::GetControlFlags() const
{
    if( IsEnabled() && m_iSepWidth != 0 )
		return UIFLAG_SETCURSOR;
    else return 0;
}

void CListHeaderItemUI::SetEnabled(bool bEnable)
{
    CUIControl::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

bool CListHeaderItemUI::IsDragable() const
{
	return m_bDragable;
}

void CListHeaderItemUI::SetDragable(bool bDragable)
{
    m_bDragable = bDragable;
    if ( !m_bDragable ) m_uButtonState &= ~UISTATE_CAPTURED;
}

DWORD CListHeaderItemUI::GetSepWidth() const
{
	return m_iSepWidth;
}

void CListHeaderItemUI::SetSepWidth(int iWidth)
{
    m_iSepWidth = iWidth;
}

DWORD CListHeaderItemUI::GetTextStyle() const
{
	return m_uTextStyle;
}

void CListHeaderItemUI::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

DWORD CListHeaderItemUI::GetTextColor() const
{
	return m_dwTextColor;
}


void CListHeaderItemUI::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
}

CUIRect CListHeaderItemUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CListHeaderItemUI::SetTextPadding(const CUIRect& rc)
{
	m_rcTextPadding = rc;
	Invalidate();
}

void CListHeaderItemUI::SetFont(const CUIString& strFontName)
{
    m_strFontName = strFontName;
}

bool CListHeaderItemUI::IsShowHtml()
{
    return m_bShowHtml;
}

void CListHeaderItemUI::SetShowHtml(bool bShowHtml)
{
    if( m_bShowHtml == bShowHtml ) return;

    m_bShowHtml = bShowHtml;
    Invalidate();
}

CUIString CListHeaderItemUI::GetNormalImage() const
{
	return m_sNormalImage;
}

void CListHeaderItemUI::SetNormalImage(const CUIString& pStrImage)
{
    m_sNormalImage = pStrImage;
    Invalidate();
}

CUIString CListHeaderItemUI::GetHotImage() const
{
    return m_sHotImage;
}

void CListHeaderItemUI::SetHotImage(const CUIString& pStrImage)
{
    m_sHotImage = pStrImage;
    Invalidate();
}

CUIString CListHeaderItemUI::GetPushedImage() const
{
    return m_sPushedImage;
}

void CListHeaderItemUI::SetPushedImage(const CUIString& pStrImage)
{
    m_sPushedImage = pStrImage;
    Invalidate();
}

CUIString CListHeaderItemUI::GetFocusedImage() const
{
    return m_sFocusedImage;
}

void CListHeaderItemUI::SetFocusedImage(const CUIString& pStrImage)
{
    m_sFocusedImage = pStrImage;
    Invalidate();
}

CUIString CListHeaderItemUI::GetSepImage() const
{
    return m_sSepImage;
}

void CListHeaderItemUI::SetSepImage(const CUIString& pStrImage)
{
    m_sSepImage = pStrImage;
    Invalidate();
}

void CListHeaderItemUI::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
    CUIString strName = pstrName;
    if(strName == L"dragable"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetDragable(bSet);
    }
    else if(strName == L"sepwidth"){
        SetSepWidth(wcstol(pstrValue, NULL, 10));
    }
    else if(strName == L"align"){
        if( wcsstr(pstrValue, L"left") != NULL ) {
            m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
            m_uTextStyle |= DT_LEFT;
        }
        if( wcsstr(pstrValue, L"center") != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
            m_uTextStyle |= DT_CENTER;
        }
        if( wcsstr(pstrValue, L"right") != NULL ) {
            m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
            m_uTextStyle |= DT_RIGHT;
        }
    }
    else if(strName == L"endellipsis"){
        if( wcscmp(pstrValue, L"true") == 0 )
            m_uTextStyle |= DT_END_ELLIPSIS;
        else m_uTextStyle &= ~DT_END_ELLIPSIS;
    }
    else if(strName == L"font"){
        SetFont(pstrValue);
    }
    else if(strName == L"textcolor"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetTextColor(dwColor);
    }
	else if(strName == L"textpadding"){
        CUIRect rcTextPadding;
        assert(ParseRectString(pstrValue, rcTextPadding));
        SetTextPadding(rcTextPadding);
	}
    else if(strName == L"showhtml"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetShowHtml(bSet);
    }
    else if(strName == L"normalimage"){
        SetNormalImage(pstrValue);
    }
    else if(strName == L"hotimage"){
        SetHotImage(pstrValue);
    }
    else if(strName == L"pushedimage"){
        SetPushedImage(pstrValue);
    }
    else if(strName == L"focusedimage"){
        SetFocusedImage(pstrValue);
    }
    else if(strName == L"sepimage"){
        SetSepImage(pstrValue);
    }
    else
        CUIControl::SetAttribute(pstrName, pstrValue);
}

void CListHeaderItemUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_Parent != NULL ) m_Parent->DoEvent(event);
        else CUIControl::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) {
        Invalidate();
    }

    if( event.Type == UIEVENT_KILLFOCUS ) {
        Invalidate();
    }

    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ){
        if(!IsEnabled())
            return;

        CUIRect rcSeparator = GetThumbRect();

		//
		// 增加分隔符区域，方便用户拖动
		//

		if (m_iSepWidth>=0)
			rcSeparator.left -= 4;
		else
			rcSeparator.right += 4;
        if(rcSeparator.IsPtIn(event.ptMouse)){
            if( m_bDragable ) {
                m_uButtonState |= UISTATE_CAPTURED;
                ptLastMouse = event.ptMouse;
            }
        }else{
            m_uButtonState |= UISTATE_PUSHED;
            m_Window->SendNotify(this, DUI_MSGTYPE_HEADERCLICK);
            Invalidate();
        }
        return;
    }

    if(event.Type == UIEVENT_BUTTONUP){
        if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            m_uButtonState &= ~UISTATE_CAPTURED;
            if( GetParent() )
                GetParent()->NeedParentUpdate();
        }
        else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
            m_uButtonState &= ~UISTATE_PUSHED;
            Invalidate();
        }
        return;
    }

    if(event.Type == UIEVENT_MOUSEMOVE){
        if((m_uButtonState & UISTATE_CAPTURED) != 0){
            RECT rc = m_rcItem;
            if( m_iSepWidth >= 0 ) {
                rc.right -= ptLastMouse.x - event.ptMouse.x;
            }else{
                rc.left -= ptLastMouse.x - event.ptMouse.x;
            }

            if( rc.right - rc.left > GetMinWidth() ) {
                m_cxyFixed.cx = rc.right - rc.left;
                ptLastMouse = event.ptMouse;
                if( GetParent() )
                    GetParent()->NeedParentUpdate();
            }
        }
        return;
    }

    if(event.Type == UIEVENT_SETCURSOR){
        CUIRect rcSeparator = GetThumbRect();

		//
		// 增加分隔符区域，方便用户拖动
		//

		if (m_iSepWidth>=0)
			rcSeparator.left-=4;
		else
			rcSeparator.right+=4;
        if( IsEnabled() && m_bDragable && rcSeparator.IsPtIn(event.ptMouse)) {

            //
            // TODO SetCursor
            //

            //::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
            return;
        }
    }

    if(event.Type == UIEVENT_MOUSEENTER){
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }

    if(event.Type == UIEVENT_MOUSELEAVE){
        if( IsEnabled() ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    CUIControl::DoEvent(event);
}

SIZE CListHeaderItemUI::EstimateSize(SIZE szAvailable)
{
    if( m_cxyFixed.cy == 0 )
        return CUISize(m_cxyFixed.cx, CResourceMgr::GetInstance().GetDefaultFontHeight() + 14);
    return CUIControl::EstimateSize(szAvailable);
}

CUIRect CListHeaderItemUI::GetThumbRect() const
{
    if( m_iSepWidth >= 0 ) return CUIRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
    else return CUIRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
}

void CListHeaderItemUI::PaintStatusImage(cairo_t *cr)
{
    if(IsFocused())
        m_uButtonState |= UISTATE_FOCUSED;
    else
        m_uButtonState &= ~ UISTATE_FOCUSED;

    if((m_uButtonState & UISTATE_PUSHED) != 0){
        if( m_sPushedImage.IsEmpty() && !m_sNormalImage.IsEmpty() )
			DrawImage(cr, (LPCWSTR)m_sNormalImage);
        if( !DrawImage(cr, (LPCWSTR)m_sPushedImage) )
			m_sPushedImage.Empty();
    }else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( m_sHotImage.IsEmpty() && !m_sNormalImage.IsEmpty() )
			DrawImage(cr, (LPCWSTR)m_sNormalImage);
        if( !DrawImage(cr, (LPCWSTR)m_sHotImage) )
			m_sHotImage.Empty();
    }else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
        if( m_sFocusedImage.IsEmpty() && !m_sNormalImage.IsEmpty() )
			DrawImage(cr, (LPCWSTR)m_sNormalImage);
        if( !DrawImage(cr, (LPCWSTR)m_sFocusedImage) )
			m_sFocusedImage.Empty();
    }else{
        if( !m_sNormalImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)m_sNormalImage) )
				m_sNormalImage.Empty();
        }
    }

    if( !m_sSepImage.IsEmpty() ) {
        RECT rcThumb = GetThumbRect();
        rcThumb.left -= m_rcItem.left;
        rcThumb.top -= m_rcItem.top;
        rcThumb.right -= m_rcItem.left;
        rcThumb.bottom -= m_rcItem.top;

        m_sSepImageModify.Empty();
        m_sSepImageModify.Format(L"dest='%d,%d,%d,%d'", rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
        if( !DrawImage(cr, (LPCWSTR)m_sSepImage, (LPCWSTR)m_sSepImageModify) )
			m_sSepImage.Empty();
    }
}

void CListHeaderItemUI::PaintText(cairo_t *cr)
{
    if( m_dwTextColor == 0 )
		m_dwTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;

	RECT rcText = m_rcItem;
	rcText.left += m_rcTextPadding.left;
	rcText.top += m_rcTextPadding.top;
	rcText.right -= m_rcTextPadding.right;
	rcText.bottom -= m_rcTextPadding.bottom;

    if( m_strText.IsEmpty() ) return;
    int nLinks = 0;
    CUIRender::DrawText(cr, rcText, m_strText, DT_SINGLELINE | m_uTextStyle,
        m_strFontName, m_dwTextColor, m_bShowHtml);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CListElementUI::CListElementUI() :
m_iIndex(-1),
m_pOwner(NULL),
m_bSelected(false),
m_uButtonState(0)
{
}

CUIString CListElementUI::GetClass() const
{
    return L"ListElementUI";
}

UINT CListElementUI::GetControlFlags() const
{
    return UIFLAG_WANTRETURN;
}

LPVOID CListElementUI::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LISTITEM)
		return static_cast<IListItemUI*>(this);
	if(strName == DUI_CTR_LISTELEMENT)
		return static_cast<CListElementUI*>(this);
    return
        CUIControl::GetInterface(strName);
}

IListOwnerUI* CListElementUI::GetOwner()
{
    return m_pOwner;
}

void CListElementUI::SetOwner(CUIControl* pOwner)
{
    m_pOwner = static_cast<IListOwnerUI*>(pOwner->GetInterface(L"IListOwner"));
}

void CListElementUI::SetVisible(bool bVisible)
{
    CUIControl::SetVisible(bVisible);
    if( !IsVisible() && m_bSelected)
    {
        m_bSelected = false;
        if( m_pOwner != NULL )
			m_pOwner->SelectItem(-1);
    }
}

void CListElementUI::SetEnabled(bool bEnable)
{
    CUIControl::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

int CListElementUI::GetIndex() const
{
    return m_iIndex;
}

void CListElementUI::SetIndex(int iIndex)
{
    m_iIndex = iIndex;
}

void CListElementUI::Invalidate()
{
    if( !IsVisible() ) return;

    if( GetParent() ) {
        CUIContainer* pParentContainer = static_cast<CUIContainer*>(GetParent()->GetInterface(L"Container"));
        if( pParentContainer ) {
            RECT rc = pParentContainer->GetPos();
            RECT rcInset = pParentContainer->GetInset();
            rc.left += rcInset.left;
            rc.top += rcInset.top;
            rc.right -= rcInset.right;
            rc.bottom -= rcInset.bottom;
            CUIScrollBar* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
            if( pVerticalScrollBar && pVerticalScrollBar->IsVisible() ) rc.right -= pVerticalScrollBar->GetFixedWidth();
            CUIScrollBar* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
            if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rc.bottom -= pHorizontalScrollBar->GetFixedHeight();

            RECT invalidateRc = m_rcItem;
            if( !UIIntersectRect(&invalidateRc, &m_rcItem, &rc) ) {
                return;
            }

            CUIControl* pParent = GetParent();
            RECT rcTemp;
            RECT rcParent;
            while( pParent = pParent->GetParent() ){
                rcTemp = invalidateRc;
                rcParent = pParent->GetPos();
                if( !UIIntersectRect(&invalidateRc, &rcTemp, &rcParent) ) {
                    return;
                }
            }

            if(m_Window != NULL)
                m_Window->Invalidate(invalidateRc);
        }else{
            CUIControl::Invalidate();
        }
    }else{
        CUIControl::Invalidate();
    }
}

bool CListElementUI::Activate()
{
    if(!CUIControl::Activate())
		return false;
    if(m_Window != NULL)
		m_Window->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
    return true;
}

bool CListElementUI::IsSelected() const
{
    return m_bSelected;
}

bool CListElementUI::Select(bool bSelect)
{
    if(!IsEnabled())
		return false;
    if(bSelect == m_bSelected)
		return true;
    m_bSelected = bSelect;
    if(bSelect && m_pOwner != NULL)
		m_pOwner->SelectItem(m_iIndex);
    Invalidate();

    return true;
}

bool CListElementUI::IsExpanded() const
{
    return false;
}

bool CListElementUI::Expand(bool /*bExpand = true*/)
{
    return false;
}

void CListElementUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL )
			m_pOwner->DoEvent(event);
        else
			CUIControl::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_DBLCLICK ){
        if( IsEnabled() ) {
            Activate();
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_KEYDOWN && IsEnabled() ){
        if( event.chKey == VK_RETURN ) {
            Activate();
            Invalidate();
            return;
        }
    }

	//
    // An important twist: The list-item will send the event not to its immediate
    // parent but to the "attached" list. A list may actually embed several components
    // in its path to the item, but key-presses etc. needs to go to the actual list.
    //

	if( m_pOwner != NULL )
		m_pOwner->DoEvent(event);
	else
		CUIControl::DoEvent(event);
}

void CListElementUI::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
    CUIString strName = pstrName;
    if(strName == L"selected")
		Select();
    else
		CUIControl::SetAttribute(pstrName, pstrValue);
}

void CListElementUI::DrawItemBk(cairo_t *cr, const CUIRect& rcItem)
{
    assert(m_pOwner);
    if( m_pOwner == NULL )
		return;

    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iBackColor = 0;
    if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 )
		iBackColor = pInfo->dwBkColor;

    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iBackColor = pInfo->dwHotBkColor;
    }

    if( IsSelected() ) {
        iBackColor = pInfo->dwSelectedBkColor;
    }

    if( !IsEnabled() ) {
        iBackColor = pInfo->dwDisabledBkColor;
    }

    if ( iBackColor != 0 ) {
        CUIRender::DrawColor(cr, m_rcItem, iBackColor);
    }

    if( !IsEnabled() ) {
        if( !pInfo->sDisabledImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sDisabledImage) ) pInfo->sDisabledImage.Empty();
            else return;
        }
    }

    if( IsSelected() ) {
        if( !pInfo->sSelectedImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sSelectedImage) ) pInfo->sSelectedImage.Empty();
            else return;
        }
    }

    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !pInfo->sHotImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sHotImage) ) pInfo->sHotImage.Empty();
            else return;
        }
    }

    if( !m_strBackImage.IsEmpty() ) {
        if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) {
            if( !DrawImage(cr, (LPCWSTR)m_strBackImage) )
                m_strBackImage.Empty();
        }
    }

    if( m_strBackImage.IsEmpty() ) {
        if( !pInfo->sBkImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sBkImage) )
                pInfo->sBkImage.Empty();
            else return;
        }
    }

    if (pInfo->dwLineColor != 0){
        RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
        CUIRender::DrawLine(cr, rcLine, 1, pInfo->dwLineColor, 0);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CListLabelElementUI::CListLabelElementUI()
{
}

CUIString CListLabelElementUI::GetClass() const
{
    return L"ListLabelElementUI";
}

LPVOID CListLabelElementUI::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LISTLABELELEMENT)
        return static_cast<CListLabelElementUI*>(this);
    return
        CListElementUI::GetInterface(strName);
}

void CListLabelElementUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL )
			m_pOwner->DoEvent(event);
        else
			CListElementUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN ){
        if(IsEnabled()){
            m_Window->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
            Select();
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_MOUSEMOVE ) {
        return;
    }

    if( event.Type == UIEVENT_BUTTONUP ){
        return;
    }

    if( event.Type == UIEVENT_MOUSEENTER ){
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_MOUSELEAVE ){
        if( (m_uButtonState & UISTATE_HOT) != 0 ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }
    CListElementUI::DoEvent(event);
}

SIZE CListLabelElementUI::EstimateSize(SIZE szAvailable)
{
    if( m_pOwner == NULL ) return CUISize(0, 0);

    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    SIZE cXY = m_cxyFixed;
    if(cXY.cy == 0){
        cXY.cy = CResourceMgr::GetInstance().GetDefaultFontHeight() + 8;
        cXY.cy += pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
    }

    if(cXY.cx == 0 && m_Window){
        CUIRect rcText(0, 0, 9999, cXY.cy);
        CUIRect rcTemp;
        CUIRender::DrawText(m_Window->GetPaintContext(), rcText, m_strText,
            DT_SINGLELINE | DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER,
            pInfo->strFontName, 0xFF000000, pInfo->bShowHtml, &rcTemp);
        cXY.cx = rcTemp.right - rcTemp.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right;
    }

    return cXY;
}

void CListLabelElementUI::DoPaint(cairo_t *cr, const CUIRect& rcPaint)
{
    if( !UIIntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) )
		return;
    DrawItemBk(cr, m_rcItem);
    DrawItemText(cr, m_rcItem);
}

void CListLabelElementUI::DrawItemText(cairo_t *cr, const CUIRect& rcItem)
{
    if(m_strText.IsEmpty()) return;

    if(m_pOwner == NULL) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    int nLinks = 0;
    RECT rcText = rcItem;
    rcText.left += pInfo->rcTextPadding.left;
    rcText.right -= pInfo->rcTextPadding.right;
    rcText.top += pInfo->rcTextPadding.top;
    rcText.bottom -= pInfo->rcTextPadding.bottom;

    CUIRender::DrawText(cr, rcText, m_strText,
        DT_SINGLELINE | pInfo->uTextStyle,
        pInfo->strFontName, iTextColor, pInfo->bShowHtml);

}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CListTextElementUI::CListTextElementUI() : m_nLinks(0), m_nHoverLink(-1), m_pOwner(NULL)
{
    ::ZeroMemory(&m_rcLinks, sizeof(m_rcLinks));
}

CListTextElementUI::~CListTextElementUI()
{
    CUIString* pText;
    for( int it = 0; it < m_aTexts.GetSize(); it++ ) {
        pText = static_cast<CUIString*>(m_aTexts[it]);
        if( pText ) delete pText;
    }
    m_aTexts.Empty();
}

CUIString CListTextElementUI::GetClass() const
{
    return L"ListTextElementUI";
}

LPVOID CListTextElementUI::GetInterface(const CUIString& strName)
{
    if(strName ==  DUI_CTR_LISTTEXTELEMENT)
        return static_cast<CListTextElementUI*>(this);
    return
        CListLabelElementUI::GetInterface(strName);
}

UINT CListTextElementUI::GetControlFlags() const
{
    return UIFLAG_WANTRETURN | ( (IsEnabled() && m_nLinks > 0) ? UIFLAG_SETCURSOR : 0);
}

LPCWSTR CListTextElementUI::GetText(int iIndex) const
{
    CUIString* pText = static_cast<CUIString*>(m_aTexts.GetAt(iIndex));
    if(pText)
        return pText->GetData();
    return NULL;
}

void CListTextElementUI::SetText(int iIndex, LPCWSTR pstrText)
{
    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    if( iIndex < 0 || iIndex >= pInfo->nColumns ) return;
    while( m_aTexts.GetSize() < pInfo->nColumns ) { m_aTexts.Add(NULL); }

    CUIString* pText = static_cast<CUIString*>(m_aTexts[iIndex]);
    if( (pText == NULL && pstrText == NULL) || (pText && *pText == pstrText) ) return;

	if ( pText )
		pText->Assign(pstrText);
	else
		m_aTexts.SetAt(iIndex, new CUIString(pstrText));
    Invalidate();
}

/*
void CListTextElementUI::SetIcon(int iIndex,HICON hicon)
{
    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();

    if( iIndex < 0 || iIndex >= pInfo->nColumns )
		return;

    while( m_Icons.GetSize() < pInfo->nColumns ) {
		m_Icons.Add(NULL);
	}

    HICON hIcon = static_cast<HICON>(m_Icons[iIndex]);
    if( (hIcon == NULL && hicon == NULL) || (hIcon && hIcon == hicon) )
		return;

    if ( hIcon )
        hIcon = hicon;
    else
        m_Icons.SetAt(iIndex, hicon);

    Invalidate();
}

HICON CListTextElementUI::GetIcon(int iIndex) const
{
    HICON hicon = static_cast<HICON>(m_Icons.GetAt(iIndex));
    return hicon;
}
*/

void CListTextElementUI::SetOwner(CUIControl* pOwner)
{
    CListElementUI::SetOwner(pOwner);
    m_pOwner = static_cast<IListUI*>(pOwner->GetInterface(L"IList"));
}

CUIString* CListTextElementUI::GetLinkContent(int iIndex)
{
    if( iIndex >= 0 && iIndex < m_nLinks ) return &m_sLinks[iIndex];
    return NULL;
}

void CListTextElementUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL )
			m_pOwner->DoEvent(event);
        else
			CListLabelElementUI::DoEvent(event);
        return;
    }

	//
    // When you hover over a link
    //

	if( event.Type == UIEVENT_SETCURSOR ) {
        for( int i = 0; i < m_nLinks; i++ ) {
            if(m_rcLinks[i].IsPtIn(event.ptMouse)) {

                //
                // TODO SetCursor
                //

                //::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
                return;
            }
        }
    }

    if( event.Type == UIEVENT_BUTTONUP && IsEnabled() ) {
        for( int i = 0; i < m_nLinks; i++ ) {
            if(m_rcLinks[i].IsPtIn(event.ptMouse)) {
                m_Window->SendNotify(this, DUI_MSGTYPE_LINK, (WPARAM)i);
                return;
            }
        }
    }

    if( m_nLinks > 0 && event.Type == UIEVENT_MOUSEMOVE ) {
        int nHoverLink = -1;
        for( int i = 0; i < m_nLinks; i++ ) {
            if(m_rcLinks[i].IsPtIn(event.ptMouse)) {
                nHoverLink = i;
                break;
            }
        }

        if(m_nHoverLink != nHoverLink) {
            Invalidate();
            m_nHoverLink = nHoverLink;
        }
    }

    if( m_nLinks > 0 && event.Type == UIEVENT_MOUSELEAVE ) {
        if(m_nHoverLink != -1) {
            Invalidate();
            m_nHoverLink = -1;
        }
    }
    CListLabelElementUI::DoEvent(event);
}

SIZE CListTextElementUI::EstimateSize(SIZE szAvailable)
{
    TListInfoUI* pInfo = NULL;
    if( m_pOwner ) pInfo = m_pOwner->GetListInfo();

    SIZE cXY = m_cxyFixed;
    if(cXY.cy == 0) {
        cXY.cy = CResourceMgr::GetInstance().GetDefaultFontHeight() + 8;
        if( pInfo )
            cXY.cy += pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
    }

    return cXY;
}

void CListTextElementUI::DrawItemText(cairo_t* cr, const CUIRect& rcItem)
{
    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iTextColor = pInfo->dwTextColor;

    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iTextColor = pInfo->dwHotTextColor;
    }
    if( IsSelected() ) {
        iTextColor = pInfo->dwSelectedTextColor;
    }
    if( !IsEnabled() ) {
        iTextColor = pInfo->dwDisabledTextColor;
    }
    IListCallbackUI* pCallback = m_pOwner->GetTextCallback();

    m_nLinks = 0;
    int nLinks = MAX_LINK;
    for( int i = 0; i < pInfo->nColumns; i++ ){

        /*HICON hicon =GetIcon(i);
        int nIconWidth = hicon != NULL ? 18:0;
        RECT rcItem = { pInfo->rcColumn[i].left+ nIconWidth, m_rcItem.top, pInfo->rcColumn[i].right+nIconWidth, m_rcItem.bottom };
        RECT rcItemLine = rcItem;

        rcItem.left += pInfo->rcTextPadding.left;
        rcItem.right -= pInfo->rcTextPadding.right;
        rcItem.top += pInfo->rcTextPadding.top;
        rcItem.bottom -= pInfo->rcTextPadding.bottom;

        if (hicon){
            CRenderEngine::DrawIcon(hDC, m_pManager, rcItem.left-18, rcItem.top, 16, 16, rcItem, hicon);
        }*/

        CUIString strText;
        if( pCallback )
			strText = pCallback->GetItemText(this, m_iIndex, i);
        else
			strText.Assign(GetText(i));

        CUIRender::DrawText(cr, rcItem, strText, DT_SINGLELINE | pInfo->uTextStyle,
            pInfo->strFontName, iTextColor, pInfo->bShowHtml);

        m_nLinks += nLinks;
        nLinks = MAX_LINK - m_nLinks;
    }

    for( int i = m_nLinks; i < MAX_LINK; i++ ) {
        ::ZeroMemory(m_rcLinks + i, sizeof(RECT));
        ((CUIString*)(m_sLinks + i))->Empty();
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//

CListContainerElementUI::CListContainerElementUI() :
m_iIndex(-1),
m_pOwner(NULL),
m_bSelected(false),
m_uButtonState(0)
{
}

CUIString CListContainerElementUI::GetClass() const
{
    return L"ListContainerElementUI";
}

UINT CListContainerElementUI::GetControlFlags() const
{
    return UIFLAG_WANTRETURN;
}

LPVOID CListContainerElementUI::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LISTITEM)
		return static_cast<IListItemUI*>(this);
	if(strName == DUI_CTR_LISTCONTAINERELEMENT)
		return static_cast<CListContainerElementUI*>(this);
    return CUIContainer::GetInterface(strName);
}

IListOwnerUI* CListContainerElementUI::GetOwner()
{
    return m_pOwner;
}

void CListContainerElementUI::SetOwner(CUIControl* pOwner)
{
    m_pOwner = static_cast<IListOwnerUI*>(pOwner->GetInterface(L"IListOwner"));
}

void CListContainerElementUI::SetVisible(bool bVisible)
{
    CUIContainer::SetVisible(bVisible);
    if( !IsVisible() && m_bSelected){
        m_bSelected = false;
        if( m_pOwner != NULL ) m_pOwner->SelectItem(-1);
    }
}

void CListContainerElementUI::SetEnabled(bool bEnable)
{
    CUIControl::SetEnabled(bEnable);
    if( !IsEnabled() ) {
        m_uButtonState = 0;
    }
}

int CListContainerElementUI::GetIndex() const
{
    return m_iIndex;
}

void CListContainerElementUI::SetIndex(int iIndex)
{
    m_iIndex = iIndex;
}

void CListContainerElementUI::Invalidate()
{
    if( !IsVisible() ) return;

    if( GetParent() ) {
        CUIContainer* pParentContainer = static_cast<CUIContainer*>(GetParent()->GetInterface(L"Container"));
        if( pParentContainer ) {
            RECT rc = pParentContainer->GetPos();
            RECT rcInset = pParentContainer->GetInset();
            rc.left += rcInset.left;
            rc.top += rcInset.top;
            rc.right -= rcInset.right;
            rc.bottom -= rcInset.bottom;
            CUIScrollBar* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
            if( pVerticalScrollBar && pVerticalScrollBar->IsVisible() ) rc.right -= pVerticalScrollBar->GetFixedWidth();
            CUIScrollBar* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
            if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rc.bottom -= pHorizontalScrollBar->GetFixedHeight();

            RECT invalidateRc = m_rcItem;
            if( !UIIntersectRect(&invalidateRc, &m_rcItem, &rc) ){
                return;
            }

            CUIControl* pParent = GetParent();
            RECT rcTemp;
            RECT rcParent;
            while( pParent = pParent->GetParent() ){
                rcTemp = invalidateRc;
                rcParent = pParent->GetPos();
                if( !UIIntersectRect(&invalidateRc, &rcTemp, &rcParent) ) {
                    return;
                }
            }

            if( m_Window != NULL )
                m_Window->Invalidate(invalidateRc);
        }else {
            CUIContainer::Invalidate();
        }
    }else {
        CUIContainer::Invalidate();
    }
}

bool CListContainerElementUI::Activate()
{
    if( !CUIContainer::Activate() )
		return false;
    if( m_Window != NULL )
		m_Window->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
    return true;
}

bool CListContainerElementUI::IsSelected() const
{
    return m_bSelected;
}

bool CListContainerElementUI::Select(bool bSelect)
{
    if( !IsEnabled() )
		return false;
    if( bSelect == m_bSelected )
		return true;

    m_bSelected = bSelect;
    if( bSelect && m_pOwner != NULL )
		m_pOwner->SelectItem(m_iIndex);
    Invalidate();

    return true;
}

bool CListContainerElementUI::IsExpanded() const
{
    return false;
}

bool CListContainerElementUI::Expand(bool /*bExpand = true*/)
{
    return false;
}

void CListContainerElementUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pOwner != NULL )
			m_pOwner->DoEvent(event);
        else
			CUIContainer::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_DBLCLICK ){
        if( IsEnabled() ) {
            Activate();
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_KEYDOWN && IsEnabled() ){
        if( event.chKey == VK_RETURN ) {
            Activate();
            Invalidate();
            return;
        }
    }

    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN ){
        if( IsEnabled() ){
            m_Window->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
            Select();
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_BUTTONUP ){
        return;
    }

    if( event.Type == UIEVENT_MOUSEMOVE ){
        return;
    }

    if( event.Type == UIEVENT_MOUSEENTER ){
        if( IsEnabled() ) {
            m_uButtonState |= UISTATE_HOT;
            Invalidate();
        }
        return;
    }

    if( event.Type == UIEVENT_MOUSELEAVE ){
        if( (m_uButtonState & UISTATE_HOT) != 0 ) {
            m_uButtonState &= ~UISTATE_HOT;
            Invalidate();
        }
        return;
    }

	//
    // An important twist: The list-item will send the event not to its immediate
    // parent but to the "attached" list. A list may actually embed several components
    // in its path to the item, but key-presses etc. needs to go to the actual list.
    //

	if( m_pOwner != NULL )
		m_pOwner->DoEvent(event);
	else
		CUIControl::DoEvent(event);
}

void CListContainerElementUI::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
    CUIString strName = pstrName;
    if( strName == L"selected" )
		Select();
    else
		CUIContainer::SetAttribute(pstrName, pstrValue);
}

void CListContainerElementUI::DoPaint(cairo_t *cr, const CUIRect& rcPaint)
{
    if( !UIIntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) )
		return;
    DrawItemBk(cr, m_rcItem);
    CUIContainer::DoPaint(cr, rcPaint);
}

void CListContainerElementUI::DrawItemText(cairo_t *cr, const CUIRect& rcItem)
{
    return;
}

void CListContainerElementUI::DrawItemBk(cairo_t *cr, const CUIRect& rcItem)
{
    assert(m_pOwner);
    if( m_pOwner == NULL )
		return;

    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iBackColor = 0;
    if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 )
		iBackColor = pInfo->dwBkColor;

    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iBackColor = pInfo->dwHotBkColor;
    }

    if( IsSelected() ) {
        iBackColor = pInfo->dwSelectedBkColor;
    }

    if( !IsEnabled() ) {
        iBackColor = pInfo->dwDisabledBkColor;
    }

    if ( iBackColor != 0 ) {
        CUIRender::DrawColor(cr, m_rcItem, iBackColor);
    }

    if( !IsEnabled() ) {
        if( !pInfo->sDisabledImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sDisabledImage) )
				pInfo->sDisabledImage.Empty();
            else
				return;
        }
    }
    if( IsSelected() ) {
        if( !pInfo->sSelectedImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sSelectedImage) )
				pInfo->sSelectedImage.Empty();
            else
				return;
        }
    }
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !pInfo->sHotImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sHotImage) )
				pInfo->sHotImage.Empty();
            else
				return;
        }
    }

    if( !m_strBackImage.IsEmpty() ) {
        if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) {
            if( !DrawImage(cr, (LPCWSTR)m_strBackImage) )
				m_strBackImage.Empty();
        }
    }

    if( m_strBackImage.IsEmpty() ) {
        if( !pInfo->sBkImage.IsEmpty() ) {
            if( !DrawImage(cr, (LPCWSTR)pInfo->sBkImage) )
				pInfo->sBkImage.Empty();
            else
				return;
        }
    }

    if ( pInfo->dwLineColor != 0 ) {
        CUIRect rcLine( m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 );
        CUIRender::DrawLine(cr, rcLine, 1, pInfo->dwLineColor, 0);
    }
}

