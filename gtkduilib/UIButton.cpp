#include "stdafx.h"
#include "UIButton.h"

CUIButton::CUIButton()
	: m_uButtonState(0)
	, m_dwHotTextColor(0)
	, m_dwPushedTextColor(0)
	, m_dwFocusedTextColor(0)
	, m_dwHotBkColor(0)
{
	m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
}

CUIString CUIButton::GetClass() const
{
	return L"ButtonUI";
}

LPVOID CUIButton::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_BUTTON)
        return static_cast<CUIButton*>(this);
	return
        CUILabel::GetInterface(strName);
}

UINT CUIButton::GetControlFlags() const
{
	//return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
    return IsEnabled() ? UIFLAG_SETCURSOR : 0;
}

void CUIButton::DoEvent(TEventUI& Event)
{
	if(!IsMouseEnabled() && Event.Type > UIEVENT__MOUSEBEGIN && Event.Type < UIEVENT__MOUSEEND){
		if(m_Parent != NULL)
            m_Parent->DoEvent(Event);
		else
            CUILabel::DoEvent(Event);
		return;
	}

	if(Event.Type == UIEVENT_SETFOCUS){
		Invalidate();
	}
	if(Event.Type == UIEVENT_KILLFOCUS){
		Invalidate();
	}
	if(Event.Type == UIEVENT_KEYDOWN){

        //
        // TODO: keyboard support.
        //

		/*if (IsKeyboardEnabled()) {
			if( Event.chKey == VK_SPACE || Event.chKey == VK_RETURN ) {
				Activate();
				return;
			}
		}*/
	}
	if(Event.Type == UIEVENT_BUTTONDOWN || Event.Type == UIEVENT_DBLCLICK){
		if(m_rcItem.IsPtIn(Event.ptMouse) && IsEnabled()){
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();
		}
		return;
	}

	if(Event.Type == UIEVENT_MOUSEMOVE){
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
            UINT State = m_uButtonState;
			if(m_rcItem.IsPtIn(Event.ptMouse))
                m_uButtonState |= UISTATE_PUSHED;
			else
                m_uButtonState &= ~UISTATE_PUSHED;
            if (State != m_uButtonState){
                Invalidate();
            }
		}
		return;
	}
	if(Event.Type == UIEVENT_BUTTONUP){
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if(m_rcItem.IsPtIn(Event.ptMouse))
                Activate();
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
		}
		return;
	}
	if(Event.Type == UIEVENT_CONTEXTMENU){
		/*if( IsContextMenuUsed() ) {
			m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, Event.wParam, Event.lParam);
		}*/
		return;
	}
	if(Event.Type == UIEVENT_MOUSEENTER){
		if(IsEnabled()){
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		// return;
	}
	if(Event.Type == UIEVENT_MOUSELEAVE)
	{
		if(IsEnabled()){
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		// return;
	}
	if(Event.Type == UIEVENT_SETCURSOR){
		//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return;
	}
	CUILabel::DoEvent(Event);
}

bool CUIButton::Activate()
{
	if(!CUIControl::Activate())
        return false;

	if(m_Window)
        m_Window->SendNotify(this, DUI_MSGTYPE_CLICK);

	return true;
}

void CUIButton::SetEnabled(bool bEnable)
{
	CUIControl::SetEnabled(bEnable);
	if(!IsEnabled()) {
		m_uButtonState = 0;
	}
}

void CUIButton::SetHotBkColor( DWORD dwColor )
{
	m_dwHotBkColor = dwColor;
}

DWORD CUIButton::GetHotBkColor() const
{
	return m_dwHotBkColor;
}

void CUIButton::SetHotTextColor(DWORD dwColor)
{
	m_dwHotTextColor = dwColor;
}

DWORD CUIButton::GetHotTextColor() const
{
	return m_dwHotTextColor;
}

void CUIButton::SetPushedTextColor(DWORD dwColor)
{
	m_dwPushedTextColor = dwColor;
}

DWORD CUIButton::GetPushedTextColor() const
{
	return m_dwPushedTextColor;
}

void CUIButton::SetFocusedTextColor(DWORD dwColor)
{
	m_dwFocusedTextColor = dwColor;
}

DWORD CUIButton::GetFocusedTextColor() const
{
	return m_dwFocusedTextColor;
}

LPCWSTR CUIButton::GetNormalImage()
{
	return m_sNormalImage;
}

void CUIButton::SetNormalImage(LPCWSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetHotImage()
{
	return m_sHotImage;
}

void CUIButton::SetHotImage(LPCWSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetPushedImage()
{
	return m_sPushedImage;
}

void CUIButton::SetPushedImage(LPCWSTR pStrImage)
{
	m_sPushedImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CUIButton::SetFocusedImage(LPCWSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CUIButton::SetDisabledImage(LPCWSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetForeImage()
{
	return m_sForeImage;
}

void CUIButton::SetForeImage(LPCWSTR pStrImage)
{
	m_sForeImage = pStrImage;
	Invalidate();
}

LPCWSTR CUIButton::GetHotForeImage()
{
	return m_sHotForeImage;
}

void CUIButton::SetHotForeImage(LPCWSTR pStrImage)
{
	m_sHotForeImage = pStrImage;
	Invalidate();
}

SIZE CUIButton::EstimateSize(SIZE szAvailable)
{
	if(m_cxyFixed.cy == 0)
        return CUISize(m_cxyFixed.cx, CResourceMgr::GetInstance().GetFontHeight(m_strFontName) + 8);
	return CUIControl::EstimateSize(szAvailable);
}

void CUIButton::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if( wcscmp(pstrName, L"normalimage") == 0 ) SetNormalImage(pstrValue);
	else if( wcscmp(pstrName, L"hotimage") == 0 ) SetHotImage(pstrValue);
	else if( wcscmp(pstrName, L"pushedimage") == 0 ) SetPushedImage(pstrValue);
	else if( wcscmp(pstrName, L"focusedimage") == 0 ) SetFocusedImage(pstrValue);
	else if( wcscmp(pstrName, L"disabledimage") == 0 ) SetDisabledImage(pstrValue);
	else if( wcscmp(pstrName, L"foreimage") == 0 ) SetForeImage(pstrValue);
	else if( wcscmp(pstrName, L"hotforeimage") == 0 ) SetHotForeImage(pstrValue);
	else if( wcscmp(pstrName, L"hotbkcolor") == 0 )
	{
		if(*pstrValue == L'#')
            pstrValue = pstrValue + 1;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetHotBkColor(clrColor);
	}
	else if( wcscmp(pstrName, L"hottextcolor") == 0 )
	{
		if(*pstrValue == L'#')
            pstrValue = /*::CharNext(pstrValue)*/pstrValue + 1;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetHotTextColor(clrColor);
	}
	else if( wcscmp(pstrName, L"pushedtextcolor") == 0 )
	{
		if(*pstrValue == L'#')
            pstrValue = /*::CharNext(pstrValue)*/pstrValue + 1;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetPushedTextColor(clrColor);
	}
	else if( wcscmp(pstrName, L"focusedtextcolor") == 0 )
	{
		if(*pstrValue == L'#')
            pstrValue = /*::CharNext(pstrValue)*/pstrValue + 1;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetFocusedTextColor(clrColor);
	}
	else CUILabel::SetAttribute(pstrName, pstrValue);
}

void CUIButton::PaintText(cairo_t *cr)
{
	if(IsFocused())
        m_uButtonState |= UISTATE_FOCUSED;
	else
        m_uButtonState &= ~ UISTATE_FOCUSED;
	if(!IsEnabled())
        m_uButtonState |= UISTATE_DISABLED;
	else
        m_uButtonState &= ~ UISTATE_DISABLED;

    //
    // TODO: default color
    //

	if(m_dwTextColor == 0)
        m_dwTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;
	if(m_dwDisabledTextColor == 0)
        m_dwDisabledTextColor = 0xFFA7A6AA/*m_pManager->GetDefaultDisabledColor()*/;

	if(m_strText.IsEmpty())
        return;

	int nLinks = 0;
	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	DWORD clrColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;

	if(((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0))
		clrColor = GetPushedTextColor();
	else if(((m_uButtonState & UISTATE_HOT) != 0) && (GetHotTextColor() != 0))
		clrColor = GetHotTextColor();
	else if(((m_uButtonState & UISTATE_FOCUSED) != 0) && (GetFocusedTextColor() != 0))
		clrColor = GetFocusedTextColor();

    CUIRender::DrawText(cr, rc, m_strText, DT_SINGLELINE | m_uTextStyle,
        m_strFontName, clrColor, m_bShowHtml);
}

void CUIButton::PaintStatusImage(cairo_t *cr)
{
	if(IsFocused())
        m_uButtonState |= UISTATE_FOCUSED;
	else
        m_uButtonState &= ~ UISTATE_FOCUSED;
	if(!IsEnabled())
        m_uButtonState |= UISTATE_DISABLED;
	else
        m_uButtonState &= ~ UISTATE_DISABLED;

	if((m_uButtonState & UISTATE_DISABLED) != 0){
		if(!m_sDisabledImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sDisabledImage))
                m_sDisabledImage.Empty();
			else
                goto Label_ForeImage;
		}
	}else if((m_uButtonState & UISTATE_PUSHED) != 0){
		if(!m_sPushedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sPushedImage)){
				m_sPushedImage.Empty();
			}
			if(!m_sPushedForeImage.IsEmpty()){
				if(!DrawImage(cr, (LPCWSTR)m_sPushedForeImage))
					m_sPushedForeImage.Empty();
				return;
			}
			else
                goto Label_ForeImage;
		}
	}else if((m_uButtonState & UISTATE_HOT) != 0){
		if(!m_sHotImage.IsEmpty()){
			if( !DrawImage(cr, (LPCWSTR)m_sHotImage) ){
				m_sHotImage.Empty();
			}
			if( !m_sHotForeImage.IsEmpty() ) {
				if( !DrawImage(cr, (LPCWSTR)m_sHotForeImage) )
					m_sHotForeImage.Empty();
				return;
			}
			else goto Label_ForeImage;
		}else if(m_dwHotBkColor != 0){

            //
            // TODO: DrawColor
            //

			/*CRenderEngine::DrawColor(cr, m_rcPaint, GetAdjustColor(m_dwHotBkColor));*/
			return;
		}
	}else if((m_uButtonState & UISTATE_FOCUSED) != 0){
		if(!m_sFocusedImage.IsEmpty()){
			if(!DrawImage(cr, (LPCWSTR)m_sFocusedImage))
                m_sFocusedImage.Empty();
			else
                goto Label_ForeImage;
		}
	}

	if(!m_sNormalImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sNormalImage))
            m_sNormalImage.Empty();
		else
            goto Label_ForeImage;
	}

	if(!m_sForeImage.IsEmpty())
		goto Label_ForeImage;

	return;

Label_ForeImage:
	if(!m_sForeImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sForeImage))
            m_sForeImage.Empty();
	}
}
