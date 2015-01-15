#include "Confetti.h"

CConfetti* CConfetti::Create(const VECTOR3& pos)
{
	CConfetti* effect = new CConfetti;
	if (effect == nullptr){return nullptr;}

	effect->_Pos = pos;
	effect->SetSize(VECTOR2(10.0f,10.0f));
	effect->Init();

	return effect;
}

CConfetti* CConfetti::Create(void)
{
	CConfetti* effect = new CConfetti;
	if (effect == nullptr){ return nullptr; }

	VECTOR3 pos = VECTOR3(0,0,400.0f);
	pos.x = rand() % 301 - 150.0f;
	pos.y = 300.0f;
	pos.z = rand() % 131 - 150.0f;

	effect->_Pos = pos;
	effect->SetSize(VECTOR2(10.0f,10.0f));
	effect->Init();

	return effect;
}

void CConfetti::Init(void)
{
	switch (rand()%7)
	{
	case 0:
		_Color = WHITE(1.0f);
		break;
	case 1:
		_Color = RED(1.0f);
		break;
	case 2:
		_Color = BLUE(1.0f);
		break;
	case 3:
		_Color = GREEN(1.0f);
		break;
	case 4:
		_Color = CYAN(1.0f);
		break;
	case 5:
		_Color = PURPLE(1.0f);
		break;
	case 6:
		_Color = YELLOW(1.0f);
		break;
	default:
		break;
	}

	Speed.x = rand()%5-2.0f;
	Speed.y = -(rand()%10)+5.0f;
	Speed.z = (rand()%3-1.0f)*(10.0f/300.0f);

	SpinSpeed.x += rand() % 11 - 5;
	SpinSpeed.y += rand() % 11 - 5;
	SpinSpeed.z += rand() % 11 - 5;

	frame = 0;
	_Rot.x = 90.0f;
	//Texture = CTexture::Texture(TEX_LIGHT);

	CPolygon3D::Init();
}

void CConfetti::Update(void)
{
	_Pos += Speed;

	_Rot += SpinSpeed;

	frame++;

	if (frame >= 300)
	{
		Release();
	}
}

void CConfetti::Draw(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	//描画設定
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glRotatef(_Rot.y,0,1.0f,0);
	glRotatef(_Rot.x,1.0f,0,0);
	glScalef(1.0f,1.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	for (int cnt = 0;cnt<4;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[cnt].x,Nor[cnt].y,Nor[cnt].z);
		glTexCoord2f(uv.tex[cnt].x,uv.tex[cnt].y);
		glVertex3f(Vtx[cnt].x,Vtx[cnt].y,Vtx[cnt].z);
	}

	glEnd();

	glPopMatrix();//ビューマトリックスを戻す
	glBindTexture(GL_TEXTURE_2D,0);

	glEnable(GL_CULL_FACE);//カリングする
	glEnable(GL_LIGHTING);
}