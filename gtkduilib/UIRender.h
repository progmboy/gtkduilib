#pragma once

#define UIGetAValue(rgb) (LOBYTE((rgb)>>24))
#define UIGetBValue(rgb) (LOBYTE(rgb))
#define UIGetGValue(rgb) (LOBYTE(((WORD)(rgb)) >> 8))
#define UIGetRValue(rgb) (LOBYTE((rgb)>>16))

class CUIRender
{
public:
    CUIRender(void);
    virtual ~CUIRender(void);
public:
    static void DrawLinerGradient(cairo_t * cr, int x, int y, int nWidth, 
        int nHeight, DWORD dwFirst, DWORD dwSecond, 
        BOOL bVertial, int nSteps );
    static void DrawLinerGradient( cairo_t * cr, const CUIRect &rc, DWORD dwFirst, 
        DWORD dwSecond, BOOL bVertial, int nSteps );
    static void DrawColor(cairo_t *cr, int x, int y, int nWidth, int nHeight, DWORD dwColor);
    static void DrawColor(cairo_t *cr, const CUIRect& rc, DWORD dwColor);
    static void DrawRect(cairo_t* cr, const CUIRect& rc, int nSize, DWORD dwColor);
    static bool DrawImage( cairo_t* cr, GdkPixbuf* Pixbuf, const CUIRect& rc, 
        const CUIRect& rcPaint, const CUIRect& rcImage, const CUIRect& rcCorners, 
        int alpha, BOOL hole, BOOL xtiled, BOOL ytiled);
    static bool DrawImage(cairo_t* cr, /*CUIManager* pManager,*/ const CUIString& strImage,
        const CUIRect& rcControl, const CUIRect& rcPaint, const CUIRect& rcDest,
        CUIRect& rcImage, const CUIRect& rcCorners,
        int alpha, BOOL hole, BOOL xtiled, BOOL ytiled);
    static bool AlphaBlend(cairo_t* cr, GdkPixbuf *sPixbuf, int dx, int dy, int dWidth, 
            int dHeight, int sx, int sy, int sWidth, int sHeight, int alpha);
    static bool DrawImageString(cairo_t *cr, /*CUIManager* pManager,*/ const CUIRect& rcControl, 
        const CUIRect& rcPaint, LPCWSTR lpStrImage, LPCWSTR lpStrModify);
    static void DrawText(cairo_t * cr, int x, int y, int nWidth, int nHeight, 
        LPCWSTR lpszText, DWORD dwStyle, LPCWSTR lpszFontName, DWORD dwColor, bool bShowHtml = false,
        CUIRect *rcOut = NULL);
    static void DrawText(cairo_t * cr, const CUIRect& rc, const CUIString& strText, 
        DWORD dwStyle, const CUIString& strFontName, DWORD dwColor, 
        bool bShowHtml = false, CUIRect *rcOut = NULL);
    static void DrawLine(cairo_t *cr, const CUIRect& rc, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/);
};