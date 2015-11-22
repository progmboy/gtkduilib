/*! \file UITabLayout.h
	\brief 选项卡型布局类的声明
	\author duilib
	\date 13:10:2013   12:23
	\copyright GNU Public License
	\version 1.0
*/

#ifndef __UITABLAYOUT_H__
#define __UITABLAYOUT_H__

/*! \class CTabLayoutUI
	\brief 这个类是来描述选项卡类型布局的
*/
class CUITabLayout : public CUIContainer
{
public:
	CUITabLayout();

	/** \fn CUIString GetClass() const
	    \brief 获取此类的名称
	    \return 类名
	*/

	CUIString GetClass() const;

	/** \fn LPVOID GetInterface(const CUIString&  strName)
	    \brief 通过名称获取接口
		\param [in] pstrName 接口名称
	    \return 接口的指针
	*/

	LPVOID GetInterface(const CUIString& strName);

	/** \fn bool Add(CUIControl* pControl)
	    \brief 添加一个选项卡
		\param [in] pControl 新选项卡控件
	    \return 成功true 否则false
	*/

	bool Add(CUIControl* pControl);

	/** \fn bool AddAt(CUIControl* pControl, int iIndex)
	    \brief 在指定位置添加一个选项卡
		\param [in] pControl 新选项卡控件
		\param [in] iIndex 新选项卡的索引
	    \return 成功true 否则false
	*/

	bool AddAt(CUIControl* pControl, int iIndex);

	/** \fn bool Remove(CUIControl* pControl)
	    \brief 删除一个选项卡
		\param [in] pControl 新选项卡控件
	    \return 成功true 否则false
	*/

	bool Remove(CUIControl* pControl);

	/** \fn void RemoveAll()
	    \brief 删除所有选项卡
	    \return 成功true 否则false
	*/

	void RemoveAll();

	/** \fn int GetCurSel() const
	    \brief 获取当前选项卡
	    \return 当前选项卡的索引
	*/

	int GetCurSel() const;

	/** \fn bool SelectItem(int iIndex)
	    \brief 通过索引选中指定选项卡
		\param [in] iIndex 要选中选项卡的索引
	    \return 成功true.否则false
	*/

	bool SelectItem(int iIndex);

	/** \fn bool SelectItem(CControlUI* pControl)
	    \brief 通过控件指针选中指定选项卡
		\param [in] pControl 要选中选项卡的控件的指针
	    \return 成功true.否则false
	*/

	bool SelectItem(CUIControl* pControl);

	/** \fn void SetPos(const CUIRect& rc)
	    \brief 设置控件新的位置
		\param [in] rc 控件的新位置
	    \return 成功true.否则false
	*/

	void SetPos(const CUIRect& rc);

	/** \fn void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	    \brief 设置此控件的属性
		\param [in] pstrName 属性名称
		\param [in] pstrValue 属性值
		\return 是true否false
	*/

	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

protected:
	int m_iCurSel;
};
#endif // __UITABLAYOUT_H__
