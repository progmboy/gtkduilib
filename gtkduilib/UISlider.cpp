#include "stdafx.h"
#include "UISlider.h"

CUISlider::CUISlider():m_uButtonState(0), m_nStep(1), m_bDelyInVisible(false), m_nDownValue(0), m_bSendMove(false)
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
	m_szThumb.cx = m_szThumb.cy = 10;
}

CUIString CUISlider::GetClass() const
{
	return L"SliderUI";
}

UINT CUISlider::GetControlFlags() const
{
	return IsEnabled() ? UIFLAG_SETCURSOR : 0;
}

LPVOID CUISlider::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_SLIDER) 
		return static_cast<CUISlider*>(this);
	return 
        CUIProgress::GetInterface(strName);
}

void CUISlider::SetEnabled(bool bEnable)
{
	CUIControl::SetEnabled(bEnable);
	if( !IsEnabled() ) {
		m_uButtonState = 0;
	}
}

int CUISlider::GetChangeStep()
{
	return m_nStep;
}

void CUISlider::SetChangeStep(int step)
{
	m_nStep = step;
}

void CUISlider::SetThumbSize(SIZE szXY)
{
	m_szThumb = szXY;
}

CUIRect CUISlider::GetThumbRect() const
{
	if( m_bHorizontal ) {
		int left = m_rcItem.left + (m_rcItem.right - m_rcItem.left - m_szThumb.cx) * (m_nValue - m_nMin) / (m_nMax - m_nMin);
		int top = (m_rcItem.bottom + m_rcItem.top - m_szThumb.cy) / 2;
		return CUIRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
	}
	else {
		int left = (m_rcItem.right + m_rcItem.left - m_szThumb.cx) / 2;
		int top = m_rcItem.bottom - m_szThumb.cy - (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy) * (m_nValue - m_nMin) / (m_nMax - m_nMin);
		return CUIRect(left, top, left + m_szThumb.cx, top + m_szThumb.cy); 
	}
}

CUIString CUISlider::GetThumbImage() const
{
	return m_sThumbImage;
}

void CUISlider::SetThumbImage(const CUIString& strImage)
{
	m_sThumbImage = strImage;
	Invalidate();
}

CUIString CUISlider::GetThumbHotImage() const
{
	return m_sThumbHotImage;
}

void CUISlider::SetThumbHotImage(const CUIString& strImage)
{
	m_sThumbHotImage = strImage;
	Invalidate();
}

CUIString CUISlider::GetThumbPushedImage() const
{
	return m_sThumbPushedImage;
}

void CUISlider::SetThumbPushedImage(const CUIString& strImage)
{
	m_sThumbPushedImage = strImage;
	Invalidate();
}

void CUISlider::SetValue(int nValue)
{
	if( (m_uButtonState & UISTATE_CAPTURED) != 0 ){
		return;
	}
	CUIProgress::SetValue(nValue);
}

void CUISlider::SetVisible(bool bVisible /* = true */)
{
	if(bVisible == false && (m_uButtonState & UISTATE_CAPTURED) != 0 ){
		m_bDelyInVisible = true;
		return;
	}
	CUIProgress::SetVisible(bVisible);
}

void CUISlider::SetSendMove(bool bSendMove){
	m_bSendMove = bSendMove;
}

bool CUISlider::IsSendMove(){
	return m_bSendMove;
}

void CUISlider::DoEvent(TEventUI& event)
{
	if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND){
		if(m_Parent != NULL) 
			m_Parent->DoEvent(event);
		else 
			CUIProgress::DoEvent(event);
		return;
	}

	if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK){
		if(IsEnabled()){
			CUIRect rcThumb = GetThumbRect();
			if(rcThumb.IsPtIn(event.ptMouse)){
				m_uButtonState |= UISTATE_CAPTURED;
				m_nDownValue = m_nValue;
			}
		}
		return;
	}
	if(event.Type == UIEVENT_BUTTONUP){
		int nValue;
		if((m_uButtonState & UISTATE_CAPTURED) != 0) {
			m_uButtonState &= ~UISTATE_CAPTURED;
			if (m_bDelyInVisible){
				CUIProgress::SetVisible(false);
				m_bDelyInVisible = false;
			}
		}

		if(m_bHorizontal) {
			if(event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2) 
				nValue = m_nMax;
			else if(event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2) 
				nValue = m_nMin;
			else 
				nValue = m_nMin + (m_nMax - m_nMin) * (event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2 ) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
		}else{
			if(event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2) 
				nValue = m_nMin;
			else if(event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2) 
				nValue = m_nMax;
			else 
				nValue = m_nMin + (m_nMax - m_nMin) * (m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2 ) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
		}

		if(m_nDownValue != nValue && nValue >= m_nMin && nValue <= m_nMax){
			m_nValue = nValue;
            if (m_Window){
                m_Window->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
            }
			Invalidate();
		}

		return;
	}

	if( event.Type == UIEVENT_CONTEXTMENU ){
		return;
	}

	if(event.Type == UIEVENT_SCROLLWHEEL){
		switch(LOWORD(event.wParam)){
	    case SB_LINEUP:
            {
		        SetValue(GetValue() + GetChangeStep());
                if (m_Window){
                    m_Window->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
                }
            }
		    return;
	    case SB_LINEDOWN:
            {
		        SetValue(GetValue() - GetChangeStep());
                if (m_Window){
                    m_Window->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
                }
            }
		    return;
	    }
	}
	if( event.Type == UIEVENT_MOUSEMOVE ){
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			int nValue;
			if( m_bHorizontal ) {
				if( event.ptMouse.x >= m_rcItem.right - m_szThumb.cx / 2 ) 
					nValue = m_nMax;
				else if( event.ptMouse.x <= m_rcItem.left + m_szThumb.cx / 2 ) 
					nValue = m_nMin;
				else 
					nValue = m_nMin + (m_nMax - m_nMin) * (event.ptMouse.x - m_rcItem.left - m_szThumb.cx / 2 ) / (m_rcItem.right - m_rcItem.left - m_szThumb.cx);
			}else{
				if( event.ptMouse.y >= m_rcItem.bottom - m_szThumb.cy / 2 ) 
					nValue = m_nMin;
				else if( event.ptMouse.y <= m_rcItem.top + m_szThumb.cy / 2  ) 
					nValue = m_nMax;
				else 
					nValue = m_nMin + (m_nMax - m_nMin) * (m_rcItem.bottom - event.ptMouse.y - m_szThumb.cy / 2 ) / (m_rcItem.bottom - m_rcItem.top - m_szThumb.cy);
			}

			if(m_nValue != nValue && nValue >= m_nMin && nValue <= m_nMax){
				m_nValue = nValue;
				if (m_bSendMove && m_Window){
					m_Window->SendNotify(this, DUI_MSGTYPE_VALUECHANGED);
				}
			}

			Invalidate();
		}
		return;
	}

	if(event.Type == UIEVENT_SETCURSOR){
		/*RECT rcThumb = GetThumbRect();
		if( IsEnabled() && ::PtInRect(&rcThumb, event.ptMouse) ) {
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}*/
	}

	if(event.Type == UIEVENT_MOUSEENTER){
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		//return;
	}

	if(event.Type == UIEVENT_MOUSELEAVE){
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		//return;
	}
	CUIControl::DoEvent(event);
}


void CUISlider::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if( wcscmp(pstrName, L"thumbimage") == 0 ) 
		SetThumbImage(pstrValue);
	else if( wcscmp(pstrName, L"thumbhotimage") == 0 ) 
		SetThumbHotImage(pstrValue);
	else if( wcscmp(pstrName, L"thumbpushedimage") == 0 ) 
		SetThumbPushedImage(pstrValue);
	else if( wcscmp(pstrName, L"thumbsize") == 0 ) {
		CUISize szXY;
		LPWSTR pstr = NULL;
		szXY.cx = wcstol(pstrValue, &pstr, 10);  //ASSERT(pstr);
		szXY.cy = wcstol(pstr + 1, &pstr, 10);   // ASSERT(pstr);
		SetThumbSize(szXY);
	}else if( wcscmp(pstrName, L"step") == 0 ) {
		SetChangeStep(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	}else if( wcscmp(pstrName, L"forepadding") == 0 )
		CUIProgress::SetForePadding(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if( wcscmp(pstrName, L"sendmove") == 0 )
		SetSendMove(wcscmp(pstrValue, L"true") == 0);
	else 
		CUIProgress::SetAttribute(pstrName, pstrValue);
}

void CUISlider::PaintStatusImage(cairo_t *cr)
{
	CUIProgress::PaintStatusImage(cr);

	RECT rcThumb = GetThumbRect();
	rcThumb.left -= m_rcItem.left;
	rcThumb.top -= m_rcItem.top;
	rcThumb.right -= m_rcItem.left;
	rcThumb.bottom -= m_rcItem.top;
	if((m_uButtonState & UISTATE_CAPTURED) != 0){
		if( !m_sThumbPushedImage.IsEmpty() ) {
			m_sImageModify.Empty();
			m_sImageModify.Format(L"dest='%d,%d,%d,%d'", rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
			if(!DrawImage(cr, (LPCWSTR)m_sThumbPushedImage, (LPCWSTR)m_sImageModify)) 
				m_sThumbPushedImage.Empty();
			else {
				//DUITRACE(TEXT("Thumb %s"), m_sImageModify);
				return;
			}
		}
	}

	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if( !m_sThumbHotImage.IsEmpty() ) {
			m_sImageModify.Empty();
			m_sImageModify.Format(L"dest='%d,%d,%d,%d'", rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
			if( !DrawImage(cr, (LPCWSTR)m_sThumbHotImage, (LPCWSTR)m_sImageModify) ) 
				m_sThumbHotImage.Empty();
			else{ 
				//DUITRACE(TEXT("Thumb %s"), m_sImageModify);
				return;
			}
		}
	}

	if( !m_sThumbImage.IsEmpty() ) {
		m_sImageModify.Empty();
		m_sImageModify.Format(L"dest='%d,%d,%d,%d'", rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
		if( !DrawImage(cr, (LPCWSTR)m_sThumbImage, (LPCWSTR)m_sImageModify) ) m_sThumbImage.Empty();
		else{ 
			//DUITRACE(TEXT("Thumb %s"), m_sImageModify);
			return;
		}
	}
}