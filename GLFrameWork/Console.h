//=================================================================================
//�R���\�[���N���X
//=================================================================================
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

//=================================================================================
//�}�N����`
//=================================================================================
#define LENGTH_STRING_BUFF (1028)	//�o�b�t�@��
class Console
{
public:
	
	static void Sys(char* command);//�V�X�e���R�}���h���g�p
	static void Print(char* fmt,...);//������\��(�����ŏ���ɉ��s�����)
	static void Cls(void);//��ʂ��N���A
	static void SetCursorPos(SHORT x,SHORT y);//�J�[�\�����ړ�

	static void Initialize();//������
	static void Finalize();//�I��

	static void SetActive(bool flag){ _Active = flag; system("cls");}
	static bool Active(void){ return _Active; }
private:
	Console();
	~Console();
	Console(const Console& other){}
	Console &operator=(const Console &other){}
	static Console* Self;
	static COORD Cursor;
	static HANDLE ConsoleHandle;
	static bool _Active;

};


#endif