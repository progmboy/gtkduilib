
#ifndef __INDCLUDE_UISTRING__H__
#define __INDCLUDE_UISTRING__H__

#include <glib.h>

#define MAX_LOCAL_STRING_LEN 63

class CUIString
{
public:
    CUIString();
    CUIString(const WCHAR ch);
    CUIString(const CUIString& src);
    CUIString(LPCWSTR lpsz, int nLen = -1);
    ~CUIString();

    void Empty();
    int GetLength() const;
    bool IsEmpty() const;
    WCHAR GetAt(int nIndex) const;
    void Append(LPCWSTR pstr);
    void Assign(LPCWSTR pstr, int nLength = -1);
    LPCWSTR GetData() const;

    void SetAt(int nIndex, WCHAR ch);
    operator LPWSTR() const;

    WCHAR operator[] (int nIndex) const;
    const CUIString& operator=(const CUIString& src);
    const CUIString& operator=(const WCHAR ch);
    const CUIString& operator=(LPCWSTR pstr);
    CUIString operator+(const CUIString& src) const;
    CUIString operator+(LPCWSTR pstr) const;
    const CUIString& operator+=(const CUIString& src);
    const CUIString& operator+=(LPCWSTR pstr);
    const CUIString& operator+=(const WCHAR ch);

    bool operator == (const CUIString& str) const;
    bool operator == (LPCWSTR str) const;
    bool operator != (const CUIString& str) const;
    bool operator != (LPCWSTR str) const;
    bool operator <= (LPCWSTR str) const;
    bool operator <  (LPCWSTR str) const;
    bool operator >= (LPCWSTR str) const;
    bool operator >  (LPCWSTR str) const;

    int Compare(LPCWSTR pstr) const;
    int CompareNoCase(LPCWSTR pstr) const;

    void MakeUpper();
    void MakeLower();

    CUIString Left(int nLength) const;
    CUIString Mid(int iPos, int nLength = -1) const;
    CUIString Right(int nLength) const;

    int Find(WCHAR ch, int iPos = 0) const;
    int Find(LPCWSTR pstr, int iPos = 0) const;
    int ReverseFind(WCHAR ch) const;
    int Replace(LPCWSTR pstrFrom, LPCWSTR pstrTo);

    int __cdecl Format(LPCWSTR pstrFormat, ...);
    int __cdecl FormatV(LPCWSTR lpFormat, va_list Args);
    LONG toLong(int Radix = 10);
    ULONG toULong(int Radix = 10);
protected:
    LPWSTR m_pstr;
    WCHAR m_szBuffer[MAX_LOCAL_STRING_LEN + 1];
};

class CU82W
{
public:
    CU82W(IN LPCSTR psz) : m_psz(NULL)
    {
        Init(psz);
    }

    ~CU82W()
    {
        if (m_psz){
            g_free(m_psz);
        }
    }

    operator LPWSTR() const
    {
        return (m_psz);
    }

private:
    void Init(IN LPCSTR psz)
    {
        GError* Error;

        if (psz == NULL){
            m_psz = NULL;
            return;
        }

        if(sizeof(wchar_t) == 4){
            m_psz = (LPWSTR)g_utf8_to_ucs4(psz, -1, NULL, NULL, &Error);
        }else if(sizeof(wchar_t) == 2){
            m_psz = (LPWSTR)g_utf8_to_utf16(psz, -1, NULL, NULL, &Error);
        }


        if (m_psz == NULL){
            throw Error->code;
        }
    }

public:
    LPWSTR m_psz;
};

class CW2U8
{
public:
    CW2U8(IN LPCWSTR psz) : m_psz(NULL)
    {
        Init(psz);
    }

    ~CW2U8() throw()
    {

        //
        // free the buffer.
        //

        if (m_psz){
            g_free(m_psz);
        }
    }

    operator LPSTR() const
    {
        return(m_psz);
    }

private:
    void Init(IN LPCWSTR psz)
    {
        GError* Error;
        if (psz == NULL){
            m_psz = NULL;
            return;
        }

        if(sizeof(wchar_t) == 4){
            m_psz = (LPSTR)g_ucs4_to_utf8 ((const gunichar *)psz, -1, NULL, NULL, &Error);
        }else if(sizeof(wchar_t) == 2){
            m_psz = (LPSTR)g_utf16_to_utf8((const gunichar2 *)psz, -1, NULL, NULL, &Error);
        }

        if (m_psz == NULL){
            throw Error->code;
        }
    }

private:
    LPSTR m_psz;
};

#endif
