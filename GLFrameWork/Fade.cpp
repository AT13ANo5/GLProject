#include "Fade.h"

CFade* CFade::Self = nullptr;

void CFade::Initialize(void)
{
	Self = new CFade;
	Self->Vtx[0] = VECTOR3(SCREEN_WIDTH,0,0);
	Self->Vtx[1] = VECTOR3(0,0,0);
	Self->Vtx[2] = VECTOR3(0,SCREEN_HEIGHT,0);
	Self->Vtx[3] = VECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0);
	Self->_Color = BLACK(1.0f);
	Self->_State = FADE_NONE;
}

void CFade::Finalize(void)
{
	delete Self;
}

bool CFade::Set(float destAlpha,int frame,bool infinit)
{
	if (Self->_State == FADE_NONE || Self->_Infinit == false)
	{
		Self->DestAlpha = destAlpha;
		Self->SubAlpha = destAlpha - Self->_Color.a;
		(Self->SubAlpha < 0)? Self->_State = FADE_IN : Self->_State = FADE_OUT;
		Self->_Infinit = infinit;

		return true;
	}
	
	return false;
}

void CFade::Update(void)
{
	switch(_State)
	{
	case FADE_IN:
		if (_Color.a > DestAlpha)
		{
			_Color.a += SubAlpha;
		}
		else
		{
			_State = FADE_NONE;
			_Color.a = DestAlpha;
			SubAlpha = 0;
			_Infinit = false;
		}
		break;
	case FADE_OUT:
		if (_Color.a < DestAlpha)
		{
			_Color.a += SubAlpha;
		}
		else
		{
			_State = FADE_NONE;
			_Color.a = DestAlpha;
			SubAlpha = 0;
			_Infinit = false;
		}
		break;
	default:
		break;
	}

}

void CFade::Draw(void)
{
	//ライティング無効
	glDisable(GL_LIGHTING);

	//2D用マトリクス設定

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();//プロジェクションマトリックスを退避
	glLoadIdentity();
	glOrtho(0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);
	UV uv(0,0,1.0f,1.0f);
	
	for (int cnt = 0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D,0);

	//ビュー行列を戻す
	glPopMatrix();
	//プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}