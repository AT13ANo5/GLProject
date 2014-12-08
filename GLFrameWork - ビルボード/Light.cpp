#include "Light.h"
#include "main.h"
CLight::CLight()
{
	Pos = VECTOR4(0,0,0,0);
	Diffuse = COLOR(1.0f,1.0f,1.0f,0.0f);
	Ambient = COLOR(0.3f,0.3f,0.3f,0.3f);
	Specular= COLOR(1.0f,1.0f,1.0f,0.0f);
}

void CLight::Create(VECTOR4 Vec,COLOR Dif,COLOR Amb,COLOR Speq)
{
	Pos = Vec;
	Diffuse = Dif;
	Ambient = Amb;
	Specular = Speq;

	glLightfv(GL_LIGHT0,GL_POSITION,(float*)&Pos);
	glLightfv(GL_LIGHT0,GL_AMBIENT,(float*)&Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,(float*)&Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,(float*)&Specular);
	glEnable(GL_LIGHT0);
	
}

void CLight::Update(void)
{
	glLightfv(GL_LIGHT0,GL_POSITION,(float*)&Pos);
	glLightfv(GL_LIGHT0,GL_AMBIENT,(float*)&Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,(float*)&Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,(float*)&Specular);
}
