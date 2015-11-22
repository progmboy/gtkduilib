/*!
 * \file ResourceMgr.h
 * \date 2015/05/18 15:37
 *
 * \author progmboy
 * Contact: user@company.com
 *
 * \brief 
 *
 * Resource manager 是用来管理字体/图片/语言的. 
 * 这个类采用单例模式.
 *
 * \note
*/

#pragma once

class CResourceMgr
{
public:
    CResourceMgr(void);
    virtual ~CResourceMgr(void);

//
// 字体信息管理
//

public:
    CUIFont* GetFont(const CUIString& strName);
    CUIFont* GetFont(const CUIString&  lpszName, const CUIString& lpszFacename, BOOL bDefault = FALSE, 
        int nSize = 0, BOOL bBold = FALSE, BOOL bUnderLine = FALSE, BOOL bItalic = FALSE);
    bool AddFont(const CUIString& strName, const CUIString& strFacename, BOOL bDefault = FALSE, 
        int nSize = 0, BOOL bBold = FALSE, BOOL bUnderLine = FALSE, BOOL bItalic = FALSE);
    BOOL SetDefault(const CUIString&  strName);
    VOID ReleaseAllFont();
    CUIFont* GetDefault();
    UINT GetFontHeight(const CUIString& strName);
    UINT GetDefaultFontHeight();

private:
    CStdStringPtrMap m_FontMap;
    CUIFont* m_DefaultFont;

//
// 图片相关管理
//

public:
    bool AddImage(const CUIString& lpstrImage);
    GdkPixbuf* GetImage(const CUIString& lpstrImage, bool bAdd = false);
    bool RemoveImage(const CUIString& lpstrImage);
    void SetResourePath(const CUIString& strResPath);
    CUIString GetResourePath();

private:
    CStdStringPtrMap m_strImageMap;
    CUIString m_strResDir;

public:
    static CResourceMgr& GetInstance();
    
};
