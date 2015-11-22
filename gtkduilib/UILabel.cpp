#include "stdafx.h"
#include "UILabel.h"

CUILabel::CUILabel() : m_uTextStyle(DT_VCENTER), m_dwTextColor(0), 
m_dwDisabledTextColor(0), m_bShowHtml(false), m_strFontName(L"")
{
}

CUIString CUILabel::GetClass() const
{
    return L"LabelUI";
}

LPVOID CUILabel::GetInterface(const CUIString& strName)
{
    if(strName == DUI_CTR_LABEL) 
        return static_cast<CUILabel*>(this);
    return CUIControl::GetInterface(strName);
}

void CUILabel::SetTextStyle(UINT uStyle)
{
    m_uTextStyle = uStyle;
    Invalidate();
}

UINT CUILabel::GetTextStyle() const
{
    return m_uTextStyle;
}

void CUILabel::SetTextColor(DWORD dwTextColor)
{
    m_dwTextColor = dwTextColor;
    Invalidate();
}

DWORD CUILabel::GetTextColor() const
{
    return m_dwTextColor;
}

void CUILabel::SetDisabledTextColor(DWORD dwTextColor)
{
    m_dwDisabledTextColor = dwTextColor;
    Invalidate();
}

DWORD CUILabel::GetDisabledTextColor() const
{
    return m_dwDisabledTextColor;
}

void CUILabel::SetFont(const CUIString &strFontName)
{
    m_strFontName = strFontName;
    Invalidate();
}

CUIString CUILabel::GetFont() const
{
    return m_strFontName;
}

CUIRect CUILabel::GetTextPadding() const
{
    return m_rcTextPadding;
}

void CUILabel::SetTextPadding(const CUIRect& rc)
{
    m_rcTextPadding = rc;
    Invalidate();
}

bool CUILabel::IsShowHtml()
{
    return m_bShowHtml;
}

void CUILabel::SetShowHtml(bool bShowHtml)
{
    if(m_bShowHtml == bShowHtml) 
        return;

    m_bShowHtml = bShowHtml;
    Invalidate();
}

SIZE CUILabel::EstimateSize(SIZE szAvailable)
{
    if(m_cxyFixed.cy == 0) {
        return CUISize(m_cxyFixed.cx, CResourceMgr::GetInstance().GetFontHeight(m_strFontName) + 4);
    }
    return 
        CUIControl::EstimateSize(szAvailable);
}

void CUILabel::DoEvent(TEventUI& Event)
{
    if(Event.Type == UIEVENT_SETFOCUS){
        m_bFocused = true;
        return;
    }
    if(Event.Type == UIEVENT_KILLFOCUS){
        m_bFocused = false;
        return;
    }
    if(Event.Type == UIEVENT_MOUSEENTER){
        // return;
    }
    if(Event.Type == UIEVENT_MOUSELEAVE){
        // return;
    }
    CUIControl::DoEvent(Event);
}

void CUILabel::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
    CUIString strName = pstrName;
    if(strName == L"align"){
        if(wcsstr(pstrValue, L"left") != NULL){
            m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
            m_uTextStyle |= DT_LEFT;
        }
        if(wcsstr(pstrValue, L"center") != NULL){
            m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
            m_uTextStyle |= DT_CENTER;
        }
        if(wcsstr(pstrValue, L"right") != NULL){
            m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
            m_uTextStyle |= DT_RIGHT;
        }
        if(wcsstr(pstrValue, L"top") != NULL){
            m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER);
            m_uTextStyle |= DT_TOP;
        }
        if(wcsstr(pstrValue, L"vcenter") != NULL){
            m_uTextStyle &= ~(DT_TOP | DT_BOTTOM);                  
            m_uTextStyle |= DT_VCENTER;
        }
        if(wcsstr(pstrValue, L"bottom") != NULL){
            m_uTextStyle &= ~(DT_TOP | DT_VCENTER);
            m_uTextStyle |= DT_BOTTOM;
        }
    }else if(wcscmp(pstrName, L"endellipsis") == 0){
        if(wcscmp(pstrValue, L"true") == 0) 
            m_uTextStyle |= DT_END_ELLIPSIS;
        else 
            m_uTextStyle &= ~DT_END_ELLIPSIS;
    }else if(wcscmp(pstrName, L"font") == 0){
        SetFont(pstrValue);
    }else if(wcscmp(pstrName, L"textcolor") == 0){
        if(*pstrValue == L'#')
            pstrValue = pstrValue + 1;
        LPWSTR pstr = NULL;
        DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
        SetTextColor(clrColor);
    }
    else if(wcscmp(pstrName, L"disabledtextcolor") == 0){
        if(*pstrValue == L'#') 
            pstrValue = pstrValue + 1;
        LPWSTR pstr = NULL;
        DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
        SetDisabledTextColor(clrColor);
    }else if(wcscmp(pstrName, L"textpadding") == 0){
        CUIRect rcTextPadding;
        LPWSTR pstr = NULL;
        rcTextPadding.left = wcstol(pstrValue, &pstr, 10);  //ASSERT(pstr);    
        rcTextPadding.top = wcstol(pstr + 1, &pstr, 10);    //ASSERT(pstr);    
        rcTextPadding.right = wcstol(pstr + 1, &pstr, 10);  //ASSERT(pstr);    
        rcTextPadding.bottom = wcstol(pstr + 1, &pstr, 10); //ASSERT(pstr);    
        SetTextPadding(rcTextPadding);
    }else if(wcscmp(pstrName, L"showhtml") == 0) 
        SetShowHtml(wcscmp(pstrValue, L"true") == 0);
    else 
        CUIControl::SetAttribute(pstrName, pstrValue);
}

void CUILabel::PaintText(cairo_t *cr)
{
    DWORD dwTextColor;
    if(m_dwTextColor == 0) 
        m_dwTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;
    if(m_dwDisabledTextColor == 0) 
        m_dwDisabledTextColor = 0xFFA7A6AA/*m_pManager->GetDefaultDisabledColor()*/;

    if(m_strText.IsEmpty()) 
        return;

    int nLinks = 0;
    CUIRect rc = m_rcItem;
    rc.left += m_rcTextPadding.left;
    rc.right -= m_rcTextPadding.right;
    rc.top += m_rcTextPadding.top;
    rc.bottom -= m_rcTextPadding.bottom;

    dwTextColor = IsEnabled() ? m_dwTextColor : m_dwDisabledTextColor;
    
    CUIRender::DrawText(cr, rc, m_strText, m_uTextStyle, 
        m_strFontName, dwTextColor, m_bShowHtml);
}