#include "stdafx.h"

#include "UIControl.h"


CUIControl::CUIControl(void):m_Parent(NULL),m_Window(NULL),
m_dwBackColor(0), m_dwBackColor2(0), m_dwBackColor3(0), m_bFloat(false), m_bEnabled(true),
m_bMouseEnabled(true), m_bSetPos(false), m_nBorderSize(0), m_dwBorderColor(0xFF000000), m_bUpdateNeeded(true),
m_bVisible(true)
{
    m_cxyMax.cx = m_cxyMax.cy = 999;
}

CUIControl::~CUIControl(void)
{
}

LPVOID CUIControl::GetInterface(const CUIString& strName)
{
    if (strName == DUI_CTR_CONTROL){
        return this;
    }

    return NULL;
}

CUIString CUIControl::GetName() const
{
    return m_strName;
}

void CUIControl::PaintStatusImage(cairo_t *cr)
{
    return;
}

void CUIControl::DoPaint(cairo_t *cr, const CUIRect& rcPaint)
{

    //
    // get the rect that need paint
    //

    if (UIIntersectRect(&m_rcPaint, &rcPaint, &m_rcItem)){

        //
        // here the control area is intersect with rcPaint.
        // do painting operation
        //
        // First paint background color
        //

        PaintBkColor(cr);

        //
        // paint background image
        //

        PaintBkImage(cr);

        //
        //
        //

        PaintStatusImage(cr);

        //
        // paint text
        //

        PaintText(cr);

        //
        // print border
        //

        PaintBorder(cr);
    }

}

void CUIControl::SetName(const CUIString &strName)
{
    m_strName = strName;
}

CUIString CUIControl::GetClass() const
{
    return DUICONTROL_CLASSNAME;
}

DWORD CUIControl::GetBkColor() const
{
    return m_dwBackColor;
}

void CUIControl::SetBkColor(DWORD dwBackColor)
{
    if (m_dwBackColor != dwBackColor){
        m_dwBackColor = dwBackColor;
        Invalidate();
    }
}

DWORD CUIControl::GetBkColor2() const
{
    return m_dwBackColor2;
}

void CUIControl::SetBkColor2(DWORD dwBackColor)
{
    if (m_dwBackColor2 != dwBackColor){
        m_dwBackColor2 = dwBackColor;
        Invalidate();
    }
}

DWORD CUIControl::GetBkColor3() const
{
    return m_dwBackColor3;
}

void CUIControl::SetBkColor3(DWORD dwBackColor)
{
    if (m_dwBackColor3 != dwBackColor){
        m_dwBackColor3 = dwBackColor;
        Invalidate();
    }
}

CUIString CUIControl::GetBkImage() const
{
    return m_strBackImage;
}

void CUIControl::SetBkImage(const CUIString& strImage)
{
    if(m_strBackImage != strImage){
        m_strBackImage = strImage;
        Invalidate();
    }
}

void CUIControl::SetBorderSize(int nSize)
{
    if (m_nBorderSize != nSize){
        m_nBorderSize = nSize;
        Invalidate();
    }
}

int CUIControl::GetBorderSize() const
{
    return m_nBorderSize;
}

DWORD CUIControl::GetBorderColor() const
{
    return m_dwBorderColor;
}

void CUIControl::SetBorderColor(DWORD dwBorderColor)
{
    if(m_dwBorderColor != dwBorderColor){
        m_dwBorderColor = dwBorderColor;
        Invalidate();
    }
}


void CUIControl::PaintBkColor(cairo_t *cr)
{
    if(m_dwBackColor != 0){
        if(m_dwBackColor2 != 0){
            if(m_dwBackColor3 != 0){

                //
                // first draw color1 to color2 witch step 8
                //

                CUIRect rc = m_rcItem;
                rc.bottom = (rc.bottom + rc.top) / 2;
                CUIRender::DrawLinerGradient(cr, rc, m_dwBackColor, m_dwBackColor2, TRUE, 8);

                //
                // then draw color1 to color2 with step 8
                //

                rc.top = rc.bottom;
                rc.bottom = m_rcItem.bottom;
                CUIRender::DrawLinerGradient(cr, rc, m_dwBackColor2, m_dwBackColor3, TRUE, 8);
            }else{

                //
                // draw color1 to color2 witch step 16
                //

                CUIRender::DrawLinerGradient(cr, m_rcItem, m_dwBackColor, m_dwBackColor2, TRUE, 16);
            }
        }else{
            CUIRender::DrawColor(cr, m_rcItem, m_dwBackColor);
        }
    }
}

void CUIControl::PaintBkImage(cairo_t *cr)
{
    if (!m_strBackImage.IsEmpty()){
        DrawImage(cr, m_strBackImage);
    }
}

bool CUIControl::DrawImage(cairo_t *cr, LPCWSTR lpstrImage, LPCWSTR lpstrModify /*= NULL*/)
{
    return CUIRender::DrawImageString(cr, m_rcItem, m_rcPaint, lpstrImage, lpstrModify);
}

void CUIControl::PaintText(cairo_t *cr)
{

}

void CUIControl::PaintBorder(cairo_t *cr)
{
    if (m_nBorderSize > 0){
        CUIRender::DrawRect(cr, m_rcItem, m_nBorderSize, m_dwBorderColor);
    }
}

bool CUIControl::IsVisible() const
{

    return m_bVisible;
}

void CUIControl::SetVisible(bool bVisible)
{
    if(m_bVisible != bVisible){
        bool v = IsVisible();
        m_bVisible = bVisible;
        if(m_bFocused)
            m_bFocused = false;
        //if (!bVisible && m_Manager && m_Manager->GetFocus() == this) {
        //    m_Manager->SetFocus(NULL) ;
        //}
        if(IsVisible() != v) {
            NeedParentUpdate();
        }
    }
}

void CUIControl::NeedParentUpdate()
{
    if(m_Parent) {
        m_Parent->NeedUpdate();
        m_Parent->Invalidate();
    }else{
        NeedUpdate();
    }

    if(m_Window != NULL)
        m_Window->NeedUpdate();
}

bool CUIControl::IsUpdateNeeded() const
{
    return m_bUpdateNeeded;
}

void CUIControl::NeedUpdate()
{
    if(IsVisible()){
        m_bUpdateNeeded = true;
        Invalidate();
        if(m_Window != NULL)
            m_Window->NeedUpdate();
    }
}

void CUIControl::SetPos(const CUIRect& rc)
{
    CUIRect rectTemp = rc;
    if( rectTemp.right < rectTemp.left )
        rectTemp.right = rc.left;
    if( rectTemp.bottom < rectTemp.top )
        rectTemp.bottom = rectTemp.top;

    CUIRect invalidateRc = m_rcItem;
    if(invalidateRc.IsEmpty())
        invalidateRc = rectTemp;

    m_rcItem = rectTemp;

    if(m_Window == NULL)
        return;

    if(m_bFloat) {
        CUIControl* pParent = GetParent();
        if( pParent != NULL ) {
            RECT rcParentPos = pParent->GetPos();
            if(m_cXY.cx >= 0)
                m_cXY.cx = m_rcItem.left - rcParentPos.left;
            else
                m_cXY.cx = m_rcItem.right - rcParentPos.right;
            if(m_cXY.cy >= 0)
                m_cXY.cy = m_rcItem.top - rcParentPos.top;
            else
                m_cXY.cy = m_rcItem.bottom - rcParentPos.bottom;
            m_cxyFixed.cx = m_rcItem.right - m_rcItem.left;
            m_cxyFixed.cy = m_rcItem.bottom - m_rcItem.top;
        }
    }

    m_bUpdateNeeded = false;
    invalidateRc.Join(m_rcItem);

    CUIControl* pParent = this;
    CUIRect rcTemp;
    CUIRect rcParent;
    while(pParent = pParent->GetParent()){
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if(!UIIntersectRect(&invalidateRc, &rcTemp, &rcParent)){
            return;
        }
    }
    m_Window->Invalidate(invalidateRc);
}

CUIControl* CUIControl::GetParent() const
{
    return m_Parent;
}

CUIRect CUIControl::GetPos() const
{
    return m_rcItem;
}

void CUIControl::Invalidate()
{
    if(!IsVisible())
        return;

    CUIRect invalidateRc = m_rcItem;

    CUIControl* pParent = this;
    RECT rcTemp;
    RECT rcParent;
    while(pParent = pParent->GetParent()){
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if(!UIIntersectRect(&invalidateRc, &rcTemp, &rcParent)){
            return;
        }
    }

    if(m_Window != NULL)
        m_Window->Invalidate(invalidateRc);
}

bool CUIControl::IsEnabled() const
{
    return m_bEnabled;
}

void CUIControl::SetEnabled(bool bEnabled)
{
    if(m_bEnabled != bEnabled){
        m_bEnabled = bEnabled;
        Invalidate();
    }
}

bool CUIControl::IsFloat() const
{
    return m_bFloat;
}

void CUIControl::SetFloat(bool bFloat)
{
    if( m_bFloat != bFloat ){
        m_bFloat = bFloat;
        NeedParentUpdate();
    }
}

CUIControl* CUIControl::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
    if((uFlags & UIFIND_VISIBLE) != 0 && !IsVisible())
        return NULL;
    if((uFlags & UIFIND_ENABLED) != 0 && !IsEnabled())
        return NULL;
    if((uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !m_rcItem.IsPtIn(* static_cast<LPPOINT>(pData))))
        return NULL;
    return Proc(this, pData);
}

bool CUIControl::IsMouseEnabled() const
{
    return m_bMouseEnabled;
}

void CUIControl::SetMouseEnabled(bool bEnabled)
{
    m_bMouseEnabled = bEnabled;
}

void CUIControl::DoEvent(TEventUI& Event)
{
    switch(Event.Type){
    case UIEVENT_SETCURSOR:
        //::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        break;
    case UIEVENT_SETFOCUS:
        m_bFocused = true;
        Invalidate();
        break;
    case UIEVENT_KILLFOCUS:
        m_bFocused = false;
        Invalidate();
        break;
    case UIEVENT_TIMER:

        //
        // send timer notify to all listener
        //

        m_Window->SendNotify(this, DUI_MSGTYPE_TIMER, Event.wParam, Event.lParam);
        break;
    case UIEVENT_CONTEXTMENU:

        //
        // TODO:
        //if( IsContextMenuUsed() ) {
        //    m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, Event.wParam, Event.lParam);
        //    return;
        //}
        //

        break;
    case UIEVENT_MOUSEENTER:
        {
            if (IsEnabled() && IsMouseEnabled()){
                m_Window->SendNotify(this, DUI_MSGTYPE_MOUSEENTER, Event.wParam, Event.lParam);
            }
        }
        break;
    case UIEVENT_MOUSELEAVE:
        {
            if (IsEnabled() && IsMouseEnabled()){
                m_Window->SendNotify(this, DUI_MSGTYPE_MOUSELEAVE, Event.wParam, Event.lParam);
            }
        }
        break;
    default:
        if (m_Parent){
            m_Parent->DoEvent(Event);
        }
    }
}

SIZE CUIControl::GetFixedXY() const
{
    return m_cXY;
}

void CUIControl::SetFixedXY(SIZE szXY)
{
    m_cXY.cx = szXY.cx;
    m_cXY.cy = szXY.cy;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

int CUIControl::GetFixedWidth() const
{
    return m_cxyFixed.cx;
}

void CUIControl::SetFixedWidth(int cx)
{
    if(cx < 0)
        return;

    m_cxyFixed.cx = cx;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

int CUIControl::GetFixedHeight() const
{
    return m_cxyFixed.cy;
}

void CUIControl::SetFixedHeight(int cy)
{
    if(cy < 0)
        return;
    m_cxyFixed.cy = cy;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

void CUIControl::SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue)
{

    CUIString strName = pstrName;

    if (strName == L"name"){
        SetName(pstrValue);
    }
    else if(strName == L"pos"){
        CUIRect rcPos;
        assert(ParseRectString(pstrValue, rcPos));
        SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
        SetFixedXY(szXY);
        SetFixedWidth(rcPos.right - rcPos.left);
        SetFixedHeight(rcPos.bottom - rcPos.top);
    }
    else if(strName == L"padding"){
        CUIRect rcPadding;
        assert(ParseRectString(pstrValue, rcPadding));
        SetPadding(rcPadding);
    }
    else if(strName == L"bkcolor" || strName == L"bkcolor1"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetBkColor(dwColor);
    }
    else if(strName == L"bkcolor2"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetBkColor2(dwColor);
    }
    else if(strName == L"bkcolor3"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));
        SetBkColor3(dwColor);
    }
    else if(strName == L"bordercolor"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));

        //
        // TODO:SetBorderColor
        //

        SetBorderColor(dwColor);
    }
    else if(strName == L"focusbordercolor"){
        DWORD dwColor;
        assert(ParseColorString(pstrValue, dwColor));

        //
        // TODO:SetFocusBorderColor
        //

        //SetFocusBorderColor(dwColor);
    }
    else if(strName == L"colorhsl"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);

        //
        // TODO:SetColorHSL
        //

        //SetColorHSL(bSet);
    }
    else if(strName == L"bordersize"){
        SetBorderSize(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"borderstyle"){

        //
        // TODO:SetBorderStyle
        //

        //SetBorderStyle(_wtoi(pstrValue));
    }
    else if (strName == L"borderround"){

        //
        // TODO
        //

    }
    else if(strName == L"borderround" ){
        CUISize cxyRound;
        assert(ParseSizeString(pstrValue, cxyRound));

        //
        // TODO: SetBorderRound
        //


        //SetBorderRound(cxyRound);
    }
    else if (strName == L"bkimage"){
        SetBkImage(pstrValue);
    }
    else if (strName == L"width"){
        SetFixedWidth(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"height"){
        SetFixedHeight(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"minwidth"){
        SetMinWidth(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"minheight"){
        SetMinHeight(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"maxwidth"){
        SetMaxWidth(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"maxheight"){
        SetMaxHeight(wcstol(pstrValue, NULL, 10));
    }
    else if (strName == L"text"){
        SetText(pstrValue);
    }
    else if (strName == L"tooltip"){

        //
        // TODO:SetToolTip
        //

        //SetToolTip(pstrValue);
    }
    else if (strName == L"userdata"){

        //
        // TODO:SetToolTip
        //

        //SetUserData(pstrValue);
    }
    else if (strName == L"enabled"){

        bool bSet;
        ParseBoolString(pstrValue, bSet);

        //
        // TODO:SetToolTip
        //

        SetEnabled(bSet);
    }
    else if (strName == L"mouse"){

        bool bSet;
        ParseBoolString(pstrValue, bSet);
        SetMouseEnabled(bSet);
    }
    else if (strName == L"keyboard"){
        bool bSet;
        ParseBoolString(pstrValue, bSet);

        //
        // TODO:SetKeyboardEnabled
        //

        //SetKeyboardEnabled(bSet);
    }
    else if (strName == L"visible"){
        bool bSet;

        ParseBoolString(pstrValue, bSet);
        SetVisible(bSet);
    }
    else if (strName == L"float"){
        bool bSet;

        ParseBoolString(pstrValue, bSet);
        SetFloat(bSet);
    }
    else if (strName == L"shortcut"){

        //
        // TODO
        //

        //SetShortcut(pstrValue[0]);
    }
    else if (strName == L"menu"){
        bool bSet;

        ParseBoolString(pstrValue, bSet);

        //
        // TODO:SetContextMenuUsed
        //

        //SetContextMenuUsed(bSet);
    }
    else if (strName == L"virtualwnd"){

        //
        // TODO:SetVirtualWnd
        //

        //SetVirtualWnd(pstrValue);
    }

}

void CUIControl::Init()
{

}

void CUIControl::SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit)
{
    m_Window = pWindow;
    m_Parent = pParent;
    if (bInit){
        Init();
    }
}

SIZE CUIControl::EstimateSize(SIZE szAvailable)
{
    return m_cxyFixed;
}

void CUIControl::SetText(LPCWSTR lpszText)
{
    if (m_strText != lpszText){
        m_strText = lpszText;
        Invalidate();
    }
}

bool CUIControl::Activate()
{
    if(!IsVisible() || !IsEnabled())
        return false;
    return true;
}

bool CUIControl::IsFocused() const
{
    return m_bFocused;
}

int CUIControl::GetMinWidth() const
{
    return m_cxyMin.cx;
}

void CUIControl::SetMinWidth(int cx)
{
    if(m_cxyMin.cx == cx)
        return;

    if(cx < 0)
        return;
    m_cxyMin.cx = cx;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

int CUIControl::GetMaxWidth() const
{
    return m_cxyMax.cx;
}

void CUIControl::SetMaxWidth(int cx)
{
    if(m_cxyMax.cx == cx)
        return;

    if(cx < 0)
        return;
    m_cxyMax.cx = cx;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

int CUIControl::GetMinHeight() const
{
    return m_cxyMin.cy;
}

void CUIControl::SetMinHeight(int cy)
{
    if(m_cxyMin.cy == cy)
        return;

    if(cy < 0)
        return;
    m_cxyMin.cy = cy;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

int CUIControl::GetMaxHeight() const
{
    return m_cxyMax.cy;
}

void CUIControl::SetMaxHeight(int cy)
{
    if(m_cxyMax.cy == cy)
        return;

    if(cy < 0)
        return;
    m_cxyMax.cy = cy;
    if(!m_bFloat)
        NeedParentUpdate();
    else
        NeedUpdate();
}

CUIRect CUIControl::GetPadding() const
{
    return m_rcPadding;
}

void CUIControl::SetPadding(const CUIRect& rcPadding)
{
    m_rcPadding = rcPadding;
    NeedParentUpdate();
}

void CUIControl::DoPostPaint(cairo_t *cr, const CUIRect& rcPaint)
{

}

void CUIControl::ApplyAttributeList(LPCWSTR pstrList)
{
    CUIString strItem;
    CUIString strValue;

    while(*pstrList != L'\0'){
        strItem.Empty();
        strValue.Empty();

        //
        // loop while find a '='
        //

        while(*pstrList != L'\0' && *pstrList != L'='){
            strItem += *pstrList++;
        }
        assert(*pstrList == L'=');
        if(*pstrList++ != L'=')
            return;

        //
        // skip space
        //

        while (*pstrList == L' '){
            pstrList ++;
        }

        assert( *pstrList == L'\"' );

        if(*pstrList++ != L'\"')
            return;

        //
        // loop while find another '\"'
        //

        while(*pstrList != L'\0' && *pstrList != L'\"'){
            strValue += *pstrList++;
        }
        assert(*pstrList == L'\"');
        if(*pstrList++ != L'\"')
            return;

        //
        // finsh parse jusr set it
        //

        SetAttribute(strItem, strValue);
        if(*pstrList++ != L' ')
            return;
    }
}
