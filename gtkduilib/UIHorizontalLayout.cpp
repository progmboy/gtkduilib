#include "stdafx.h"
#include "UIHorizontalLayout.h"

CUIHorizontalLayout::CUIHorizontalLayout() : m_iSepWidth(0), m_uButtonState(0), m_bImmMode(false)
{
	ptLastMouse.x = ptLastMouse.y = 0;
	::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
}

CUIString CUIHorizontalLayout::GetClass() const
{
	return L"HorizontalLayoutUI";
}

LPVOID CUIHorizontalLayout::GetInterface(const CUIString& strName)
{
	if(strName ==  DUI_CTR_HORIZONTALLAYOUT) 
		return static_cast<CUIHorizontalLayout*>(this);
	return 
        CUIContainer::GetInterface(strName);
}

UINT CUIHorizontalLayout::GetControlFlags() const
{
	if(IsEnabled() && m_iSepWidth != 0) 
        return UIFLAG_SETCURSOR;
	else 
        return 0;
}

void CUIHorizontalLayout::SetPos(const CUIRect& rc)
{
    CUIRect rcTemp = rc;
	CUIControl::SetPos(rc);
	rcTemp = m_rcItem;

	//
	// 调整内边距
	//

	rcTemp.left += m_rcInset.left;
	rcTemp.top += m_rcInset.top;
	rcTemp.right -= m_rcInset.right;
	rcTemp.bottom -= m_rcInset.bottom;
	
	//
	// 没有子控件不处理
	//
	
	if( m_items.GetSize() == 0) {
		ProcessScrollBar(rcTemp, 0, 0);
		return;
	}
	
	//
	// 滚动条不变
	//
	
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
		rcTemp.right -= m_pVerticalScrollBar->GetFixedWidth();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		rcTemp.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	//
	// Determine the width of elements that are sizeable
	// 加上滚动条的宽度
	//

	CUISize szAvailable(rcTemp.GetWidth(), rcTemp.GetHeight());
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

	int nAdjustables = 0;
	int cxFixed = 0;
	int nEstimateNum = 0;

	//
	// 计算子控件的总的固定宽度
	//
	
	for(int it1 = 0; it1 < m_items.GetSize(); it1++) {
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it1]);
		
		//
		// 跳过不可见和属性设置为Float的。
		//
		
		if(!pControl->IsVisible()) 
			continue;
		if(pControl->IsFloat()) 
			continue;

		SIZE sz = pControl->EstimateSize(szAvailable);
		if( sz.cx == 0 ) {
			nAdjustables++;
		}else{
			if(sz.cx < pControl->GetMinWidth()) 
				sz.cx = pControl->GetMinWidth();
			if(sz.cx > pControl->GetMaxWidth()) 
				sz.cx = pControl->GetMaxWidth();
		}
		cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
		nEstimateNum++;
	}
	cxFixed += (nEstimateNum - 1) * m_iChildPadding;

	int cxExpand = 0;
    int cxNeeded = 0;
	
	//
	// cxExpand 为要调整宽度的平均大小
	//
	
	if(nAdjustables > 0) 
		cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);

	//
	// Position the elements
	// 确定子控件的位置
	//

	SIZE szRemaining = szAvailable;
	int iPosX = rcTemp.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}
	int iAdjustable = 0;
	int cxFixedRemaining = cxFixed;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it2]);
		if( !pControl->IsVisible() ) 
			continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it2);
			continue;
		}
		
		//
		// 调整带Padding属性的控件
		//
		
		RECT rcPadding = pControl->GetPadding();
		szRemaining.cx -= rcPadding.left;
		SIZE sz = pControl->EstimateSize(szRemaining);
		if( sz.cx == 0 ) {
			iAdjustable++;
			sz.cx = cxExpand;

			//
			// Distribute remaining to last element (usually round-off left-overs)
			// 最后一个特殊处理一下，将计算平均值剩下的放在最后一个里面
			//

			if( iAdjustable == nAdjustables ) {
				sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
			}
			if( sz.cx < pControl->GetMinWidth() ) 
				sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) 
				sz.cx = pControl->GetMaxWidth();
		}else{
			if( sz.cx < pControl->GetMinWidth() ) 
				sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) 
				sz.cx = pControl->GetMaxWidth();

			cxFixedRemaining -= sz.cx;
		}

		sz.cy = pControl->GetFixedHeight();
		if(sz.cy == 0) 
			sz.cy = rcTemp.bottom - rcTemp.top - rcPadding.top - rcPadding.bottom;
		if(sz.cy < 0) 
			sz.cy = 0;
		if(sz.cy < pControl->GetMinHeight()) 
			sz.cy = pControl->GetMinHeight();
		if(sz.cy > pControl->GetMaxHeight()) 
			sz.cy = pControl->GetMaxHeight();
		
		//
		// 生成并设置控件的新的Pos
		//
		
		CUIRect rcCtrl(iPosX + rcPadding.left, rcTemp.top + rcPadding.top, 
            iPosX + sz.cx + rcPadding.left + rcPadding.right, 
            rcTemp.top + rcPadding.top + sz.cy);
		pControl->SetPos(rcCtrl);
		
		//
		// 调整ChildPadding
		//
		
		iPosX += sz.cx + m_iChildPadding + rcPadding.left + rcPadding.right;
        cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
		szRemaining.cx -= sz.cx + m_iChildPadding + rcPadding.right;
	}
    cxNeeded += (nEstimateNum - 1) * m_iChildPadding;

	//
	// Process the scrollbar
	//

	ProcessScrollBar(rcTemp, cxNeeded, 0);
}

void CUIHorizontalLayout::DoPostPaint(cairo_t *cr, const CUIRect& rcPaint)
{
	if((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode){
		CUIRect rcSeparator = GetThumbRect(true);
		CUIRender::DrawColor(cr, rcSeparator, 0xAA000000);
	}
}

void CUIHorizontalLayout::SetSepWidth(int iWidth)
{
	m_iSepWidth = iWidth;
}

int CUIHorizontalLayout::GetSepWidth() const
{
	return m_iSepWidth;
}

void CUIHorizontalLayout::SetSepImmMode(bool bImmediately)
{
	if( m_bImmMode == bImmediately ) return;
	if((m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode /*&& m_pManager != NULL*/){
		//m_pManager->RemovePostPaint(this);
	}

	m_bImmMode = bImmediately;
}

bool CUIHorizontalLayout::IsSepImmMode() const
{
	return m_bImmMode;
}

void CUIHorizontalLayout::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if(wcscmp(pstrName, L"sepwidth") == 0)
        SetSepWidth(wcstol(pstrValue, NULL, 10));
	else if(wcscmp(pstrName, L"sepimm") == 0)
        SetSepImmMode(wcscmp(pstrValue, L"true") == 0);
	else 
        CUIContainer::SetAttribute(pstrName, pstrValue);
}

void CUIHorizontalLayout::DoEvent(TEventUI& event)
{
	if( m_iSepWidth != 0 ) {
		if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() ){
			CUIRect rcSeparator = GetThumbRect(false);
			if(rcSeparator.IsPtIn(event.ptMouse)){
				m_uButtonState |= UISTATE_CAPTURED;
				ptLastMouse = event.ptMouse;
				m_rcNewPos = m_rcItem;
				if(!m_bImmMode /*&& m_pManager*/) 
                    //m_pManager->AddPostPaint(this);
				return;
			}
		}
		if(event.Type == UIEVENT_BUTTONUP){
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				m_uButtonState &= ~UISTATE_CAPTURED;
				m_rcItem = m_rcNewPos;
                if(!m_bImmMode /*&& m_pManager*/){
                    //m_pManager->RemovePostPaint(this);
                }
				NeedParentUpdate();
				return;
			}
		}
		if( event.Type == UIEVENT_MOUSEMOVE ){
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				LONG cx = event.ptMouse.x - ptLastMouse.x;
				ptLastMouse = event.ptMouse;
				RECT rc = m_rcNewPos;
				if( m_iSepWidth >= 0 ) {
					if( cx > 0 && event.ptMouse.x < m_rcNewPos.right - m_iSepWidth ) return;
					if( cx < 0 && event.ptMouse.x > m_rcNewPos.right ) return;
					rc.right += cx;
					if( rc.right - rc.left <= GetMinWidth() ) {
						if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
						rc.right = rc.left + GetMinWidth();
					}
					if( rc.right - rc.left >= GetMaxWidth() ) {
						if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
						rc.right = rc.left + GetMaxWidth();
					}
				}else{
					if( cx > 0 && event.ptMouse.x < m_rcNewPos.left ) return;
					if( cx < 0 && event.ptMouse.x > m_rcNewPos.left - m_iSepWidth ) return;
					rc.left += cx;
					if( rc.right - rc.left <= GetMinWidth() ) {
						if( m_rcNewPos.right - m_rcNewPos.left <= GetMinWidth() ) return;
						rc.left = rc.right - GetMinWidth();
					}
					if( rc.right - rc.left >= GetMaxWidth() ) {
						if( m_rcNewPos.right - m_rcNewPos.left >= GetMaxWidth() ) return;
						rc.left = rc.right - GetMaxWidth();
					}
				}

				CUIRect rcInvalidate = GetThumbRect(true);
				m_rcNewPos = rc;
				m_cxyFixed.cx = m_rcNewPos.right - m_rcNewPos.left;

				if( m_bImmMode ) {
					m_rcItem = m_rcNewPos;
					NeedParentUpdate();
				}else{
					rcInvalidate.Join(GetThumbRect(true));
					rcInvalidate.Join(GetThumbRect(false));
					if(m_Window) 
                        m_Window->Invalidate(rcInvalidate);
				}
				return;
			}
		}
		if(event.Type == UIEVENT_SETCURSOR){
			CUIRect rcSeparator = GetThumbRect(false);
			if( IsEnabled() && rcSeparator.IsPtIn(event.ptMouse)) {

                //
                // TODO:SetCursor
                //

				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
				return;
			}
		}
	}
	CUIContainer::DoEvent(event);
}

CUIRect CUIHorizontalLayout::GetThumbRect(bool bUseNew) const
{
	
	//
	// m_iSepWidth 为负数在左边，正数在右边
	//
	
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) {
		if( m_iSepWidth >= 0 ) 
			return CUIRect(m_rcNewPos.right - m_iSepWidth, m_rcNewPos.top, m_rcNewPos.right, m_rcNewPos.bottom);
		else 
			return CUIRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.left - m_iSepWidth, m_rcNewPos.bottom);
	}else{
		if( m_iSepWidth >= 0 ) 
			return CUIRect(m_rcItem.right - m_iSepWidth, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
		else 
			return CUIRect(m_rcItem.left, m_rcItem.top, m_rcItem.left - m_iSepWidth, m_rcItem.bottom);
	}
}
