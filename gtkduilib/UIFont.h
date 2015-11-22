#pragma once

#include "UIRef.h"
#include "UIString.h"
#include "UIUtils.h"


class CUIFont: public CRefBase
{
public:
    CUIFont(void);
    CUIFont(const CUIString& strName, const CUIString& strFaceName, int nSize = 0, BOOL bBold = FALSE, 
        BOOL bUnderline = FALSE, BOOL bItalic = FALSE);
    virtual ~CUIFont(void);
    VOID SetItalic(BOOL bItalic);
    VOID SetBold(BOOL bBold);
    PangoFontDescription* GetHandle();
    PangoFontDescription* Create();
    VOID SetUnderLine(BOOL bUnderline);
    VOID SetSize(int nSize);
    VOID SetName(const CUIString& strName);
    VOID SetFaceName(const CUIString& strFaceName);
    VOID ReleaseFont();
    UINT GetFontHeight();

private:

    CUIString m_strName;
    CUIString m_strFaceName;
    int m_nSize;
    BOOL m_bBold;
    BOOL m_bUnderLine;
    BOOL m_bItalic;
    PangoFontDescription * m_hFont;
};
