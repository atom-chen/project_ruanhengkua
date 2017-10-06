/******************************************************************************
* ����: hezhr
* ʱ��: 2012-09-06
* ����: ����һЩͨ�ú����İ�װ
******************************************************************************/
#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <Windows.h>
#include <string>
#include <vector>



//----------------------------------------------------------------------
// Win32 API ��װ
//----------------------------------------------------------------------

/*
��	�ܣ�ִ��Dos�����ע���⿪��һ���½���
��	����cmd - ����copyָ��.
����ֵ��void
*/
extern void execDosCommand(const std::string& cmd);

/*
��	�ܣ��ȴ�ִ��Dos������˺������ã�����������Dos�����ִ�н��̽�����ż�����ע������һ�����̵ĵȴ�
��	����cmd - ָ��
����ֵ��void
*/
extern void waitExceDosCommand(const std::string& cmd);

/*
��	�ܣ�ִ��exe
��	����cmd - ָ�bBlocking - Ϊtrueʱ��ȴ�exeִ����ϲŷ���
����ֵ��void
*/
extern void runExe(const std::string& cmd, bool bBlocking);



//----------------------------------------------------------------------
// �ִ���غ���
//----------------------------------------------------------------------

/*
��	�ܣ��ж��ַ��Ƿ�Ϊ����
��	c - �ַ�
����ֵ��bool
*/
extern bool isDigit(char c);

/*
��	�ܣ��ж��ַ����Ƿ�Ϊ����(������С��)
��	����str - �ַ���
����ֵ��bool
*/
extern bool isDigit(const std::string& str);

/*
��	�ܣ��ַ���תΪint
��	����str - �ַ���
����ֵ��int
*/
extern int toInt(const std::string& str);

/*
��	�ܣ��ַ���תΪlong
��	����str - �ַ���
����ֵ��int
*/
extern long toLong(const std::string& str);

/*
��	�ܣ��ַ���תΪdouble
��	����str - �ַ���
����ֵ��int
*/
extern double toDouble(const std::string& str);

/*
��	�ܣ�ȡ�û��з�
��	����void
����ֵ��string
*/
extern std::string newLineString(void);

/*
��	�ܣ�����n��ch�ַ���ɵ��ִ�������StringOfChar(5, '0')���Ϊ�ִ�"00000"
��	����n - ������ch - �ַ�
����ֵ��string
*/
extern std::string stringOfChar(size_t n, char ch);

/*
��	�ܣ�תΪ�ַ���
��	����u - ������
����ֵ��string
*/
extern std::string toString(size_t u);

/*
��	�ܣ�תΪ�ַ���
��	����n - ����
����ֵ��string
*/
extern std::string toString(int n);

/*
��	�ܣ�תΪ�ַ���
��	����l - ������
����ֵ��string
*/
extern std::string toString(long l);

/*
��	�ܣ�תΪ�ַ���
��	����f - ������
����ֵ��string
*/
extern std::string toString(float f);

/*
��	�ܣ�תΪ�ַ���
��	����d - ˫����
����ֵ��string
*/
extern std::string toString(double d);

/*
��	�ܣ�����ת�ִ�����ǰ�油0
��	����n - ������width - ���
����ֵ��string
*/
extern std::string intToStringFillZero(int n, size_t width);

/*
��	�ܣ��ָ��ַ���
��	����str - �ַ�����pattern - �ָ����
����ֵ��vector
*/
extern std::vector<std::string> splitString(std::string str, std::string pattern);

/*
��	�ܣ��滻�ַ���
��	����str - �ַ�����src - Ҫ���滻���ַ�����dest - �滻���ַ���
����ֵ��vector
*/
extern std::string replaceString(std::string str, std::string src, std::string dest);

/*
��	�ܣ���ʽ���ִ�
��	����str - ��ʽ��... - �ɱ����
����ֵ��string
*/
extern std::string formatString(const char* format, ...);

/*
��	�ܣ�ANSIת��Unicode
��	����str - �ַ���
����ֵ��wstring
*/
extern std::wstring ANSIToUnicode(const char* str);

/*
��	�ܣ�Unicodeת��ANSI
��	����str - �ַ���
����ֵ��wstring
*/
extern std::string UnicodeToANSI(const wchar_t* str);

/*
��	�ܣ�GBKת��Unicode
��	����str - �ַ���
����ֵ��wstring
*/
extern std::wstring GB2312ToUnicode(const char* str);

/*
��	�ܣ�Unicodeת��GBK
��	����str - �ַ���
����ֵ��string
*/
extern std::string UnicodeToGB2312(const wchar_t* str);

/*
��	�ܣ�UTF8ת��Unicode
��	����str - �ַ���
����ֵ��wstring
*/
extern std::wstring UTF8ToUnicode(const char* str);

/*
��	�ܣ�Unicodeת��UTF8
��	����str - �ַ���
����ֵ��string
*/
extern std::string UnicodeToUTF8(const wchar_t* str);

/*
��	�ܣ�GBKת��UTF8
��	����str - �ַ���
����ֵ��string
*/
extern std::string GB2312ToUTF8(const char* str);

/*
��	�ܣ�UTF8ת��GBK
��	����str - �ַ���
����ֵ��string
*/
extern std::string UTF8ToGB2312(const char* str);

/*
��	�ܣ�UTF8����
��	����str - �ַ���
����ֵ��int
*/
extern int UTF8Length(const char* str);



//----------------------------------------------------------------------
// ʱ����غ���		
//----------------------------------------------------------------------

/*
��	�ܣ�ȡ������ʱ���ִ�(yyyymmddhhnnssxxx) 
��	����st - �ṹ��
����ֵ��string
*/
extern std::string getDateTimeString(const ::_SYSTEMTIME& st);
 
/*
��	�ܣ�ȡ�õ�ǰ����ʱ���ִ�(yyyymmddhhnnssxxx)
��	����void
����ֵ��string
*/
extern std::string getCurDateTimeString(void);

/*
��	�ܣ��滻�ַ����е��ַ�Ϊ����
��	����str - �ַ�����chars - Ҫ�滻���ַ���n - ����
����ֵ��void
*/
extern void replaceCharsWithNumber(std::string& str, const std::string& chars, size_t n);

/*
��	�ܣ���ʽ��ʱ�䣬����formatTime("hhʱnn��ss��xxx����", 5, 6, 7, 8)�����н��Ϊ "05ʱ06��07��008����"
��	����format - ʱ���ʽ�ִ���h - Сʱ����Ӧ�ĸ�ʽ��Ϊ'h'��n - ���ӣ���Ӧ�ĸ�ʽ��Ϊ'n'��ms - ���룬��Ӧ�ĸ�ʽ��Ϊ'x'
����ֵ��string
*/
extern std::string formatTime(const std::string& format, size_t h, size_t n, size_t s, size_t ms);

/*
��	�ܣ���ʽ������ʱ��
��	����format - ʱ���ʽ�ִ���y - �꣬��Ӧ�ĸ�ʽ��Ϊ'y'��m - �£���Ӧ�ĸ�ʽ��Ϊ'm'��d - �գ���Ӧ�ĸ�ʽ��Ϊ'd'��
		h - Сʱ����Ӧ�ĸ�ʽ��Ϊ'h'��n - ���ӣ���Ӧ�ĸ�ʽ��Ϊ'n'��ms - ���룬��Ӧ�ĸ�ʽ��Ϊ'x'
����ֵ��string
*/
extern std::string formatDateTime(const std::string& format, size_t y, size_t m, size_t d, size_t h, size_t n, size_t s, size_t ms);

/*
��	�ܣ���ʽ������ʱ��
��	����format - ʱ���ʽ�ִ���st - ���ڽ��
����ֵ��string
*/
extern std::string formatDateTime(const std::string& sFormat, const ::SYSTEMTIME& st);

/*
��	�ܣ���ʽ������
��	����format - ʱ���ʽ�ִ���y - �꣬��Ӧ�ĸ�ʽ��Ϊ'y'��m - �£���Ӧ�ĸ�ʽ��Ϊ'm'��d - �գ���Ӧ�ĸ�ʽ��Ϊ'd'
����ֵ��string
*/
extern std::string formatDate(const std::string& sFormat, size_t y, size_t m, size_t d);

/*
��	�ܣ������ڣ�������ʱ���룩����Ϊ���룩
��	����y,m,d,h,n,s(�꣬�£��գ�ʱ���֣���)
����ֵ������
*/
extern long dateToSeconds(int y, int m, int d, int h, int n, int s);
/*
��	�ܣ���ȡ����ʱ���ʱ�����أ��룩
��	����s1����һ��ʱ�䣩��s2���ڶ���ʱ�䣩����λ��Ϊ���룩
����ֵ��double���ͣ�<0��s1��s2�磩��=0��s1����s2����>0��s1��s2��
*/
extern double timeDiffSeconds(long s1, long s2);
/*
��	�ܣ���ȡ����ʱ���ʱ�����أ��죩
��	����s1����һ��ʱ�䣩��s2���ڶ���ʱ�䣩����λ��Ϊ���룩
����ֵ��double���ͣ����Ե���㼸�죩��<0��s1��s2�磩��=0��s1����s2����>0��s1��s2��
*/
extern double timeDiffDayDouble(long s1, long s2);
/*
��	�ܣ���ȡ����ʱ���ʱ�����أ��죩
��	����s1����һ��ʱ�䣩��s2���ڶ���ʱ�䣩����λ��Ϊ���룩
����ֵ��int���ͣ�ֻ�ܵ������죩��<0��s1��s2�磩��=0��s1����s2����>0��s1��s2��
*/
extern int timeDiffDayInt(long s1, long s2);



//----------------------------------------------------------------------
// �ļ�Ŀ¼��غ���
//----------------------------------------------------------------------

/*
��	�ܣ��ж�Ŀ¼�Ƿ���ڣ�һ�����existDir
��	����path - ·��������·����
����ֵ��bool
*/
extern bool existDirectory(const std::string& path);

/*
��	�ܣ��ж�Ŀ¼�Ƿ����
��	����path - ·��������·����
����ֵ��bool
*/
extern bool existDir(const std::string& path);

/*
��	�ܣ�����Ŀ¼��һ�����forceDir
��	����dir - Ŀ¼
����ֵ��bool
*/
extern bool forceDirectory(const std::string& dir);

/*
��	�ܣ�����Ŀ¼
��	����dir - Ŀ¼
����ֵ��bool
*/
extern bool forceDir(const std::string& dir);

/*
��	�ܣ�ȡ�õ�ǰĿ¼,����Ŀ¼�����Զ�����'\'��
��	����void
����ֵ��string
*/
extern std::string getCurrDir(void);

/*
��	�ܣ�ȡ��Ŀ¼��
��	����dir - ����Ŀ¼
����ֵ��string
*/
extern std::string getDirName(const std::string& dir);

/*
��	�ܣ���Ŀ¼
��	����dir - Ŀ¼
����ֵ��void
*/
extern void openDir(const std::string& dir);

/*
��	�ܣ���ȡ�ļ������ļ���������չ����(myfile.bmp)
��	����file - �����ļ���������·����
����ֵ��string
*/
extern std::string getFileName(const std::string& file);

/*
��	�ܣ���ȡ�ļ�������չ��
��	����file - �����ļ���������·������name - �ļ�����������չ����ext - ���������".exe"
����ֵ��void
*/
extern void getFileNameAndExt(const std::string& file, std::string& name, std::string& ext);

/*
��	�ܣ���ȡĿ¼�����ļ������ļ���������չ����(myfile.bmp),����Ŀ¼�����Զ�����'\'��
��	����file - �����ļ���������·������dir - ·����name - �ļ�����������չ����
����ֵ��void
*/
extern void getDirAndFileName(const std::string& file, std::string& dir, std::string& name);

/*
��	�ܣ���ȡ����·������Ҫ���滻��"..\", ".\"֮��ķ���
��	����file - �ļ���
����ֵ��string
*/
extern std::string getFullPath(const std::string& file);

/*
��	�ܣ�ȡ�ó������ƣ��������赱ǰӦ�ó�������ΪD:\\myPath\\myApp.exe, �ǽ��Ϊ"myApp"
��	����void
����ֵ��strig
*/
extern std::string getModuleName(void);



//----------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------

/*
��	�ܣ�ȡ�þ�̬����Ԫ�ظ���
��	����arr - ����
����ֵ��size_t
*/
template <typename T, typename TArray>
size_t getArraySize(const TArray& arr)
{
	return sizeof(arr) / sizeof(T);
}

/*
��	�ܣ��򵥼���
��	����data - ����
����ֵ��string
*/
extern std::string simpleXOR(const std::string& data);

/*
��	�ܣ�num����divisor�����̽�һ��ȡ��
��	����num - ��������divisor - ����
����ֵ��size_t
*/
extern size_t ceilUInt(size_t num, size_t divisor);



#endif	// _COMMON_FUNCTION_H_


