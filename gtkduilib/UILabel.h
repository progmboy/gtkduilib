#ifndef __UILABEL_H__
#define __UILABEL_H__

//#pragma once

class CUILabel : public CUIControl
{
public:
    CUILabel();

    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& strName);

    void SetTextStyle(UINT uStyle);
    UINT GetTextStyle() const;
    void SetTextColor(DWORD dwTextColor);
    DWORD GetTextColor() const;
    void SetDisabledTextColor(DWORD dwTextColor);
    DWORD GetDisabledTextColor() const;
    void SetFont(const CUIString &strFontName);
    CUIString GetFont() const;
    CUIRect GetTextPadding() const;
    void SetTextPadding(const CUIRect& rc);
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);

    SIZE EstimateSize(SIZE szAvailable);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

    void PaintText(cairo_t *cr);

protected:
    DWORD m_dwTextColor;
    DWORD m_dwDisabledTextColor;
    CUIString m_strFontName;
    UINT m_uTextStyle;
    CUIRect m_rcTextPadding;
    bool m_bShowHtml;
};

#endif // __UILABEL_H__