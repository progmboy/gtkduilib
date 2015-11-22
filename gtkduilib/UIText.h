#ifndef __UITEXT_H__
#define __UITEXT_H__

class CUIText : public CUILabel
{
public:
	CUIText();
	~CUIText();

	CUIString GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(const CUIString& strName);

	CUIString* GetLinkContent(int iIndex);

	void DoEvent(TEventUI& event);
	SIZE EstimateSize(SIZE szAvailable);

	void PaintText(cairo_t *cr);

protected:
	enum { MAX_LINK = 8 };
	int m_nLinks;
	CUIRect m_rcLinks[MAX_LINK];
	CUIString m_sLinks[MAX_LINK];
	int m_nHoverLink;
};


#endif //__UITEXT_H__