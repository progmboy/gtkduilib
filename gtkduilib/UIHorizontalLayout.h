
/*! \file UIHorizontalLayout.h
	\brief 名义了横向布局类
	\author duilib
	\date 11:10:2013   19:19
	\copyright GNU Public License
	\version 1.0
*/

#ifndef __UIHORIZONTALLAYOUT_H__
#define __UIHORIZONTALLAYOUT_H__

	
/*! \class CHorizontalLayoutUI
    \brief 这个类主要封装了横向布局的一个功能的实现
*/

class CUIHorizontalLayout : public CUIContainer
{
public:
	CUIHorizontalLayout();

	
	/** \fn LPCTSTR GetClass() const
	    \brief 获取此类的名称
	    \return 类名
	*/
	
	CUIString GetClass() const;
	
	/** \fn LPVOID GetInterface(LPCTSTR pstrName)
	    \brief 通过名称获取接口
		\param [in] pstrName 接口名称
	    \return 接口的指针
	*/
	
	LPVOID GetInterface(const CUIString& pstrName);
	
	/** \fn LPVOID GetInterface(LPCTSTR pstrName)
	    \brief 通过名称获取接口
		\param [in] pstrName 接口名称
	    \return 接口的指针
	*/
	
	UINT GetControlFlags() const;
	
	/** \fn void SetSepWidth(int iWidth)
	    \brief 分隔符宽,正负表示分隔符在左边还是右边,如(-4)
	*/
	
	void SetSepWidth(int iWidth);
	
	/** \fn int GetSepWidth() const
	    \brief 获取分隔符宽
	    \return 分隔符宽
		\see SetSepWidth()
	*/
	
	int GetSepWidth() const;
	
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
	
	/** \fn void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
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
	
	/** \fn void SetPos(const CUIRect& rc)
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
	
	/** 分隔符宽度*/
	int m_iSepWidth;
	
	/** 按钮状态*/
	UINT m_uButtonState;
	
	/** 上一次鼠标的位置*/
	POINT ptLastMouse;

	/** 控件新位置*/
	CUIRect m_rcNewPos;
	
	/** 拖动分隔符是否立即改变大小*/
	bool m_bImmMode;
};

#endif // __UIHORIZONTALLAYOUT_H__
