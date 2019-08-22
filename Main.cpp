#include"BasicString.h"
#include<string>
#include<conio.h>

int main()
{
	WSTRING obj(L"Xin chào"), obj1(32, 'c');

	std::string s(32, 'c'), s1("22");
	s = s1;
	s = "2" + s1;

	_getch();
	return 0;
}