
#include "stdafx.h"
#include "UIUtils.h"

#ifndef WIN32
BOOL SetRectEmpty(
    LPRECT prc)
{
    if (prc == NULL) return FALSE;
    memset(prc, 0, sizeof(RECT));
    return TRUE;
}

BOOL UnionRect(
    LPRECT prcDst,
    CONST RECT *prcSrc1,
    CONST RECT *prcSrc2)
{
    BOOL frc1Empty, frc2Empty;

    if ((prcDst == NULL) || (prcSrc1 == NULL) || (prcSrc2 == NULL)) return FALSE;

    frc1Empty = ((prcSrc1->left >= prcSrc1->right) ||
            (prcSrc1->top >= prcSrc1->bottom));

    frc2Empty = ((prcSrc2->left >= prcSrc2->right) ||
            (prcSrc2->top >= prcSrc2->bottom));

    if (frc1Empty && frc2Empty) {
        SetRectEmpty(prcDst);
        return FALSE;
    }

    if (frc1Empty) {
        *prcDst = *prcSrc2;
        return TRUE;
    }

    if (frc2Empty) {
        *prcDst = *prcSrc1;
        return TRUE;
    }

    /*
     * form the union of the two non-empty rects
     */
    prcDst->left   = MIN(prcSrc1->left,   prcSrc2->left);
    prcDst->top    = MIN(prcSrc1->top,    prcSrc2->top);
    prcDst->right  = MAX(prcSrc1->right,  prcSrc2->right);
    prcDst->bottom = MAX(prcSrc1->bottom, prcSrc2->bottom);

    return TRUE;
}

#endif

BOOL UIIntersectRect(
    LPRECT prcDst,
    CONST RECT *prcSrc1,
    CONST RECT *prcSrc2)

{
#ifndef WIN32
        if ((prcDst == NULL) || (prcSrc1 == NULL) || (prcSrc2 == NULL))
        return FALSE;

    prcDst->left  = MAX(prcSrc1->left, prcSrc2->left);
    prcDst->right = MIN(prcSrc1->right, prcSrc2->right);

    /*
     * check for empty rect
     */
    if (prcDst->left < prcDst->right) {

        prcDst->top = MAX(prcSrc1->top, prcSrc2->top);
        prcDst->bottom = MIN(prcSrc1->bottom, prcSrc2->bottom);

        /*
         * check for empty rect
         */
        if (prcDst->top < prcDst->bottom) {
            return TRUE;        // not empty
        }
    }

    /*
     * empty rect
     */
    SetRectEmpty(prcDst);

    return FALSE;
#else 
    return IntersectRect(prcDst, prcSrc1, prcSrc2);
#endif
}

ULONG UIGetTickCount()
{
#ifndef WIN32
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else
    return GetTickCount();
#endif
    
}

CUIRect::CUIRect()
{
    left = top = right = bottom = 0;
}

CUIRect::CUIRect(const RECT& src)
{
    left = src.left;
    top = src.top;
    right = src.right;
    bottom = src.bottom;
}

CUIRect::CUIRect(const GdkRectangle& src)
{
    left = src.x;
    top = src.y;
    right = src.x + src.width;
    bottom = src.y + src.height;
}

CUIRect::CUIRect(int iLeft, int iTop, int iRight, int iBottom)
{
    left = iLeft;
    top = iTop;
    right = iRight;
    bottom = iBottom;
}

int CUIRect::GetWidth() const
{
    return right - left;
}

int CUIRect::GetHeight() const
{
    return bottom - top;
}

void CUIRect::Empty()
{
    left = top = right = bottom = 0;
}

bool CUIRect::IsEmpty()
{
    return ((left >= right) || (top >= bottom));
}

bool CUIRect::IsNull() const
{
    return (left == 0 && right == 0 && top == 0 && bottom == 0); 
}

void CUIRect::Join(const RECT& rc)
{
    if( rc.left < left ) left = rc.left;
    if( rc.top < top ) top = rc.top;
    if( rc.right > right ) right = rc.right;
    if( rc.bottom > bottom ) bottom = rc.bottom;
}

void CUIRect::ResetOffset()
{
    Offset(-left, -top);
}

void CUIRect::Normalize()
{
    if(left > right) { int iTemp = left; left = right; right = iTemp; }
    if(top > bottom) { int iTemp = top; top = bottom; bottom = iTemp; }
}

void CUIRect::Offset(int cx, int cy)
{
    left   += cx;
    right  += cx;
    bottom += cy;
    top    += cy;
}

void CUIRect::Inflate(int cx, int cy)
{
    left   -= cx;
    right  += cx;
    top    -= cy;
    bottom += cy;
}

void CUIRect::Deflate(int cx, int cy)
{
    Inflate(-cx, -cy);
}

void CUIRect::Union(CUIRect& rc)
{
    UnionRect(this, this, &rc);
}

bool CUIRect::IsPtIn(POINT pt)
{
    return ((pt.x >= left) && (pt.x < right) &&
            (pt.y >= top)  && (pt.y < bottom));
}

CUISize::CUISize()
{
    cx = cy = 0;
}

CUISize::CUISize(const SIZE& src)
{
    cx = src.cx;
    cy = src.cy;
}

CUISize::CUISize(const RECT rc)
{
    cx = rc.right - rc.left;
    cy = rc.bottom - rc.top;
}

CUISize::CUISize(int _cx, int _cy)
{
    cx = _cx;
    cy = _cy;
}

bool CUISize::IsNull() const
{
    return (cx == 0 && cy == 0); 
}

static UINT HashKey(LPCWSTR Key)
{
    UINT i = 0;
    int len = wcslen(Key);
    while(len-- > 0)
        i = (i << 5) + i + Key[len];
    return i;
}

static UINT HashKey(const CUIString& Key)
{
    return HashKey((LPCWSTR)Key);
};

CStdStringPtrMap::CStdStringPtrMap(int nSize) : m_nCount(0)
{
    if(nSize < 16) 
        nSize = 16;
    m_nBuckets = nSize;
    m_aT = new TITEM*[nSize];
    memset(m_aT, 0, nSize * sizeof(TITEM*));
}

CStdStringPtrMap::~CStdStringPtrMap()
{
    if(m_aT) {
        int len = m_nBuckets;
        while(len--) {
            TITEM* pItem = m_aT[len];
            while( pItem ) {
                TITEM* pKill = pItem;
                pItem = pItem->pNext;
                delete pKill;
            }
        }
        delete [] m_aT;
        m_aT = NULL;
    }
}

void CStdStringPtrMap::RemoveAll()
{
    this->Resize(m_nBuckets);
}

void CStdStringPtrMap::Resize(int nSize)
{
    if(m_aT){
        int len = m_nBuckets;
        while(len--){
            TITEM* pItem = m_aT[len];
            while( pItem ) {
                TITEM* pKill = pItem;
                pItem = pItem->pNext;
                delete pKill;
            }
        }
        delete [] m_aT;
        m_aT = NULL;
    }

    if(nSize < 0) 
        nSize = 0;
    if(nSize > 0) {
        m_aT = new TITEM*[nSize];
        memset(m_aT, 0, nSize * sizeof(TITEM*));
    } 
    m_nBuckets = nSize;
    m_nCount = 0;
}

LPVOID CStdStringPtrMap::Find(LPCWSTR key, bool optimize) const
{
    if(m_nBuckets == 0 || GetSize() == 0) 
        return NULL;

    UINT slot = HashKey(key) % m_nBuckets;
    for(TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext) {
        if( pItem->Key == key ) {
            if (optimize && pItem != m_aT[slot]) {
                if (pItem->pNext) {
                    pItem->pNext->pPrev = pItem->pPrev;
                }
                pItem->pPrev->pNext = pItem->pNext;
                pItem->pPrev = NULL;
                pItem->pNext = m_aT[slot];
                pItem->pNext->pPrev = pItem;
                m_aT[slot] = pItem;
            }
            return pItem->Data;
        }
    }

    return NULL;
}

bool CStdStringPtrMap::Insert(LPCWSTR key, LPVOID pData)
{
    if(m_nBuckets == 0 ) 
        return false;
    if(Find(key)) 
        return false;

    UINT slot = HashKey(key) % m_nBuckets;
    TITEM* pItem = new TITEM;
    pItem->Key = key;
    pItem->Data = pData;
    pItem->pPrev = NULL;
    pItem->pNext = m_aT[slot];
    if (pItem->pNext)
        pItem->pNext->pPrev = pItem;
    m_aT[slot] = pItem;
    m_nCount++;
    return true;
}

LPVOID CStdStringPtrMap::Set(LPCWSTR key, LPVOID pData)
{
    if(m_nBuckets == 0) 
        return pData;

    if (GetSize()>0) {
        UINT slot = HashKey(key) % m_nBuckets;
        for( TITEM* pItem = m_aT[slot]; pItem; pItem = pItem->pNext ) {
            if( pItem->Key == key ) {
                LPVOID pOldData = pItem->Data;
                pItem->Data = pData;
                return pOldData;
            }
        }
    }

    Insert(key, pData);
    return NULL;
}

bool CStdStringPtrMap::Remove(LPCWSTR key)
{
    if(m_nBuckets == 0 || GetSize() == 0) 
        return false;

    UINT slot = HashKey(key) % m_nBuckets;
    TITEM** ppItem = &m_aT[slot];
    while(*ppItem){
        if((*ppItem)->Key == key){
            TITEM* pKill = *ppItem;
            *ppItem = (*ppItem)->pNext;
            if (*ppItem)
                (*ppItem)->pPrev = pKill->pPrev;
            delete pKill;
            m_nCount--;
            return true;
        }
        ppItem = &((*ppItem)->pNext);
    }

    return false;
}

int CStdStringPtrMap::GetSize() const
{
    return m_nCount;
}

LPCWSTR CStdStringPtrMap::GetAt(int iIndex) const
{
    if(m_nBuckets == 0 || GetSize() == 0)
        return NULL;

    int pos = 0;
    int len = m_nBuckets;
    while(len--){
        for(TITEM* pItem = m_aT[len]; pItem; pItem = pItem->pNext){
            if(pos++ == iIndex) {
                return pItem->Key.GetData();
            }
        }
    }

    return NULL;
}

LPCWSTR CStdStringPtrMap::operator[] (int nIndex) const
{
    return GetAt(nIndex);
}

CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
    //ASSERT(iPreallocSize>=0);
    if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
}

CStdPtrArray::CStdPtrArray(const CStdPtrArray& src) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(0)
{
    for(int i=0; i<src.GetSize(); i++)
        Add(src.GetAt(i));
}

CStdPtrArray::~CStdPtrArray()
{
    if( m_ppVoid != NULL ) free(m_ppVoid);
}

void CStdPtrArray::Empty()
{
    if( m_ppVoid != NULL ) free(m_ppVoid);
    m_ppVoid = NULL;
    m_nCount = m_nAllocated = 0;
}

void CStdPtrArray::Resize(int iSize)
{
    Empty();
    m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
    ::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
    m_nAllocated = iSize;
    m_nCount = iSize;
}

bool CStdPtrArray::IsEmpty() const
{
    return m_nCount == 0;
}

bool CStdPtrArray::Add(LPVOID pData)
{
    if( ++m_nCount >= m_nAllocated) {
        int nAllocated = m_nAllocated * 2;
        if( nAllocated == 0 ) nAllocated = 11;
        LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
        if( ppVoid != NULL ) {
            m_nAllocated = nAllocated;
            m_ppVoid = ppVoid;
        }
        else {
            --m_nCount;
            return false;
        }
    }
    m_ppVoid[m_nCount - 1] = pData;
    return true;
}

bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
{
    if( iIndex == m_nCount ) return Add(pData);
    if( iIndex < 0 || iIndex > m_nCount ) return false;
    if( ++m_nCount >= m_nAllocated) {
        int nAllocated = m_nAllocated * 2;
        if( nAllocated == 0 ) nAllocated = 11;
        LPVOID* ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, nAllocated * sizeof(LPVOID)));
        if( ppVoid != NULL ) {
            m_nAllocated = nAllocated;
            m_ppVoid = ppVoid;
        }
        else {
            --m_nCount;
            return false;
        }
    }
    memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
    m_ppVoid[iIndex] = pData;
    return true;
}

bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
{
    if( iIndex < 0 || iIndex >= m_nCount ) return false;
    m_ppVoid[iIndex] = pData;
    return true;
}

bool CStdPtrArray::Remove(int iIndex)
{
    if( iIndex < 0 || iIndex >= m_nCount )
        return false;

    if( iIndex < --m_nCount )
        memcpy(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
    return true;
}

int CStdPtrArray::Find(LPVOID pData) const
{
    for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
    return -1;
}

int CStdPtrArray::GetSize() const
{
    return m_nCount;
}

LPVOID* CStdPtrArray::GetData()
{
    return m_ppVoid;
}

LPVOID CStdPtrArray::GetAt(int iIndex) const
{
    if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
    return m_ppVoid[iIndex];
}

LPVOID CStdPtrArray::operator[] (int iIndex) const
{
    //ASSERT(iIndex>=0 && iIndex<m_nCount);
    return m_ppVoid[iIndex];
}

bool ParseRectString(LPCWSTR lpszRect, CUIRect& rc)
{
    LPWSTR pstr = NULL;
    
    //
    // left
    //
    
    rc.left   = wcstol(lpszRect, &pstr, 10);
    if (pstr == NULL && *pstr != L','){
        return false;
    }
    
    //
    // top
    //
    
    rc.top    = wcstol(pstr + 1, &pstr, 10);
    if (pstr == NULL && *pstr != L','){
        return false;
    }
    
    //
    // right
    //
    
    rc.right  = wcstol(pstr + 1, &pstr, 10);
    if (pstr == NULL && *pstr != L','){
        return false;
    }

    rc.bottom = wcstol(pstr + 1, &pstr, 10);
    if (pstr == NULL){
        return false;
    }

    return true;
}

bool ParseColorString(LPCWSTR lpszClolor, DWORD &dwColor)
{
    LPWSTR pstr = NULL;
    if (lpszClolor[0] == L'#'){
        lpszClolor ++;
        dwColor = wcstoul(lpszClolor, &pstr, 16);
        return true;
    }
    return false;
}

bool ParseBoolString(LPCWSTR lpszBool, bool& bValue)
{
    LPWSTR pstr = NULL;
    bValue = (wcscmp(lpszBool, L"true") == 0);
    return true;
}

bool ParseSizeString(LPCWSTR lpszSize, CUISize& Size)
{
    LPWSTR pstr = NULL;
    
    //
    // parse x
    //

    Size.cx = wcstol(lpszSize, &pstr, 10);
    if (pstr == NULL && *pstr != L','){
        return false;
    }

    //
    // parse y
    //

    Size.cy = wcstol(pstr + 1, &pstr, 10);
    if (pstr == NULL){
        return false;
    }

    return true;
}