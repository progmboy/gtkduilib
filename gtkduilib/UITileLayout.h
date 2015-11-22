/*! \file UITileLayout.h
	\brief 这个文件描述了瓷砖型布局的类及其函数的定义
	\author duilib
	\date 12:10:2013   16:38
	\copyright GNU Public License
	\version 1.0
*/

#ifndef __UITILELAYOUT_H__
#define __UITILELAYOUT_H__

class CUITileLayout : public CUIContainer
{
public:
	CUITileLayout();

	CUIString GetClass() const;
	LPVOID GetInterface(const CUIString& strName);

	void SetPos(const CUIRect& rc);

	SIZE GetItemSize() const;
	void SetItemSize(SIZE szItem);
	int GetColumns() const;
	void SetColumns(int nCols);

	void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);

protected:
	CUISize m_szItem;
	int m_nColumns;
};

#endif // __UITILELAYOUT_H__
