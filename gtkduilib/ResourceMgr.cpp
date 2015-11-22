#include "stdafx.h"

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

#include "UIUtils.h"
#include "UIString.h"
#include "UIFont.h"

#include "ResourceMgr.h"

#define DEFAULT_FONT_KEY    L"__default__"

CResourceMgr::CResourceMgr(void)
{
}

CResourceMgr::~CResourceMgr(void)
{
    
    //
    // release all font 
    //
    
    ReleaseAllFont();
}

CResourceMgr& CResourceMgr::GetInstance()
{
    static CResourceMgr Instance;
    return Instance;
}

bool CResourceMgr::AddImage(const CUIString& strImage)
{
    GdkPixbuf *Pixbuf = NULL;
    int i = 1;

    //
    // first add the resource path
    //
    
    CUIString strPath = m_strResDir + strImage;

    while (i-- > 0){

        //
        // load by gdk-pixbuf.
        //

        Pixbuf = gdk_pixbuf_new_from_file((LPCSTR)CW2U8(strPath), NULL);
        if (Pixbuf){
            break;
        }
        
        //
        // read from user param directly
        // try again
        //
        
        strPath = strImage;
    }
    
    if (Pixbuf){

        //
        // add pixbuf to map
        //

        if(!m_strImageMap.Insert(strImage, Pixbuf)){

            //
            // insert error. free the pixbuf.
            //

            g_object_unref(Pixbuf);
            return false;
        }
        return true;
    }


    //
    // image load error.
    //

    return false;

}

bool CResourceMgr::RemoveImage(const CUIString& lpstrImage)
{

    //
    // first find it.
    //

    GdkPixbuf *Pixbuf = (GdkPixbuf *)m_strImageMap.Find(lpstrImage);
    if (Pixbuf){

        //
        // if we find the pixbuf release it and move it from hash table
        //

        g_object_unref(Pixbuf);
        return m_strImageMap.Remove(lpstrImage);
    }
    return false;
}

GdkPixbuf* CResourceMgr::GetImage(const CUIString& lpstrImage, bool bAdd)
{

    GdkPixbuf *Pixbuf;

    //
    // first find the pixbuf by image
    //

    Pixbuf = (GdkPixbuf*)m_strImageMap.Find(lpstrImage);
    if (Pixbuf == NULL){

        //
        // can not found the pixbuf. check if we should add it to map
        //

        if (bAdd && AddImage(lpstrImage)){
            Pixbuf = (GdkPixbuf*)m_strImageMap.Find(lpstrImage);
        }
    }

    return Pixbuf;
}

CUIFont* CResourceMgr::GetFont(const CUIString&  strName, const CUIString& strFacename /*= NULL*/, 
                         BOOL bDefault /*= FALSE*/, int nSize /*= 0*/, BOOL bBold /*= FALSE*/, 
                         BOOL bUnderLine /*= FALSE*/, BOOL bItalic /*= FALSE*/)
{
    CUIFont* font = (CUIFont*)m_FontMap.Find((LPCWSTR)strName);
    if (!font){
        
        //
        // do not found it, create new font and add it to map.
        //
        
        font = new CUIFont(strName, strFacename, nSize, bBold, bUnderLine, bItalic);
        font->Create();
        
        m_FontMap.Set(strName, (PVOID)font);
        
        if (bDefault){
            m_DefaultFont = font;
        }
    }

    return font;
}

bool CResourceMgr::AddFont(const CUIString&  strName, const CUIString& strFacename /*= NULL*/, 
                               BOOL bDefault /*= FALSE*/, int nSize /*= 0*/, BOOL bBold /*= FALSE*/, 
                               BOOL bUnderLine /*= FALSE*/, BOOL bItalic /*= FALSE*/)
{
    CUIFont* font = (CUIFont*)m_FontMap.Find((LPCWSTR)strName);
    if (!font){

        //
        // do not found it, create new font and add it to map.
        //

        font = new CUIFont(strName, strFacename, nSize, bBold, bUnderLine, bItalic);
        font->Create();

        m_FontMap.Set(strName, (PVOID)font);

        if (bDefault){
            m_DefaultFont = font;
        }
        return true;
    }

    return false;
}

CUIFont* CResourceMgr::GetFont(const CUIString& strName)
{
    return (CUIFont*) m_FontMap.Find(strName);
}

UINT CResourceMgr::GetFontHeight(const CUIString& strName)
{
    CUIFont* font = NULL;
    if (strName.IsEmpty()){
        font = GetDefault();
    }else{
        font = (CUIFont*)m_FontMap.Find(strName);
    }
    if (font){
        return font->GetFontHeight();
    }
    return 12;
}

BOOL CResourceMgr::SetDefault(const CUIString& strName)
{
    CUIFont* font = (CUIFont*)m_FontMap.Find(strName);
    if (font){
        m_DefaultFont = font;
        return TRUE;
    }
    return FALSE;
}

CUIFont* CResourceMgr::GetDefault()
{
    if (!m_DefaultFont){
        m_DefaultFont = new CUIFont;
        m_DefaultFont->Create();
        m_FontMap.Set(DEFAULT_FONT_KEY, m_DefaultFont);
    }

    return m_DefaultFont;
}

UINT CResourceMgr::GetDefaultFontHeight()
{
    return GetDefault()->GetFontHeight();
}

VOID CResourceMgr::ReleaseAllFont()
{
    CUIFont *font;
    for( int i = 0; i< m_FontMap.GetSize(); i++ ) {
        if(LPCWSTR key = m_FontMap.GetAt(i)) {
            font = static_cast<CUIFont *>(m_FontMap.Find(key));
            delete font;
        }
    }

    m_FontMap.RemoveAll();
}

void CResourceMgr::SetResourePath(const CUIString& strResPath)
{
    if (strResPath.IsEmpty()){
        return;
    }
    m_strResDir = strResPath;
    WCHAR cEnd = m_strResDir.GetAt(m_strResDir.GetLength() - 1);
    if(cEnd != L'\\' && cEnd != L'/'){
    #ifdef WIN32
        m_strResDir += L'\\';
    #else
        m_strResDir += L'/';
    #endif // WIN32
    }

}

CUIString CResourceMgr::GetResourePath()
{
    return m_strResDir;
}
