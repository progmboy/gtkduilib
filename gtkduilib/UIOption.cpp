#include "stdafx.h"
#include "UIOption.h"

CUIOption::CUIOption() : m_bSelected(false), m_dwSelectedTextColor(0)
{
}

CUIOption::~CUIOption()
{
	if(!m_sGroupName.IsEmpty() && m_Window) 
        m_Window->RemoveOptionGroup(m_sGroupName, this);
}

CUIString CUIOption::GetClass() const
{
	return L"OptionUI";
}

LPVOID CUIOption::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_OPTION) 
        return static_cast<CUIOption*>(this);
	return 
        CUIButton::GetInterface(strName);
}

void CUIOption::SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit)
{
    CUIControl::SetOwnWindow(pWindow, pParent, bInit);
    if(bInit && !m_sGroupName.IsEmpty()){
        if (m_Window) 
            m_Window->AddOptionGroup(m_sGroupName, this);
    }
}

CUIString CUIOption::GetGroup() const
{
	return m_sGroupName;
}

void CUIOption::SetGroup(const CUIString& strGroupName)
{
	if(strGroupName.IsEmpty()){
		if(m_sGroupName.IsEmpty()) 
            return;
		m_sGroupName.Empty();
	}else{
		if(m_sGroupName == strGroupName) 
            return;
		if(!m_sGroupName.IsEmpty() && m_Window) 
            m_Window->RemoveOptionGroup(m_sGroupName, this);
		m_sGroupName = strGroupName;
	}

	if(!m_sGroupName.IsEmpty()){
		if(m_Window) 
            m_Window->AddOptionGroup(m_sGroupName, this);
	}else{
		if(m_Window) 
            m_Window->RemoveOptionGroup(m_sGroupName, this);
	}

	Selected(m_bSelected);
}

bool CUIOption::IsSelected() const
{
	return m_bSelected;
}

void CUIOption::Selected(bool bSelected)
{
	if(m_bSelected == bSelected) 
        return;
	m_bSelected = bSelected;
	if(m_bSelected) 
        m_uButtonState |= UISTATE_SELECTED;
	else m_uButtonState &= ~UISTATE_SELECTED;

	if(m_Window != NULL){
		if(!m_sGroupName.IsEmpty()){
			if( m_bSelected ) {
				CStdPtrArray* aOptionGroup = m_Window->GetOptionGroup(m_sGroupName);
				for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
					CUIOption* pControl = static_cast<CUIOption*>(aOptionGroup->GetAt(i));
					if( pControl != this ) {
						pControl->Selected(false);
					}
				}

                if (m_Window){
                    m_Window->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
                }
				
			}
		}else{
            if (m_Window){
                m_Window->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
            }
		}
	}

	Invalidate();
}

bool CUIOption::Activate()
{
	if(!CUIButton::Activate()) 
        return false;
	if(!m_sGroupName.IsEmpty()) 
        Selected(true);
	else 
        Selected(!m_bSelected);

	return true;
}

void CUIOption::SetEnabled(bool bEnable)
{
	CUIControl::SetEnabled(bEnable);
	if(!IsEnabled()){
		if(m_bSelected) m_uButtonState = UISTATE_SELECTED;
		else m_uButtonState = 0;
	}
}

CUIString CUIOption::GetSelectedImage()
{
	return m_sSelectedImage;
}

void CUIOption::SetSelectedImage(const CUIString& strImage)
{
	m_sSelectedImage = strImage;
	Invalidate();
}

CUIString CUIOption::GetSelectedHotImage()
{
	return m_sSelectedHotImage;
}

void CUIOption::SetSelectedHotImage(const CUIString& strImage )
{
	m_sSelectedHotImage = strImage;
	Invalidate();
}

void CUIOption::SetSelectedTextColor(DWORD dwTextColor)
{
	m_dwSelectedTextColor = dwTextColor;
}

DWORD CUIOption::GetSelectedTextColor()
{
    
    //
    // TODO: GetDefaultFontColor
    //
    
	if (m_dwSelectedTextColor == 0) 
        m_dwSelectedTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;
	return 
        m_dwSelectedTextColor;
}

void CUIOption::SetSelectedBkColor( DWORD dwBkColor )
{
	m_dwSelectedBkColor = dwBkColor;
}

DWORD CUIOption::GetSelectBkColor()
{
	return m_dwSelectedBkColor;
}

CUIString CUIOption::GetForeImage()
{
	return m_sForeImage;
}

void CUIOption::SetForeImage(const CUIString& strImage)
{
	m_sForeImage = strImage;
	Invalidate();
}

SIZE CUIOption::EstimateSize(SIZE szAvailable)
{
	if(m_cxyFixed.cy == 0) 
        return CUISize(m_cxyFixed.cx, CResourceMgr::GetInstance().GetFontHeight(m_strFontName) + 8);
	return 
        CUIControl::EstimateSize(szAvailable);
}

void CUIOption::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if(wcscmp(pstrName, L"group") == 0) 
		SetGroup(pstrValue);
	else if(wcscmp(pstrName, L"selected") == 0) 
		Selected(wcscmp(pstrValue, L"true") == 0);
	else if(wcscmp(pstrName, L"selectedimage") == 0) 
		SetSelectedImage(pstrValue);
	else if(wcscmp(pstrName, L"selectedhotimage") == 0) 
		SetSelectedHotImage(pstrValue);
	else if(wcscmp(pstrName, L"foreimage") == 0) 
		SetForeImage(pstrValue);
	else if(wcscmp(pstrName, L"selectedbkcolor") == 0){
		if( *pstrValue == L'#')
            pstrValue = pstrValue + 1/*::CharNext(pstrValue)*/;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetSelectedBkColor(clrColor);
	}else if(wcscmp(pstrName, L"selectedtextcolor") == 0){
		if(*pstrValue == L'#') 
			pstrValue = pstrValue + 1/*::CharNext(pstrValue)*/;
		LPWSTR pstr = NULL;
		DWORD clrColor = wcstoul(pstrValue, &pstr, 16);
		SetSelectedTextColor(clrColor);
	}else 
		CUIButton::SetAttribute(pstrName, pstrValue);
}

void CUIOption::PaintStatusImage(cairo_t *cr)
{
	m_uButtonState &= ~UISTATE_PUSHED;

	if((m_uButtonState & UISTATE_HOT) != 0 && IsSelected() && !m_sSelectedHotImage.IsEmpty()) {
		if(!DrawImage(cr, (LPCWSTR)m_sSelectedHotImage))
			m_sSelectedHotImage.Empty();
		else 
            goto Label_ForeImage;
	}else if((m_uButtonState & UISTATE_SELECTED) != 0){
		if( !m_sSelectedImage.IsEmpty() ) {
			if(!DrawImage(cr, (LPCWSTR)m_sSelectedImage)) 
                m_sSelectedImage.Empty();
			else 
                goto Label_ForeImage;
		}else if(m_dwSelectedBkColor != 0) {
            
            //
            // TODO m_dwSelectedBkColor
            //
            
			CUIRender::DrawColor(cr, m_rcPaint, /*GetAdjustColor(*/m_dwSelectedBkColor/*)*/);
			return;
		}	
	}

	CUIButton::PaintStatusImage(cr);

Label_ForeImage:
	if(!m_sForeImage.IsEmpty()){
		if(!DrawImage(cr, (LPCWSTR)m_sForeImage)) 
            m_sForeImage.Empty();
	}
}

void CUIOption::PaintText(cairo_t *cr)
{
	if( (m_uButtonState & UISTATE_SELECTED) != 0 )
	{
		DWORD oldTextColor = m_dwTextColor;
		if( m_dwSelectedTextColor != 0 ) 
            m_dwTextColor = m_dwSelectedTextColor;
        
        //
        // TODO GetDefaultFontColor GetDefaultDisabledColor
        //
        
		if(m_dwTextColor == 0) 
            m_dwTextColor = 0xFF000000/*m_pManager->GetDefaultFontColor()*/;
		if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = 0xFFA7A6AA/*m_pManager->GetDefaultDisabledColor()*/;

		if(m_strText.IsEmpty()) 
            return;

		int nLinks = 0;
		RECT rc = m_rcItem;
		rc.left += m_rcTextPadding.left;
		rc.right -= m_rcTextPadding.right;
		rc.top += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;

        CUIRender::DrawText(cr, rc, m_strText, m_uTextStyle, m_strFontName, 
            IsEnabled()?m_dwTextColor:m_dwDisabledTextColor, m_bShowHtml);

		m_dwTextColor = oldTextColor;
    }else{
		CUIButton::PaintText(cr);
    }
}