#include "stdafx.h"

#include "UIContainer.h"


CUIContainer::CUIContainer()
	: m_iChildPadding(0),
	m_bAutoDestroy(true),
	m_bDelayedDestroy(true),
	m_bMouseChildEnabled(true),
	m_pVerticalScrollBar(NULL),
	m_pHorizontalScrollBar(NULL),
	m_bScrollProcess(false)
{
	::ZeroMemory(&m_rcInset, sizeof(m_rcInset));
}

CUIContainer::~CUIContainer()
{
	m_bDelayedDestroy = false;
	RemoveAll();
	if(m_pVerticalScrollBar) 
        delete m_pVerticalScrollBar;
	if(m_pHorizontalScrollBar) 
        delete m_pHorizontalScrollBar;
}

CUIString CUIContainer::GetClass() const
{
	return L"ContainerUI";
}

LPVOID CUIContainer::GetInterface(const CUIString& strName)
{
	if(strName == L"IContainer") 
        return static_cast<IContainerUI*>(this);
	else if(strName == DUI_CTR_CONTAINER) 
        return static_cast<CUIContainer*>(this);
	return CUIControl::GetInterface(strName);
}

CUIControl* CUIContainer::GetItemAt(int iIndex) const
{
	if(iIndex < 0 || iIndex >= m_items.GetSize()) return NULL;
	return static_cast<CUIControl*>(m_items[iIndex]);
}

int CUIContainer::GetItemIndex(CUIControl* pControl) const
{
	for(int it = 0; it < m_items.GetSize(); it++) {
		if(static_cast<CUIControl*>(m_items[it]) == pControl){
			return it;
		}
	}

	return -1;
}

bool CUIContainer::SetItemIndex(CUIControl* pControl, int iIndex)
{
	for(int it = 0; it < m_items.GetSize(); it++) {
		if(static_cast<CUIControl*>(m_items[it]) == pControl) {
			NeedUpdate();            
			m_items.Remove(it);
			return m_items.InsertAt(iIndex, pControl);
		}
	}

	return false;
}

int CUIContainer::GetCount() const
{
	return m_items.GetSize();
}

bool CUIContainer::Add(CUIControl* pControl)
{
	if(pControl == NULL) 
        return false;

	if(m_Window) 
        m_Window->InitControls(pControl, this);
	
    if(IsVisible()) 
        NeedUpdate();
	/*else 
        pControl->SetInternVisible(false);*/
	return m_items.Add(pControl);   
}

bool CUIContainer::AddAt(CUIControl* pControl, int iIndex)
{
	if(pControl == NULL) 
        return false;

    if(m_Window) 
        m_Window->InitControls(pControl, this);
	
    if(IsVisible()) 
        NeedUpdate();
	//else 
    //    pControl->SetInternVisible(false);
	return m_items.InsertAt(iIndex, pControl);
}

bool CUIContainer::Remove(CUIControl* pControl)
{
	if( pControl == NULL) return false;

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		if( static_cast<CUIControl*>(m_items[it]) == pControl ) {
			NeedUpdate();
			/*if(m_bAutoDestroy){
				if(m_bDelayedDestroy && m_Manager) 
					m_Manager->AddDelayedCleanup(pControl);
				else 
					delete pControl;
			}*/
			return m_items.Remove(it);
		}
	}
	return false;
}

bool CUIContainer::RemoveAt(int iIndex)
{
	CUIControl* pControl = GetItemAt(iIndex);
	if (pControl != NULL){
		return CUIContainer::Remove(pControl);
	}

	return false;
}

void CUIContainer::RemoveAll()
{
	/*for(int it = 0; m_bAutoDestroy && it < m_items.GetSize(); it++) {
		if(m_bDelayedDestroy && m_Manager) 
            m_Manager->AddDelayedCleanup(static_cast<CUIControl*>(m_items[it]));
		else 
            delete static_cast<CUIControl*>(m_items[it]);
	}*/
	m_items.Empty();
	NeedUpdate();
}

CUIRect CUIContainer::GetInset() const
{
	return m_rcInset;
}

void CUIContainer::SetInset(const CUIRect& rcInset)
{
	m_rcInset = rcInset;
	NeedUpdate();
}

int CUIContainer::GetChildPadding() const
{
	return m_iChildPadding;
}

void CUIContainer::SetChildPadding(int iPadding)
{
	m_iChildPadding = iPadding;
	NeedUpdate();
}

bool CUIContainer::IsMouseChildEnabled() const
{
	return m_bMouseChildEnabled;
}

void CUIContainer::SetMouseChildEnabled(bool bEnable)
{
	m_bMouseChildEnabled = bEnable;
}

void CUIContainer::SetVisible(bool bVisible)
{
	if(m_bVisible == bVisible) 
        return;
	CUIControl::SetVisible(bVisible);
	/*for(int it = 0; it < m_items.GetSize(); it++) {
		static_cast<CUIControl*>(m_items[it])->SetInternVisible(IsVisible());
	}*/
}

//
// 逻辑上，对于Container控件不公开此方法
// 调用此方法的结果是，内部子控件隐藏，控件本身依然显示，背景等效果存在
//

void CUIContainer::SetInternVisible(bool bVisible)
{
	//CUIControl::SetInternVisible(bVisible);
	//if(m_items.IsEmpty()) return;
	//for(int it = 0; it < m_items.GetSize(); it++){

        //
		// 控制子控件显示状态
		// InternVisible状态应由子控件自己控制
		//

        //static_cast<CUIControl*>(m_items[it])->SetInternVisible(IsVisible());
	//}
}

void CUIContainer::SetMouseEnabled(bool bEnabled)
{
	if(m_pVerticalScrollBar != NULL) 
        m_pVerticalScrollBar->SetMouseEnabled(bEnabled);
	if(m_pHorizontalScrollBar != NULL) 
        m_pHorizontalScrollBar->SetMouseEnabled(bEnabled);
	CUIControl::SetMouseEnabled(bEnabled);
}

void CUIContainer::DoEvent(TEventUI& Event)
{
    
    //
    // if this container do not enable the mouse event.
    // check parent's
    //
    
	if(!IsMouseEnabled() && IS_MOUSE_EVENT(Event.Type)){
		if(m_Parent != NULL) 
            m_Parent->DoEvent(Event);
		else 
            CUIControl::DoEvent(Event);
		return;
	}

	if(Event.Type == UIEVENT_SETFOCUS){
		m_bFocused = true;
		return;
	}
	if(Event.Type == UIEVENT_KILLFOCUS){
		m_bFocused = false;
		return;
	}
	if(m_pVerticalScrollBar != NULL      && 
       m_pVerticalScrollBar->IsVisible() && 
       m_pVerticalScrollBar->IsEnabled()){
		if(Event.Type == UIEVENT_KEYDOWN){
			switch(Event.chKey){
		    case VK_DOWN:
			    LineDown();
			    return;
		    case VK_UP:
			    LineUp();
			    return;
		    case VK_NEXT:
			    PageDown();
			    return;
		    case VK_PRIOR:
			    PageUp();
			    return;
		    case VK_HOME:
			    HomeUp();
			    return;
		    case VK_END:
			    EndDown();
			    return;
			}
		}else if( Event.Type == UIEVENT_SCROLLWHEEL){
			switch( LOWORD(Event.wParam) ) {
		    case SB_LINEUP:
			    LineUp();
			    return;
		    case SB_LINEDOWN:
			    LineDown();
			    return;
			}
		}
	}else if(m_pHorizontalScrollBar != NULL && 
             m_pHorizontalScrollBar->IsVisible() && 
             m_pHorizontalScrollBar->IsEnabled()){
		if(Event.Type == UIEVENT_KEYDOWN){
			switch(Event.chKey) {
		    case VK_DOWN:
			    LineRight();
			    return;
		    case VK_UP:
			    LineLeft();
			    return;
		    case VK_NEXT:
			    PageRight();
			    return;
		    case VK_PRIOR:
			    PageLeft();
			    return;
		    case VK_HOME:
			    HomeLeft();
			    return;
		    case VK_END:
			    EndRight();
			    return;
			}
		}else if(Event.Type == UIEVENT_SCROLLWHEEL){
			switch(LOWORD(Event.wParam)){
		    case SB_LINEUP:
			    LineLeft();
			    return;
		    case SB_LINEDOWN:
			    LineRight();
			    return;
			}
		}
	}
	CUIControl::DoEvent(Event);
}

SIZE CUIContainer::GetScrollPos() const
{
	SIZE sz = {0, 0};
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
        sz.cy = m_pVerticalScrollBar->GetScrollPos();
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
        sz.cx = m_pHorizontalScrollBar->GetScrollPos();
	return sz;
}

SIZE CUIContainer::GetScrollRange() const
{
	SIZE sz = {0, 0};
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
        sz.cy = m_pVerticalScrollBar->GetScrollRange();
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
        sz.cx = m_pHorizontalScrollBar->GetScrollRange();
	return sz;
}

void CUIContainer::SetScrollPos(SIZE szPos)
{
	int cx = 0;
	int cy = 0;
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()){
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()){
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( cx == 0 && cy == 0 ) 
        return;

	RECT rcPos;
	for(int it2 = 0; it2 < m_items.GetSize(); it2++){
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it2]);
		if(!pControl->IsVisible() || pControl->IsFloat())
            continue;

		rcPos = pControl->GetPos();
		rcPos.left -= cx;
		rcPos.right -= cx;
		rcPos.top -= cy;
		rcPos.bottom -= cy;
		pControl->SetPos(rcPos);
	}

	Invalidate();
}

void CUIContainer::LineUp()
{
	int cyLine = 8;
	if(m_Window) 
        cyLine = CResourceMgr::GetInstance().GetDefaultFontHeight() + 8;

	SIZE sz = GetScrollPos();
	sz.cy -= cyLine;
	SetScrollPos(sz);
}

void CUIContainer::LineDown()
{
	int cyLine = 8;
    if(m_Window) 
        cyLine = CResourceMgr::GetInstance().GetDefaultFontHeight() + 8;

	SIZE sz = GetScrollPos();
	sz.cy += cyLine;
	SetScrollPos(sz);
}

void CUIContainer::PageUp()
{
	SIZE sz = GetScrollPos();
	int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
        iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
	sz.cy -= iOffset;
	SetScrollPos(sz);
}

void CUIContainer::PageDown()
{
	SIZE sz = GetScrollPos();
	int iOffset = m_rcItem.bottom - m_rcItem.top - m_rcInset.top - m_rcInset.bottom;
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
        iOffset -= m_pHorizontalScrollBar->GetFixedHeight();
	sz.cy += iOffset;
	SetScrollPos(sz);
}

void CUIContainer::HomeUp()
{
	SIZE sz = GetScrollPos();
	sz.cy = 0;
	SetScrollPos(sz);
}

void CUIContainer::EndDown()
{
	SIZE sz = GetScrollPos();
	sz.cy = GetScrollRange().cy;
	SetScrollPos(sz);
}

void CUIContainer::LineLeft()
{
	SIZE sz = GetScrollPos();
	sz.cx -= 8;
	SetScrollPos(sz);
}

void CUIContainer::LineRight()
{
	SIZE sz = GetScrollPos();
	sz.cx += 8;
	SetScrollPos(sz);
}

void CUIContainer::PageLeft()
{
	SIZE sz = GetScrollPos();
	int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible())
        iOffset -= m_pVerticalScrollBar->GetFixedWidth();
	sz.cx -= iOffset;
	SetScrollPos(sz);
}

void CUIContainer::PageRight()
{
	SIZE sz = GetScrollPos();
	int iOffset = m_rcItem.right - m_rcItem.left - m_rcInset.left - m_rcInset.right;
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
        iOffset -= m_pVerticalScrollBar->GetFixedWidth();
	sz.cx += iOffset;
	SetScrollPos(sz);
}

void CUIContainer::HomeLeft()
{
	SIZE sz = GetScrollPos();
	sz.cx = 0;
	SetScrollPos(sz);
}

void CUIContainer::EndRight()
{
	SIZE sz = GetScrollPos();
	sz.cx = GetScrollRange().cx;
	SetScrollPos(sz);
}

void CUIContainer::EnableScrollBar(bool bEnableVertical, bool bEnableHorizontal)
{
	if(bEnableVertical && !m_pVerticalScrollBar){
		m_pVerticalScrollBar = new CUIScrollBar;
		m_pVerticalScrollBar->SetOwner(this);
        m_pVerticalScrollBar->SetOwnWindow(m_Window, NULL, false);
		//m_pVerticalScrollBar->SetManager(m_Manager, NULL, false);
		/*if (m_Manager){
			LPCWSTR pDefaultAttributes = m_Manager->GetDefaultAttributeList(L"VScrollBar");
			if(pDefaultAttributes){
				m_pVerticalScrollBar->ApplyAttributeList(pDefaultAttributes);
			}
		}*/
	}else if(!bEnableVertical && m_pVerticalScrollBar){
		delete m_pVerticalScrollBar;
		m_pVerticalScrollBar = NULL;
	}

	if(bEnableHorizontal && !m_pHorizontalScrollBar) {
		m_pHorizontalScrollBar = new CUIScrollBar;
		m_pHorizontalScrollBar->SetHorizontal(true);
		m_pHorizontalScrollBar->SetOwner(this);
        m_pVerticalScrollBar->SetOwnWindow(m_Window, NULL, false);
		//m_pHorizontalScrollBar->SetManager(m_Manager, NULL, false);
		/*if (m_Manager){
			LPCTSTR pDefaultAttributes = m_Manager->GetDefaultAttributeList(_T("HScrollBar"));
			if(pDefaultAttributes){
				m_pHorizontalScrollBar->ApplyAttributeList(pDefaultAttributes);
			}
		}*/
	}else if(!bEnableHorizontal && m_pHorizontalScrollBar){
		delete m_pHorizontalScrollBar;
		m_pHorizontalScrollBar = NULL;
	}

	NeedUpdate();
}

CUIScrollBar* CUIContainer::GetVerticalScrollBar() const
{
	return m_pVerticalScrollBar;
}

CUIScrollBar* CUIContainer::GetHorizontalScrollBar() const
{
	return m_pHorizontalScrollBar;
}

int CUIContainer::FindSelectable(int iIndex, bool bForward /*= true*/) const
{
    //
	// NOTE: This is actually a helper-function for the list/combo/ect controls
	//       that allow them to find the next enabled/available selectable item
	//

    if(GetCount() == 0) 
        return -1;

	iIndex = CLAMP(iIndex, 0, GetCount() - 1);
	if(bForward){
		for(int i = iIndex; i < GetCount(); i++) {
			if(GetItemAt(i)->GetInterface(L"ListItem") != NULL 
			   && GetItemAt(i)->IsVisible()
               && GetItemAt(i)->IsEnabled()){
                return i;
            } 
		}
		return -1;
	}else{
		for(int i = iIndex; i >= 0; --i){
			if(GetItemAt(i)->GetInterface(L"ListItem") != NULL && 
               GetItemAt(i)->IsVisible() &&
               GetItemAt(i)->IsEnabled()){
                return i;
            }
		}
		return FindSelectable(0, true);
	}
}

void CUIContainer::SetPos(const CUIRect& rc)
{
    CUIRect rcTmp = rc;
	CUIControl::SetPos(rc);
	if(m_items.IsEmpty()) 
        return;

	rcTmp.left += m_rcInset.left;
	rcTmp.top += m_rcInset.top;
	rcTmp.right -= m_rcInset.right;
	rcTmp.bottom -= m_rcInset.bottom;

	for(int it = 0; it < m_items.GetSize(); it++){
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it]);
		if(!pControl->IsVisible()) 
            continue;
		if(pControl->IsFloat()){
			SetFloatPos(it);
		}else{
            
            //
            // 所有非float子控件放大到整个客户区
            //
            
			pControl->SetPos(rcTmp);
		}
	}
}

void CUIContainer::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if(wcscmp(pstrName, L"inset") == 0){
		CUIRect rcInset;
		LPWSTR pstr = NULL;
		rcInset.left = wcstol(pstrValue, &pstr, 10);
		rcInset.top = wcstol(pstr + 1, &pstr, 10);
		rcInset.right = wcstol(pstr + 1, &pstr, 10);
		rcInset.bottom = wcstol(pstr + 1, &pstr, 10);
		SetInset(rcInset);
	}else if(wcscmp(pstrName, L"mousechild") == 0){
        SetMouseChildEnabled(wcscmp(pstrValue, L"true") == 0);
    }else if(wcscmp(pstrName, L"vscrollbar") == 0){
		EnableScrollBar(wcscmp(pstrValue, L"true") == 0, GetHorizontalScrollBar() != NULL);
	}else if(wcscmp(pstrName, L"vscrollbarstyle") == 0 ) {
		EnableScrollBar(true, GetHorizontalScrollBar() != NULL);
		if(m_pVerticalScrollBar) 
            m_pVerticalScrollBar->ApplyAttributeList(pstrValue);
	}else if(wcscmp(pstrName, L"hscrollbar") == 0 ) {
		EnableScrollBar(m_pVerticalScrollBar != NULL, wcscmp(pstrValue, L"true") == 0);
	}else if(wcscmp(pstrName, L"hscrollbarstyle") == 0 ) {
		EnableScrollBar(m_pVerticalScrollBar != NULL, true);
		if(m_pHorizontalScrollBar) 
            m_pHorizontalScrollBar->ApplyAttributeList(pstrValue);
	}else if(wcscmp(pstrName, L"childpadding") == 0 ){
        SetChildPadding(wcstol(pstrValue, NULL, 10));
	}
	else
        CUIControl::SetAttribute(pstrName, pstrValue);
}

void CUIContainer::SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit)
{
    for(int it = 0; it < m_items.GetSize(); it++){
        static_cast<CUIControl*>(m_items[it])->SetOwnWindow(pWindow, this, bInit);
    }

	if(m_pVerticalScrollBar != NULL) 
        m_pVerticalScrollBar->SetOwnWindow(pWindow, this, bInit);
	if(m_pHorizontalScrollBar != NULL) 
        m_pHorizontalScrollBar->SetOwnWindow(pWindow, this, bInit);

    CUIControl::SetOwnWindow(pWindow, pParent, bInit);
}


CUIControl* CUIContainer::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{   
    //
	// Check if this guy is valid
	//

    if((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible()) 
        return NULL;
	if((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled()) 
        return NULL;
	if((uFlags & UIFIND_HITTEST) != 0){
		if(!m_rcItem.IsPtIn(*(static_cast<LPPOINT>(pData))) /*::PtInRect(&m_rcItem, *(static_cast<LPPOINT>(pData)))*/)
            return NULL;
		if(!m_bMouseChildEnabled){
			CUIControl* pResult = NULL;
			if(m_pVerticalScrollBar != NULL) 
                pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
			if(pResult == NULL && m_pHorizontalScrollBar != NULL) 
                pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
			if(pResult == NULL) 
                pResult = CUIControl::FindControl(Proc, pData, uFlags);
			return pResult;
		}
	}

	CUIControl* pResult = NULL;
	if(m_pVerticalScrollBar != NULL) 
        pResult = m_pVerticalScrollBar->FindControl(Proc, pData, uFlags);
	if(pResult == NULL && m_pHorizontalScrollBar != NULL) 
        pResult = m_pHorizontalScrollBar->FindControl(Proc, pData, uFlags);
	if(pResult != NULL) 
        return pResult;

	if((uFlags & UIFIND_ME_FIRST) != 0) {
		CUIControl* pControl = CUIControl::FindControl(Proc, pData, uFlags);
		if(pControl != NULL) 
            return pControl;
	}

	CUIRect rc = m_rcItem;
	rc.left += m_rcInset.left;
	rc.top += m_rcInset.top;
	rc.right -= m_rcInset.right;
	rc.bottom -= m_rcInset.bottom;
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
        rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
        rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	if((uFlags & UIFIND_TOP_FIRST) != 0 ) {
		for(int it = m_items.GetSize() - 1; it >= 0; it--) {
			CUIControl* pControl = static_cast<CUIControl*>(m_items[it])->FindControl(Proc, pData, uFlags);
			if(pControl != NULL){
				if((uFlags & UIFIND_HITTEST) != 0 &&
                    !pControl->IsFloat() &&
                    !rc.IsPtIn(*(static_cast<LPPOINT>(pData))) /*::PtInRect(&rc, *(static_cast<LPPOINT>(pData)))*/)
					continue;
				else 
					return pControl;
			}            
		}
	}else{
		for(int it = 0; it < m_items.GetSize(); it++){
			CUIControl* pControl = static_cast<CUIControl*>(m_items[it])->FindControl(Proc, pData, uFlags);
			if(pControl != NULL){
				if( (uFlags & UIFIND_HITTEST) != 0 && !pControl->IsFloat() &&
                    //!::PtInRect(&rc, *(static_cast<LPPOINT>(pData))) )
                    !rc.IsPtIn(*(static_cast<LPPOINT>(pData))) /*::PtInRect(&rc, *(static_cast<LPPOINT>(pData)))*/)
					continue;
				else 
					return pControl;
			} 
		}
	}

	if( pResult == NULL && (uFlags & UIFIND_ME_FIRST) == 0 ) 
        pResult = CUIControl::FindControl(Proc, pData, uFlags);
	return 
        pResult;
}

void CUIContainer::DoPaint(cairo_t* cr, const CUIRect& rcPaint)
{
	CUIRect rcTemp;
	if(!UIIntersectRect(&rcTemp, &rcPaint, &m_rcItem))
		return;

    //
    // 设置绘制的区域, 先绘制自身.
    //
    
	//CRenderClip clip;
	//CRenderClip::GenerateClip(hDC, rcTemp, clip);
	CUIControl::DoPaint(cr, rcPaint);

	if(m_items.GetSize() > 0) {
		CUIRect rc = m_rcItem;

        //
        // 去掉内边距
        //
        
		rc.left += m_rcInset.left;
		rc.top += m_rcInset.top;
		rc.right -= m_rcInset.right;
		rc.bottom -= m_rcInset.bottom;
        
        //
        // 去掉滚动条
        //
        
		if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
			rc.right -= m_pVerticalScrollBar->GetFixedWidth();
		if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
			rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
		
		//
		// 绘制子控件
		//

		if(!UIIntersectRect(&rcTemp, &rcPaint, &rc)){

            //
            // 如果要绘制的区域和Container没有相交的地方
            //
            
			for(int it = 0; it < m_items.GetSize(); it++){
				CUIControl* pControl = static_cast<CUIControl*>(m_items[it]);
				if(!pControl->IsVisible()) 
					continue;
                CUIRect posTemp = pControl->GetPos();
				if(!UIIntersectRect(&rcTemp, &rcPaint, &posTemp))
					continue;
                
                //
                // Float时子要绘制的子控件区域恰好在Container的内边距里的情况
                //
                
				if(pControl ->IsFloat()){
					if(!UIIntersectRect(&rcTemp, &m_rcItem, &posTemp))
						continue;
					pControl->DoPaint(cr, rcPaint);
				}
			}
		}else{
            
            //
            // 进一步缩小绘制区域
            //
            
			//CRenderClip childClip;
			//CRenderClip::GenerateClip(hDC, rcTemp, childClip);
			for(int it = 0; it < m_items.GetSize(); it++) {
				CUIControl* pControl = static_cast<CUIControl*>(m_items[it]);
				if(!pControl->IsVisible()) 
					continue;

                CUIRect posTemp = pControl->GetPos();
				if(!UIIntersectRect(&rcTemp, &rcPaint, &posTemp))
					continue;
				if(pControl ->IsFloat()){
					if(!UIIntersectRect(&rcTemp, &m_rcItem, &posTemp))
						continue;
					//CRenderClip::UseOldClipBegin(hDC, childClip);
					pControl->DoPaint(cr, rcPaint);
					//CRenderClip::UseOldClipEnd(hDC, childClip);
				}else{
					if(!UIIntersectRect(&rcTemp, &rc, &posTemp))
						continue;
					pControl->DoPaint(cr, rcPaint);
				}
			}
		}
	}
	
	//
	// 绘制滚动条
	//
	
	if(m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible()){
        CUIRect posTemp = m_pVerticalScrollBar->GetPos();
		if(UIIntersectRect(&rcTemp, &rcPaint, &posTemp)){
			m_pVerticalScrollBar->DoPaint(cr, rcPaint);
		}
	}

	if(m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible()){
        CUIRect posTemp = m_pHorizontalScrollBar->GetPos();
		if(UIIntersectRect(&rcTemp, &rcPaint, &posTemp)) {
			m_pHorizontalScrollBar->DoPaint(cr, rcPaint);
		}
	}
}

void CUIContainer::SetFloatPos(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_items.GetSize()) 
		return;

	CUIControl* pControl = static_cast<CUIControl*>(m_items[iIndex]);

	if(!pControl->IsVisible() || !pControl->IsFloat()) 
        return;

	SIZE szXY = pControl->GetFixedXY();
	SIZE sz = {pControl->GetFixedWidth(), pControl->GetFixedHeight()};
	CUIRect rcCtrl;
	if( szXY.cx >= 0 ) {
		rcCtrl.left = m_rcItem.left + szXY.cx;
		rcCtrl.right = m_rcItem.left + szXY.cx + sz.cx;
	}else{
		rcCtrl.left = m_rcItem.right + szXY.cx - sz.cx;
		rcCtrl.right = m_rcItem.right + szXY.cx;
	}
	if(szXY.cy >= 0){
		rcCtrl.top = m_rcItem.top + szXY.cy;
		rcCtrl.bottom = m_rcItem.top + szXY.cy + sz.cy;
	}else{
		rcCtrl.top = m_rcItem.bottom + szXY.cy - sz.cy;
		rcCtrl.bottom = m_rcItem.bottom + szXY.cy;
	}
	/*if( pControl->IsRelativePos() )
	{
		TRelativePosUI tRelativePos = pControl->GetRelativePos();
		SIZE szParent = {m_rcItem.right-m_rcItem.left,m_rcItem.bottom-m_rcItem.top};
		if(tRelativePos.szParent.cx != 0)
		{
			int nIncrementX = szParent.cx-tRelativePos.szParent.cx;
			int nIncrementY = szParent.cy-tRelativePos.szParent.cy;
			rcCtrl.left += (nIncrementX*tRelativePos.nMoveXPercent/100);
			rcCtrl.top += (nIncrementY*tRelativePos.nMoveYPercent/100);
			rcCtrl.right = rcCtrl.left+sz.cx+(nIncrementX*tRelativePos.nZoomXPercent/100);
			rcCtrl.bottom = rcCtrl.top+sz.cy+(nIncrementY*tRelativePos.nZoomYPercent/100);
		}
		pControl->SetRelativeParentSize(szParent);
	}*/
	pControl->SetPos(rcCtrl);
}

void CUIContainer::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
{
	if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
		RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
		m_pHorizontalScrollBar->SetPos(rcScrollBarPos);
	}

	if( m_pVerticalScrollBar == NULL ) return;

	if( cyRequired > rc.bottom - rc.top && !m_pVerticalScrollBar->IsVisible() ) {
		m_pVerticalScrollBar->SetVisible(true);
		m_pVerticalScrollBar->SetScrollRange(cyRequired - (rc.bottom - rc.top));
		m_pVerticalScrollBar->SetScrollPos(0);
		m_bScrollProcess = true;
		SetPos(m_rcItem);
		m_bScrollProcess = false;
		return;
	}

	//
	// No scrollbar required
	//

	if( !m_pVerticalScrollBar->IsVisible() ) 
        return;

	//
	// Scroll not needed anymore?
	//

	int cyScroll = cyRequired - (rc.bottom - rc.top);
	if( cyScroll <= 0 && !m_bScrollProcess) {
		m_pVerticalScrollBar->SetVisible(false);
		m_pVerticalScrollBar->SetScrollPos(0);
		m_pVerticalScrollBar->SetScrollRange(0);
		SetPos(m_rcItem);
	}else{

		RECT rcScrollBarPos = {rc.right, rc.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), rc.bottom};
		m_pVerticalScrollBar->SetPos(rcScrollBarPos);

		if(m_pVerticalScrollBar->GetScrollRange() != cyScroll) {
			int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll));
			if(m_pVerticalScrollBar->GetScrollRange() == 0) {
				m_pVerticalScrollBar->SetVisible(false);
				m_pVerticalScrollBar->SetScrollPos(0);
			}
			if(iScrollPos > m_pVerticalScrollBar->GetScrollPos()){
				SetPos(m_rcItem);
			}
		}
	}
}

bool CUIContainer::SetSubControlText(LPCWSTR pstrSubControlName,LPCWSTR pstrText)
{
	CUIControl* pSubControl=NULL;
	pSubControl=this->FindSubControl(pstrSubControlName);
	if (pSubControl!=NULL){
		pSubControl->SetText(pstrText);
		return TRUE;
    }else{
		return FALSE;
    }
}

bool CUIContainer::SetSubControlFixedHeight(LPCWSTR pstrSubControlName,int cy)
{
	CUIControl* pSubControl=NULL;
	pSubControl = FindSubControl(pstrSubControlName);
	if (pSubControl!=NULL){
		pSubControl->SetFixedHeight(cy);
		return TRUE;
    }else{
		return FALSE;
    }
}

bool CUIContainer::SetSubControlFixedWdith(LPCWSTR pstrSubControlName,int cx)
{
	CUIControl* pSubControl=NULL;
	pSubControl = this->FindSubControl(pstrSubControlName);
	if (pSubControl!=NULL){
		pSubControl->SetFixedWidth(cx);
		return TRUE;
    }else{
		return FALSE;
    }
}

CUIString CUIContainer::GetSubControlText( LPCWSTR pstrSubControlName )
{
	CUIControl* pSubControl=NULL;
	pSubControl=this->FindSubControl(pstrSubControlName);
	if (pSubControl==NULL)
		return L"";
	//else
		//return pSubControl->GetText();
    return L"";
}

int CUIContainer::GetSubControlFixedHeight( LPCWSTR pstrSubControlName )
{
	CUIControl* pSubControl=NULL;
	pSubControl=this->FindSubControl(pstrSubControlName);
	if (pSubControl==NULL)
		return -1;
	else
		return pSubControl->GetFixedHeight();
}

int CUIContainer::GetSubControlFixedWdith( LPCWSTR pstrSubControlName )
{
	CUIControl* pSubControl=NULL;
	pSubControl=this->FindSubControl(pstrSubControlName);
	if (pSubControl==NULL)
		return -1;
	else
		return pSubControl->GetFixedWidth();
}

CUIControl* CUIContainer::FindSubControl(LPCWSTR pstrSubControlName)
{
	return static_cast<CUIControl*>(m_Window->FindSubControlByName(this, pstrSubControlName));
}

