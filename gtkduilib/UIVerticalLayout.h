
/*! \file UIVerticalLayout.h
	\brief 这个文件描述了纵向布局的类的一些函数及其函数
	\author duilib
	\date 12:10:2013   16:00
	\copyright GNU Public License
	\version 1.0
*/

#ifndef __UIVERTICALLAYOUT_H__
#define __UIVERTICALLAYOUT_H__

/*! \class CVerticalLayoutUI
    \brief 这个类主要封装了纵向布局的一个功能的实现
*/

class CUIVerticalLayout : public CUIContainer
{
public:
	CUIVerticalLayout();

	/** \fn CUIString GetClass() const
	    \brief 获取此类的名称
	    \return 类名
	*/

	CUIString GetClass() const;

	/** \fn LPVOID GetInterface(const CUIString& strName)
	    \brief 通过名称获取接口
		\param [in] strName 接口名称
	    \return 接口的指针
	*/

	LPVOID GetInterface(const CUIString& strName);

	/** \fn UINT GetControlFlags() const
	    \brief 获取控制标识
	    \return 此类的控制符
	*/

	UINT GetControlFlags() const;

	/** \fn void SetSepHeight(int iHeight)
		\param [in] iHeight 分隔符长度
	    \brief 分隔符长度,正负表示分隔符在左边还是右边,如(-4)
	*/

	void SetSepHeight(int iHeight);

	/** \fn int GetSepHeight() const
	    \brief 获取分隔符长度
	    \return 分隔符宽
		\see SetSepWidth()
	*/

	int GetSepHeight() const;

	/** \fn void SetSepImmMode(bool bImmediately)
		\param [in] bImmediately 是否设置
	    \brief 设置拖动分隔符是否立即改变大小,如(false)
	*/

	void SetSepImmMode(bool bImmediately);

	/** \fn bool IsSepImmMode() const
	    \brief 是否设置了拖动分隔符立即改变大小的属性
	    \return true 已经设置 false 没有设置
		\see SetSepImmMode()
	*/

	bool IsSepImmMode() const;

	/** \fn void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
	    \brief 设置此控件的属性
		\param [in] pstrName 属性名称
		\param [in] pstrValue 属性值
	*/

	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

	/** \fn void DoEvent(TEventUI& event)
	    \brief 响应事件函数
		\param [in,out] event 事件的描述
	*/

	void DoEvent(TEventUI& event);

	/** \fn void SetPos(RECT rc)
	    \brief 设置控件新的位置
		\param [in] rc 新的位置结构
		\note 这个函数会在窗口大小改变时被调到。如果它里面的子布局控件的float属性设置为
		      false的话（代表相对定位）这个函数会重新计算子布局的大小。然后对子布局进行
			  SetPos。从而调整整个布局的大小。
	*/

	void SetPos(const CUIRect& rc);

	/** \fn void DoPostPaint(HDC hDC, const RECT& rcPaint)
	    \brief 这个函数用来绘制分隔符的背景
		\param hDC 画刷句柄
		\param rcPaint 这个参数没有用到
		\note 只有在sepimm设置成了false，拖动才会出现分隔符。
	*/

	void DoPostPaint(cairo_t *cr, const CUIRect& rcPaint);

	/** \fn RECT GetThumbRect(bool bUseNew = false) const
	    \brief 这个函数时获取分隔符的RECT.
		\param [in] bUseNew 是否使用新位置 
	    \return 分隔符的RECT
	*/

	CUIRect GetThumbRect(bool bUseNew = false) const;

protected:

	/** 分隔符长度*/
	int m_iSepHeight;

	/** 按钮状态*/
	UINT m_uButtonState;

	/** 上一次鼠标的位置*/
	POINT ptLastMouse;

	/** 控件新位置*/
	CUIRect m_rcNewPos;

	/** 拖动分隔符是否立即改变大小*/
	bool m_bImmMode;
};
#endif // __UIVERTICALLAYOUT_H__
