#include "stdafx.h"
#include "UIString.h"

CUIString::CUIString() : m_pstr(m_szBuffer)
{
    m_szBuffer[0] = L'\0';
}

CUIString::CUIString(const WCHAR ch) : m_pstr(m_szBuffer)
{
    m_szBuffer[0] = ch;
    m_szBuffer[1] = L'\0';
}

CUIString::CUIString(LPCWSTR lpsz, int nLen) : m_pstr(m_szBuffer)
{
    m_szBuffer[0] = L'\0';
    Assign(lpsz, nLen);
}

CUIString::CUIString(const CUIString& src) : m_pstr(m_szBuffer)
{
    m_szBuffer[0] = L'\0';
    Assign(src.m_pstr);
}

CUIString::~CUIString()
{
    if(m_pstr != m_szBuffer) 
        free(m_pstr);
}

int CUIString::GetLength() const
{ 
    return (int)wcslen(m_pstr); 
}

CUIString::operator LPWSTR() const 
{ 
    return m_pstr; 
}

void CUIString::Append(LPCWSTR pstr)
{
    int nNewLength = GetLength() + (int)wcslen(pstr);
    if(nNewLength >= MAX_LOCAL_STRING_LEN) {
        if( m_pstr == m_szBuffer ) {
            m_pstr = static_cast<LPWSTR>(malloc((nNewLength + 1) * sizeof(WCHAR)));
            wcscpy(m_pstr, m_szBuffer);
            wcscat(m_pstr, pstr);
        }else{
            m_pstr = static_cast<LPWSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(WCHAR)));
            wcscat(m_pstr, pstr);
        }
    }else{
        if(m_pstr != m_szBuffer){
            free(m_pstr);
            m_pstr = m_szBuffer;
        }
        wcscat(m_szBuffer, pstr);
    }
}

void CUIString::Assign(LPCWSTR pstr, int cchMax)
{
    if(pstr == NULL) 
        pstr = L"";
    cchMax = (cchMax < 0 ? (int) wcslen(pstr) : cchMax);
    if(cchMax < MAX_LOCAL_STRING_LEN) {
        if(m_pstr != m_szBuffer){
            free(m_pstr);
            m_pstr = m_szBuffer;
        }
    }else if(cchMax > GetLength() || m_pstr == m_szBuffer) {
        if(m_pstr == m_szBuffer) 
            m_pstr = NULL;
        m_pstr = static_cast<LPWSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(WCHAR)));
    }
    wcsncpy(m_pstr, pstr, cchMax);
    m_pstr[cchMax] = L'\0';
}

bool CUIString::IsEmpty() const 
{ 
    return m_pstr[0] == L'\0'; 
}

void CUIString::Empty() 
{ 
    if(m_pstr != m_szBuffer) 
        free(m_pstr);
    
    //
    // rest buffer to cache.
    //
    
    m_pstr = m_szBuffer;
    m_szBuffer[0] = L'\0'; 
}

LPCWSTR CUIString::GetData() const
{
    return m_pstr;
}

WCHAR CUIString::GetAt(int nIndex) const
{
    return m_pstr[nIndex];
}

WCHAR CUIString::operator[] (int nIndex) const
{ 
    return m_pstr[nIndex];
}   

const CUIString& CUIString::operator=(const CUIString& src)
{      
    Assign(src);
    return *this;
}

const CUIString& CUIString::operator=(LPCWSTR lpStr)
{      
    if (lpStr){
        Assign(lpStr);
    }else{
        Empty();
    }
    return *this;
}

const CUIString& CUIString::operator=(const WCHAR ch)
{
    Empty();
    m_szBuffer[0] = ch;
    m_szBuffer[1] = '\0';
    return *this;
}

CUIString CUIString::operator+(const CUIString& src) const
{
    CUIString sTemp = *this;
    sTemp.Append(src);
    return sTemp;
}

CUIString CUIString::operator+(LPCWSTR lpStr) const
{
    if (lpStr){
        CUIString sTemp = *this;
        sTemp.Append(lpStr);
        return sTemp;
    }
    return *this;
}

const CUIString& CUIString::operator+=(const CUIString& src)
{      
    Append(src);
    return *this;
}

const CUIString& CUIString::operator+=(LPCWSTR lpStr)
{      
    if (lpStr){
        Append(lpStr);
    }

    return *this;
}

const CUIString& CUIString::operator+=(const WCHAR ch)
{      
    WCHAR str[] = {ch, L'\0'};
    Append(str);
    return *this;
}

bool CUIString::operator == (const CUIString& str) const 
{ 
    return (Compare(str.GetData()) == 0); 
}

bool CUIString::operator == (LPCWSTR str) const 
{ 
    return (Compare(str) == 0); 
}

bool CUIString::operator != (const CUIString& str) const 
{ 
    return (Compare(str.GetData()) != 0); 
}

bool CUIString::operator != (LPCWSTR str) const 
{ 
    return (Compare(str) != 0); 
}

bool CUIString::operator <= (LPCWSTR str) const 
{ 
    return (Compare(str) <= 0); 
}

bool CUIString::operator <  (LPCWSTR str) const 
{ 
    return (Compare(str) <  0); 
}

bool CUIString::operator >= (LPCWSTR str) const 
{ 
    return (Compare(str) >= 0); 
}

bool CUIString::operator >  (LPCWSTR str) const 
{ 
    return (Compare(str) >  0); 
}

void CUIString::SetAt(int nIndex, WCHAR ch)
{
    m_pstr[nIndex] = ch;
}

int CUIString::Compare(LPCWSTR lpsz) const 
{ 
    return wcscmp(m_pstr, lpsz); 
}

int CUIString::CompareNoCase(LPCWSTR lpsz) const
{
#ifndef WIN32
    return wcscasecmp(m_pstr, lpsz);
#else
    return wcsicmp(m_pstr, lpsz);
#endif // WIN32

}

void CUIString::MakeUpper()
{
#ifdef WIN32
    wcsupr(m_pstr);
#else
    WCHAR *pstr = m_pstr;
    while(*pstr){
        *pstr = (WCHAR)toupper(*pstr);
        pstr ++;
    }
#endif // WIN32
}

void CUIString::MakeLower()
{
#ifdef WIN32
    wcslwr(m_pstr);
#else
    WCHAR *pstr = m_pstr;
    while(*pstr){
        *pstr = (WCHAR)tolower(*pstr);
        pstr ++;
    }
#endif // WIN32
}

CUIString CUIString::Left(int nLength) const
{
    if(nLength < 0) 
        nLength = 0;

    if(nLength > GetLength()) 
        nLength = GetLength();

    return CUIString(m_pstr, nLength);
}

CUIString CUIString::Mid(int nPos, int nLength) const
{
    if(nLength < 0) 
        nLength = GetLength() - nPos;
    if(nPos + nLength > GetLength()) 
        nLength = GetLength() - nPos;
    if(nLength <= 0) 
        return CUIString();
    return CUIString(m_pstr + nPos, nLength);
}

CUIString CUIString::Right(int nLength) const
{
    int nPos = GetLength() - nLength;
    if(nPos < 0) {
        nPos = 0;
        nLength = GetLength();
    }
    return CUIString(m_pstr + nPos, nLength);
}

int CUIString::Find(WCHAR ch, int nPos /*= 0*/) const
{
    if(nPos != 0 && (nPos < 0 || nPos >= GetLength())) 
        return -1;
    LPCWSTR p = wcschr(m_pstr + nPos, ch);
    if(p == NULL) 
        return -1;
    return (int)(p - m_pstr);
}

int CUIString::Find(LPCWSTR pstrSub, int nPos) const
{
    if(nPos != 0 && (nPos < 0 || nPos > GetLength())) 
        return -1;
    LPCWSTR p = wcsstr(m_pstr + nPos, pstrSub);
    if(p == NULL) 
        return -1;
    return (int)(p - m_pstr);
}

int CUIString::ReverseFind(WCHAR ch) const
{
    
    //
    // just use wcsrchr
    //
    
    LPCWSTR p = wcsrchr(m_pstr, ch);
    if(p == NULL) 
        return -1;
    return (int)(p - m_pstr);
}

LONG CUIString::toLong(int Radix)
{
    WCHAR *pStrEnd;
    return wcstol(m_pstr, &pStrEnd, Radix);
}

ULONG CUIString::toULong(int Radix)
{
    WCHAR *pStrEnd;
    return wcstoul(m_pstr, &pStrEnd, Radix);
}

int CUIString::Replace(LPCWSTR pstrFrom, LPCWSTR pstrTo)
{
    CUIString sTemp;
    int nCount = 0;
    
    //
    // search the string we want to replace
    //
    
    int nPos = Find(pstrFrom);
    if(nPos < 0)
        return 0;
    
    //
    // here the string exist
    //
    
    int cchFrom = (int)wcslen(pstrFrom);
    int cchTo = (int)wcslen(pstrTo);
    while(nPos >= 0) {
        
        //
        // save the left string at position we searched
        //
        
        sTemp = Left(nPos);
        
        //
        // append the replaced string.
        //
        
        sTemp += pstrTo;
        
        //
        // append the last string
        //
        
        sTemp += Mid(nPos + cchFrom);
        
        //
        // assign buffer to this class
        //
        
        Assign(sTemp);
        
        //
        // Find again until not found
        //
        
        nPos = Find(pstrFrom, nPos + cchTo);
        nCount++;
    }
    
    //
    // return how many places we replaced
    //
    
    return nCount;
}

int CUIString::Format(LPCWSTR lpFormat, ...)
{
    int nRet;
    va_list Args;
    
    //
    // just call FormatV
    //
    
    va_start(Args, lpFormat);
    nRet = FormatV(lpFormat, Args);
    va_end(Args);
    return nRet;
}

#define STRING_INIT_SIZE 512

int CUIString::FormatV(LPCWSTR lpFormat, va_list Args)
{
    int nSize = STRING_INIT_SIZE;
    WCHAR* szBuffer;
    int nRet;
    
    //
    // initialize string buffer.
    //
    
    szBuffer = (WCHAR*)malloc(nSize * sizeof(WCHAR));
    if (!szBuffer){
        return 0;
    }

    memset(szBuffer, 0, nSize * sizeof(WCHAR));

    while (TRUE){

        //
        // call the vsnprintf to format the buffer.
        //
#ifdef WIN32
    nRet = _vsnwprintf(szBuffer, nSize, lpFormat, Args);
#else
    nRet = vswprintf(szBuffer, nSize, lpFormat, Args);
#endif // WIN32
        if (nRet != -1 && nRet < nSize){

            //
            // work done.
            //

            break;
        }

        //
        // there buffer too small, we must expand it.
        //

        if (nRet == -1){

            //
            // expend buffer size
            //

            nSize *= 2;
        }else{

            //
            // the buffer size equal format size. not include the terminating null.
            // so we must add a null terminating to the buffer
            //

            nSize += sizeof(WCHAR);
        }

        //
        // reallocate the buffer to hold full string
        //

        if ((szBuffer = (WCHAR*)realloc(szBuffer, nSize * sizeof(WCHAR))) != NULL){
            memset(szBuffer, 0, nSize * sizeof(WCHAR));
        }else{

            //
            // no more memory
            //

            nRet = 0;
            break;
        }
    }

    //
    // every thing is ok. check it and assign buffer to string.
    //

    Assign(szBuffer);
    free(szBuffer);
    return nRet;
}