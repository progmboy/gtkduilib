#ifndef __UIPROGRESS_H__
#define __UIPROGRESS_H__

class  CUIProgress : public CUILabel
{
public:
	CUIProgress();

	CUIString GetClass() const;
	LPVOID GetInterface(const CUIString& strName);

	bool IsHorizontal();
	void SetHorizontal(bool bHorizontal = true);
	bool IsStretchForeImage();
	void SetStretchForeImage(bool bStretchForeImage = true);
	int GetMinValue() const;
	void SetMinValue(int nMin);
	int GetMaxValue() const;
	void SetMaxValue(int nMax);
	int GetValue() const;
	void SetValue(int nValue);
	CUIString GetForeImage() const;
	void SetForeImage(const CUIString& strImage);
	void SetForePadding(int nPadding);

	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
	void PaintStatusImage(cairo_t *cr);
	int GetForePadding();


protected:
	bool m_bHorizontal;
	bool m_bStretchForeImage;
	int m_nMax;
	int m_nMin;
	int m_nValue;
	int m_nPadding;

	CUIString m_strForeImage;
	//CUIString m_sForeImageModify;
};


#endif // __UIPROGRESS_H__
