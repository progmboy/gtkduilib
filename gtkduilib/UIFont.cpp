#include "stdafx.h"

#include "UIFont.h"

CUIFont::CUIFont(void):m_bBold(FALSE),
m_bUnderLine(FALSE), m_bItalic(FALSE), m_hFont(NULL), m_nSize(8)
{
    
}


CUIFont::CUIFont(const CUIString& strName, const CUIString& strFaceName, 
               int nSize, BOOL bBold, BOOL bUnderline, BOOL bItalic)
{
    m_nSize = nSize * PANGO_SCALE;
    m_strName = strName;
    m_strFaceName = strFaceName;
    m_bBold = bBold;
    m_bUnderLine = bUnderline;
    m_bItalic = bItalic;
    m_hFont = NULL;
}

CUIFont::~CUIFont(void)
{
    ReleaseFont();
}


PangoFontDescription* CUIFont::Create()
{
    
    //
    // create a new desc
    //
    
    PangoFontDescription *desc = pango_font_description_new();
    
    //
    // set font attribute
    //
    
    if (!m_strFaceName.IsEmpty()){
        pango_font_description_set_family(desc, (LPCSTR)CW2U8(m_strFaceName.GetData()));
    }
    
    //
    // set font size
    //
    
    if (m_nSize){
        pango_font_description_set_size(desc, m_nSize);
    }
    
    //
    // set Italic
    //
    
    if (m_bItalic){
        pango_font_description_set_style(desc, PANGO_STYLE_ITALIC);
    }else{
        pango_font_description_set_style(desc, PANGO_STYLE_NORMAL);
    }

    //
    // set bold
    //
    
    if (m_bBold){
        pango_font_description_set_weight(desc, PANGO_WEIGHT_BOLD);
    }else{
        pango_font_description_set_weight(desc, PANGO_WEIGHT_NORMAL);
    }
    
    m_hFont = desc;

    return m_hFont;

}

VOID CUIFont::SetItalic(BOOL bItalic)
{
    m_bItalic = bItalic;
}

VOID CUIFont::SetBold(BOOL bBold)
{
    m_bBold = bBold;
}

VOID CUIFont::SetUnderLine(BOOL bUnderline)
{
    m_bUnderLine = bUnderline;
}

VOID CUIFont::SetSize(int nSize)
{
    m_nSize = nSize;
}

VOID CUIFont::SetName(const CUIString& strName)
{
    m_strName = strName;
}

VOID CUIFont::SetFaceName(const CUIString& strFaceName)
{
    m_strFaceName = strFaceName;
}

PangoFontDescription * CUIFont::GetHandle()
{
    return m_hFont;
}

VOID CUIFont::ReleaseFont()
{
    if (m_hFont){
        pango_font_description_free(m_hFont);
        m_hFont = NULL;
    }
}

UINT CUIFont::GetFontHeight()
{
    if (m_hFont){
        return pango_font_description_get_size(m_hFont) / PANGO_SCALE;
    }
    return 0;
}
