#ifndef __UILIST_H__
#define __UILIST_H__

#include "UIVerticalLayout.h"
#include "UIHorizontalLayout.h"


typedef int (CALLBACK *PULVCompareFunc)(UINT_PTR, UINT_PTR, UINT_PTR);

class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

typedef struct tagTListInfoUI
{
    int nColumns;
    CUIRect rcColumn[UILIST_MAX_COLUMNS];
    CUIString strFontName;
    UINT uTextStyle;
    CUIRect rcTextPadding;
    DWORD dwTextColor;
    DWORD dwBkColor;
    CUIString sBkImage;
    bool bAlternateBk;
    DWORD dwSelectedTextColor;
    DWORD dwSelectedBkColor;
    CUIString sSelectedImage;
    DWORD dwHotTextColor;
    DWORD dwHotBkColor;
    CUIString sHotImage;
    DWORD dwDisabledTextColor;
    DWORD dwDisabledBkColor;
    CUIString sDisabledImage;
    DWORD dwLineColor;
    bool bShowHtml;
    bool bMultiExpandable;
} TListInfoUI;


/////////////////////////////////////////////////////////////////////////////////////
//

class IListCallbackUI
{
public:
    virtual LPCWSTR GetItemText(CUIControl* pList, int iItem, int iSubItem) = 0;
};

class IListOwnerUI
{
public:
    virtual TListInfoUI* GetListInfo() = 0;
    virtual int GetCurSel() const = 0;
    virtual bool SelectItem(int iIndex, bool bTakeFocus = false) = 0;
    virtual void DoEvent(TEventUI& event) = 0;
};

class IListUI : public IListOwnerUI
{
public:
    virtual CListHeaderUI* GetHeader() const = 0;
    virtual CUIContainer* GetList() const = 0;
    virtual IListCallbackUI* GetTextCallback() const = 0;
    virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
    virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
    virtual int GetExpandedItem() const = 0;
};

class IListItemUI
{
public:
    virtual int GetIndex() const = 0;
    virtual void SetIndex(int iIndex) = 0;
    virtual IListOwnerUI* GetOwner() = 0;
    virtual void SetOwner(CUIControl* pOwner) = 0;
    virtual bool IsSelected() const = 0;
    virtual bool Select(bool bSelect = true) = 0;
    virtual bool IsExpanded() const = 0;
    virtual bool Expand(bool bExpand = true) = 0;
    virtual void DrawItemText(cairo_t *cr, const CUIRect& rcItem) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListBodyUI;
class CListHeaderUI;

class CListUI : public CUIVerticalLayout, public IListUI
{
public:
    CListUI();

    CUIString GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(const CUIString& strName);

    bool GetScrollSelect();
    void SetScrollSelect(bool bScrollSelect);
    int GetCurSel() const;
    bool SelectItem(int iIndex, bool bTakeFocus = false);

    CListHeaderUI* GetHeader() const;
    CUIContainer* GetList() const;
    TListInfoUI* GetListInfo();

    CUIControl* GetItemAt(int iIndex) const;
    int GetItemIndex(CUIControl* pControl) const;
    bool SetItemIndex(CUIControl* pControl, int iIndex);
    int GetCount() const;
    bool Add(CUIControl* pControl);
    bool AddAt(CUIControl* pControl, int iIndex);
    bool Remove(CUIControl* pControl);
    bool RemoveAt(int iIndex);
    void RemoveAll();

    void EnsureVisible(int iIndex);
    void Scroll(int dx, int dy);

    int GetChildPadding() const;
    void SetChildPadding(int iPadding);

    void SetItemFont(const CUIString& strFontName);
    void SetItemTextStyle(UINT uStyle);
    void SetItemTextPadding(RECT rc);
    void SetItemTextColor(DWORD dwTextColor);
    void SetItemBkColor(DWORD dwBkColor);
    void SetItemBkImage(const CUIString& strImage);
    void SetAlternateBk(bool bAlternateBk);
    void SetSelectedItemTextColor(DWORD dwTextColor);
    void SetSelectedItemBkColor(DWORD dwBkColor);
    void SetSelectedItemImage(const CUIString& strImage);
    void SetHotItemTextColor(DWORD dwTextColor);
    void SetHotItemBkColor(DWORD dwBkColor);
    void SetHotItemImage(const CUIString& strImage);
    void SetDisabledItemTextColor(DWORD dwTextColor);
    void SetDisabledItemBkColor(DWORD dwBkColor);
    void SetDisabledItemImage(const CUIString& strImage);
    void SetItemLineColor(DWORD dwLineColor);
    bool IsItemShowHtml();
    void SetItemShowHtml(bool bShowHtml = true);
	RECT GetItemTextPadding() const;
	DWORD GetItemTextColor() const;
	DWORD GetItemBkColor() const;
	CUIString GetItemBkImage() const;
    bool IsAlternateBk() const;
	DWORD GetSelectedItemTextColor() const;
	DWORD GetSelectedItemBkColor() const;
	CUIString GetSelectedItemImage() const;
	DWORD GetHotItemTextColor() const;
	DWORD GetHotItemBkColor() const;
	CUIString GetHotItemImage() const;
	DWORD GetDisabledItemTextColor() const;
	DWORD GetDisabledItemBkColor() const;
	CUIString GetDisabledItemImage() const;
	DWORD GetItemLineColor() const;

    void SetMultiExpanding(bool bMultiExpandable);
    int GetExpandedItem() const;
    bool ExpandItem(int iIndex, bool bExpand = true);

    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

    IListCallbackUI* GetTextCallback() const;
    void SetTextCallback(IListCallbackUI* pCallback);

    SIZE GetScrollPos() const;
    SIZE GetScrollRange() const;
    void SetScrollPos(SIZE szPos);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();
    void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
    virtual CUIScrollBar* GetVerticalScrollBar() const;
    virtual CUIScrollBar* GetHorizontalScrollBar() const;
    BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);
protected:
    bool m_bScrollSelect;
    int m_iCurSel;
    int m_iExpandedItem;
    IListCallbackUI* m_pCallback;
    CListBodyUI* m_pList;
    CListHeaderUI* m_pHeader;
    TListInfoUI m_ListInfo;
};

/////////////////////////////////////////////////////////////////////////////////////
//


class CListBodyUI : public CUIVerticalLayout
{
public:
    CListBodyUI(CListUI* pOwner);

    void SetScrollPos(SIZE szPos);
    void SetPos(RECT rc);
    void DoEvent(TEventUI& event);
    BOOL SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData);
protected:
	static int __cdecl ItemComareFunc(void *pvlocale, const void *item1, const void *item2);
	int __cdecl ItemComareFunc(const void *item1, const void *item2);
protected:
    CListUI* m_pOwner;
	PULVCompareFunc m_pCompareFunc;
	UINT_PTR m_compareData;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CListHeaderUI : public CUIHorizontalLayout
{
public:
    CListHeaderUI();

    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& strName);

    SIZE EstimateSize(SIZE szAvailable);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListHeaderItemUI : public CUIControl
{
public:
    CListHeaderItemUI();

    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& pstrName);
    UINT GetControlFlags() const;

    void SetEnabled(bool bEnable = true);

	bool IsDragable() const;
    void SetDragable(bool bDragable);
	DWORD GetSepWidth() const;
    void SetSepWidth(int iWidth);
	DWORD GetTextStyle() const;
    void SetTextStyle(UINT uStyle);
	DWORD GetTextColor() const;
    void SetTextColor(DWORD dwTextColor);
	void SetTextPadding(const CUIRect& rc);
	CUIRect GetTextPadding() const;
    void SetFont(const CUIString& strFontName);
    bool IsShowHtml();
    void SetShowHtml(bool bShowHtml = true);
    CUIString GetNormalImage() const;
    void SetNormalImage(const CUIString& pStrImage);
    CUIString GetHotImage() const;
    void SetHotImage(const CUIString&  pStrImage);
    CUIString GetPushedImage() const;
    void SetPushedImage(const CUIString&  pStrImage);
    CUIString GetFocusedImage() const;
    void SetFocusedImage(const CUIString&  pStrImage);
    CUIString GetSepImage() const;
    void SetSepImage(const CUIString&  pStrImage);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
    CUIRect GetThumbRect() const;

    void PaintText(cairo_t *cr);
    void PaintStatusImage(cairo_t *cr);

protected:
    POINT ptLastMouse;
    bool m_bDragable;
    UINT m_uButtonState;
    int m_iSepWidth;
    DWORD m_dwTextColor;
    CUIString m_strFontName;
    UINT m_uTextStyle;
    bool m_bShowHtml;
	CUIRect m_rcTextPadding;
    CUIString m_sNormalImage;
    CUIString m_sHotImage;
    CUIString m_sPushedImage;
    CUIString m_sFocusedImage;
    CUIString m_sSepImage;
    CUIString m_sSepImageModify;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListElementUI : public CUIControl, public IListItemUI
{
public:
    CListElementUI();

    CUIString GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(const CUIString& strName);

    void SetEnabled(bool bEnable = true);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CUIControl* pOwner);
    void SetVisible(bool bVisible = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

    void DrawItemBk(cairo_t* cr, const CUIRect& rcItem);

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListLabelElementUI : public CListElementUI
{
public:
    CListLabelElementUI();

    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& strName);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);
    void DoPaint(cairo_t* cr, const CUIRect& rcPaint);

    void DrawItemText(cairo_t *cr, const CUIRect& rcItem);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class CListTextElementUI : public CListLabelElementUI
{
public:
    CListTextElementUI();
    ~CListTextElementUI();

    CUIString GetClass() const;
    LPVOID GetInterface(const CUIString& strName);
    UINT GetControlFlags() const;

    LPCWSTR GetText(int iIndex) const;
    void SetText(int iIndex, LPCWSTR strText);

    void SetOwner(CUIControl* pOwner);
    CUIString* GetLinkContent(int iIndex);

    void DoEvent(TEventUI& event);
    SIZE EstimateSize(SIZE szAvailable);

    void DrawItemText(cairo_t *cr, const CUIRect& rcItem);

protected:
    enum { MAX_LINK = 8 };
    int m_nLinks;
    CUIRect m_rcLinks[MAX_LINK];
    CUIString m_sLinks[MAX_LINK];
    int m_nHoverLink;
    IListUI* m_pOwner;
    CStdPtrArray m_Icons;
    CStdPtrArray m_aTexts;
};

/////////////////////////////////////////////////////////////////////////////////////
//

class CListContainerElementUI : public CUIContainer, public IListItemUI
{
public:
    CListContainerElementUI();

    CUIString GetClass() const;
    UINT GetControlFlags() const;
    LPVOID GetInterface(const CUIString& strName);

    int GetIndex() const;
    void SetIndex(int iIndex);

    IListOwnerUI* GetOwner();
    void SetOwner(CUIControl* pOwner);
    void SetVisible(bool bVisible = true);
    void SetEnabled(bool bEnable = true);

    bool IsSelected() const;
    bool Select(bool bSelect = true);
    bool IsExpanded() const;
    bool Expand(bool bExpand = true);

    void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
    bool Activate();

    void DoEvent(TEventUI& event);
    void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
    void DoPaint(cairo_t* cr, const CUIRect& rcPaint);

    void DrawItemText(cairo_t* cr, const CUIRect& rcItem);
    void DrawItemBk(cairo_t* cr, const CUIRect& rcItem);

protected:
    int m_iIndex;
    bool m_bSelected;
    UINT m_uButtonState;
    IListOwnerUI* m_pOwner;
};

#endif // __UILIST_H__
