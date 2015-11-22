#include "stdafx.h"
#include "UIProgress.h"

CUIProgress::CUIProgress() : m_bHorizontal(true), m_nMin(0), m_nMax(100), 
m_nValue(0), m_bStretchForeImage(true), 
m_nPadding(0)
{
	m_uTextStyle = DT_SINGLELINE | DT_CENTER;
	SetFixedHeight(12);
}

CUIString CUIProgress::GetClass() const
{
	return L"ProgressUI";
}

LPVOID CUIProgress::GetInterface(const CUIString& strName)
{
	if(strName == DUI_CTR_PROGRESS) 
        return static_cast<CUIProgress*>(this);
	return CUILabel::GetInterface(strName);
}

bool CUIProgress::IsHorizontal()
{
	return m_bHorizontal;
}

void CUIProgress::SetHorizontal(bool bHorizontal)
{
	if( m_bHorizontal == bHorizontal ) return;

	m_bHorizontal = bHorizontal;
	Invalidate();
}

int CUIProgress::GetMinValue() const
{
	return m_nMin;
}

void CUIProgress::SetMinValue(int nMin)
{
	m_nMin = nMin;
	Invalidate();
}

int CUIProgress::GetMaxValue() const
{
	return m_nMax;
}

void CUIProgress::SetMaxValue(int nMax)
{
	m_nMax = nMax;
	Invalidate();
}

int CUIProgress::GetValue() const
{
	return m_nValue;
}

void CUIProgress::SetValue(int nValue)
{
	if(nValue == m_nValue || nValue<m_nMin || nValue > m_nMax)
	{
		return;
	}
	m_nValue = nValue;
	Invalidate();
}


CUIString CUIProgress::GetForeImage() const
{
	return m_strForeImage;
}

void CUIProgress::SetForeImage(const CUIString& strImage)
{
	if( m_strForeImage == strImage ) return;

	m_strForeImage = strImage;
	Invalidate();
}


int CUIProgress::GetForePadding()
{
	return m_nPadding;
}

void CUIProgress::SetForePadding(int nPadding)
{
	m_nPadding = nPadding;
}

void CUIProgress::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{
	if( wcscmp(pstrName, L"foreimage") == 0 ) 
		SetForeImage(pstrValue);
	else if( wcscmp(pstrName, L"hor") == 0 ) 
		SetHorizontal(wcscmp(pstrValue, L"true") == 0);
	else if( wcscmp(pstrName, L"min") == 0 )
		SetMinValue(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if( wcscmp(pstrName, L"max") == 0 )
		SetMaxValue(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if( wcscmp(pstrName, L"value") == 0 )
		SetValue(/*_wtoi(pstrValue)*/wcstol(pstrValue, NULL, 10));
	else if( wcscmp(pstrName, L"isstretchfore")==0)
		SetStretchForeImage(wcscmp(pstrValue, L"true") == 0);
	else 
		CUILabel::SetAttribute(pstrName, pstrValue);
}


void CUIProgress::PaintStatusImage(cairo_t *cr)
{
    
    CUIString strImageModify;

    if(m_nMax <= m_nMin) 
        m_nMax = m_nMin + 1;
    if(m_nValue > m_nMax) 
        m_nValue = m_nMax;
    if(m_nValue < m_nMin) 
        m_nValue = m_nMin;

    CUIRect rc;
    if(m_bHorizontal){
        rc.right = (m_nValue - m_nMin) * (m_rcItem.right - m_rcItem.left - 2 * m_nPadding) / (m_nMax - m_nMin);
        rc.bottom = m_rcItem.bottom - m_rcItem.top;
    }else{
        rc.top = (m_rcItem.bottom - m_rcItem.top - 2 * m_nPadding) * (m_nMax - m_nValue) / (m_nMax - m_nMin);
        rc.right = m_rcItem.right - m_rcItem.left;
        rc.bottom = m_rcItem.bottom - m_rcItem.top;
    }

    if(!m_strForeImage.IsEmpty()) {
        if (m_bStretchForeImage){
            rc.left += m_nPadding;
            rc.right += m_nPadding;
            strImageModify.Format(L"dest='%d,%d,%d,%d'", rc.left, rc.top, rc.right, rc.bottom);
        }else{
            rc.right += m_nPadding;
            strImageModify.Format(L"dest='%d,%d,%d,%d' source='%d,%d,%d,%d'", rc.left, 
                rc.top, rc.right, rc.bottom, rc.left, rc.top, rc.right, rc.bottom);
        }
        if(!DrawImage(cr, (LPCWSTR)m_strForeImage, (LPCWSTR)strImageModify)) 
            m_strForeImage.Empty();
        else 
            return;
    }

}

bool CUIProgress::IsStretchForeImage()
{
	return m_bStretchForeImage;
}

void CUIProgress::SetStretchForeImage(bool bStretchForeImage /*= true*/)
{
	if (m_bStretchForeImage == bStretchForeImage)
		return;

	m_bStretchForeImage = bStretchForeImage;
	Invalidate();
}

