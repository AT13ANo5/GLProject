//=================================================================================
//コンソールクラス
//=================================================================================
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

//=================================================================================
//マクロ定義
//=================================================================================
#define LENGTH_STRING_BUFF (1028)	//バッファ量
class Console
{
public:
	
	static void Sys(char* command);//システムコマンドを使用
	static void Print(char* fmt,...);//文字を表示(文末で勝手に改行される)
	static void Cls(void);//画面をクリア
	static void SetCursorPos(SHORT x,SHORT y);//カーソルを移動

	static void Initialize();//初期化
	static void Finalize();//終了
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