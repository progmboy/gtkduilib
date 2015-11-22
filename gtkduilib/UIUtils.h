#ifndef __INCLUDE_UIUTILS__H__
#define __INCLUDE_UIUTILS__H__

#include <gdk/gdk.h>
#include "UIString.h"

BOOL UIIntersectRect(
                     LPRECT prcDst,
                     CONST RECT *prcSrc1,
                     CONST RECT *prcSrc2);
ULONG UIGetTickCount();


class CUIRect : public tagRECT
{
public:
    CUIRect();
    CUIRect(const RECT& src);
    CUIRect(int iLeft, int iTop, int iRight, int iBottom);
    CUIRect(const GdkRectangle& src);
    int GetWidth() const;
    int GetHeight() const;
    void Empty();
    bool IsEmpty();
    bool IsNull() const;
    void Join(const RECT& rc);
    void ResetOffset();
    void Normalize();
    void Offset(int cx, int cy);
    void Inflate(int cx, int cy);
    void Deflate(int cx, int cy);
    void Union(CUIRect& rc);
    bool IsPtIn(POINT pt);
};

class CUISize : public tagSIZE
{
public:
    CUISize();
    CUISize(const SIZE& src);
    CUISize(const RECT rc);
    CUISize(int cx, int cy);
    bool IsNull() const;
};

struct TITEM
{
    CUIString Key;
    LPVOID Data;
    struct TITEM* pPrev;
    struct TITEM* pNext;
};

class CStdStringPtrMap
{
public:
    CStdStringPtrMap(int nSize = 100);
    ~CStdStringPtrMap();

    void Resize(int nSize = 100);
    LPVOID Find(LPCWSTR key, bool optimize = true) const;
    bool Insert(LPCWSTR key, LPVOID pData);
    LPVOID Set(LPCWSTR key, LPVOID pData);
    bool Remove(LPCWSTR key);
    void RemoveAll();
    int GetSize() const;
    LPCWSTR GetAt(int iIndex) const;
    LPCWSTR operator[] (int nIndex) const;

protected:
    TITEM** m_aT;
    int m_nBuckets;
    int m_nCount;
};

class CStdPtrArray
{
public:
    CStdPtrArray(int iPreallocSize = 0);
    CStdPtrArray(const CStdPtrArray& src);
    ~CStdPtrArray();

    void Empty();
    void Resize(int iSize);
    bool IsEmpty() const;
    int Find(LPVOID iIndex) const;
    bool Add(LPVOID pData);
    bool SetAt(int iIndex, LPVOID pData);
    bool InsertAt(int iIndex, LPVOID pData);
    bool Remove(int iIndex);
    int GetSize() const;
    LPVOID* GetData();

    LPVOID GetAt(int iIndex) const;
    LPVOID operator[] (int nIndex) const;

protected:
    LPVOID* m_ppVoid;
    int m_nCount;
    int m_nAllocated;
};
bool ParseRectString(LPCWSTR lpszRect, CUIRect& rc);
bool ParseColorString(LPCWSTR lpszClolor, DWORD &dwColor);
bool ParseBoolString(LPCWSTR lpszBool, bool& bValue);
bool ParseSizeString(LPCWSTR lpszSize, CUISize& Size);
#endif