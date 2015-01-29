#include "MeshFiled.h"
#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)
CMeshField::CMeshField(int priority):CObject(priority)
{
	_Pos = VECTOR3(0,0,0);
	_Rot = VECTOR3(0,0,0);
	Vtx = nullptr;
	Tex = nullptr;
	Nor = nullptr;
}

CMeshField::~CMeshField()
{
	SafeDeletes(Vtx);
	SafeDeletes(Tex);
	SafeDeletes(Nor);
	SafeDeletes(Index);
}

CMeshField* CMeshField::Create(VECTOR3 Pos,VECTOR2 PanelSize,VECTOR2 PanelNum)
{
	CMeshField* Field = new CMeshField;
	if(Field == nullptr)
	{
		return nullptr;
	}
	Field->_Pos = Pos;
	Field->PanelSize = PanelSize;
	Field->PanelNum = PanelNum;
	Field->Init();

	return Field;
}

void CMeshField::Init(void)
{
	IndexNum =		(int)(SUM_INDEX(PanelNum.x,PanelNum.y));
	VertexNum =		(int)((PanelNum.x+1)*(PanelNum.y+1));
	PolygonNum =	(int)(((PanelNum.x*2)*PanelNum.y)+((PanelNum.y-1)*4));

	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];
	

	float OffsetX = (PanelNum.x*PanelSize.x)/2;
	float OffsetZ = (PanelNum.y*PanelSize.y)/2;

	for(int LoopZ=0,num=0;LoopZ<PanelNum.y+1;LoopZ++)
	{
		for(int LoopX=0;LoopX<PanelNum.x+1;LoopX++)
		{
			if (num < VertexNum)
			{
				Vtx[num] = VECTOR3(OffsetX + (-PanelSize.x*LoopX),0,-OffsetZ + (PanelSize.y*LoopZ));
				Tex[num] = VECTOR2((float)LoopX,(float)LoopZ);
				Nor[num] = VECTOR3(0,1.0f,0);
			}
			_Color = COLOR(1.0f,1.0f,1.0f,1.0f);
			num++;
		}
	}

	int LoopX=0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int cnt = 0;cnt < IndexNum;cnt++)
	{
		Index[cnt] = 0;
	}
	for(int LoopZ=0;LoopZ<PanelNum.y;LoopZ++)
	{
		if(LoopZ != 0)
		{
			LoopX = 0;
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		for(LoopX=0;LoopX<(PanelNum.x+1)*2;LoopX++)
		{
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		if(LoopZ==PanelNum.y-1)
		{
			break;
		}
		if (VtxNo < IndexNum && VtxNo > 0)
		{
			Index[VtxNo] = Index[VtxNo - 1];
		}
		VtxNo++;
	}
}

void CMeshField::Uninit(void)
{
	delete this;
}
void CMeshField::Update(void)
{
	//Rot.x++;

}
void CMeshField::Draw(void)
{
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//ビューマトリックスを退避
	//描画設定
	glTranslatef(_Pos.x,_Pos.y,_Pos.z);
	glRotatef(_Rot.z,0,0,1.0f);
	glRotatef(_Rot.y,0,1.0f,0);
	glRotatef(_Rot.x,1.0f,0,0);
	glScalef(1.0f,1.0f,1.0f);
	
	glBindTexture(GL_TEXTURE_2D,Texture.TexID);

	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	//glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);
	
	for(int cnt=0;cnt<IndexNum;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[Index[cnt]].x,Nor[Index[cnt]].y,Nor[Index[cnt]].z);
		glTexCoord2f(Tex[Index[cnt]].x,Tex[Index[cnt]].y);
		glVertex3f(Vtx[Index[cnt]].x,Vtx[Index[cnt]].y,Vtx[Index[cnt]].z);
	}

	glEnd();

	glPopMatrix();//ビューマトリックスを戻す
	glBindTexture(GL_TEXTURE_2D,0);

}

