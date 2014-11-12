#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>
#include "Console.h"

COORD Console::Cursor = { 0,0 };
HANDLE Console::ConsoleHandle = nullptr;

Console* Console::Self = nullptr;

Console::Console()
{
	Cursor.X = 1;
	Cursor.Y = 1;
	::AllocConsole();
	ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	system("mode CON: COLS=100 LINES=135");//��ʃT�C�Y�ݒ�
	freopen("CONIN$","r",stdin);  // �W�����͂����蓖�Ă�
	freopen("CONOUT$","w",stdout);  // �W���o�͂����蓖�Ă�
	SetConsoleTitle("Debug");
	SMALL_RECT rect = {0,0,60,20};
	SetConsoleWindowInfo(ConsoleHandle,TRUE,&rect);

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
	if (Self)
	{
		char Buff[LENGTH_STRING_BUFF] = { 0 };
		va_list argp;
		va_start(argp,fmt);
		vsprintf_s(Buff,256,fmt,argp);
		va_end(argp);

		if (Cursor.Y >= 135)
		{
			Cls();
		}

		printf(Buff);
		Cursor.Y++;
		SetConsoleCursorPosition(ConsoleHandle,Cursor);
	}
#endif
}

void Console::Cls(void)
{
#ifdef _DEBUG
	if (Self)
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
	if (Self)
	{
		Cursor.X = x;
		Cursor.Y = y;
		SetConsoleCursorPosition(ConsoleHandle,Cursor);
	}
#endif
}