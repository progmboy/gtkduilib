#include "stdafx.h"
#include "UIText.h"

CUIText::CUIText() : m_nLinks(0), m_nHoverLink(-1)
{
	m_uTextStyle = DT_WORDBREAK;
	m_rcTextPadding.left = 2;
	m_rcTextPadding.right = 2;
	::ZeroMemory(m_rcLinks, sizeof(m_rcLinks));
}

CUIText::~CUIText()
{
}

CUIString CUIText::GetClass() const
{
	return L"TextUI";
}

LPVOID CUIText::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_TEXT)
        return static_cast<CUIText*>(this);
	return
        CUILabel::GetInterface(strName);
}

UINT CUIText::GetControlFlags() const
{
	if( IsEnabled() && m_nLinks > 0 )
        return UIFLAG_SETCURSOR;
	else
        return 0;
}

CUIString* CUIText::GetLinkContent(int iIndex)
{
	if(iIndex >= 0 && iIndex < m_nLinks)
        return &m_sLinks[iIndex];
	return NULL;
}

void CUIText::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_Parent != NULL )
            m_Parent->DoEvent(event);
		else
            CUILabel::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETCURSOR ) {
		for( int i = 0; i < m_nLinks; i++ ) {
			if(/* ::PtInRect(&m_rcLinks[i], event.ptMouse) */m_rcLinks[i].IsPtIn(event.ptMouse)) {
				//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
				return;
			}
		}
	}
	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK && IsEnabled() ) {
		for( int i = 0; i < m_nLinks; i++ ) {
			if( /*::PtInRect(&m_rcLinks[i], event.ptMouse)*/ m_rcLinks[i].IsPtIn(event.ptMouse)) {
				Invalidate();
				return;
			}
		}
	}
	if( event.Type == UIEVENT_BUTTONUP && IsEnabled() ) {
		for( int i = 0; i < m_nLinks; i++ ) {
			if( /*::PtInRect(&m_rcLinks[i], event.ptMouse) */m_rcLinks[i].IsPtIn(event.ptMouse)) {
				m_Window->SendNotify(this, DUI_MSGTYPE_LINK, (WPARAM)i);
				return;
			}
		}
	}
	if( event.Type == UIEVENT_CONTEXTMENU )
	{
		return;
	}
	// When you move over a link
	if( m_nLinks > 0 && event.Type == UIEVENT_MOUSEMOVE && IsEnabled() ) {
		int nHoverLink = -1;
		for( int i = 0; i < m_nLinks; i++ ) {
			if( /*::PtInRect(&m_rcLinks[i], event.ptMouse)*/ m_rcLinks[i].IsPtIn(event.ptMouse)) {
				nHoverLink = i;
				break;
			}
		}

		if(m_nHoverLink != nHoverLink) {
			m_nHoverLink = nHoverLink;
			Invalidate();
			return;
		}
	}
	if( event.Type == UIEVENT_MOUSELEAVE ) {
		if( m_nLinks > 0 && IsEnabled() ) {
			if(m_nHoverLink != -1) {
				m_nHoverLink = -1;
				Invalidate();
				return;
			}
		}
	}

	CUILabel::DoEvent(event);
}

SIZE CUIText::EstimateSize(SIZE szAvailable)
{
	CUIRect rcText(0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999);
	rcText.left += m_rcTextPadding.left;
	rcText.right -= m_rcTextPadding.right;
    CUIRect rcOut = rcText;

    CUIRender::DrawText(m_Window->GetPaintContext(), rcText, m_strText, m_uTextStyle|DT_CALCRECT,
        m_strFontName, m_dwTextColor, m_bShowHtml, &rcOut);

	SIZE cXY = {rcText.right - rcText.left + m_rcTextPadding.left + m_rcTextPadding.right,
		rcText.bottom - rcText.top + m_rcTextPadding.top + m_rcTextPadding.bottom};

	if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
	return cXY;
}

void CUIText::PaintText(cairo_t *cr)
{
	if( m_strText.IsEmpty() ) {
		m_nLinks = 0;
		return;
	}

	if( m_dwTextColor == 0 ) m_dwTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = 0xFFA7A6AA/*m_pManager->GetDefaultDisabledColor()*/;

	if( m_strText.IsEmpty() ) return;

	m_nLinks = MAX_LINK/*lengthof(m_rcLinks)*/;
	CUIRect rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if(IsEnabled()) {
        CUIRender::DrawText(cr, rc, m_strText, m_uTextStyle,
            m_strFontName, m_dwTextColor, m_bShowHtml);
	}else{
        CUIRender::DrawText(cr, rc, m_strText, m_uTextStyle,
            m_strFontName, m_dwDisabledTextColor, m_bShowHtml);
	}
}
