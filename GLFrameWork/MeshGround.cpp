#include "MeshGround.h"
#include "Texture.h"
#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)

CMeshGround::CMeshGround(int priority) :CObject(priority)
{
	_Pos = VECTOR3(0,0,0);
	_Rot = VECTOR3(0, 0, 0);
	Vtx = nullptr;
	Tex = nullptr;
	Nor = nullptr;
	NormalMap = nullptr;
	HeightMap = nullptr;
}
CMeshGround::~CMeshGround()
{
	if (Vtx!=nullptr)
	{
		delete[] Vtx;
		Vtx = nullptr;
	}
	if (Tex!=nullptr)
	{
		delete[] Tex;
		Tex = nullptr;
	}
	if (Nor!=nullptr)
	{
		delete[] Nor;
		Nor = nullptr;
	}
	if (NormalMap!=nullptr)
	{
		delete[] NormalMap;
		NormalMap = nullptr;
	}
	if (Index != nullptr)
	{
		delete[] Index;
		Index = nullptr;
	}
	if (HeightMap!=nullptr)
	{
		delete[] HeightMap;
		HeightMap = nullptr;
	}
}
CMeshGround* CMeshGround::Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum)
{
	CMeshGround* Ground = new CMeshGround;
	if (Ground==nullptr)
	{
		return nullptr;
	}
	Ground->_Pos = pos;
	Ground->PanelSize = PanelSize;
	Ground->PanelNum = PanelNum;

	Ground->Init();

	return Ground;

}
void CMeshGround::Init(void)
{
	IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
	VertexNum = (int)((PanelNum.x+1)*(PanelNum.y+1));
	PolygonNum = (int)(((PanelNum.x*2)*PanelNum.y)+((PanelNum.y-1)*4));
	MapNum = (int)(PanelNum.x*PanelNum.y*2);

	NormalMap = new VECTOR3[MapNum];
	HeightMap = new float[VertexNum];
	for (int cnt = 0;cnt<VertexNum;cnt++)
	{
		HeightMap[cnt] = 2.0f * (rand() % 60) + 0.0f; //20.0f * cnt;// rand() % 60 + 0.0f;
	}

	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];

	_Size.x = PanelNum.x*PanelSize.x;
	_Size.y = 0;
	_Size.z = PanelNum.y*PanelSize.y;

	float OffsetX = (PanelNum.x*PanelSize.x)/2;
	float OffsetZ = (PanelNum.y*PanelSize.y)/2;

	for (int LoopZ = 0,num = 0;LoopZ<PanelNum.y+1;LoopZ++)
	{
		for (int LoopX = 0;LoopX<PanelNum.x+1;LoopX++)
		{
			Vtx[num] = VECTOR3(OffsetX+(-PanelSize.x*LoopX),HeightMap[num],-OffsetZ+(PanelSize.y*LoopZ));
			Tex[num] = VECTOR2((float)LoopX,(float)LoopZ);
			Nor[num] = VECTOR3(0,1.0f,0);
			_Color = COLOR(1.0f,1.0f,1.0f,1.0f);
			num++;
		}
	}

	//法線マップを作成
	VECTOR3 Vec1,Vec2,Cross;
	int cnt = 0;
	for (int Z = 0,num = 0;Z<PanelNum.y;Z++)
	{
		for (int X = 0;X<PanelNum.x;X++)
		{
			//四角形が／で分割された上側の法線の計算
			Vec1 = Vtx[(int)(PanelNum.x+1)+num]-Vtx[num];
			Vec2 = Vtx[1+num]-Vtx[num];
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			NormalMap[cnt] = Cross;
			cnt++;

			//四角形が／で分割された下側の法線の計算
			Vec1 = Vtx[num+1]-Vtx[(int)(PanelNum.x+1)+(num+1)];
			Vec2 = Vtx[(int)(PanelNum.x+1)+num]-Vtx[(int)(PanelNum.x+1)+(num+1)];
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			NormalMap[cnt] = Cross;
			cnt++;
			num++;
		}
		//頂点がX軸の端まで来たので折り返す
		num++;
	}
	cnt = 0;

	//法線を計算しつつ頂点に設定
	for (int Y = 0,num = 0;Y<=PanelNum.y;Y++)
	{//numは面の番号
		for (int X = 0;X<=PanelNum.x;X++)
		{
			VECTOR3 AddCross = VECTOR3(0,0,0);//一時変数を初期化 計算結果格納用
			if (X==0)
			{//X軸の左端の時
				if (Y==0)
				{//一番最初の段なら
					AddCross = NormalMap[num]+NormalMap[num+1];
				}
				else if (Y==PanelNum.y)
				{//一番最後の段
					num = num-(int)PanelNum.x*2;//一列前の番号に戻す
					AddCross = NormalMap[num];
					num--;//次も同じ番号なので一つ戻しておく
				}
				else
				{//中間
					AddCross = NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num+1]+NormalMap[num];
				}
			}
			else
			{
				if (Y==0)
				{//一番最初の段
					if (X==PanelNum.x)
					{//右端の時
						AddCross = NormalMap[num]+NormalMap[num+1];
					}
					else
					{
						AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2];
					}
				}
				else if (Y==PanelNum.y)
				{//一番最後の段
					if (X==PanelNum.x)
					{//右端の時
						AddCross = NormalMap[num]+NormalMap[num+1];
					}
					else
					{
						AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2];
					}
				}
				else if (X==PanelNum.x)
				{//Yが端ではなく、同時に右端だったとき
					AddCross = NormalMap[num]+NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num-((int)PanelNum.x*2)-1];
				}
				else
				{
					AddCross = NormalMap[num]+NormalMap[num+1]+NormalMap[num+2]+NormalMap[num-((int)PanelNum.x*2)-1]+NormalMap[num-((int)PanelNum.x*2)]+NormalMap[num-((int)PanelNum.x*2)+1];
				}
				if (X<PanelNum.x)
				{//右端以外なら面の番号を一つ進める
					num++;
				}
			}

			//正規化した法線データを頂点に設定
			AddCross.Normalize();
			Nor[cnt] = AddCross;
			cnt++;//頂点番号を進める
			num++;//面の番号を進める
		}

	}

	int LoopX = 0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int LoopZ = 0;LoopZ<PanelNum.y;LoopZ++)
	{
		if (LoopZ!=0)
		{
			LoopX = 0;
			Index[VtxNo] = (int)((LoopZ*(PanelNum.x+1))+(((LoopX+1)%2)*(PanelNum.x+1)+(LoopX/2)));
			VtxNo++;
		}
		for (LoopX = 0;LoopX<(PanelNum.x+1)*2;LoopX++)
		{
			Index[VtxNo] = (int)((LoopZ*(PanelNum.x+1))+(((LoopX+1)%2)*(PanelNum.x+1)+(LoopX/2)));
			VtxNo++;
		}
		if (LoopZ==PanelNum.y-1)
		{
			break;
		}
		Index[VtxNo] = Index[VtxNo-1];
		VtxNo++;
	}
}

void CMeshGround::Uninit(void)
{
	delete this;
}
void CMeshGround::Update(void)
{
	//Rot.x++;

}
void CMeshGround::Draw(void)
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
	MATERIAL Material;
	Material.ambient = COLOR(0.3f,0.3f,0.3f,1.0f);
	Material.diffuse = _Color;
	Material.specular = COLOR(0,0,0,1.0f);
	Material.emission = COLOR(0,0,0,1.0f);
	Material.shininess = 0.0f;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(float*)&Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(float*)&Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(float*)&Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,(float*)&Material.emission);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material.shininess);

	//ポリゴン描画
	glBegin(GL_TRIANGLE_STRIP);

	for (int cnt = 0;cnt<IndexNum;cnt++)
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

void CMeshGround::GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY)
{
	pos.x = _Size.x / 2 + (pos.x);
	pos.z = _Size.z / 2 + (pos.z);

	*OutIndexX = (int)(PanelNum.x - pos.x / PanelSize.x);
	*OutIndexY = (int)(pos.z / PanelSize.y);

	Console::SetCursorPos(1, 3);
	Console::Print("GroundIndex : (%03d, %03d)\n", *OutIndexX, *OutIndexY);
}

float CMeshGround::GetHeight(VECTOR3 pos,VECTOR3* normal)
{
	int IndexX = 0;
	int IndexY = 0;
	GetPanelIndex(pos,&IndexX,&IndexY);
	int Index = (int)(IndexX+(IndexY*(PanelNum.x+1)));
	VECTOR3 VertexPos[4];
	VertexPos[0] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,		HeightMap[Index],							(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[1] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,	HeightMap[Index + 1],						(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[2] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,	HeightMap[Index + ((int)PanelNum.x + 2)],	(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);
	VertexPos[3] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,		HeightMap[Index + ((int)PanelNum.x + 1)],	(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);

	for (int cntVertex = 0; cntVertex < 4; ++cntVertex)
	{
		Console::SetCursorPos(1, 4 + cntVertex);
		Console::Print("Vertex[%d] : (%9.3f, %9.3f) : Height(%9.3f)\n", cntVertex, VertexPos[cntVertex].x, VertexPos[cntVertex].z, VertexPos[cntVertex].y);
	}
	Console::SetCursorPos(1, 8);
	Console::Print("VertexIndex[0] : %2d : %9.3f\n", Index,							VertexPos[0].y);
	Console::Print("VertexIndex[1] : %2d : %9.3f\n", Index + 1,						VertexPos[1].y);
	Console::Print("VertexIndex[2] : %2d : %9.3f\n", Index + ((int)PanelNum.x + 2),	VertexPos[2].y);
	Console::Print("VertexIndex[3] : %2d : %9.3f\n", Index + ((int)PanelNum.x + 1),	VertexPos[3].y);
#if 0
	for (int cnt = 0;cnt<4;cnt++)
	{
		VertexPos[cnt].x -= _Size.x/2;
		VertexPos[cnt].z -= _Size.z/2;
		VertexPos[cnt].z *= -1;
	}
#endif
	VECTOR3 Vec0 = VertexPos[1]-VertexPos[0];
	VECTOR3 Vec1 = VECTOR3(0,0,0);

	bool flag = false;
	for (int cnt = 0;cnt<3;cnt++)
	{
		Vec0 = VertexPos[((cnt+1)%3)]-VertexPos[(cnt%3)];
		Vec1 = pos-VertexPos[(cnt%3)];
		if ((Vec0.z*Vec1.x-Vec0.x*Vec1.z)>=0)
		{
			flag = true;
		}
		else
		{
			flag = false;
			break;
		}
	}
	if (flag)
	{
		Console::SetCursorPos(1, 12);
		Console::Print("上\n");
		return GetHeightPolygon(VertexPos[1],VertexPos[2],VertexPos[0],pos, normal);
	}
	else
	{
		Console::SetCursorPos(1, 12);
		Console::Print("下\n");
		return GetHeightPolygon(VertexPos[3],VertexPos[0],VertexPos[2],pos, normal);
	}

	return 0;
}

float CMeshGround::GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal)
{
	Console::SetCursorPos(1, 13);
	Console::Print("Pos[0] : (%9.3f, %9.3f) : Height(%9.3f)\n", p0.x, p0.z, p0.y);
	Console::Print("Pos[1] : (%9.3f, %9.3f) : Height(%9.3f)\n", p1.x, p1.z, p1.y);
	Console::Print("Pos[2] : (%9.3f, %9.3f) : Height(%9.3f)\n", p2.x, p2.z, p2.y);
	VECTOR3 Vec1,Vec0;
	VECTOR3 normal = VECTOR3(0,0,0);
	Vec0 = p1 - p0;
	Vec1 = p2 - p0;
	Console::Print("Vec[0] : (%9.3f, %9.3f, %9.3f)\n", Vec0.x, Vec0.y, Vec0.z);
	Console::Print("Vec[1] : (%9.3f, %9.3f, %9.3f)\n", Vec1.x, Vec1.y, Vec1.z);
	VECTOR3::Cross(&normal,Vec0,Vec1);
	normal.Normalize();
	if (normal.y==0.0f)
	{
		return 0;
	}
	*Normal = normal;

	float Height = p0.y - (-normal.x*(pos.x - p0.x) + normal.z*(pos.z - p0.z)) / normal.y;
	Console::Print("Height : %9.3f\n", Height);
	Console::Print("Normal : (%9.3f, %9.3f, %9.3f)\n", normal.x, normal.y, normal.z);

	return Height;

}