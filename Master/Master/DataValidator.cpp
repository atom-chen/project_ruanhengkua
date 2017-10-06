#include "StdAfx.h"
#include "DataValidator.h"


CDataValidator::CDataValidator(void)
{
}


CDataValidator::~CDataValidator(void)
{
}

bool CDataValidator::IsInteger(const _TCHAR *psz)
{
    if(IsSign(*psz))
        ++psz;

    if(!IsDigit(*psz))
        return false;

    for(;;)
    {
        if(IsEnd(*psz))
            return true;

        if(IsDigit(*psz))
        {
            ++psz;
            continue;
        } // if

        return false;
    } // for

    return false;
}

bool CDataValidator::IsFloat(const _TCHAR *psz)
{
    enum State { m1, m2, m3, m4, e1, e2, e3 } state = m1;
    while(true)
    {
        _TCHAR tc = *(psz++);
        switch(state)
        {
        case m1:
            if(IsSign(tc))
                state = m2;
            else if(IsDigit(tc)) 
                state = m3;
            else 
                return false;
            break;
        case m2:
            if(IsDigit(tc))
                state = m3;
            else 
                return false;
            break;
        case m3:
            if(IsDigit(tc))
                state = m3;
            else if(IsDot(tc))
                state = m4;
            else if(IsEnd(tc))
                return true;
            else 
                return false;
            break;
        case m4:
            if(IsDigit(tc))
                state = m4;
            else if(IsEnd(tc))
                return true;
            else 
                return false;
            break;
        default:
            //assert(false);
            break;
        } // switch
    } // while
}

//判断是否是float,且第一个字符不能使'+'/'-'
bool CDataValidator::IsUnsignFloat(const _TCHAR *psz)
{
    _TCHAR tc = *psz;
    if(IsSign(tc))
    {
        return false;
    }
    return IsFloat(psz);
}