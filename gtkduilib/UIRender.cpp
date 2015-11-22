#include "stdafx.h"
#include "UIRender.h"

CUIRender::CUIRender(void)
{
}

CUIRender::~CUIRender(void)
{
}

void CUIRender::DrawLinerGradient(
    cairo_t * cr, int x, int y, int nWidth, int nHeight, 
    DWORD dwFirst, DWORD dwSecond, 
    BOOL bVertial, int nSteps
)
{
    cairo_pattern_t *linpat;

    //
    // create a new pattern linear
    //

    if (bVertial){
        linpat = cairo_pattern_create_linear(0, 0, 0, nHeight);
    }else{
        linpat = cairo_pattern_create_linear(0, 0, nWidth, 0);
    }


    //
    // set the start end stop rgb
    //

    cairo_pattern_add_color_stop_rgba(linpat, 0.0,  UIGetRValue(dwFirst)/255.0,
        UIGetGValue(dwFirst)/255.0, UIGetBValue(dwFirst)/255.0, 1);
    cairo_pattern_add_color_stop_rgba(linpat, 1.0,  UIGetRValue(dwSecond)/255.0,
        UIGetGValue(dwSecond)/255.0, UIGetBValue(dwSecond)/255.0, 1);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    //
    // set the destination rectangle
    //

    cairo_rectangle(cr, x, y, nWidth, nHeight);

    //
    // set draw source
    //

    cairo_set_source(cr, linpat);

    //
    // fill it
    //

    cairo_fill(cr);

    cairo_pattern_destroy(linpat);
}

void CUIRender::DrawLinerGradient(
    cairo_t * cr, const CUIRect &rc, DWORD dwFirst, 
    DWORD dwSecond, BOOL bVertial, int nSteps)
{
    DrawLinerGradient(cr, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(),
        dwFirst, dwSecond, bVertial, nSteps);
}


void CUIRender::DrawColor(cairo_t *cr, int x, int y, int nWidth, int nHeight, DWORD dwColor)
{
    
    //
    // set source rgba
    //

    cairo_set_source_rgba(cr, UIGetRValue(dwColor)/255.0, UIGetGValue(dwColor)/255.0,
        UIGetBValue(dwColor)/255.0, UIGetAValue(dwColor)/255.0);

    //
    // set the rect we want paint
    //
    
    cairo_rectangle(cr, x, y, nWidth, nHeight);
    
    //
    // fill witch source
    //
    
    cairo_fill(cr);
}

void CUIRender::DrawLine(cairo_t *cr, const CUIRect& rc, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
{

    cairo_antialias_t OldMode;

    //
    // save the original mode.
    //

    OldMode = cairo_get_antialias(cr);

    //
    // set line width
    //

    cairo_set_line_width(cr, nSize);

    //
    // set antialiasing mode (抗锯齿模式)
    //

    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

    //
    // move to start point.
    //

    cairo_move_to(cr, (double)rc.left, (double)rc.top);

    //
    // draw the line
    //

    cairo_line_to(cr, (double)rc.right, (double)rc.bottom);

    cairo_stroke(cr);

    //
    // restore the original antialiasing mode
    //

    cairo_set_antialias(cr, OldMode);

}

void CUIRender::DrawColor(cairo_t *cr, const CUIRect& rc, DWORD dwColor)
{
    DrawColor(cr, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), dwColor);
}

void CUIRender::DrawText(cairo_t * cr, const CUIRect& rc, const CUIString& strText, 
                         DWORD dwStyle, const CUIString& strFontName, 
                         DWORD dwColor, bool bShowHtml, CUIRect *rcOut)
{
    return DrawText(cr, rc.left, rc.top, rc.GetWidth(), 
            rc.GetHeight(), strText.GetData(),dwStyle, 
            strFontName.GetData(), dwColor, bShowHtml, rcOut);
}

void CUIRender::DrawText(cairo_t * cr, int x, int y, int nWidth, int nHeight, 
                         LPCWSTR lpszText, DWORD dwStyle, LPCWSTR lpszFontName, 
                         DWORD dwColor, bool bShowHtml,
                         CUIRect *rcOut)
{
    PangoLayout *Layout;
    PangoFontDescription *FontDesc;
    int nFixY = y;

    //
    // create pango layout with cairo
    //
    
    Layout = pango_cairo_create_layout(cr);
    //FontDesc = pango_font_description_from_string((LPCSTR)CW2A(FONT, CP_UTF8));
    
    //
    // save cr.
    //

    cairo_save(cr);
    
    //
    // clip the rect to make sure do not draw outsize the rectangle
    //
    
    cairo_rectangle(cr, x, y, nWidth, nHeight);
    cairo_clip(cr);

    //
    // set text color
    //
    

    cairo_set_source_rgb(cr, UIGetRValue(dwColor)/255.0, UIGetGValue(dwColor)/255.0,
        UIGetBValue(dwColor)/255.0);

    //
    // get font from resource manager
    //
    
    CUIFont* clsFont = CResourceMgr::GetInstance().GetFont(lpszFontName);
    if (clsFont){
        FontDesc = (PangoFontDescription *)clsFont->GetHandle();
    }else{
        FontDesc = (PangoFontDescription *)CResourceMgr::GetInstance().GetDefault()->GetHandle();
    }
    
    
    //
    // set font to layout
    //
    
    pango_layout_set_font_description(Layout, FontDesc);

    //
    // set alignment
    //
    
    if (dwStyle & DT_LEFT){
        pango_layout_set_alignment(Layout, PANGO_ALIGN_LEFT);
    }else if (dwStyle & DT_CENTER){
        pango_layout_set_alignment(Layout, PANGO_ALIGN_CENTER);
    }else if (dwStyle & DT_RIGHT){
        pango_layout_set_alignment(Layout, PANGO_ALIGN_RIGHT);
    }

    if (dwStyle & DT_END_ELLIPSIS){
        pango_layout_set_ellipsize(Layout, PANGO_ELLIPSIZE_END);
    }
    
    CUIString strTest = lpszText;
    
    if (dwStyle & DT_SINGLELINE){
        pango_layout_set_single_paragraph_mode(Layout, TRUE);
    }else{
        
        //
        // 替换掉字符串中的\r
        //
        
        strTest.Replace(L"\\n", L"\n");
        pango_layout_set_single_paragraph_mode(Layout, FALSE);
    }
    

    //
    // set draw width
    //

    pango_layout_set_width(Layout, nWidth * PANGO_SCALE/*-1*/);

    //
    // set drawing text
    //
    
    if (bShowHtml){
        pango_layout_set_markup(Layout, (LPCSTR)CW2U8(strTest), -1);
    }else{
        pango_layout_set_text(Layout, (LPCSTR)CW2U8(strTest), -1);
    }
    
    //
    // set vertical alignment width
    //
    
    int textWidth, textHeigth;
    pango_layout_get_pixel_size(Layout, &textWidth, &textHeigth);

    if (dwStyle & DT_TOP){
        
        //
        // do not change y
        //
        
        nFixY = y;
    }else if (dwStyle & DT_BOTTOM){
        nFixY = y + MAX(nHeight - textHeigth, 0);
    }else if (dwStyle & DT_VCENTER){
        nFixY = y + MAX((nHeight - textHeigth) / 2, 0);
    }

    //
    // move to draw start point
    //
    
    if(dwStyle & DT_CALCRECT && rcOut != NULL){
        rcOut->left = x;
        rcOut->top = nFixY;
        rcOut->right = x + textWidth;
        rcOut->bottom = nFixY + textHeigth;

    }else{
        cairo_move_to(cr, x, nFixY);
        pango_cairo_update_layout(cr, Layout);
        pango_cairo_show_layout(cr, Layout);
    }
    

    g_object_unref(Layout);
    cairo_restore(cr);
}

bool CUIRender::AlphaBlend(cairo_t* cr, GdkPixbuf *sPixbuf, int dx, int dy, int dWidth, int dHeight, 
                         int sx, int sy, int sWidth, int sHeight, int alpha)
{

    GdkPixbuf *SubPixbuf;
    GdkPixbuf *NewPixbuf;

    //
    // get the sub pixbuf from source pixbuf
    //

    SubPixbuf = gdk_pixbuf_new_subpixbuf(sPixbuf, sx, sy, sWidth, sHeight);
    if (!SubPixbuf){
        return false;
    }

    //
    // create a new buffer for destination
    //

    NewPixbuf = gdk_pixbuf_new(gdk_pixbuf_get_colorspace(sPixbuf), 
        gdk_pixbuf_get_has_alpha(sPixbuf), 8, dWidth, dHeight);
    if (!NewPixbuf){
        g_object_unref(SubPixbuf);
        return false;
    }

    //
    // scale the buffer for destination
    //

    gdk_pixbuf_scale(SubPixbuf, NewPixbuf, 0, 0, dWidth, dHeight, 0, 0, 
        (double)dWidth/sWidth, (double)dHeight/sHeight, GDK_INTERP_BILINEAR);

    //
    // draw the pixbuf
    //

    gdk_cairo_set_source_pixbuf(cr, NewPixbuf, dx , dy);
    cairo_paint_with_alpha(cr, (double)alpha/255);

    //
    // clean up
    //

    g_object_unref(NewPixbuf);
    g_object_unref(SubPixbuf);

    return true;
}

bool CUIRender::DrawImage(
    cairo_t* cr, GdkPixbuf* Pixbuf, const CUIRect& rc, 
    const CUIRect& rcPaint, const CUIRect& rcImage, const CUIRect& rcCorners, 
    int alpha, BOOL hole, BOOL xtiled, BOOL ytiled)
{
    RECT rcDest;
    RECT rcTemp;

    //
    // middle
    //

    if(!hole){
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            if(!xtiled && !ytiled) {
                rcDest.right -= rcDest.left;
                rcDest.bottom -= rcDest.top;
                AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                    rcImage.left + rcCorners.left, rcImage.top + rcCorners.top, \
                    rcImage.right - rcImage.left - rcCorners.left - rcCorners.right, \
                    rcImage.bottom - rcImage.top - rcCorners.top - rcCorners.bottom, alpha);

            }else if(xtiled && ytiled){
                LONG lWidth = rcImage.right - rcImage.left - rcCorners.left - rcCorners.right;
                LONG lHeight = rcImage.bottom - rcImage.top - rcCorners.top - rcCorners.bottom;
                int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                for(int j = 0; j < iTimesY; ++j){
                    LONG lDestTop = rcDest.top + lHeight * j;
                    LONG lDestBottom = rcDest.top + lHeight * (j + 1);
                    LONG lDrawHeight = lHeight;
                    if(lDestBottom > rcDest.bottom){
                        lDrawHeight -= lDestBottom - rcDest.bottom;
                        lDestBottom = rcDest.bottom;
                    }
                    for(int i = 0; i < iTimesX; ++i){
                        LONG lDestLeft = rcDest.left + lWidth * i;
                        LONG lDestRight = rcDest.left + lWidth * (i + 1);
                        LONG lDrawWidth = lWidth;
                        if( lDestRight > rcDest.right ) {
                            lDrawWidth -= lDestRight - rcDest.right;
                            lDestRight = rcDest.right;
                        }
                        AlphaBlend(cr, Pixbuf, rcDest.left + lWidth * i, rcDest.top + lHeight * j, 
                            lDestRight - lDestLeft, lDestBottom - lDestTop,
                            rcImage.left + rcCorners.left, rcImage.top + rcCorners.top, lDrawWidth, lDrawHeight, alpha);
                    }
                }
            }else if(xtiled){
                LONG lWidth = rcImage.right - rcImage.left - rcCorners.left - rcCorners.right;
                int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
                for(int i = 0; i < iTimes; ++i){
                    LONG lDestLeft = rcDest.left + lWidth * i;
                    LONG lDestRight = rcDest.left + lWidth * (i + 1);
                    LONG lDrawWidth = lWidth;
                    if(lDestRight > rcDest.right){
                        lDrawWidth -= lDestRight - rcDest.right;
                        lDestRight = rcDest.right;
                    }
                    AlphaBlend(cr, Pixbuf, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
                        rcImage.left + rcCorners.left, rcImage.top + rcCorners.top, \
                        lDrawWidth, rcImage.bottom - rcImage.top - rcCorners.top - rcCorners.bottom, alpha);
                }
            }else{ 

                //
                // ytiled
                //

                LONG lHeight = rcImage.bottom - rcImage.top - rcCorners.top - rcCorners.bottom;
                int iTimes = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
                for(int i = 0; i < iTimes; ++i){
                    LONG lDestTop = rcDest.top + lHeight * i;
                    LONG lDestBottom = rcDest.top + lHeight * (i + 1);
                    LONG lDrawHeight = lHeight;
                    if(lDestBottom > rcDest.bottom){
                        lDrawHeight -= lDestBottom - rcDest.bottom;
                        lDestBottom = rcDest.bottom;
                    }
                    AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
                        rcImage.left + rcCorners.left, rcImage.top + rcCorners.top, \
                        rcImage.right - rcImage.left - rcCorners.left - rcCorners.right, lDrawHeight, alpha);                    
                }
            }
        }
    }

    //
    // left-top
    //

    if(rcCorners.left > 0 && rcCorners.top > 0){
        rcDest.left = rc.left;
        rcDest.top = rc.top;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if( UIIntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.left, rcImage.top, rcCorners.left, rcCorners.top, alpha);
        }
    }

    //
    // top
    //

    if(rcCorners.top > 0){
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.top;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)) {
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.left + rcCorners.left, rcImage.top, rcImage.right - rcImage.left - \
                rcCorners.left - rcCorners.right, rcCorners.top, alpha);
        }
    }

    //
    // right-top
    //

    if(rcCorners.right > 0 && rcCorners.top > 0){
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.top;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rcCorners.top;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.right - rcCorners.right, rcImage.top, rcCorners.right, rcCorners.top, alpha);
        }
    }

    //
    // left
    //

    if(rcCorners.left > 0){
        rcDest.left = rc.left;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.left, rcImage.top + rcCorners.top, rcCorners.left, rcImage.bottom - \
                rcImage.top - rcCorners.top - rcCorners.bottom, alpha);
        }
    }

    //
    // right
    //

    if(rcCorners.right > 0){
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.top + rcCorners.top;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.right - rcCorners.right, rcImage.top + rcCorners.top, rcCorners.right, \
                rcImage.bottom - rcImage.top - rcCorners.top - rcCorners.bottom, alpha);
        }
    }

    //
    // left-bottom
    //

    if(rcCorners.left > 0 && rcCorners.bottom > 0){
        rcDest.left = rc.left;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rcCorners.left;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if( UIIntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.left, rcImage.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, alpha);
        }
    }

    //
    // bottom
    //

    if(rcCorners.bottom > 0){
        rcDest.left = rc.left + rcCorners.left;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.left + rcCorners.left, rcImage.bottom - rcCorners.bottom, \
                rcImage.right - rcImage.left - rcCorners.left - rcCorners.right, rcCorners.bottom, alpha);
        }
    }

    //
    // right-bottom
    //

    if(rcCorners.right > 0 && rcCorners.bottom > 0){
        rcDest.left = rc.right - rcCorners.right;
        rcDest.top = rc.bottom - rcCorners.bottom;
        rcDest.right = rcCorners.right;
        rcDest.bottom = rcCorners.bottom;
        rcDest.right += rcDest.left;
        rcDest.bottom += rcDest.top;
        if(UIIntersectRect(&rcTemp, &rcPaint, &rcDest)){
            rcDest.right -= rcDest.left;
            rcDest.bottom -= rcDest.top;
            AlphaBlend(cr, Pixbuf, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, \
                rcImage.right - rcCorners.right, rcImage.bottom - rcCorners.bottom, rcCorners.right, \
                rcCorners.bottom, alpha);
        }
    }

    return true;
}

void CUIRender::DrawRect(cairo_t* cr, const CUIRect& rc, int nSize, DWORD dwColor)
{
    cairo_antialias_t OldMode;
    
    //
    // save the old mode
    //
    
    OldMode = cairo_get_antialias(cr);
    
    //
    // 不使用抗锯齿模式, 否则和windows下的
    // 颜色不一样
    //
    
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

    //
    // set line width
    //
    
    cairo_set_line_width(cr, nSize);
    
    //
    // set color
    //

    cairo_set_source_rgb(cr, UIGetBValue(dwColor)/255.0, UIGetGValue(dwColor)/255.0,
        UIGetRValue(dwColor)/255.0);

    //
    // rectangle.这里要设成和PS_INSIDEFRAME一样的样式.
    //
    
    cairo_rectangle(cr, (double)rc.left + nSize/2.0, (double)rc.top + nSize/2.0, 
        (double)rc.GetWidth() - nSize, (double)rc.GetHeight() - nSize);
    
    //
    // draw it.
    //
    
    cairo_stroke(cr);
    
    //
    // restore the old antialias mode
    //
    
    cairo_set_antialias(cr, OldMode);

}

bool CUIRender::DrawImage(cairo_t* cr, const CUIString& strImage, const CUIRect& rcControl, 
    const CUIRect& rcPaint, const CUIRect& rcDest, CUIRect& rcImage, const CUIRect& rcCorners, 
    int alpha, BOOL hole, BOOL xtiled, BOOL ytiled)
{
    GdkPixbuf *Pixbuf;
    
    //
    // first get the pixbuf from resource manager
    // if it don't exist. try to add it to map.
    //
    
    Pixbuf = CResourceMgr::GetInstance().GetImage(strImage, true);
    if (Pixbuf){
        
        int nWidth, nHeight;

        //
        // check the source rect
        //
        
        nWidth = gdk_pixbuf_get_width(Pixbuf);
        nHeight = gdk_pixbuf_get_height(Pixbuf);

        if(rcImage.IsNull()) {
            rcImage.right = nWidth;
            rcImage.bottom = nHeight;
        }
        if (rcImage.right > nWidth) 
            rcImage.right = nWidth;
        if (rcImage.bottom > nHeight) 
            rcImage.bottom = nHeight;
        
        //
        // check the destination rect
        //
        
        RECT rcTemp;
        if(!UIIntersectRect(&rcTemp, &rcDest, &rcControl)) 
            return true;
        if(!UIIntersectRect(&rcTemp, &rcDest, &rcPaint)) 
            return true;

        //
        // everything is ok, try to draw it.
        //

        return DrawImage(cr, Pixbuf, rcDest, rcPaint, rcImage, 
            rcCorners, alpha, hole, xtiled, ytiled);
    }
    return false;
}

bool CUIRender::DrawImageString(cairo_t *cr, const CUIRect& rcControl, 
                                const CUIRect& rcPaint, LPCWSTR lpstrImage, LPCWSTR lpStrModify)
{
    if (cr == NULL) 
        return false;
    
    //
    // 1. aaa.jpg
    // 2. file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' 
    // mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false'
    //

    CUIString strImageName = lpstrImage;
    CUIString strImageResType;
    CUIRect rcDest = rcControl;
    CUIRect rcImage;
    CUIRect rcCorner;
    DWORD dwMask = 0;
    BYTE bFade = 0xFF;
    bool bHole = false;
    bool bTiledX = false;
    bool bTiledY = false;
    int nImageCounts = 0;

    CUIString strItem;
    CUIString strValue;
    LPWSTR pstr = NULL;
    
    //
    // loop twice. the first time we get all attributes of images
    // the second time get modified attributes
    //

    for(int i = 0; i < 2; ++i, nImageCounts = 0 ) {

        if(i == 1){
            
            //
            // the second round
            //
            
            lpstrImage = lpStrModify;
        }

        if(!lpstrImage)
            continue;

        while(*lpstrImage != '\0'){
            
            //
            // clear the token and value
            //
            
            strItem.Empty();
            strValue.Empty();
            
            //
            // skip abnormal characters
            //
            
            while(*lpstrImage > L'\0' && *lpstrImage <= L' '){
                lpstrImage ++;
            }
            
            //
            // get the item string
            //
            
            while(*lpstrImage != L'\0' && *lpstrImage != L'=' && *lpstrImage > L' ') {
                
                //
                // for unicode just ++ for next character
                //
                
                strItem += *lpstrImage++;
            }
            
            //
            // skip abnormal characters
            //

            while(*lpstrImage > L'\0' && *lpstrImage <= L' '){
                lpstrImage ++;
            }

            //
            // here we already get the item string the next must be '=' character
            //

            if(*lpstrImage++ != L'='){
                
                //
                // invalid format break the loop
                //
                
                break;
            }

            //
            // skip abnormal characters
            //

            while(*lpstrImage > L'\0' && *lpstrImage <= L' '){
                lpstrImage ++;
            }
            
            //
            // the value string must be quote with " \' "
            //
            
            if(*lpstrImage++ != L'\''){
                
                //
                // invalid value string format, break the loop
                //
                
                break;
            }
            
            //
            // skip the quote and get the value string
            //
            
            while(*lpstrImage != L'\0' && *lpstrImage != L'\'') {
                strValue += *lpstrImage++;
            }
            
            //
            // the end character of value string must be " \' "
            //
            
            if(*lpstrImage++ != L'\''){
                break;
            }

            if(!strValue.IsEmpty()){

                if(strItem == L"file" || strItem == L"res"){
                    strImageName = strValue;
                }else if(strItem == L"restype"){
                    strImageResType = strValue;
                }else if(strItem == L"dest"){
                    
                    //
                    // destination rect rectangle on control's rectangle
                    //
                    
                    rcDest.left = rcControl.left + wcstol(strValue.GetData(), &pstr, 10);
                    rcDest.top = rcControl.top + wcstol(pstr + 1, &pstr, 10);
                    rcDest.right = rcControl.left + wcstol(pstr + 1, &pstr, 10);
                    if(rcDest.right > rcControl.right) 
                        rcDest.right = rcControl.right;
                    rcDest.bottom = rcControl.top + wcstol(pstr + 1, &pstr, 10);
                    if (rcDest.bottom > rcControl.bottom) 
                        rcDest.bottom = rcControl.bottom;
                }else if( strItem == L"source"){
                    
                    //
                    // source is defined the rectangle of image we want paint
                    //
                    
                    rcImage.left = wcstol(strValue.GetData(), &pstr, 10);   
                    rcImage.top = wcstol(pstr + 1, &pstr, 10);
                    rcImage.right = wcstol(pstr + 1, &pstr, 10);
                    rcImage.bottom = wcstol(pstr + 1, &pstr, 10);
                }else if(strItem == L"corner"){
                    
                    //
                    // for 9 grid draw
                    //
                    
                    rcCorner.left = wcstol(strValue.GetData(), &pstr, 10);
                    rcCorner.top = wcstol(pstr + 1, &pstr, 10);
                    rcCorner.right = wcstol(pstr + 1, &pstr, 10);
                    rcCorner.bottom = wcstol(pstr + 1, &pstr, 10);
                }else if(strItem == L"mask"){
                    if(strValue[0] == L'#') 
                        dwMask = wcstoul(strValue.GetData() + 1, &pstr, 16);
                    else 
                        dwMask = wcstoul(strValue.GetData(), &pstr, 16);
                }else if(strItem == L"fade"){
                    bFade = (BYTE)wcstoul(strValue.GetData(), &pstr, 10);
                }else if(strItem == L"hole"){
                    bHole = (strValue == L"true");
                }else if(strItem == L"xtiled"){
                    bTiledX = (strValue == L"true");
                }else if(strItem == L"ytiled"){
                    bTiledY = (strValue == L"true");
                }
            }
            
            //
            // the next attribute and the this 
            // attribute must be split with a space
            //
            
            if(*lpstrImage++ != L' ')
                break;
        }
    }
    
    //
    // draw it.
    //
    
    return DrawImage(cr, strImageName, rcControl, 
        rcPaint, rcDest, rcImage, rcCorner, 
        bFade, bHole, bTiledX, bTiledY);
}
