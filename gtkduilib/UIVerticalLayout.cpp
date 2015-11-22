#include "stdafx.h"
#include "UIVerticalLayout.h"

CUIVerticalLayout::CUIVerticalLayout() : m_iSepHeight(0), m_uButtonState(0), m_bImmMode(false)
{
	ptLastMouse.x = ptLastMouse.y = 0;
	::ZeroMemory(&m_rcNewPos, sizeof(m_rcNewPos));
}

CUIString CUIVerticalLayout::GetClass() const
{
	return L"VerticalLayoutUI";
}

LPVOID CUIVerticalLayout::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_VERTICALLAYOUT) 
		return static_cast<CUIVerticalLayout*>(this);
	return 
        CUIContainer::GetInterface(strName);
}

UINT CUIVerticalLayout::GetControlFlags() const
{
	if( IsEnabled() && m_iSepHeight != 0 ) 
		return UIFLAG_SETCURSOR;
	else 
		return 0;
}

void CUIVerticalLayout::SetPos(const CUIRect& rc)
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
	
	if( m_items.GetSize() == 0) {
		ProcessScrollBar(rcTemp, 0, 0);
		return;
	}

	//
	// 如果有滚动条，减去滚动条的宽度
	//
	
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
		rcTemp.right -= m_pVerticalScrollBar->GetFixedWidth();

	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
		rcTemp.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	//
	// Determine the minimum size
	//

	SIZE szAvailable = { rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top };
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

	int nAdjustables = 0; //要调整控件的个数
	int cyFixed = 0;	  //控件固定的宽度
	int nEstimateNum = 0; //调整控件的个数

	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it1]);
		
		//
		// 控件不可见不进行处理
		//
		
		if(!pControl->IsVisible()) 
			continue;
		
		//
		// 控件为float类型的不进行处理
		//
		
		if(pControl->IsFloat()) 
			continue;

		SIZE sz = pControl->EstimateSize(szAvailable);
		if(sz.cy == 0) {
			
			//
			// 高度等于0需要我们来计算
			//
			
			nAdjustables++;
		}else{
			
			//
			// 不能超出控件的设置的最小和最大值
			//
			
			if( sz.cy < pControl->GetMinHeight() ) 
				sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) 
				sz.cy = pControl->GetMaxHeight();
		}
		
		//
		// 加上下外边距
		//
		
		cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;
		nEstimateNum++;
	}
	
	//
	// 加上子控件之间的额外距离
	//
	
	cyFixed += (nEstimateNum - 1) * m_iChildPadding;

	//
	// Place elements
	//

	int cyNeeded = 0;
	int cyExpand = 0;
	
	//
	// 计算要调整的每个控件的平均高度
	//
	
	if( nAdjustables > 0 ) 
		cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);

	//
	// Position the elements
	// 减去滚动条的位置
	//

	SIZE szRemaining = szAvailable;
	int iPosY = rcTemp.top;
	if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
		iPosY -= m_pVerticalScrollBar->GetScrollPos();
	}

	int iPosX = rcTemp.left;
	if(m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}

	int iAdjustable = 0;
	int cyFixedRemaining = cyFixed;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CUIControl* pControl = static_cast<CUIControl*>(m_items[it2]);
		if( !pControl->IsVisible() )
			continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it2);
			continue;
		}

		RECT rcPadding = pControl->GetPadding();
		szRemaining.cy -= rcPadding.top;
		CUISize sz = pControl->EstimateSize(szRemaining);
		if(sz.cy == 0){
			iAdjustable++;
			
			//
			// 赋平均值
			//
			
			sz.cy = cyExpand;
			if( iAdjustable == nAdjustables ) {

				//
				// Distribute remaining to last element (usually round-off left-overs)
				//

				sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
			} 
			
			//
			// 不能超过控件的最大与最小值
			//
			
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
		}else{
			
			//
			// 固定高度的控件
			//
			
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			cyFixedRemaining -= sz.cy;
		}
		
		//
		// 调整宽度的值
		//
		
		sz.cx = pControl->GetFixedWidth();
		if( sz.cx == 0 ) sz.cx = szAvailable.cx - rcPadding.left - rcPadding.right;
		if( sz.cx < 0 ) sz.cx = 0;
		if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
		if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		
		//
		// 生成并设置新的POS
		//
		
		CUIRect rcCtrl(iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, 
            iPosY + sz.cy + rcPadding.top + rcPadding.bottom);
		pControl->SetPos(rcCtrl);
		
		//
		// 处理下ChildPadding
		//
		
		iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
		cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
		szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
	}
	cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

	//
	// Process the scrollbar
	//

	ProcessScrollBar(rcTemp, 0, cyNeeded);
}

void CUIVerticalLayout::DoPostPaint(cairo_t *cr, const CUIRect& rcPaint)
{
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode ) {
		RECT rcSeparator = GetThumbRect(true);
		CUIRender::DrawColor(cr, rcSeparator, 0xAA000000);
	}
}

void CUIVerticalLayout::SetSepHeight(int iHeight)
{
	m_iSepHeight = iHeight;
}

int CUIVerticalLayout::GetSepHeight() const
{
	return m_iSepHeight;
}

void CUIVerticalLayout::SetSepImmMode(bool bImmediately)
{
	if( m_bImmMode == bImmediately ) return;
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 && !m_bImmMode /*&& m_pManager != NULL*/ ) {
		//m_pManager->RemovePostPaint(this);
	}

	m_bImmMode = bImmediately;
}

bool CUIVerticalLayout::IsSepImmMode() const
{
	return m_bImmMode;
}

void CUIVerticalLayout::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if( wcscmp(pstrName, L"sepheight") == 0 )
		SetSepHeight(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if( wcscmp(pstrName, L"sepimm") == 0 )
		SetSepImmMode(wcscmp(pstrValue, L"true") == 0);
	else 
		CUIContainer::SetAttribute(pstrName, pstrValue);
}

void CUIVerticalLayout::DoEvent(TEventUI& event)
{
	if( m_iSepHeight != 0 ) {
		if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() ){
			CUIRect rcSeparator = GetThumbRect(false);
			if(rcSeparator.IsPtIn(event.ptMouse)) {
				m_uButtonState |= UISTATE_CAPTURED;
				ptLastMouse = event.ptMouse;
				m_rcNewPos = m_rcItem;
				if(!m_bImmMode /*&& m_pManager*/) 
					/*m_pManager->AddPostPaint(this);*/
				return;
			}
		}
		if( event.Type == UIEVENT_BUTTONUP ){
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				m_uButtonState &= ~UISTATE_CAPTURED;
				m_rcItem = m_rcNewPos;
				if( !m_bImmMode /*&& m_pManager*/ ) 
					/*m_pManager->RemovePostPaint(this);*/
				NeedParentUpdate();
				return;
			}
		}
		if( event.Type == UIEVENT_MOUSEMOVE ){
			if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
				LONG cy = event.ptMouse.y - ptLastMouse.y;
				ptLastMouse = event.ptMouse;
				RECT rc = m_rcNewPos;
				if( m_iSepHeight >= 0 ) {
					if( cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight ) 
						return;
					if( cy < 0 && event.ptMouse.y > m_rcNewPos.bottom ) 
						return;
					rc.bottom += cy;
					if( rc.bottom - rc.top <= GetMinHeight() ) {
						if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) 
							return;
						rc.bottom = rc.top + GetMinHeight();
					}
					if( rc.bottom - rc.top >= GetMaxHeight() ) {
						if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) 
							return;
						rc.bottom = rc.top + GetMaxHeight();
					}
				}else{
					if( cy > 0 && event.ptMouse.y < m_rcNewPos.top ) 
						return;
					if( cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight ) 
						return;
					rc.top += cy;
					if( rc.bottom - rc.top <= GetMinHeight() ) {
						if( m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight() ) 
							return;
						rc.top = rc.bottom - GetMinHeight();
					}
					if( rc.bottom - rc.top >= GetMaxHeight() ) {
						if( m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight() ) 
							return;
						rc.top = rc.bottom - GetMaxHeight();
					}
				}

				CUIRect rcInvalidate = GetThumbRect(true);
				m_rcNewPos = rc;
				m_cxyFixed.cy = m_rcNewPos.bottom - m_rcNewPos.top;

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
		if( event.Type == UIEVENT_SETCURSOR )
		{
			CUIRect rcSeparator = GetThumbRect(false);
			if( IsEnabled() && rcSeparator.IsPtIn(event.ptMouse)) {

                //
				// TODO: SetCursor
				//

				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
				return;
			}
		}
	}
	CUIContainer::DoEvent(event);
}

CUIRect CUIVerticalLayout::GetThumbRect(bool bUseNew) const
{
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 && bUseNew) {
		if( m_iSepHeight >= 0 ) 
			return CUIRect(m_rcNewPos.left, MAX(m_rcNewPos.bottom - m_iSepHeight, m_rcNewPos.top), 
			m_rcNewPos.right, m_rcNewPos.bottom);
		else 
			return CUIRect(m_rcNewPos.left, m_rcNewPos.top, m_rcNewPos.right, 
			MIN(m_rcNewPos.top - m_iSepHeight, m_rcNewPos.bottom));
	}else{
		if( m_iSepHeight >= 0 ) 
			return CUIRect(m_rcItem.left, MAX(m_rcItem.bottom - m_iSepHeight, m_rcItem.top), m_rcItem.right, 
			m_rcItem.bottom);
		else 
			return CUIRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, 
			MIN(m_rcItem.top - m_iSepHeight, m_rcItem.bottom));

	}
}
