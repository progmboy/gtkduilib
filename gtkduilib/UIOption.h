#ifndef __UIOPTION_H__
#define __UIOPTION_H__

class CUIOption : public CUIButton
{
public:
	CUIOption();
	~CUIOption();

	CUIString GetClass() const;
	LPVOID GetInterface(const CUIString& strName);

	//void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	bool Activate();
	void SetEnabled(bool bEnable = true);

	CUIString GetSelectedImage();
	void SetSelectedImage(const CUIString& strImage);

	CUIString GetSelectedHotImage();
	void SetSelectedHotImage(const CUIString& strImage);

	void SetSelectedTextColor(DWORD dwTextColor);
	DWORD GetSelectedTextColor();

	void SetSelectedBkColor(DWORD dwBkColor);
	DWORD GetSelectBkColor();

	CUIString GetForeImage();
	void SetForeImage(const CUIString& strImage);

	CUIString GetGroup() const;
	void SetGroup(const CUIString& strImage);
	bool IsSelected() const;
	virtual void Selected(bool bSelected);

	SIZE EstimateSize(SIZE szAvailable);
	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

	void PaintStatusImage(cairo_t *cr);
	void PaintText(cairo_t *cr);
    void SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit);

protected:
	bool			m_bSelected;
	CUIString		m_sGroupName;

	DWORD			m_dwSelectedBkColor;
	DWORD			m_dwSelectedTextColor;

	CUIString		m_sSelectedImage;
	CUIString		m_sSelectedHotImage;
	CUIString		m_sForeImage;
};


#endif // __UIOPTION_H__