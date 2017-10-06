#pragma once
class CDataValidator
{
public:
    CDataValidator(void);
    ~CDataValidator(void);

    static bool IsSign(_TCHAR tc)
    {
        return((_T('-') == tc) || (_T('+') == tc));
    }

    //
    // Returns true if tc is digit
    static bool IsDigit(_TCHAR tc)
    {
        return (0 != _istdigit(tc));
    }

    //
    // Returns true if tc is a 0 character
    static bool IsEnd(_TCHAR tc)
    {
        return(_T('\0') == tc);
    }

    //
    // Returns true if tc is a dot
    static bool IsDot(_TCHAR tc)
    {
        return (_T('.') == tc);
    }

public:
    // Returns true if psz is an Integer
    static bool IsInteger(const _TCHAR *psz);
    static bool IsFloat(const _TCHAR *psz);
    static bool IsUnsignFloat(const _TCHAR *psz);
};

