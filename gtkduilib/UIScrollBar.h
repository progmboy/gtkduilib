#ifndef __UISCROLLBAR_H__
#define __UISCROLLBAR_H__

#pragma once

class CUIContainer;

class CUIScrollBar : public CUIControl
{
public:
	CUIScrollBar();

	CUIString GetClass() const;
	LPVOID GetInterface(const CUIString& strName);

	CUIContainer* GetOwner() const;
	void SetOwner(CUIContainer* pOwner);

	void SetVisible(bool bVisible = true);
	void SetEnabled(bool bEnable = true);
	void SetFocus();

	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);
	int GetScrollRange() const;
	void SetScrollRange(int nRange);
	int GetScrollPos() const;
	void SetScrollPos(int nPos);
	int GetLineSize() const;
	void SetLineSize(int nSize);

	bool GetShowButton1();
	void SetShowButton1(bool bShow);
	CUIString GetButton1NormalImage();
	void SetButton1NormalImage(const CUIString& strImage);
	CUIString GetButton1HotImage();
	void SetButton1HotImage(const CUIString& strImage);
	CUIString GetButton1PushedImage();
	void SetButton1PushedImage(const CUIString& strImage);
	CUIString GetButton1DisabledImage();
	void SetButton1DisabledImage(const CUIString& strImage);

	bool GetShowButton2();
	void SetShowButton2(bool bShow);
	CUIString GetButton2NormalImage();
	void SetButton2NormalImage(const CUIString& strImage);
	CUIString GetButton2HotImage();
	void SetButton2HotImage(const CUIString& strImage);
	CUIString GetButton2PushedImage();
	void SetButton2PushedImage(const CUIString& strImage);
	CUIString GetButton2DisabledImage();
	void SetButton2DisabledImage(const CUIString& strImage);

	CUIString GetThumbNormalImage();
	void SetThumbNormalImage(const CUIString& strImage);
	CUIString GetThumbHotImage();
	void SetThumbHotImage(const CUIString& strImage);
	CUIString GetThumbPushedImage();
	void SetThumbPushedImage(const CUIString& strImage);
	CUIString GetThumbDisabledImage();
	void SetThumbDisabledImage(const CUIString& strImage);

	CUIString GetRailNormalImage();
	void SetRailNormalImage(const CUIString& strImage);
	CUIString GetRailHotImage();
	void SetRailHotImage(const CUIString& strImage);
	CUIString GetRailPushedImage();
	void SetRailPushedImage(const CUIString& strImage);
	CUIString GetRailDisabledImage();
	void SetRailDisabledImage(const CUIString& strImage);

	CUIString GetBkNormalImage();
	void SetBkNormalImage(const CUIString& strImage);
	CUIString GetBkHotImage();
	void SetBkHotImage(const CUIString& strImage);
	CUIString GetBkPushedImage();
	void SetBkPushedImage(const CUIString& strImage);
	CUIString GetBkDisabledImage();
	void SetBkDisabledImage(const CUIString& strImage);

	void SetPos(const CUIRect& rc);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

	void DoPaint(cairo_t *cr, const CUIRect& rcPaint);

	void PaintBk(cairo_t *cr);
	void PaintButton1(cairo_t *cr);
	void PaintButton2(cairo_t *cr);
	void PaintThumb(cairo_t *cr);
	void PaintRail(cairo_t *cr);

protected:

	enum
	{ 
		DEFAULT_SCROLLBAR_SIZE = 16,
		DEFAULT_TIMERID = 10,
	};

	bool m_bHorizontal;
	int m_nRange;
	int m_nScrollPos;
	int m_nLineSize;
	CUIContainer* m_pOwner;
	POINT ptLastMouse;
	int m_nLastScrollPos;
	int m_nLastScrollOffset;
	int m_nScrollRepeatDelay;
    UINT m_TimerId;

	CUIString m_sBkNormalImage;
	CUIString m_sBkHotImage;
	CUIString m_sBkPushedImage;
	CUIString m_sBkDisabledImage;

	bool m_bShowButton1;
	CUIRect m_rcButton1;
	UINT m_uButton1State;
	CUIString m_sButton1NormalImage;
	CUIString m_sButton1HotImage;
	CUIString m_sButton1PushedImage;
	CUIString m_sButton1DisabledImage;

	bool m_bShowButton2;
	CUIRect m_rcButton2;
	UINT m_uButton2State;
	CUIString m_sButton2NormalImage;
	CUIString m_sButton2HotImage;
	CUIString m_sButton2PushedImage;
	CUIString m_sButton2DisabledImage;

	CUIRect m_rcThumb;
	UINT m_uThumbState;
	CUIString m_sThumbNormalImage;
	CUIString m_sThumbHotImage;
	CUIString m_sThumbPushedImage;
	CUIString m_sThumbDisabledImage;

	CUIString m_sRailNormalImage;
	CUIString m_sRailHotImage;
	CUIString m_sRailPushedImage;
	CUIString m_sRailDisabledImage;

	CUIString m_sImageModify;
};

#endif // __UISCROLLBAR_H__