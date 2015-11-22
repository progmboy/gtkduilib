#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

class CUIButton : public CUILabel
{
public:
	CUIButton();

	CUIString GetClass() const;
	LPVOID GetInterface(const CUIString& strName);
	UINT GetControlFlags() const;

	bool Activate();
	void SetEnabled(bool bEnable = true);
	void DoEvent(TEventUI& event);

	LPCWSTR GetNormalImage();
	void SetNormalImage(LPCWSTR pStrImage);
	LPCWSTR GetHotImage();
	void SetHotImage(LPCWSTR pStrImage);
	LPCWSTR GetPushedImage();
	void SetPushedImage(LPCWSTR pStrImage);
	LPCWSTR GetFocusedImage();
	void SetFocusedImage(LPCWSTR pStrImage);
	LPCWSTR GetDisabledImage();
	void SetDisabledImage(LPCWSTR pStrImage);
	LPCWSTR GetForeImage();
	void SetForeImage(LPCWSTR pStrImage);
	LPCWSTR GetHotForeImage();
	void SetHotForeImage(LPCWSTR pStrImage);

	void SetHotBkColor(DWORD dwColor);
	DWORD GetHotBkColor() const;
	void SetHotTextColor(DWORD dwColor);
	DWORD GetHotTextColor() const;
	void SetPushedTextColor(DWORD dwColor);
	DWORD GetPushedTextColor() const;
	void SetFocusedTextColor(DWORD dwColor);
	DWORD GetFocusedTextColor() const;
	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

	void PaintText(cairo_t *cr);
	void PaintStatusImage(cairo_t *cr);

protected:
	UINT m_uButtonState;

	DWORD m_dwHotBkColor;
	DWORD m_dwHotTextColor;
	DWORD m_dwPushedTextColor;
	DWORD m_dwFocusedTextColor;

	CUIString m_sNormalImage;
	CUIString m_sHotImage;
	CUIString m_sHotForeImage;
	CUIString m_sPushedImage;
	CUIString m_sPushedForeImage;
	CUIString m_sFocusedImage;
	CUIString m_sDisabledImage;
    CUIString m_sForeImage;
};


#endif // __UIBUTTON_H__