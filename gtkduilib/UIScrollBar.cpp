#include "stdafx.h"
#include "UIScrollBar.h"

CUIScrollBar::CUIScrollBar() : 
    m_bHorizontal(false), 
    m_nRange(100), 
    m_nScrollPos(0), 
    m_nLineSize(8), 
	m_pOwner(NULL), 
    m_nLastScrollPos(0), 
    m_nLastScrollOffset(0), 
    m_nScrollRepeatDelay(0), 
    m_uButton1State(0), 
	m_uButton2State(0), 
    m_uThumbState(0), 
    m_bShowButton1(true), 
    m_bShowButton2(true),
    m_TimerId(0)
{
	m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
	ptLastMouse.x = ptLastMouse.y = 0;
}

CUIString CUIScrollBar::GetClass() const
{
	return L"ScrollBarUI";
}

LPVOID CUIScrollBar::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_SCROLLBAR) 
        return static_cast<CUIScrollBar*>(this);
	return CUIControl::GetInterface(strName);
}

CUIContainer* CUIScrollBar::GetOwner() const
{
	return m_pOwner;
}

void CUIScrollBar::SetOwner(CUIContainer* pOwner)
{
	m_pOwner = pOwner;
}

void CUIScrollBar::SetVisible(bool bVisible)
{
	if(m_bVisible == bVisible) 
        return;

	bool v = IsVisible();
	m_bVisible = bVisible;
	if(m_bFocused) 
        m_bFocused = false;
}

void CUIScrollBar::SetEnabled(bool bEnable)
{
	CUIControl::SetEnabled(bEnable);
	if(!IsEnabled()){
		m_uButton1State = 0;
		m_uButton2State = 0;
		m_uThumbState = 0;
	}
}

void CUIScrollBar::SetFocus()
{
	/*if(m_pOwner != NULL) 
        m_pOwner->SetFocus();
	else 
        CUIControl::SetFocus();*/
}

bool CUIScrollBar::IsHorizontal()
{
	return m_bHorizontal;
}

void CUIScrollBar::SetHorizontal(bool bHorizontal)
{
	if(m_bHorizontal == bHorizontal) 
        return;

	m_bHorizontal = bHorizontal;
	if(m_bHorizontal) {
		if(m_cxyFixed.cy == 0){
			m_cxyFixed.cx = 0;
			m_cxyFixed.cy = DEFAULT_SCROLLBAR_SIZE;
		}
	}else{
		if(m_cxyFixed.cx == 0){
			m_cxyFixed.cx = DEFAULT_SCROLLBAR_SIZE;
			m_cxyFixed.cy = 0;
		}
	}

	if(m_pOwner != NULL) 
        m_pOwner->NeedUpdate(); 
    else 
        NeedParentUpdate();
}

int CUIScrollBar::GetScrollRange() const
{
	return m_nRange;
}

void CUIScrollBar::SetScrollRange(int nRange)
{
	if(m_nRange == nRange) 
        return;

	m_nRange = nRange;
	if(m_nRange < 0) 
        m_nRange = 0;
	if(m_nScrollPos > m_nRange) 
        m_nScrollPos = m_nRange;
	SetPos(m_rcItem);
}

int CUIScrollBar::GetScrollPos() const
{
	return m_nScrollPos;
}

void CUIScrollBar::SetScrollPos(int nPos)
{
	if(m_nScrollPos == nPos) 
        return;

	m_nScrollPos = nPos;
	if(m_nScrollPos < 0) 
        m_nScrollPos = 0;
	if(m_nScrollPos > m_nRange) 
        m_nScrollPos = m_nRange;
	SetPos(m_rcItem);
}

int CUIScrollBar::GetLineSize() const
{
	return m_nLineSize;
}

void CUIScrollBar::SetLineSize(int nSize)
{
	m_nLineSize = nSize;
}

bool CUIScrollBar::GetShowButton1()
{
	return m_bShowButton1;
}

void CUIScrollBar::SetShowButton1(bool bShow)
{
	m_bShowButton1 = bShow;
	SetPos(m_rcItem);
}

CUIString CUIScrollBar::GetButton1NormalImage()
{
	return m_sButton1NormalImage;
}

void CUIScrollBar::SetButton1NormalImage(const CUIString& strImage)
{
	m_sButton1NormalImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton1HotImage()
{
	return m_sButton1HotImage;
}

void CUIScrollBar::SetButton1HotImage(const CUIString& strImage)
{
	m_sButton1HotImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton1PushedImage()
{
	return m_sButton1PushedImage;
}

void CUIScrollBar::SetButton1PushedImage(const CUIString& strImage)
{
	m_sButton1PushedImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton1DisabledImage()
{
	return m_sButton1DisabledImage;
}

void CUIScrollBar::SetButton1DisabledImage(const CUIString& strImage)
{
	m_sButton1DisabledImage = strImage;
	Invalidate();
}

bool CUIScrollBar::GetShowButton2()
{
	return m_bShowButton2;
}

void CUIScrollBar::SetShowButton2(bool bShow)
{
	m_bShowButton2 = bShow;
	SetPos(m_rcItem);
}

CUIString CUIScrollBar::GetButton2NormalImage()
{
	return m_sButton2NormalImage;
}

void CUIScrollBar::SetButton2NormalImage(const CUIString& strImage)
{
	m_sButton2NormalImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton2HotImage()
{
	return m_sButton2HotImage;
}

void CUIScrollBar::SetButton2HotImage(const CUIString& strImage)
{
	m_sButton2HotImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton2PushedImage()
{
	return m_sButton2PushedImage;
}

void CUIScrollBar::SetButton2PushedImage(const CUIString& strImage)
{
	m_sButton2PushedImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetButton2DisabledImage()
{
	return m_sButton2DisabledImage;
}

void CUIScrollBar::SetButton2DisabledImage(const CUIString& strImage)
{
	m_sButton2DisabledImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetThumbNormalImage()
{
	return m_sThumbNormalImage;
}

void CUIScrollBar::SetThumbNormalImage(const CUIString& strImage)
{
	m_sThumbNormalImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetThumbHotImage()
{
	return m_sThumbHotImage;
}

void CUIScrollBar::SetThumbHotImage(const CUIString& strImage)
{
	m_sThumbHotImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetThumbPushedImage()
{
	return m_sThumbPushedImage;
}

void CUIScrollBar::SetThumbPushedImage(const CUIString& strImage)
{
	m_sThumbPushedImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetThumbDisabledImage()
{
	return m_sThumbDisabledImage;
}

void CUIScrollBar::SetThumbDisabledImage(const CUIString& strImage)
{
	m_sThumbDisabledImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetRailNormalImage()
{
	return m_sRailNormalImage;
}

void CUIScrollBar::SetRailNormalImage(const CUIString& strImage)
{
	m_sRailNormalImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetRailHotImage()
{
	return m_sRailHotImage;
}

void CUIScrollBar::SetRailHotImage(const CUIString& strImage)
{
	m_sRailHotImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetRailPushedImage()
{
	return m_sRailPushedImage;
}

void CUIScrollBar::SetRailPushedImage(const CUIString& strImage)
{
	m_sRailPushedImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetRailDisabledImage()
{
	return m_sRailDisabledImage;
}

void CUIScrollBar::SetRailDisabledImage(const CUIString& strImage)
{
	m_sRailDisabledImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetBkNormalImage()
{
	return m_sBkNormalImage;
}

void CUIScrollBar::SetBkNormalImage(const CUIString& strImage)
{
	m_sBkNormalImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetBkHotImage()
{
	return m_sBkHotImage;
}

void CUIScrollBar::SetBkHotImage(const CUIString& strImage)
{
	m_sBkHotImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetBkPushedImage()
{
	return m_sBkPushedImage;
}

void CUIScrollBar::SetBkPushedImage(const CUIString& strImage)
{
	m_sBkPushedImage = strImage;
	Invalidate();
}

CUIString CUIScrollBar::GetBkDisabledImage()
{
	return m_sBkDisabledImage;
}

void CUIScrollBar::SetBkDisabledImage(const CUIString& strImage)
{
	m_sBkDisabledImage = strImage;
	Invalidate();
}

void CUIScrollBar::SetPos(const CUIRect& rc)
{
    CUIRect rcTemp = rc;
	CUIControl::SetPos(rc);
	rcTemp = m_rcItem;
    
	if(m_bHorizontal){

        //
        // 横向滚动条
        //

		int cx = rcTemp.GetWidth();
        
		if(m_bShowButton1) 
            cx -= m_cxyFixed.cy;

		if(m_bShowButton2) 
            cx -= m_cxyFixed.cy;

		if(cx > m_cxyFixed.cy){
			m_rcButton1.left = rcTemp.left;
			m_rcButton1.top = rcTemp.top;
			if(m_bShowButton1){
				m_rcButton1.right = rcTemp.left + m_cxyFixed.cy;
				m_rcButton1.bottom = rcTemp.top + m_cxyFixed.cy;
			}else{
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.top = rcTemp.top;
			m_rcButton2.right = rcTemp.right;
			if(m_bShowButton2){
				m_rcButton2.left = rcTemp.right - m_cxyFixed.cy;
				m_rcButton2.bottom = rcTemp.top + m_cxyFixed.cy;
			}else{
				m_rcButton2.left = m_rcButton2.right;
				m_rcButton2.bottom = m_rcButton2.top;
			}

			m_rcThumb.top = rcTemp.top;
			m_rcThumb.bottom = rcTemp.top + m_cxyFixed.cy;
			if(m_nRange > 0){
				int cxThumb = cx * (rcTemp.right - rcTemp.left) / (m_nRange + rcTemp.right - rcTemp.left);
				if(cxThumb < m_cxyFixed.cy) 
                    cxThumb = m_cxyFixed.cy;

				m_rcThumb.left = m_nScrollPos * (cx - cxThumb) / m_nRange + m_rcButton1.right;
				m_rcThumb.right = m_rcThumb.left + cxThumb;
				if(m_rcThumb.right > m_rcButton2.left){
					m_rcThumb.left = m_rcButton2.left - cxThumb;
					m_rcThumb.right = m_rcButton2.left;
				}
			}else{
				m_rcThumb.left = m_rcButton1.right;
				m_rcThumb.right = m_rcButton2.left;
			}
		}else{

			int cxButton = (rcTemp.right - rcTemp.left) / 2;
			if(cxButton > m_cxyFixed.cy) 
                cxButton = m_cxyFixed.cy;

			m_rcButton1.left = rcTemp.left;
			m_rcButton1.top = rcTemp.top;
			if(m_bShowButton1){
				m_rcButton1.right = rcTemp.left + cxButton;
				m_rcButton1.bottom = rcTemp.top + m_cxyFixed.cy;
			}else{
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.top = rcTemp.top;
			m_rcButton2.right = rcTemp.right;
			if(m_bShowButton2){
				m_rcButton2.left = rcTemp.right - cxButton;
				m_rcButton2.bottom = rcTemp.top + m_cxyFixed.cy;
			}else{
				m_rcButton2.left = m_rcButton2.right;
				m_rcButton2.bottom = m_rcButton2.top;
			}

			::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
		}
	}else{

        //
        // 竖向滚动条
        //

		int cy = rcTemp.bottom - rcTemp.top;
		if(m_bShowButton1) 
            cy -= m_cxyFixed.cx;
		if(m_bShowButton2) 
            cy -= m_cxyFixed.cx;
		if(cy > m_cxyFixed.cx){
			m_rcButton1.left = rcTemp.left;
			m_rcButton1.top = rcTemp.top;
			if(m_bShowButton1){
				m_rcButton1.right = rcTemp.left + m_cxyFixed.cx;
				m_rcButton1.bottom = rcTemp.top + m_cxyFixed.cx;
			}else{
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.left = rcTemp.left;
			m_rcButton2.bottom = rcTemp.bottom;
			if(m_bShowButton2){
				m_rcButton2.top = rcTemp.bottom - m_cxyFixed.cx;
				m_rcButton2.right = rcTemp.left + m_cxyFixed.cx;
			}else{
				m_rcButton2.top = m_rcButton2.bottom;
				m_rcButton2.right = m_rcButton2.left;
			}

			m_rcThumb.left = rcTemp.left;
			m_rcThumb.right = rcTemp.left + m_cxyFixed.cx;
			if(m_nRange > 0){
				int cyThumb = cy * (rcTemp.bottom - rcTemp.top) / (m_nRange + rcTemp.bottom - rcTemp.top);
				if(cyThumb < m_cxyFixed.cx) 
                    cyThumb = m_cxyFixed.cx;

				m_rcThumb.top = m_nScrollPos * (cy - cyThumb) / m_nRange + m_rcButton1.bottom;
				m_rcThumb.bottom = m_rcThumb.top + cyThumb;
				if(m_rcThumb.bottom > m_rcButton2.top){
					m_rcThumb.top = m_rcButton2.top - cyThumb;
					m_rcThumb.bottom = m_rcButton2.top;
				}
			}else{
				m_rcThumb.top = m_rcButton1.bottom;
				m_rcThumb.bottom = m_rcButton2.top;
			}
		}else{
			int cyButton = (rcTemp.bottom - rcTemp.top) / 2;
			if(cyButton > m_cxyFixed.cx) 
                cyButton = m_cxyFixed.cx;
			m_rcButton1.left = rcTemp.left;
			m_rcButton1.top = rcTemp.top;
			if(m_bShowButton1){
				m_rcButton1.right = rcTemp.left + m_cxyFixed.cx;
				m_rcButton1.bottom = rcTemp.top + cyButton;
			}else{
				m_rcButton1.right = m_rcButton1.left;
				m_rcButton1.bottom = m_rcButton1.top;
			}

			m_rcButton2.left = rcTemp.left;
			m_rcButton2.bottom = rcTemp.bottom;
			if(m_bShowButton2){
				m_rcButton2.top = rcTemp.bottom - cyButton;
				m_rcButton2.right = rcTemp.left + m_cxyFixed.cx;
			}else{
				m_rcButton2.top = m_rcButton2.bottom;
				m_rcButton2.right = m_rcButton2.left;
			}

			::ZeroMemory(&m_rcThumb, sizeof(m_rcThumb));
		}
	}
}

void CUIScrollBar::DoEvent(TEventUI& Event)
{
	if(!IsMouseEnabled() && Event.Type > UIEVENT__MOUSEBEGIN && Event.Type < UIEVENT__MOUSEEND){
		if(m_pOwner != NULL) 
            m_pOwner->DoEvent(Event);
		else 
            CUIControl::DoEvent(Event);
		return;
	}

	if(Event.Type == UIEVENT_SETFOCUS || Event.Type == UIEVENT_KILLFOCUS){
		return;
	}

	if(Event.Type == UIEVENT_BUTTONDOWN || Event.Type == UIEVENT_DBLCLICK){
		if(!IsEnabled()) 
            return;

		m_nLastScrollOffset = 0;
		m_nScrollRepeatDelay = 0;
		m_TimerId = m_Window->SetTimer(this, 50U);

		if(m_rcButton1.IsPtIn(Event.ptMouse)){
			m_uButton1State |= UISTATE_PUSHED;
			if(!m_bHorizontal){
				if(m_pOwner != NULL) 
                    m_pOwner->LineUp(); 
				else 
                    SetScrollPos(m_nScrollPos - m_nLineSize);
			}else{
				if(m_pOwner != NULL) 
                    m_pOwner->LineLeft(); 
				else 
                    SetScrollPos(m_nScrollPos - m_nLineSize);
			}
		}else if(m_rcButton2.IsPtIn(Event.ptMouse)){
			m_uButton2State |= UISTATE_PUSHED;
			if( !m_bHorizontal ) {
				if(m_pOwner != NULL) 
                    m_pOwner->LineDown(); 
				else 
                    SetScrollPos(m_nScrollPos + m_nLineSize);
			}else{
				if(m_pOwner != NULL) 
                    m_pOwner->LineRight(); 
				else 
                    SetScrollPos(m_nScrollPos + m_nLineSize);
			}
		}
		else if(m_rcThumb.IsPtIn(Event.ptMouse)){
			m_uThumbState |= UISTATE_CAPTURED | UISTATE_PUSHED;
			ptLastMouse = Event.ptMouse;
			m_nLastScrollPos = m_nScrollPos;
		}else{
			if(!m_bHorizontal){
				if(Event.ptMouse.y < m_rcThumb.top){
					if(m_pOwner != NULL) 
                        m_pOwner->PageUp(); 
					else 
                        SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
				}else if (Event.ptMouse.y > m_rcThumb.bottom){
					if(m_pOwner != NULL) 
                        m_pOwner->PageDown(); 
					else 
                        SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
				}
			}else{
				if(Event.ptMouse.x < m_rcThumb.left){
					if(m_pOwner != NULL) 
                        m_pOwner->PageLeft(); 
					else 
                        SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
				}else if (Event.ptMouse.x > m_rcThumb.right){
					if(m_pOwner != NULL) 
                        m_pOwner->PageRight(); 
					else 
                        SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
				}
			}
		}
		if(m_Window && m_pOwner == NULL)
            m_Window->SendNotify(this, DUI_MSGTYPE_SCROLL);
		return;
	}
	if(Event.Type == UIEVENT_BUTTONUP){
		m_nScrollRepeatDelay = 0;
		m_nLastScrollOffset = 0;
		m_Window->KillTimer(this, /*DEFAULT_TIMERID*/m_TimerId);
        m_TimerId = 0;

		if((m_uThumbState & UISTATE_CAPTURED) != 0){
			m_uThumbState &= ~( UISTATE_CAPTURED | UISTATE_PUSHED );
			Invalidate();
		}else if((m_uButton1State & UISTATE_PUSHED) != 0){
			m_uButton1State &= ~UISTATE_PUSHED;
			Invalidate();
		}else if((m_uButton2State & UISTATE_PUSHED) != 0){
			m_uButton2State &= ~UISTATE_PUSHED;
			Invalidate();
		}
		return;
	}

	if(Event.Type == UIEVENT_MOUSEMOVE){
		if((m_uThumbState & UISTATE_CAPTURED) != 0) {
			if(!m_bHorizontal){
				int vRange = m_rcItem.bottom - m_rcItem.top - \
                    m_rcThumb.bottom + m_rcThumb.top - 2 * m_cxyFixed.cx;
				if (vRange != 0)
					m_nLastScrollOffset = (Event.ptMouse.y - ptLastMouse.y) * m_nRange / vRange;
				
			}else{
				int hRange = m_rcItem.right - m_rcItem.left - \
                    m_rcThumb.right + m_rcThumb.left - 2 * m_cxyFixed.cy;
				if (hRange != 0)
					m_nLastScrollOffset = (Event.ptMouse.x - ptLastMouse.x) * m_nRange / hRange;
			}
		}else{
			if((m_uThumbState & UISTATE_HOT) != 0){
				if(m_rcThumb.IsPtIn(Event.ptMouse)){
					m_uThumbState &= ~UISTATE_HOT;
					Invalidate();
				}
			}else{
				if(!IsEnabled()) 
                    return;
				if(m_rcThumb.IsPtIn(Event.ptMouse)){
					m_uThumbState |= UISTATE_HOT;
					Invalidate();
				}
			}
		}
		return;
	}
	if(Event.Type == UIEVENT_CONTEXTMENU){
		return;
	}

	if(Event.Type == UIEVENT_TIMER && Event.wParam == (WPARAM)m_TimerId/*DEFAULT_TIMERID*/){
		++m_nScrollRepeatDelay;
		if((m_uThumbState & UISTATE_CAPTURED) != 0){
			if(!m_bHorizontal) {
				if(m_pOwner != NULL) 
                    m_pOwner->SetScrollPos(CUISize(m_pOwner->GetScrollPos().cx, \
					    m_nLastScrollPos + m_nLastScrollOffset)); 
				else 
                    SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
			}else{
				if(m_pOwner != NULL) 
                    m_pOwner->SetScrollPos(CUISize(m_nLastScrollPos + m_nLastScrollOffset, \
					    m_pOwner->GetScrollPos().cy)); 
				else 
                    SetScrollPos(m_nLastScrollPos + m_nLastScrollOffset);
			}
			Invalidate();
		}else if((m_uButton1State & UISTATE_PUSHED) != 0){
			if(m_nScrollRepeatDelay <= 5) 
                return;
			if(!m_bHorizontal){
				if(m_pOwner != NULL) 
                    m_pOwner->LineUp(); 
				else 
                    SetScrollPos(m_nScrollPos - m_nLineSize);
			}else{
				if(m_pOwner != NULL) 
                    m_pOwner->LineLeft(); 
				else 
                    SetScrollPos(m_nScrollPos - m_nLineSize);
			}
		}else if((m_uButton2State & UISTATE_PUSHED) != 0){
			if(m_nScrollRepeatDelay <= 5) 
                return;
			if(!m_bHorizontal){
				if(m_pOwner != NULL) 
                    m_pOwner->LineDown(); 
				else 
                    SetScrollPos(m_nScrollPos + m_nLineSize);
			}else{
				if(m_pOwner != NULL) 
                    m_pOwner->LineRight(); 
				else 
                    SetScrollPos(m_nScrollPos + m_nLineSize);
			}
		}else{
			if(m_nScrollRepeatDelay <= 5) 
                return;

			POINT pt;
			ZeroMemory(&pt, sizeof(pt));

			//
			// GetCursorPos
			//

			//::GetCursorPos(&pt);
			//::ScreenToClient(m_pManager->GetPaintWindow(), &pt);
			if(!m_bHorizontal){
				if(pt.y < m_rcThumb.top){
					if(m_pOwner != NULL) 
                        m_pOwner->PageUp(); 
					else 
                        SetScrollPos(m_nScrollPos + m_rcItem.top - m_rcItem.bottom);
				}else if (pt.y > m_rcThumb.bottom){
					if(m_pOwner != NULL) 
                        m_pOwner->PageDown(); 
					else 
                        SetScrollPos(m_nScrollPos - m_rcItem.top + m_rcItem.bottom);                    
				}
			}else{
				if(pt.x < m_rcThumb.left){
					if( m_pOwner != NULL ) m_pOwner->PageLeft(); 
					else SetScrollPos(m_nScrollPos + m_rcItem.left - m_rcItem.right);
				}else if(pt.x > m_rcThumb.right){
					if(m_pOwner != NULL) 
                        m_pOwner->PageRight(); 
					else 
                        SetScrollPos(m_nScrollPos - m_rcItem.left + m_rcItem.right);                    
				}
			}
		}
		if(m_Window && m_pOwner == NULL) 
            m_Window->SendNotify(this, DUI_MSGTYPE_SCROLL);
		return;
	}
	if(Event.Type == UIEVENT_MOUSEENTER){
		if(IsEnabled()){
			m_uButton1State |= UISTATE_HOT;
			m_uButton2State |= UISTATE_HOT;
			if(m_rcThumb.IsPtIn(Event.ptMouse))
                m_uThumbState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}

	if(Event.Type == UIEVENT_MOUSELEAVE){
		if(IsEnabled()){
			m_uButton1State &= ~UISTATE_HOT;
			m_uButton2State &= ~UISTATE_HOT;
			m_uThumbState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}

	if(m_pOwner != NULL) 
        m_pOwner->DoEvent(Event); 
    else 
        CUIControl::DoEvent(Event);
}

void CUIScrollBar::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if(wcscmp(pstrName, L"button1normalimage") == 0) SetButton1NormalImage(pstrValue);
	else if(wcscmp(pstrName, L"button1hotimage") == 0) SetButton1HotImage(pstrValue);
	else if(wcscmp(pstrName, L"button1pushedimage") == 0) SetButton1PushedImage(pstrValue);
	else if(wcscmp(pstrName, L"button1disabledimage") == 0) SetButton1DisabledImage(pstrValue);
	else if(wcscmp(pstrName, L"button2normalimage") == 0) SetButton2NormalImage(pstrValue);
	else if(wcscmp(pstrName, L"button2hotimage") == 0) SetButton2HotImage(pstrValue);
	else if(wcscmp(pstrName, L"button2pushedimage") == 0) SetButton2PushedImage(pstrValue);
	else if(wcscmp(pstrName, L"button2disabledimage") == 0) SetButton2DisabledImage(pstrValue);
	else if(wcscmp(pstrName, L"thumbnormalimage") == 0) SetThumbNormalImage(pstrValue);
	else if(wcscmp(pstrName, L"thumbhotimage") == 0 ) SetThumbHotImage(pstrValue);
	else if(wcscmp(pstrName, L"thumbpushedimage") == 0) SetThumbPushedImage(pstrValue);
	else if(wcscmp(pstrName, L"thumbdisabledimage") == 0) SetThumbDisabledImage(pstrValue);
	else if(wcscmp(pstrName, L"railnormalimage") == 0) SetRailNormalImage(pstrValue);
	else if(wcscmp(pstrName, L"railhotimage") == 0 ) SetRailHotImage(pstrValue);
	else if(wcscmp(pstrName, L"railpushedimage") == 0) SetRailPushedImage(pstrValue);
	else if(wcscmp(pstrName, L"raildisabledimage") == 0) SetRailDisabledImage(pstrValue);
	else if(wcscmp(pstrName, L"bknormalimage") == 0) SetBkNormalImage(pstrValue);
	else if(wcscmp(pstrName, L"bkhotimage") == 0) SetBkHotImage(pstrValue);
	else if(wcscmp(pstrName, L"bkpushedimage") == 0) SetBkPushedImage(pstrValue);
	else if(wcscmp(pstrName, L"bkdisabledimage") == 0) SetBkDisabledImage(pstrValue);
	else if(wcscmp(pstrName, L"hor") == 0) SetHorizontal(wcscmp(pstrValue, L"true") == 0);
	else if(wcscmp(pstrName, L"linesize") == 0) SetLineSize(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if(wcscmp(pstrName, L"range") == 0) SetScrollRange(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if(wcscmp(pstrName, L"value") == 0) SetScrollPos(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if(wcscmp(pstrName, L"showbutton1") == 0) SetShowButton1(wcscmp(pstrValue, L"true") == 0);
	else if(wcscmp(pstrName, L"showbutton2") == 0) SetShowButton2(wcscmp(pstrValue, L"true") == 0);
	else CUIControl::SetAttribute(pstrName, pstrValue);
}

void CUIScrollBar::DoPaint(cairo_t *cr, const CUIRect& rcPaint)
{
	if(!UIIntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)) 
        return;
	PaintBk(cr);
	PaintButton1(cr);
	PaintButton2(cr);
	PaintThumb(cr);
	PaintRail(cr);
}

void CUIScrollBar::PaintBk(cairo_t *cr)
{
	if(!IsEnabled()) 
        m_uThumbState |= UISTATE_DISABLED;
	else 
        m_uThumbState &= ~ UISTATE_DISABLED;

	if((m_uThumbState & UISTATE_DISABLED) != 0){
		if(!m_sBkDisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sBkDisabledImage)) 
                m_sBkDisabledImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_PUSHED) != 0){
		if(!m_sBkPushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sBkPushedImage)) 
                m_sBkPushedImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_HOT) != 0) {
		if(!m_sBkHotImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sBkHotImage)) 
                m_sBkHotImage.Empty();
			else 
                return;
		}
	}

	if(!m_sBkNormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sBkNormalImage)) 
            m_sBkNormalImage.Empty();
		else 
            return;
	}
}

void CUIScrollBar::PaintButton1(cairo_t *cr)
{
	if(!m_bShowButton1) 
        return;

	if(!IsEnabled()) 
        m_uButton1State |= UISTATE_DISABLED;
	else 
        m_uButton1State &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	m_sImageModify.Format(L"dest='%d,%d,%d,%d'", m_rcButton1.left - m_rcItem.left, \
		m_rcButton1.top - m_rcItem.top, m_rcButton1.right - m_rcItem.left, 
        m_rcButton1.bottom - m_rcItem.top);

	if((m_uButton1State & UISTATE_DISABLED) != 0){
		if(!m_sButton1DisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton1DisabledImage, (LPCWSTR)m_sImageModify)) 
                m_sButton1DisabledImage.Empty();
			else 
                return;
		}
	}
	else if((m_uButton1State & UISTATE_PUSHED) != 0){
		if(!m_sButton1PushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton1PushedImage, (LPCWSTR)m_sImageModify)) 
                m_sButton1PushedImage.Empty();
			else 
                return;
		}
	}else if((m_uButton1State & UISTATE_HOT) != 0){
		if(!m_sButton1HotImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton1HotImage, (LPCWSTR)m_sImageModify)) 
                m_sButton1HotImage.Empty();
			else 
                return;
		}
	}

	if(!m_sButton1NormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sButton1NormalImage, (LPCWSTR)m_sImageModify)) 
            m_sButton1NormalImage.Empty();
		else 
            return;
	}

	DWORD dwBorderColor = 0xFF85E4FF;
	int nBorderSize = 2;
	CUIRender::DrawRect(cr, m_rcButton1, nBorderSize, dwBorderColor);
}

void CUIScrollBar::PaintButton2(cairo_t *cr)
{
	if(!m_bShowButton2) 
        return;

	if(!IsEnabled()) 
        m_uButton2State |= UISTATE_DISABLED;
	else 
        m_uButton2State &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	m_sImageModify.Format(L"dest='%d,%d,%d,%d'", m_rcButton2.left - m_rcItem.left, \
		m_rcButton2.top - m_rcItem.top, m_rcButton2.right - m_rcItem.left, m_rcButton2.bottom - m_rcItem.top);

	if((m_uButton2State & UISTATE_DISABLED) != 0){
		if(!m_sButton2DisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton2DisabledImage, (LPCWSTR)m_sImageModify)) 
                m_sButton2DisabledImage.Empty();
			else 
                return;
		}
	}else if((m_uButton2State & UISTATE_PUSHED) != 0){
		if(!m_sButton2PushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton2PushedImage, (LPCWSTR)m_sImageModify)) 
                m_sButton2PushedImage.Empty();
			else 
                return;
		}
	}else if((m_uButton2State & UISTATE_HOT) != 0){
		if(!m_sButton2HotImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sButton2HotImage, (LPCWSTR)m_sImageModify)) 
                m_sButton2HotImage.Empty();
			else 
                return;
		}
	}

	if(!m_sButton2NormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sButton2NormalImage, (LPCWSTR)m_sImageModify)) 
            m_sButton2NormalImage.Empty();
		else 
            return;
	}

	DWORD dwBorderColor = 0xFF85E4FF;
	int nBorderSize = 2;
	CUIRender::DrawRect(cr, m_rcButton2, nBorderSize, dwBorderColor);
}

void CUIScrollBar::PaintThumb(cairo_t *cr)
{
	if(m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0) 
        return;
	if(IsEnabled()) 
        m_uThumbState |= UISTATE_DISABLED;
	else 
        m_uThumbState &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	m_sImageModify.Format(L"dest='%d,%d,%d,%d'", m_rcThumb.left - m_rcItem.left, \
		m_rcThumb.top - m_rcItem.top, m_rcThumb.right - m_rcItem.left, m_rcThumb.bottom - m_rcItem.top);

	if((m_uThumbState & UISTATE_DISABLED) != 0){
		if(!m_sThumbDisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sThumbDisabledImage, (LPCWSTR)m_sImageModify)) 
                m_sThumbDisabledImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_PUSHED) != 0){
		if(!m_sThumbPushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sThumbPushedImage, (LPCWSTR)m_sImageModify)) 
                m_sThumbPushedImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_HOT) != 0){
		if(!m_sThumbHotImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sThumbHotImage, (LPCWSTR)m_sImageModify)) 
                m_sThumbHotImage.Empty();
			else 
                return;
		}
	}

	if(!m_sThumbNormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sThumbNormalImage, (LPCWSTR)m_sImageModify)) 
            m_sThumbNormalImage.Empty();
		else 
            return;
	}

	DWORD dwBorderColor = 0xFF85E4FF;
	int nBorderSize = 2;
	CUIRender::DrawRect(cr, m_rcThumb, nBorderSize, dwBorderColor);
}

void CUIScrollBar::PaintRail(cairo_t *cr)
{
	if(m_rcThumb.left == 0 && m_rcThumb.top == 0 && m_rcThumb.right == 0 && m_rcThumb.bottom == 0) 
        return;
	if(!IsEnabled()) 
        m_uThumbState |= UISTATE_DISABLED;
	else 
        m_uThumbState &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	if(!m_bHorizontal){
		m_sImageModify.Format(L"dest='%d,%d,%d,%d'", m_rcThumb.left - m_rcItem.left, \
			(m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top - m_cxyFixed.cx / 2, \
			m_rcThumb.right - m_rcItem.left, \
			(m_rcThumb.top + m_rcThumb.bottom) / 2 - m_rcItem.top + m_cxyFixed.cx - m_cxyFixed.cx / 2);
	}else{
		m_sImageModify.Format(L"dest='%d,%d,%d,%d'", \
			(m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left - m_cxyFixed.cy / 2, \
			m_rcThumb.top - m_rcItem.top, \
			(m_rcThumb.left + m_rcThumb.right) / 2 - m_rcItem.left + m_cxyFixed.cy - m_cxyFixed.cy / 2, \
			m_rcThumb.bottom - m_rcItem.top);
	}

	if((m_uThumbState & UISTATE_DISABLED) != 0){
		if(!m_sRailDisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sRailDisabledImage, (LPCWSTR)m_sImageModify)) 
                m_sRailDisabledImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_PUSHED) != 0){
		if(!m_sRailPushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sRailPushedImage, (LPCWSTR)m_sImageModify)) 
                m_sRailPushedImage.Empty();
			else 
                return;
		}
	}else if((m_uThumbState & UISTATE_HOT) != 0){
		if(!m_sRailHotImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sRailHotImage, (LPCWSTR)m_sImageModify)) 
                m_sRailHotImage.Empty();
			else 
                return;
		}
	}

	if(!m_sRailNormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sRailNormalImage, (LPCWSTR)m_sImageModify)) 
            m_sRailNormalImage.Empty();
		else 
            return;
	}
}