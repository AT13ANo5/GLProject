#ifndef _CONSOLE_H_
#define _CONSOLE_H_


#define LENGTH_STRING_BUFF (1028)	//バッファ量
class Console
{
public:
	
	static void Sys(char* command);
	static void Print(char* fmt,...);
	static void Cls(void);
	static void SetCursorPos(SHORT x,SHORT y);

	static void Initialize();
	static void Finalize();
private:
	Console();
	~Console();
	Console(const Console& other){}
	Console &operator=(const Console &other){}
	static Console* Self;
	static COORD Cursor;
	static HANDLE ConsoleHandle;
	

};


#endif