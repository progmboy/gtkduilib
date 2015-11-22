
#ifndef __UISLIDER_H__
#define __UISLIDER_H__

class  CUISlider : public CUIProgress
{
public:
    CUISlider();
	CUIString GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(const CUIString& strName);

	void SetEnabled(bool bEnable = true);

	int GetChangeStep();
	void SetChangeStep(int step);
	void SetThumbSize(SIZE szXY);
	CUIRect GetThumbRect() const;
	CUIString GetThumbImage() const;
	void SetThumbImage(const CUIString& strImage);
	CUIString GetThumbHotImage() const;
	void SetThumbHotImage(const CUIString& strImage);
	CUIString GetThumbPushedImage() const;
	void SetThumbPushedImage(const CUIString& strImage);

	void SetValue(int nValue);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
	void PaintStatusImage(cairo_t *cr);
	void SetVisible(bool bVisible /* = true */);
	void SetSendMove(bool bSendMove);
	bool IsSendMove();
protected:
	CUISize m_szThumb;
	UINT m_uButtonState;
	int m_nStep;

	CUIString m_sThumbImage;
	CUIString m_sThumbHotImage;
	CUIString m_sThumbPushedImage;

	CUIString m_sImageModify;
	bool m_bDelyInVisible;
	int m_nDownValue;
	bool m_bSendMove;
};

#endif // __UISLIDER_H__