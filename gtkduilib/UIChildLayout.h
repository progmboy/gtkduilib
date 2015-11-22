/*! \file UIChildLayout.h
	\brief 这个文件描述了子布局的类的定义
		   子布局是使用单独的一个xml来描述的
	\author duilib
	\date 14:10:2013   17:15
	\copyright GNU Public License
	\version 1.0
*/

#ifndef __UICHILDLAYOUT_H__
#define __UICHILDLAYOUT_H__


/*! \class CChildLayoutUI
    \brief 封装了子布局的一些方法
*/

class CUIChildLayout : public CUIContainer
{
public:
	CUIChildLayout();
	
	/** \fn void Init()
	    \brief 初始化子布局,如果m_pstrXMLFile设置了的话,则调用Builder去生成这个Layout
	*/
	
	void Init();

	/** \fn void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
	    \brief 设置此控件的属性
		\param [in] pstrName 属性名称
		\param [in] pstrValue 属性值
	*/

	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
	
	/** \fn void SetChildLayoutXML(CDuiString pXML)
	    \brief 设置描述子布局的xml
	    \param [in] pXML xml的路径
	*/
	
	void SetChildLayoutXML(const CUIString& strXML);
	
	/** \fn DuiLib::CDuiString GetChildLayoutXML()
	    \brief 获取描述子布局的xml
	    \return xml的路径
	*/
	
	CUIString GetChildLayoutXML();
	
	/** \fn LPVOID GetInterface(LPCTSTR pstrName)
	    \brief 通过名称获取接口
		\param [in] pstrName 接口名称
	    \return 接口的指针
	*/
	
	virtual LPVOID GetInterface(const CUIString& strName);
	
	/** \fn LPCTSTR GetClass() const
	    \brief 获取此类的名称
	    \return 类名
	*/
	
	virtual CUIString GetClass() const;
    
	VOID SetBuilderCallback(IDialogBuilderCallback* pCallback);

private:

	/** xml的路径*/
	CUIString m_pstrXMLFile;
	IDialogBuilderCallback *m_Callback;
};
#endif // __UICHILDLAYOUT_H__
