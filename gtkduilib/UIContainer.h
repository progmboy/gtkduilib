#ifndef __UICONTAINER_H__
#define __UICONTAINER_H__

class IContainerUI
{
public:
    virtual CUIControl* GetItemAt(int iIndex) const = 0;
    virtual int GetItemIndex(CUIControl* pControl) const  = 0;
    virtual bool SetItemIndex(CUIControl* pControl, int iIndex)  = 0;
    virtual int GetCount() const = 0;
    virtual bool Add(CUIControl* pControl) = 0;
    virtual bool AddAt(CUIControl* pControl, int iIndex)  = 0;
    virtual bool Remove(CUIControl* pControl) = 0;
    virtual bool RemoveAt(int iIndex)  = 0;
    virtual void RemoveAll() = 0;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CUIScrollBar;

class CUIContainer : public CUIControl, public IContainerUI
{
public:
    CUIContainer();
    virtual ~CUIContainer();

public:
    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& strName);

    CUIControl* GetItemAt(int iIndex) const;
    int GetItemIndex(CUIControl* pControl) const;
    bool SetItemIndex(CUIControl* pControl, int iIndex);
    int GetCount() const;
    bool Add(CUIControl* pControl);
    bool AddAt(CUIControl* pControl, int iIndex);
    bool Remove(CUIControl* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    //void DoEvent(TEventUI& event);
    void SetVisible(bool bVisible = true);
    void SetInternVisible(bool bVisible = true);
    void SetMouseEnabled(bool bEnable = true);

    virtual CUIRect GetInset() const;
    virtual void SetInset(const CUIRect& rcInset);
    virtual int GetChildPadding() const;
    virtual void SetChildPadding(int iPadding);
    virtual bool IsMouseChildEnabled() const;
    virtual void SetMouseChildEnabled(bool bEnable = true);

    virtual int FindSelectable(int iIndex, bool bForward = true) const;

    void SetPos(const CUIRect& rc);
    void DoPaint(cairo_t* cr, const CUIRect& rcPaint);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

    //void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    CUIControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

	bool SetSubControlText(LPCWSTR pstrSubControlName,LPCWSTR pstrText);
	bool SetSubControlFixedHeight(LPCWSTR pstrSubControlName,int cy);
	bool SetSubControlFixedWdith(LPCWSTR pstrSubControlName,int cx);
	bool SetSubControlUserData(LPCWSTR pstrSubControlName,LPCWSTR pstrText);

	CUIString GetSubControlText(LPCWSTR pstrSubControlName);
	int GetSubControlFixedHeight(LPCWSTR pstrSubControlName);
	int GetSubControlFixedWdith(LPCWSTR pstrSubControlName);
	const CUIString GetSubControlUserData(LPCWSTR pstrSubControlName);
	CUIControl* FindSubControl(LPCWSTR pstrSubControlName);

    virtual SIZE GetScrollPos() const;
    virtual SIZE GetScrollRange() const;
    virtual void SetScrollPos(SIZE szPos);
    virtual void LineUp();
    virtual void LineDown();
    virtual void PageUp();
    virtual void PageDown();
    virtual void HomeUp();
    virtual void EndDown();
    virtual void LineLeft();
    virtual void LineRight();
    virtual void PageLeft();
    virtual void PageRight();
    virtual void HomeLeft();
    virtual void EndRight();
    virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CUIScrollBar* GetVerticalScrollBar() const;
    virtual CUIScrollBar* GetHorizontalScrollBar() const;
    void DoEvent(TEventUI& Event);
    void SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit = true);

protected:
    virtual void SetFloatPos(int iIndex);
    virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);
    

protected:
    CStdPtrArray m_items;
    RECT m_rcInset;
    int m_iChildPadding;
    bool m_bAutoDestroy;
    bool m_bDelayedDestroy;
    bool m_bMouseChildEnabled;
    bool m_bScrollProcess; // ∑¿÷πSetPos—≠ª∑µ˜”√

    CUIScrollBar* m_pVerticalScrollBar;
    CUIScrollBar* m_pHorizontalScrollBar;
};

#endif // __UICONTAINER_H__
