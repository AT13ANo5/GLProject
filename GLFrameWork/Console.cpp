#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include "Console.h"

COORD Console::Cursor = { 0,0 };
HANDLE Console::ConsoleHandle = nullptr;

Console* Console::Self = nullptr;
bool Console::_Active = true;

Console::Console()
{
	Cursor.X = 0;
	Cursor.Y = 0;
	::AllocConsole();
	ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	system("mode CON: COLS=100 LINES=135");//画面サイズ設定
	freopen("CONIN$","r",stdin);  // 標準入力を割り当てる
	freopen("CONOUT$","w",stdout);  // 標準出力を割り当てる
	SetConsoleTitle("Debug");
	SMALL_RECT rect = {0,0,60,20};
	SetConsoleWindowInfo(ConsoleHandle,TRUE,&rect);
	_Active = true;
}

Console::~Console()
{
	::FreeConsole();
}

void Console::Initialize(void)
{
#ifdef _DEBUG
	Self = new Console;
#endif
}
void Console::Finalize(void)
{
	if (Self != nullptr)
	{
		delete Self;
	}
}
void Console::Sys(char* command)
{
#ifdef _DEBUG
	if (Self)
	{
		if (Cursor.Y >= 135)
		{
			Cls();
		}
		system(command);
		Cursor.Y++;
		SetConsoleCursorPosition(ConsoleHandle,Cursor);
	}
#endif
}

void Console::Print(char* fmt,...)
{
#ifdef _DEBUG
	if (Self && _Active)
	{
		char Buff[LENGTH_STRING_BUFF] = { 0 };
		va_list argp;
		va_start(argp,fmt);
		vsprintf_s(Buff,LENGTH_STRING_BUFF,fmt,argp);
		va_end(argp);

		if (Cursor.Y >= 135)
		{
			Cls();
		}
		int cnt = 0,num = 0;
		while (1)
		{
			if (Buff[cnt] == '\n')
			{
				Cursor.Y++;
				num++;
			}
			if (Buff[cnt] == '\0' || cnt >= LENGTH_STRING_BUFF)
			{
				break;
			}
			cnt++;
		}
		if (num == 0){ Cursor.Y++; }
		printf(Buff);
		SetConsoleCursorPosition(ConsoleHandle,Cursor);
	}
#endif
}

void Console::Cls(void)
{
#ifdef _DEBUG
	if (Self && _Active)
	{
		Cursor.X = 1;
		Cursor.Y = 1;
		system("cls");
	}
#endif
}

void Console::SetCursorPos(SHORT x,SHORT y)
{
#ifdef _DEBUG
	if (Self && _Active)
	{
		Cursor.X = x;
		Cursor.Y = y;
		SetConsoleCursorPosition(ConsoleHandle,Cursor);
	}
#endif
}