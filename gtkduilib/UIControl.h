#pragma once

#define DUICONTROL_CLASSNAME L"DUIControl"

class CUIControl;
class CUIWindow;

typedef CUIControl* (CALLBACK* FINDCONTROLPROC)(CUIControl*, LPVOID);

class CUIControl
{
public:
    CUIControl(void);
    virtual ~CUIControl(void);

public:
    
    /** \fn virtual CUIString GetClass() const
    	\brief 获取类名.
    	\return 类名.
    */

    virtual CUIString GetClass() const;

    /** \fn virtual LPVOID GetInterface(const CUIString& strName)
    	\brief 通过接口名称获取接口指针.
    	\param [in] strName 接口名称.
    	\return 接口指针,名称不存在返回NULL.
    */
    
    virtual LPVOID GetInterface(const CUIString& strName);

    /** \fn virtual CUIString GetName()
        \brief 获取控件名字.
        \return 控件的名字.
    */
    
    virtual CUIString GetName() const;
    
    /** \fn virtual void SetName(CUIString &strName)
    	\brief 设置控件名字.
    	\param [in] strName 控件名字所对应的字符串.
    */

    virtual void SetName(const CUIString &strName);

    /** \fn DWORD GetBkColor() const
    	\brief 获取背景颜色.
    	\return 背景颜色.
    */
    
    DWORD GetBkColor() const;
    
    /** \fn void SetBkColor(DWORD dwBackColor)
    	\brief 设置背景颜色.
    	\param [in] dwBackColor 背景颜色.
    	\return .
    */
    
    void SetBkColor(DWORD dwBackColor);
    
    /** \fn DWORD GetBkColor2() const
    	\brief 获取渐变背景颜色2.
    	\return 渐变背景颜色2.
    */
    
    DWORD GetBkColor2() const;
    
    /** \fn void SetBkColor2(DWORD dwBackColor)
    	\brief 获取渐变背景颜色2.
    	\param [in] dwBackColor 渐变背景颜色2.
    */
    
    void SetBkColor2(DWORD dwBackColor);
    
    /** \fn DWORD GetBkColor3() const
    	\brief 获取渐变背景颜色3.
    	\return 渐变背景颜色3.
    */
    
    DWORD GetBkColor3() const;
    
    /** \fn void SetBkColor3(DWORD dwBackColor)
    	\brief 获取渐变背景颜色3.
    	\param [in] dwBackColor 渐变背景颜色3.
    */
    
    void SetBkColor3(DWORD dwBackColor);
    
    /** \fn CUIString GetBkImage() const
    	\brief 获取背景图片.
    	\return 背景图片路径.
    */
    
    CUIString GetBkImage() const;
    
    /** \fn void SetBkImage(const CUIString& strImage)
    	\brief 设置背景图片.
    	\param [in] strImage 背景图片路径.
    */
    
    void SetBkImage(const CUIString& strImage);
    
    /** \fn bool DrawImage(cairo_t *cr, LPCWSTR lpstrImage, LPCWSTR lpstrModify = NULL)
    	\brief 在指定的设备Context上画图.
    	\param [in] cr cairo context.
        \param [in] lpstrImage 要绘制的图片的路径.
        \param [in] lpstrModify 修正的图片的绘制属性,默认为空.
    	\return 成功返回true, 否则false.
    */
    
    bool DrawImage(cairo_t *cr, LPCWSTR lpstrImage, LPCWSTR lpstrModify = NULL);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void DoPaint(cairo_t *cr, const CUIRect& rcPaint);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void PaintBkColor(cairo_t *cr);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void PaintBkImage(cairo_t *cr);

    //virtual void PaintStatusImage(cairo_t *cr);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void PaintText(cairo_t *cr);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void Invalidate();
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void NeedUpdate();
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    bool IsUpdateNeeded() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void NeedParentUpdate();
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual CUIControl* GetParent() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetPos(const CUIRect& rc);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual CUIRect GetPos() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetVisible(bool bVisible);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual bool IsVisible() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual bool IsEnabled() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetEnabled(bool bEnabled);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual CUIControl* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    bool IsMouseEnabled() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void SetMouseEnabled(bool bEnabled);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void DoEvent(TEventUI& event);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    bool IsFloat() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void SetFloat(bool bFloat);
    int GetFixedHeight() const;
    void SetFixedHeight(int cy);
    void SetFixedWidth(int cx);
    int GetFixedWidth() const;
    void SetFixedXY(SIZE szXY);
    SIZE GetFixedXY() const;

    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    void ApplyAttributeList(LPCWSTR pstrList);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetAttribute(LPCWSTR pstrName, LPCWSTR pstrValue);
    //virtual void PaintBorder(cairo_t *cr);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetOwnWindow(CUIWindow* pWindow, CUIControl* pParent, bool bInit = true);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    //void SetManager(CUIManager *pManager);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual SIZE EstimateSize(SIZE szAvailable);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetText(LPCWSTR pstrText);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual bool Activate();
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual bool IsFocused() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void PaintStatusImage(cairo_t *cr);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetMaxHeight(int cy);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual int GetMaxHeight() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetMinHeight(int cy);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual int GetMinHeight() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetMaxWidth(int cx);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual int GetMaxWidth() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetMinWidth(int cx);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual int GetMinWidth() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual CUIRect GetPadding() const;
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void SetPadding(const CUIRect& rcPadding);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void DoPostPaint(cairo_t *cr, const CUIRect& rcPaint);
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */

    virtual void Init();
    
    /** \fn 
    	\brief .
    	\param .
    	\return .
    */
    
    virtual void PaintBorder(cairo_t *cr);

    //
    // 边框相关
    //
    
    void SetBorderColor(DWORD dwBorderColor);
    DWORD GetBorderColor() const;
    int GetBorderSize() const;
    void SetBorderSize(int nSize);
    
private:

    /** 背景颜色*/
    DWORD m_dwBackColor;

    /** 背景颜色2*/
    DWORD m_dwBackColor2;

    /** 背景颜色2*/
    DWORD m_dwBackColor3;

    /** 全局manager*/
    /*CUIManager *m_Manager;*/
    
    /** 控件是否可用*/
    bool m_bEnabled;

    /** 鼠标在此控件上是否可用*/
    bool m_bMouseEnabled;
    
    /** 控件是否需要更新*/
    bool m_bUpdateNeeded;
    
    /** 控件是否更新了pos*/
    bool m_bSetPos;
    
    /** 控件是否使用浮点坐标*/
    bool m_bFloat;
    
    /** 控件的长宽*/
    CUISize m_cXY;

    /** 控件的最大长宽*/
    CUISize m_cxyMax;
    
    /** 控件的最小长宽*/
    CUISize m_cxyMin;

    CUIRect m_rcPadding;

protected:
    
    /** 控件名称*/
    CUIString m_strName;

    /** 控件是否可见*/
    bool m_bVisible;

    /** 用于储存父控件节点*/
    CUIControl* m_Parent;

    /** 表示控件是否在获取焦点的状态下*/
    bool m_bFocused;

    /** 窗口rect*/
    CUIRect m_rcItem;

    /** 控件固定长宽*/
    CUISize m_cxyFixed;

    /** 包含控件的window*/
    CUIWindow *m_Window;

    /** 控件paint区域*/
    CUIRect m_rcPaint;
    
    /** 用于保存控件显示的文字*/
    CUIString m_strText;
    
    /** 记录边框大小*/
    int m_nBorderSize;

    /** 边框颜色*/
    DWORD m_dwBorderColor;

    /** 背景图片*/
    CUIString m_strBackImage;
};
