#include "lib_import.h"

inline
string a_string_trim(string str)
{
	int pos = 0;
	while(pos < str.size() && (str[pos] == ' ' || str[pos] == '\t'))
		pos++;

	int cnt = str.size() - 1;
	while(cnt >= 0 && (str[cnt] == ' ' || str[cnt] == '\t'))
		cnt--;


	if (pos > cnt)
		return "";

	return str.substr(pos,cnt-pos+1);
}

inline
string a_string_to_upper(const string& src)
{
	string re = src;
	for(int cnt=0;cnt<re.size();cnt++)
	{
		re[cnt] = toupper(re[cnt]);
	}

	return re;
}

inline
string a_string_to_lower(const string& src)
{
	string re = src;
	for(int cnt=0;cnt<re.size();cnt++)
	{
		re[cnt] = tolower(re[cnt]);
	}

	return re;
}

template<typename T>
string get_str_from_type(const T a)
{
	string re;
	stringstream ss;
	ss<<a;
	ss>>re;

	return re;
}

template<typename T>
T get_type_from_string(const string a)
{
	T re;
	stringstream ss;
	ss<<a;
	ss>>re;

	return re;
}

inline
string operator+(const string & src,int a)
{
	return src + get_str_from_type(a);
}

template<typename T>
string operator*(const string& src,T a)
{
	return src + get_str_from_type(a);
}

template<typename T>
string operator-(const string& src,T a)
{
	return src + get_str_from_type(a);
}

inline
wstring a_trans_wchar_to_wstr(wchar_t wc)
{
	wchar_t ar[2];
	ar[0] = wc;
	ar[1] = 0;

	return wstring(ar);
}

template<typename T>
string operator*(const char*  src,T a)
{
	return string(src) + get_str_from_type(a);
}

template<typename T>
string operator-(const char* src,T a)
{
	return string(src) + get_str_from_type(a);
}

inline
wstring a_wstring_trim(wstring str)
{
	int pos = 0;
	while(pos < str.size() && (str[pos] == ' ' || str[pos] == '\t'))
		pos++;

	int cnt = str.size() - 1;
	while(cnt >= 0 && (str[cnt] == ' ' || str[cnt] == '\t'))
		cnt--;


	if (pos > cnt)
		return (L"");

	return str.substr(pos,cnt-pos+1);
}

inline
wstring a_wstring_to_upper(const wstring& src)
{
	wstring re = src;
	for(int cnt=0;cnt<re.size();cnt++)
	{
		re[cnt] = toupper(re[cnt]);
	}

	return re;
}

inline
wstring a_wstring_to_lower(const wstring& src)
{
	wstring re = src;
	for(int cnt=0;cnt<re.size();cnt++)
	{
		re[cnt] = tolower(re[cnt]);
	}

	return re;
}

template<typename T>
wstring get_wstr_from_type(const T a)
{
	wstring re;
	wstringstream ss;
	ss<<a;
	ss>>re;

	return re;
}

template<typename T>
T get_type_from_wstring(const wstring a)
{
	T re;
	wstringstream ss;
	ss<<a;
	ss>>re;

	return re;
}

inline
wstring operator+(const wstring & src,int a)
{
	return src + get_wstr_from_type(a);
}

template<typename T>
wstring operator*(const wstring& src,T a)
{
	return src + get_wstr_from_type(a);
}

template<typename T>
wstring operator-(const wstring& src,T a)
{
	return src + get_str_from_type(a);
}


template<typename T>
wstring operator*(const char*  src,T a)
{
	return wstring(src) + get_str_from_type(a);
}

template<typename T>
wstring operator-(const char* src,T a)
{
	return wstring(src) + get_str_from_type(a);
}

inline
string wstring_to_string(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs"); 
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

inline
wstring string_to_wstring(const string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");

	return result;
}

inline 
wstring a_get_random_wstring(int cnt)
{
	wstring str;
	for(int i=0;i<cnt;i++)
	{
		str += ('a' + rand()%26);
	}

	return str;
}

inline 
string a_get_random_string(int cnt)
{
	string str;
	for(int i=0;i<cnt;i++)
	{
		str += ('a' + rand()%26);
	}

	return str;
}