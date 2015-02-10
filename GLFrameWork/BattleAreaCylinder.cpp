#include <math.h>
#include "BattleAreaCylinder.h"
#include "Texture.h"
#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)
CBattleAreaCylinder::CBattleAreaCylinder(int priority) :CMeshCylinder(priority)
{
}

CBattleAreaCylinder::~CBattleAreaCylinder()
{
}

CBattleAreaCylinder* CBattleAreaCylinder::Create(VECTOR3 pos,float PanelHeight,VECTOR2 PanelNum,float radius,VECTOR2 TexDivide)
{
	CBattleAreaCylinder* Field = new CBattleAreaCylinder;
	if(Field == nullptr)
	{
		return nullptr;
	}
	Field->_Pos = pos;
	Field->PanelHeight = PanelHeight;
	Field->PanelNum = PanelNum;
	Field->Radius = radius;
	Field->TexDiv = TexDivide;
	Field->Init();

	return Field;
}

void CBattleAreaCylinder::Init(void)
{
	IndexNum = (int)(SUM_INDEX(PanelNum.x, PanelNum.y));
	VertexNum = (int)((PanelNum.x + 1)*(PanelNum.y + 1));
	PolygonNum = (int)(((PanelNum.x * 2)*PanelNum.y) + ((PanelNum.y - 1) * 4));

	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];
	Color = new COLOR[VertexNum];

	float angle = (360.0f / PanelNum.x)*(PI / 180.0f);

	for (int LoopZ = 0, num = 0; LoopZ<PanelNum.y + 1; LoopZ++)
	{
		for (int LoopX = 0; LoopX<PanelNum.x + 1; LoopX++)
		{
			if (num < VertexNum)
			{
				Vtx[num] = VECTOR3(cosf(angle*LoopX)*Radius, PanelHeight*LoopZ, sinf(angle*LoopX)*Radius);
				Tex[num] = VECTOR2((TexDiv.x / PanelNum.x)*LoopX, (TexDiv.y / PanelNum.y)*LoopZ);

				Nor[num] = VECTOR3(sinf(angle*LoopX), 0.0f, cosf(angle*LoopX));
				Nor[num].Normalize();
			}
			Color[num] = COLOR(1.0f, 1.0f, 1.0f, (LoopZ == static_cast< int >( PanelNum.y ) ? 0.0f : 0.5f));
			num++;
		}
	}

	int LoopX = 0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int cnt = 0; cnt < IndexNum; cnt++)
	{
		Index[cnt] = 0;
	}

	for (int LoopZ = 0; LoopZ<PanelNum.y; LoopZ++)
	{
		if (LoopZ != 0 && VtxNo < IndexNum)
		{
			LoopX = 0;
			Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			VtxNo++;
		}
		for (LoopX = 0; LoopX<(PanelNum.x + 1) * 2; LoopX++)
		{
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		if (LoopZ == PanelNum.y - 1)
		{
			break;
		}
		if (VtxNo<IndexNum && VtxNo > 0)
		{
			Index[VtxNo] = Index[VtxNo - 1];
		}
		VtxNo++;
	}

	DrawList = glGenLists(1);
	glNewList(DrawList, GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);

	for (int cnt = 0; cnt<IndexNum; cnt++)
	{
		glColor4f(Color[Index[cnt]].r, Color[Index[cnt]].g, Color[Index[cnt]].b, Color[Index[cnt]].a);
		glNormal3f(Nor[Index[cnt]].x, Nor[Index[cnt]].y, Nor[Index[cnt]].z);
		glTexCoord2f(Tex[Index[cnt]].x, Tex[Index[cnt]].y);
		glVertex3f(Vtx[Index[cnt]].x, Vtx[Index[cnt]].y, Vtx[Index[cnt]].z);
	}

	glEnd();
	glEndList();
}

void CBattleAreaCylinder::Draw(void)
{
	// •`‰æÝ’è
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ƒrƒ…[ƒ}ƒgƒŠƒbƒNƒX‚ð‘Þ”ð

	//•`‰æÝ’è
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glRotatef(_Rot.y,0,1.0f,0);
	glRotatef(_Rot.x,1.0f,0,0);
	glScalef(1.0f,1.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	//ƒ|ƒŠƒSƒ“•`‰æ
	glCallList(DrawList);

	// Ý’è‚ð–ß‚·
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D,0);

}