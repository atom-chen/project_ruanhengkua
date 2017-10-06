/******************************************************************************
* 作者: hezhr
* 时间: 2012-09-06
* 描述: 用于一些通用函数的包装
******************************************************************************/
#include "CommonFunction.h"
#include <ShellAPI.h>
#include <time.h>
#include <stdlib.h>


//--------------------------------------------------------------------------
void execDosCommand(const std::string& cmd)
{
	std::string newcmd = "cmd /c " + cmd;
	::WinExec(newcmd.c_str(),SW_HIDE);
}
//--------------------------------------------------------------------------
void waitExceDosCommand(const std::string& cmd)
{
	std::string newcmd = "cmd.exe /c " + cmd;
	runExe(newcmd, true);
}
//--------------------------------------------------------------------------
void runExe(const std::string& cmd, bool bBlocking)
{
	std::string newcmd = cmd;

	PROCESS_INFORMATION           piProcInfo;
	STARTUPINFO                   siStartInfo;

	ZeroMemory(&piProcInfo, sizeof(piProcInfo));
	ZeroMemory(&siStartInfo, sizeof(siStartInfo));

	siStartInfo.cb = sizeof(STARTUPINFO);  
	siStartInfo.lpReserved = NULL;  
	siStartInfo.lpReserved2 = NULL;  
	siStartInfo.cbReserved2 = 0;  
	siStartInfo.lpDesktop = NULL;  
	siStartInfo.dwFlags	= 0;  

	LPSTR lp_cmd;
#ifdef UNICODE
	lp_cmd = UnicodeToANSI(cmd).c_str();
#else
	lp_cmd = (LPSTR)cmd.c_str();
#endif
	::CreateProcess(NULL, lp_cmd, NULL, NULL, 0, 0, NULL, NULL, &siStartInfo, &piProcInfo);

	if (bBlocking)
	{
		::WaitForSingleObject(piProcInfo.hProcess, INFINITE);   
	}
	else
	{
		::CloseHandle(piProcInfo.hProcess);
	}
}
//--------------------------------------------------------------------------
bool isDigit(char c)
{
	return '0' == c || '1' == c || '2' == c || '3' == c || '4' == c || '5' == c || '6' == c || '7' == c || '8' == c || '9' == c;
}
//--------------------------------------------------------------------------
bool isDigit(const std::string& str)
{
	if (str.empty())
		return false;
	
	bool mask = false;
	int decimal = 0;
	for (size_t i = 0; i<str.length(); ++i)
	{
		// 如果第一个字符为+或-,不判断为非数字,可能是符号标志,继续向下判断
		if (0 == i && ('+' == str[i] || '-' == str[i]))
		{
			mask = true;
			continue;
		}
		if ('.' == str[i])
		{
			++decimal;
			if (decimal > 1)	//如果小数点个数超过一个,则表示不是数字
				return false;
		}
		else if (!isDigit(str[i]))
			return false;
	}
	if (mask && 1 == str.length())
		return false;
	
	return true;
}
//--------------------------------------------------------------------------
int toInt(const std::string& str)
{
	return atoi(str.c_str());
}
//--------------------------------------------------------------------------
long toLong(const std::string& str)
{
	return atol(str.c_str());
}
//--------------------------------------------------------------------------
double toDouble(const std::string& str)
{
	return atof(str.c_str());
}
//--------------------------------------------------------------------------
std::string newLineString(void)
{
	return std::string("\r\n");
}
//--------------------------------------------------------------------------
std::string stringOfChar(size_t n, char ch)
{
	return std::string(n, ch);
}
//--------------------------------------------------------------------------
std::string toString(size_t u)
{
	char buf[32];
	::sprintf_s(buf, sizeof(buf), "%d", u);
	return buf;
}
//--------------------------------------------------------------------------
std::string toString(int n)
{
	char buf[64];
	::_itoa_s(n, buf, 10);
	return buf;
}
//--------------------------------------------------------------------------
std::string toString(long l)
{
	char buf[128];
	::sprintf_s(buf, sizeof(buf), "%l", l);
	return buf;
}
//--------------------------------------------------------------------------
std::string toString(float f)
{
	char buf[128];
	::sprintf_s(buf, sizeof(buf), "%f", f);
	return buf;
}
//--------------------------------------------------------------------------
std::string toString(double d)
{
	char buf[256];
	::sprintf_s(buf, sizeof(buf), "%f", d);
	return buf;
}
//--------------------------------------------------------------------------
std::string intToStringFillZero(int n, size_t width)
{
	std::string str = toString(n);
	if (str.size() >= width)
	{
		return str;
	}

	std::string zero = stringOfChar(width - str.size(), '0');
	if (n < 0)
	{
		str.insert(1, zero);
		return str;
	}

	return zero + str;
}
//--------------------------------------------------------------------------
std::vector<std::string> splitString(std::string str, std::string pattern)
{
	std::vector<std::string> result;

	str += pattern;	//扩展字符串以方便操作
	std::string::size_type pos;

	for (size_t i=0; i<str.size(); ++i)
	{
		pos = str.find(pattern, i);
		if (pos < str.size())
		{
			std::string temp = str.substr(i, pos-i);
			result.push_back(temp);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
//--------------------------------------------------------------------------
std::string replaceString(std::string str, std::string src, std::string dest)
{
	std::string::size_type srclen = src.size();
	std::string::size_type destlen = dest.size();

	std::string::size_type pos = 0;
	while (std::string::npos != (pos = str.find(src, pos)))
	{
		str.replace(pos, srclen, dest);
		pos += destlen;
	}
	return str;
}
//--------------------------------------------------------------------------
std::string formatString(const char* format, ...)
{
	char buf[1024];
	va_list args;

	va_start(args, format);
	vsprintf_s(buf, format, args);
	va_end(args);

	return buf;
}
//--------------------------------------------------------------------------
std::wstring ANSIToUnicode(const char* str)
{
	int textlen = ::MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0); 	
	std::wstring buf(textlen, 0);

	::MultiByteToWideChar(CP_ACP, 0, str, -1, const_cast<LPWSTR>(buf.c_str()) , buf.size()); 

	return buf.c_str(); 
}
//--------------------------------------------------------------------------
std::string UnicodeToANSI(const wchar_t* str)
{
	int textlen = ::WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	std::string buf(textlen, 0);

	::WideCharToMultiByte(CP_ACP, 0, str, -1, const_cast<LPSTR>(buf.c_str()), buf.size(), NULL, NULL);

	return buf.c_str();
}
//--------------------------------------------------------------------------
std::wstring GB2312ToUnicode(const char* str)
{
	int textlen = ::MultiByteToWideChar(936, 0, str, -1, NULL, 0); 	
	std::wstring buf(textlen, 0);

	::MultiByteToWideChar(936, 0, str, -1, const_cast<LPWSTR>(buf.c_str()) , buf.size()); 

	return buf.c_str(); 
}
//--------------------------------------------------------------------------
std::string UnicodeToGB2312(const wchar_t* str)
{
	int textlen = ::WideCharToMultiByte(936, 0, str, -1, NULL, 0, NULL, NULL);
	std::string buf(textlen, 0);

	::WideCharToMultiByte(936, 0, str, -1, const_cast<LPSTR>(buf.c_str()), buf.size(), NULL, NULL);

	return buf.c_str();
}
//--------------------------------------------------------------------------
std::wstring UTF8ToUnicode(const char* str)
{
	int textlen = ::MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0); 	
	std::wstring buf(textlen, 0);

	::MultiByteToWideChar(CP_UTF8, 0, str, -1, const_cast<LPWSTR>(buf.c_str()), buf.size()); 

	return buf.c_str(); 
}
//--------------------------------------------------------------------------
std::string UnicodeToUTF8(const wchar_t* str)
{
	int textlen = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	std::string buf(textlen, 0);

	::WideCharToMultiByte(CP_UTF8, 0, str, -1, const_cast<LPSTR>(buf.c_str()), buf.size(), NULL, NULL);

	return buf.c_str();
}
//--------------------------------------------------------------------------
std::string GB2312ToUTF8(const char* str)
{
	return UnicodeToUTF8(GB2312ToUnicode(str).c_str());
}
//--------------------------------------------------------------------------
std::string UTF8ToGB2312(const char* str)
{
	return UnicodeToGB2312(UTF8ToUnicode(str).c_str());
}
//--------------------------------------------------------------------------
int UTF8Length(const char* str)
{
	return (::MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0) - 1);		// 减1是末尾\0结束符的字节 	
}
//--------------------------------------------------------------------------
std::string getDateTimeString(const ::SYSTEMTIME& st)
{
	const int maxLength = 32;
	char buf[maxLength];
	
	sprintf_s(buf, "%04d%02d%02d%02d%02d%02d%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return buf;
}
//--------------------------------------------------------------------------
std::string getCurDateTimeString(void)
{
	::SYSTEMTIME st;
	::GetLocalTime(&st);
	return getDateTimeString(st);
}
//--------------------------------------------------------------------------
void replaceCharsWithNumber(std::string& str, const std::string& chars, size_t n)
{
	size_t start = str.find_first_of(chars);
	if (start < str.size())
	{
		size_t end = str.find_first_not_of(chars, start + 1);
		if (end > str.size())
		{
			end = str.size();
		}
		size_t count = end - start;
		const std::string num = intToStringFillZero(n, count);
		// 将符号替换成数字
		if (num.size() > count)
		{
			const int oldSize = str.size();
			str.resize(str.size() + num.size() - count);
			std::copy_backward(str.begin() + end, str.begin() + oldSize, str.end());
		}
		std::copy(num.begin(), num.end(), str.begin() + start);
	}
}
//--------------------------------------------------------------------------
std::string formatTime(const std::string& format, size_t h, size_t n, size_t s, size_t ms)
{
	std::string temp = format;
	replaceCharsWithNumber(temp, "Hh", h);
	replaceCharsWithNumber(temp, "Nn", n);
	replaceCharsWithNumber(temp, "Ss", s);
	replaceCharsWithNumber(temp, "Xx", ms);
	return format;
}
//--------------------------------------------------------------------------
std::string formatDateTime(const std::string& format, size_t y, size_t m, size_t d, size_t h, size_t n, size_t s, size_t ms)
{
	std::string temp = format;
	replaceCharsWithNumber(temp, "Yy", y);
	replaceCharsWithNumber(temp, "Mm", m);
	replaceCharsWithNumber(temp, "Dd", d);
	replaceCharsWithNumber(temp, "Hh", h);
	replaceCharsWithNumber(temp, "Nn", n);
	replaceCharsWithNumber(temp, "Ss", s);
	replaceCharsWithNumber(temp, "Xx", ms);
	return temp;
}
//--------------------------------------------------------------------------
std::string formatDateTime(const std::string& format, const ::SYSTEMTIME& st)
{
	return formatDateTime(format, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}
//--------------------------------------------------------------------------
std::string formatDate(const std::string& format, size_t y, size_t m, size_t d)
{
	std::string temp = format;
	replaceCharsWithNumber(temp, "Yy", y);
	replaceCharsWithNumber(temp, "Mm", m);
	replaceCharsWithNumber(temp, "Dd", d);
	return temp;
}
//--------------------------------------------------------------------------
long dateToSeconds(int y, int m, int d, int h, int n, int s)
{
	if (y<1900 || m>12 || m<1 || d>31 || d<1)
	{
		return 0;
	}
	if (h>23 || h<0 || n>59 || n<0 || s>59 || s<0)
	{
		return 0;
	}
	// 日期换算成秒
	tm date;
	date.tm_year = y - 1900;
	date.tm_mon = m - 1;
	date.tm_mday = d;
	date.tm_hour = h;
	date.tm_min = n;
	date.tm_sec = s;
	return (long)::mktime(&date);
}
//--------------------------------------------------------------------------
double timeDiffSeconds(long s1, long s2)
{
	return (double)::difftime(s1, s2);
}
//--------------------------------------------------------------------------
double timeDiffDayDouble(long s1, long s2)
{
	return (double)::difftime(s1, s2)/(24*60*60);
}
//--------------------------------------------------------------------------
int timeDiffDayInt(long s1, long s2)
{
	time_t seconds = (time_t)::difftime(s1, s2);
	int day = (int)seconds/(24*60*60);
	if (0 != day)	// 不是整数天
	{
		if (day >= 0)
			day += 1;
		else
			day -= 1;
	}
	return day;
}
//--------------------------------------------------------------------------
bool existDirectory(const std::string& path)
{
	::WIN32_FIND_DATAA wfd;
    HANDLE hFind = ::FindFirstFileA(path.c_str(), &wfd);
    ::FindClose(hFind);

	return ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}
//--------------------------------------------------------------------------
bool existDir(const std::string& path)
{
	if (path.empty())
		return false;

	char ch = path[path.size() - 1];
	if ('/' == ch || '\\' == ch) 
		return existDirectory(path.substr(0, path.size() - 1));

	return existDirectory(path);
}
//--------------------------------------------------------------------------
bool forceDirectory(const std::string& dir)
{
	return  0 != ::CreateDirectoryA(dir.c_str(), NULL);
}
//--------------------------------------------------------------------------
bool forceDir(const std::string& dir)
{
	if (dir.empty())
		return false;

	for (size_t start = 0; start < dir.size();)
	{
		size_t pos = dir.find_first_of("/\\", start);
		if (pos < dir.size())
		{
			std::string temp = dir.substr(0, pos);
			if (false == existDir(temp))
			{
				if (false == forceDirectory(temp))
					return false;
			}
			start = pos + 1;
		}
		else
		{
			break;
		}
	}

	if (false == existDir(dir))
	{
		return forceDirectory(dir);
	}
	return true;
}
//--------------------------------------------------------------------------
std::string getCurrDir(void)
{
	const int length = 1024;
	char buf[length];
	::GetCurrentDirectoryA(length, buf);
	std::string str = buf;
	str.push_back('\\');
	return str;
}
//--------------------------------------------------------------------------
std::string getDirName(const std::string& dir)
{
	size_t pos = dir.find_last_of("/\\:");
	if (pos < dir.size())
	{
		return dir.substr(0, pos + 1);
	}
	return "";
}
//--------------------------------------------------------------------------
void openDir(const std::string& dir)
{
	std::string open = "open";
	LPSTR lp_open;
	LPSTR lp_dir;
#ifdef UNICODE
	lp_open = UnicodeToANSI(cmd).c_str();
	lp_dir = UnicodeToANSI(dir).c_str();
#else
	lp_open = (LPSTR)open.c_str();
	lp_dir = (LPSTR)dir.c_str();
#endif
	::ShellExecute(NULL, lp_open, lp_dir, NULL, NULL, SW_SHOWNORMAL);
}
//--------------------------------------------------------------------------
std::string getFileName(const std::string& file)
{
	size_t pos = file.find_last_of("/\\:");
	if (pos < file.size())
	{
		return file.c_str() + pos + 1;
	}
	return file;
}
//--------------------------------------------------------------------------
void getFileNameAndExt(const std::string& file, std::string& name, std::string& ext)
{
	ext = "";
	name = getFileName(file);
	size_t pos = name.find_last_of('.');
	if (pos < name.size())
	{
		ext = name.c_str() + pos;
		name.resize(pos);
	}
}
//--------------------------------------------------------------------------
void getDirAndFileName(const std::string& file, std::string& dir, std::string& name)
{
	size_t pos = file.find_last_of("/\\:");
	if (pos < file.size())
	{
		dir = file.substr(0, pos + 1);
		name = file.c_str() + pos + 1;
	}
	else
	{
		dir = "";
		name = file;
	}	
}
//--------------------------------------------------------------------------
std::string getFullPath(const std::string& file)
{	
	char buf[1024];
	::GetFullPathNameA(file.c_str(), 1024, buf, NULL);
	return buf;
}
//--------------------------------------------------------------------------
std::string getModuleName(void)
{
	const int length = 1024;
	char szModuleName[length];
	::GetModuleFileNameA(NULL, szModuleName, length);

	std::string module_name, ext;		
	getFileNameAndExt(szModuleName, module_name, ext);
	return module_name;
}
//--------------------------------------------------------------------------
std::string simpleXOR(const std::string& data)
{
	const std::string key = "X_+#@!GRa^|E%D";
	std::string adjustKey = key;
	if (data.size() < key.size())
	{
		adjustKey = std::string(key.begin(), key.begin() + data.size());
	}
	else
	{
		adjustKey.append(data.size() - adjustKey.size(), ' ');
	}

	std::string result = data;
	for (int i = 0; i < static_cast<int>(data.size()); ++i)
	{
		result[i] = data[i] ^ adjustKey[i];
	}
	return result;
}
//--------------------------------------------------------------------------
size_t ceilUInt(size_t num, size_t divisor)
{
	size_t quotient = num / divisor;
	size_t remain = num % divisor;

	return 0 == remain ? quotient : quotient + 1;
}
//--------------------------------------------------------------------------


