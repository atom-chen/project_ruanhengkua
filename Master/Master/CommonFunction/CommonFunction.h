/******************************************************************************
* 作者: hezhr
* 时间: 2012-09-06
* 描述: 用于一些通用函数的包装
******************************************************************************/
#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <Windows.h>
#include <string>
#include <vector>



//----------------------------------------------------------------------
// Win32 API 包装
//----------------------------------------------------------------------

/*
功	能：执行Dos的命令，注意这开了一个新进程
参	数：cmd - 比如copy指令.
返回值：void
*/
extern void execDosCommand(const std::string& cmd);

/*
功	能：等待执行Dos的命令，此函数慎用，这个函数会等Dos命令的执行进程结束后才继续，注意这是一个进程的等待
参	数：cmd - 指令
返回值：void
*/
extern void waitExceDosCommand(const std::string& cmd);

/*
功	能：执行exe
参	数：cmd - 指令；bBlocking - 为true时会等待exe执行完毕才返回
返回值：void
*/
extern void runExe(const std::string& cmd, bool bBlocking);



//----------------------------------------------------------------------
// 字串相关函数
//----------------------------------------------------------------------

/*
功	能：判断字符是否为数字
参	c - 字符
返回值：bool
*/
extern bool isDigit(char c);

/*
功	能：判断字符串是否为数字(整数或小数)
参	数：str - 字符串
返回值：bool
*/
extern bool isDigit(const std::string& str);

/*
功	能：字符串转为int
参	数：str - 字符串
返回值：int
*/
extern int toInt(const std::string& str);

/*
功	能：字符串转为long
参	数：str - 字符串
返回值：int
*/
extern long toLong(const std::string& str);

/*
功	能：字符串转为double
参	数：str - 字符串
返回值：int
*/
extern double toDouble(const std::string& str);

/*
功	能：取得换行符
参	数：void
返回值：string
*/
extern std::string newLineString(void);

/*
功	能：返回n个ch字符组成的字串，例，StringOfChar(5, '0')结果为字串"00000"
参	数：n - 数量；ch - 字符
返回值：string
*/
extern std::string stringOfChar(size_t n, char ch);

/*
功	能：转为字符串
参	数：u - 短整数
返回值：string
*/
extern std::string toString(size_t u);

/*
功	能：转为字符串
参	数：n - 整数
返回值：string
*/
extern std::string toString(int n);

/*
功	能：转为字符串
参	数：l - 长整数
返回值：string
*/
extern std::string toString(long l);

/*
功	能：转为字符串
参	数：f - 单精度
返回值：string
*/
extern std::string toString(float f);

/*
功	能：转为字符串
参	数：d - 双精度
返回值：string
*/
extern std::string toString(double d);

/*
功	能：整数转字串并在前面补0
参	数：n - 整数；width - 宽度
返回值：string
*/
extern std::string intToStringFillZero(int n, size_t width);

/*
功	能：分割字符串
参	数：str - 字符串；pattern - 分割符号
返回值：vector
*/
extern std::vector<std::string> splitString(std::string str, std::string pattern);

/*
功	能：替换字符串
参	数：str - 字符串；src - 要被替换的字符串；dest - 替换的字符串
返回值：vector
*/
extern std::string replaceString(std::string str, std::string src, std::string dest);

/*
功	能：格式化字串
参	数：str - 格式；... - 可变参数
返回值：string
*/
extern std::string formatString(const char* format, ...);

/*
功	能：ANSI转成Unicode
参	数：str - 字符串
返回值：wstring
*/
extern std::wstring ANSIToUnicode(const char* str);

/*
功	能：Unicode转成ANSI
参	数：str - 字符串
返回值：wstring
*/
extern std::string UnicodeToANSI(const wchar_t* str);

/*
功	能：GBK转成Unicode
参	数：str - 字符串
返回值：wstring
*/
extern std::wstring GB2312ToUnicode(const char* str);

/*
功	能：Unicode转成GBK
参	数：str - 字符串
返回值：string
*/
extern std::string UnicodeToGB2312(const wchar_t* str);

/*
功	能：UTF8转成Unicode
参	数：str - 字符串
返回值：wstring
*/
extern std::wstring UTF8ToUnicode(const char* str);

/*
功	能：Unicode转成UTF8
参	数：str - 字符串
返回值：string
*/
extern std::string UnicodeToUTF8(const wchar_t* str);

/*
功	能：GBK转成UTF8
参	数：str - 字符串
返回值：string
*/
extern std::string GB2312ToUTF8(const char* str);

/*
功	能：UTF8转成GBK
参	数：str - 字符串
返回值：string
*/
extern std::string UTF8ToGB2312(const char* str);

/*
功	能：UTF8长度
参	数：str - 字符串
返回值：int
*/
extern int UTF8Length(const char* str);



//----------------------------------------------------------------------
// 时间相关函数		
//----------------------------------------------------------------------

/*
功	能：取得日期时间字串(yyyymmddhhnnssxxx) 
参	数：st - 结构体
返回值：string
*/
extern std::string getDateTimeString(const ::_SYSTEMTIME& st);
 
/*
功	能：取得当前日期时间字串(yyyymmddhhnnssxxx)
参	数：void
返回值：string
*/
extern std::string getCurDateTimeString(void);

/*
功	能：替换字符串中的字符为数字
参	数：str - 字符串；chars - 要替换的字符；n - 数字
返回值：void
*/
extern void replaceCharsWithNumber(std::string& str, const std::string& chars, size_t n);

/*
功	能：格式化时间，例，formatTime("hh时nn分ss秒xxx毫秒", 5, 6, 7, 8)，运行结果为 "05时06分07秒008毫秒"
参	数：format - 时间格式字串；h - 小时，对应的格式符为'h'；n - 分钟，对应的格式符为'n'；ms - 毫秒，对应的格式符为'x'
返回值：string
*/
extern std::string formatTime(const std::string& format, size_t h, size_t n, size_t s, size_t ms);

/*
功	能：格式化日期时间
参	数：format - 时间格式字串；y - 年，对应的格式符为'y'；m - 月，对应的格式符为'm'；d - 日，对应的格式符为'd'；
		h - 小时，对应的格式符为'h'；n - 分钟，对应的格式符为'n'；ms - 毫秒，对应的格式符为'x'
返回值：string
*/
extern std::string formatDateTime(const std::string& format, size_t y, size_t m, size_t d, size_t h, size_t n, size_t s, size_t ms);

/*
功	能：格式化日期时间
参	数：format - 时间格式字串；st - 日期结果
返回值：string
*/
extern std::string formatDateTime(const std::string& sFormat, const ::SYSTEMTIME& st);

/*
功	能：格式化日期
参	数：format - 时间格式字串；y - 年，对应的格式符为'y'；m - 月，对应的格式符为'm'；d - 日，对应的格式符为'd'
返回值：string
*/
extern std::string formatDate(const std::string& sFormat, size_t y, size_t m, size_t d);

/*
功	能：把日期（年月日时分秒）换算为（秒）
参	数：y,m,d,h,n,s(年，月，日，时，分，秒)
返回值：秒数
*/
extern long dateToSeconds(int y, int m, int d, int h, int n, int s);
/*
功	能：获取两个时间的时间差，返回（秒）
参	数：s1（第一个时间），s2（第二个时间），单位都为（秒）
返回值：double类型，<0（s1比s2早），=0（s1等于s2），>0（s1比s2晚）
*/
extern double timeDiffSeconds(long s1, long s2);
/*
功	能：获取两个时间的时间差，返回（天）
参	数：s1（第一个时间），s2（第二个时间），单位都为（秒）
返回值：double类型（可以到零点几天），<0（s1比s2早），=0（s1等于s2），>0（s1比s2晚）
*/
extern double timeDiffDayDouble(long s1, long s2);
/*
功	能：获取两个时间的时间差，返回（天）
参	数：s1（第一个时间），s2（第二个时间），单位都为（秒）
返回值：int类型（只能到整数天），<0（s1比s2早），=0（s1等于s2），>0（s1比s2晚）
*/
extern int timeDiffDayInt(long s1, long s2);



//----------------------------------------------------------------------
// 文件目录相关函数
//----------------------------------------------------------------------

/*
功	能：判断目录是否存在，一般调用existDir
参	数：path - 路径（完整路径）
返回值：bool
*/
extern bool existDirectory(const std::string& path);

/*
功	能：判断目录是否存在
参	数：path - 路径（完整路径）
返回值：bool
*/
extern bool existDir(const std::string& path);

/*
功	能：建立目录，一般调用forceDir
参	数：dir - 目录
返回值：bool
*/
extern bool forceDirectory(const std::string& dir);

/*
功	能：建立目录
参	数：dir - 目录
返回值：bool
*/
extern bool forceDir(const std::string& dir);

/*
功	能：取得当前目录,返回目录名会自动加上'\'号
参	数：void
返回值：string
*/
extern std::string getCurrDir(void);

/*
功	能：取得目录名
参	数：dir - 完整目录
返回值：string
*/
extern std::string getDirName(const std::string& dir);

/*
功	能：打开目录
参	数：dir - 目录
返回值：void
*/
extern void openDir(const std::string& dir);

/*
功	能：提取文件名，文件名包含扩展名如(myfile.bmp)
参	数：file - 完整文件名（包括路径）
返回值：string
*/
extern std::string getFileName(const std::string& file);

/*
功	能：提取文件名及扩展名
参	数：file - 完整文件名（包括路径）；name - 文件名不包含扩展名；ext - 包含点号如".exe"
返回值：void
*/
extern void getFileNameAndExt(const std::string& file, std::string& name, std::string& ext);

/*
功	能：提取目录名，文件名，文件名包含扩展名如(myfile.bmp),返回目录名会自动加上'\'号
参	数：file - 完整文件名（包括路径）；dir - 路径；name - 文件名不包含扩展名；
返回值：void
*/
extern void getDirAndFileName(const std::string& file, std::string& dir, std::string& name);

/*
功	能：提取完整路径，主要是替换掉"..\", ".\"之类的符号
参	数：file - 文件名
返回值：string
*/
extern std::string getFullPath(const std::string& file);

/*
功	能：取得程序名称，例：假设当前应用程序名称为D:\\myPath\\myApp.exe, 那结果为"myApp"
参	数：void
返回值：strig
*/
extern std::string getModuleName(void);



//----------------------------------------------------------------------
// 其它函数
//----------------------------------------------------------------------

/*
功	能：取得静态数组元素个数
参	数：arr - 数组
返回值：size_t
*/
template <typename T, typename TArray>
size_t getArraySize(const TArray& arr)
{
	return sizeof(arr) / sizeof(T);
}

/*
功	能：简单加密
参	数：data - 数据
返回值：string
*/
extern std::string simpleXOR(const std::string& data);

/*
功	能：num除以divisor，对商进一法取整
参	数：num - 被除数；divisor - 除数
返回值：size_t
*/
extern size_t ceilUInt(size_t num, size_t divisor);



#endif	// _COMMON_FUNCTION_H_


