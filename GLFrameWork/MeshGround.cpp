#include "MeshGround.h"
#include "Texture.h"
#define SUM_INDEX(X,Z) ((X+1)*(Z-1)+((X+1)*(Z+1))+(Z-1)*2)
#define HEIGHT_MAP ("./data/TEXTURE/heightmap.bmp")

CMeshGround::CMeshGround(int priority) :CObject(priority)
{
	_Pos = VECTOR3(0,0,0);
	_Rot = VECTOR3(0,0,0);
	Vtx = nullptr;
	Tex = nullptr;
	Nor = nullptr;
	NormalMap = nullptr;
	HeightMap = nullptr;
	BufferNormal = nullptr;
}
CMeshGround::~CMeshGround()
{
	if (Vtx != nullptr)
	{
		delete[] Vtx;
		Vtx = nullptr;
	}
	if (Tex != nullptr)
	{
		delete[] Tex;
		Tex = nullptr;
	}
	if (Nor != nullptr)
	{
		delete[] Nor;
		Nor = nullptr;
	}
	if (NormalMap != nullptr)
	{
		delete[] NormalMap;
		NormalMap = nullptr;
	}
	if (Index != nullptr)
	{
		delete[] Index;
		Index = nullptr;
	}
	if (HeightMap != nullptr)
	{
		delete[] HeightMap;
		HeightMap = nullptr;
	}
	delete[] BufferNormal;
	BufferNormal = nullptr;
}
CMeshGround* CMeshGround::Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum,float heightMag)
{
	CMeshGround* Ground = new CMeshGround;
	if (Ground == nullptr)
	{
		return nullptr;
	}
	Ground->_Pos = pos;
	Ground->PanelSize = PanelSize;
	Ground->PanelNum = PanelNum;
	Ground->HeightMag = heightMag;
	Ground->Init();

	return Ground;

}

void CMeshGround::Init(void)
{
	IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
	VertexNum = (int)((PanelNum.x + 1)*(PanelNum.y + 1));
	PolygonNum = (int)(((PanelNum.x * 2)*PanelNum.y) + ((PanelNum.y - 1) * 4));
	MapNum = (int)(PanelNum.x*PanelNum.y * 2);
	LoadImg(HEIGHT_MAP);
	NormalMap = new VECTOR3[MapNum];

	
	Vtx = new VECTOR3[VertexNum];
	Tex = new VECTOR2[VertexNum];
	Nor = new VECTOR3[VertexNum];

	BufferNormal = new VECTOR3[VertexNum];

	_Size.x = PanelNum.x*PanelSize.x;
	_Size.y = 0;
	_Size.z = PanelNum.y*PanelSize.y;

	float OffsetX = (PanelNum.x*PanelSize.x) / 2;
	float OffsetZ = (PanelNum.y*PanelSize.y) / 2;

	for (int LoopZ = 0,num = 0;LoopZ < PanelNum.y + 1;LoopZ++)
	{
		for (int LoopX = 0;LoopX < PanelNum.x + 1;LoopX++)
		{
			if (num < VertexNum)
			{
				Vtx[num] = VECTOR3(OffsetX + (-PanelSize.x*LoopX),HeightMap[num],-OffsetZ + (PanelSize.y*LoopZ));
				Tex[num] = VECTOR2((float)LoopX,(float)LoopZ);
				Nor[num] = VECTOR3(0,1.0f,0);
				_Color = COLOR(1.0f,1.0f,1.0f,1.0f);
			}
			num++;
		}
	}

	//�@���}�b�v���쐬
	VECTOR3 Vec1,Vec2,Cross;
	int cnt = 0;
	for (int Z = 0,num = 0;Z < PanelNum.y;Z++)
	{
		for (int X = 0;X < PanelNum.x;X++)
		{
			//�l�p�`���^�ŕ������ꂽ�㑤�̖@���̌v�Z
			if (num + PanelNum.x + 1 < VertexNum)
			{
				Vec1 = Vtx[(int)(PanelNum.x + 1) + num] - Vtx[num];
				Vec2 = Vtx[1 + num] - Vtx[num];
			}
			VECTOR3::Cross(&Cross,Vec2,Vec1);
			Cross.Normalize();
			if (cnt < MapNum)
			{
				NormalMap[cnt] = Cross;
			}
			cnt++;

			if (num + PanelNum.x + 1 < VertexNum)
			{
				//�l�p�`���^�ŕ������ꂽ�����̖@���̌v�Z
				Vec1 = Vtx[num + 1] - Vtx[(int)(PanelNum.x + 1) + (num + 1)];
				Vec2 = Vtx[(int)(PanelNum.x + 1) + num] - Vtx[(int)(PanelNum.x + 1) + (num + 1)];
				VECTOR3::Cross(&Cross,Vec2,Vec1);
				Cross.Normalize();
				NormalMap[cnt] = Cross;
			}
			cnt++;
			num++;
		}
		//���_��X���̒[�܂ŗ����̂Ő܂�Ԃ�
		num++;
	}
	cnt = 0;

	//�@�����v�Z�����_�ɐݒ�
	for (int Y = 0,num = 0;Y <= PanelNum.y;Y++)
	{//num�͖ʂ̔ԍ�
		for (int X = 0;X <= PanelNum.x;X++)
		{
			VECTOR3 AddCross = VECTOR3(0,0,0);//�ꎞ�ϐ��������� �v�Z���ʊi�[�p
			if (X == 0)
			{//X���̍��[�̎�
				if (Y == 0)
				{//��ԍŏ��̒i�Ȃ�
					AddCross = NormalMap[num] + NormalMap[num + 1];
				}
				else if (Y == PanelNum.y)
				{//��ԍŌ�̒i
					num = num - (int)PanelNum.x * 2;//���O�̔ԍ��ɖ߂�
					AddCross = NormalMap[num];
					num--;//���������ԍ��Ȃ̂ň�߂��Ă���
				}
				else if ((num - ((int)PanelNum.x * 2)) >= 0 && num + 1 < MapNum)
				{//����
					AddCross = NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num + 1] + NormalMap[num];
				}
			}
			else
			{
				if (Y == 0)
				{//��ԍŏ��̒i
					if (X == PanelNum.x && num + 1 < MapNum)
					{//�E�[�̎�
						AddCross = NormalMap[num] + NormalMap[num + 1];
					}
					else if (num + 2 < MapNum)
					{
						AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2];
					}
				}
				else if (Y == PanelNum.y)
				{//��ԍŌ�̒i
					if (X == PanelNum.x)
					{//�E�[�̎�
						AddCross = NormalMap[num] + NormalMap[num + 1];
					}
					else
					{
						AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2];
					}
				}
				else if (X == PanelNum.x)
				{//Y���[�ł͂Ȃ��A�����ɉE�[�������Ƃ�
					AddCross = NormalMap[num] + NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num - ((int)PanelNum.x * 2) - 1];
				}
				else
				{
					AddCross = NormalMap[num] + NormalMap[num + 1] + NormalMap[num + 2] + NormalMap[num - ((int)PanelNum.x * 2) - 1] + NormalMap[num - ((int)PanelNum.x * 2)] + NormalMap[num - ((int)PanelNum.x * 2) + 1];
				}
				if (X < PanelNum.x)
				{//�E�[�ȊO�Ȃ�ʂ̔ԍ�����i�߂�
					num++;
				}
			}

			//���K�������@���f�[�^�𒸓_�ɐݒ�
			AddCross.Normalize();
			if (cnt < VertexNum)
			{
				Nor[cnt] = AddCross;
			}
			cnt++;//���_�ԍ���i�߂�
			num++;//�ʂ̔ԍ���i�߂�
		}

	}
	
	int LoopX = 0;
	int VtxNo = 0;
	Index = new int[IndexNum];
	for (int cnt = 0;cnt < IndexNum;cnt++)
	{
		Index[cnt] = 0;
	}
	for (int LoopZ = 0;LoopZ < PanelNum.y;LoopZ++)
	{
		if (LoopZ != 0)
		{
			LoopX = 0;
			if (VtxNo < IndexNum)
			{
				Index[VtxNo] = (int)((LoopZ*(PanelNum.x + 1)) + (((LoopX + 1) % 2)*(PanelNum.x + 1) + (LoopX / 2)));
			}
			VtxNo++;
		}
		for (LoopX = 0;LoopX < (PanelNum.x + 1) * 2;LoopX++)
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
		if (VtxNo > 0 && VtxNo < IndexNum)
		{
			Index[VtxNo] = Index[VtxNo - 1];
		}
		VtxNo++;
	}

	DrawList = glGenLists(1);
	glNewList(DrawList,GL_COMPILE);

	//�|���S���`��
	glBegin(GL_TRIANGLE_STRIP);

	for (int cnt = 0;cnt < IndexNum;cnt++)
	{
		glColor4f(_Color.r,_Color.g,_Color.b,_Color.a);
		glNormal3f(Nor[Index[cnt]].x,Nor[Index[cnt]].y,Nor[Index[cnt]].z);
		glTexCoord2f(Tex[Index[cnt]].x,Tex[Index[cnt]].y);
		glVertex3f(Vtx[Index[cnt]].x,Vtx[Index[cnt]].y,Vtx[Index[cnt]].z);
	}

	glEnd();

	glEndList();

	// �@���o�b�t�@�쐬
	CalculateBufferNormal();
}

void CMeshGround::Uninit(void)
{
	glDeleteLists(DrawList,1);
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
	glPushMatrix();//�r���[�}�g���b�N�X��ޔ�
	//�`��ݒ�
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

	//�|���S���`��
	glCallList(DrawList);

	glPopMatrix();//�r���[�}�g���b�N�X��߂�
	glBindTexture(GL_TEXTURE_2D,0);

}

void CMeshGround::GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY)
{
	pos.x = _Size.x / 2 + (pos.x);
	pos.z = _Size.z / 2 + (pos.z);

	*OutIndexX = (int)(PanelNum.x - pos.x / PanelSize.x);
	*OutIndexY = (int)(pos.z / PanelSize.y);

	//Console::SetCursorPos(1,3);
	//Console::Print("GroundIndex : (%03d, %03d)\n",*OutIndexX,*OutIndexY);
}

float CMeshGround::GetHeight(VECTOR3 pos,VECTOR3* normal)
{
	int IndexX = 0;
	int IndexY = 0;
	GetPanelIndex(pos,&IndexX,&IndexY);
	int Index = (int)(IndexX + (IndexY*(PanelNum.x + 1)));
	VECTOR3 VertexPos[4];
	VertexPos[0] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,HeightMap[Index],(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[1] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,HeightMap[Index + 1],(-0.5f * PanelNum.y + IndexY) * PanelSize.y);
	VertexPos[2] = VECTOR3((0.5f * PanelNum.x - IndexX - 1) * PanelSize.x,HeightMap[Index + ((int)PanelNum.x + 2)],(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);
	VertexPos[3] = VECTOR3((0.5f * PanelNum.x - IndexX) * PanelSize.x,HeightMap[Index + ((int)PanelNum.x + 1)],(-0.5f * PanelNum.y + IndexY + 1) * PanelSize.y);

	for (int cntVertex = 0; cntVertex < 4; ++cntVertex)
	{
		//Console::SetCursorPos(1,4 + cntVertex);
		//Console::Print("Vertex[%d] : (%9.3f, %9.3f) : Height(%9.3f)\n",cntVertex,VertexPos[cntVertex].x,VertexPos[cntVertex].z,VertexPos[cntVertex].y);
	}
	//Console::SetCursorPos(1,8);
	//Console::Print("VertexIndex[0] : %2d : %9.3f\n",Index,VertexPos[0].y);
	//Console::Print("VertexIndex[1] : %2d : %9.3f\n",Index + 1,VertexPos[1].y);
	//Console::Print("VertexIndex[2] : %2d : %9.3f\n",Index + ((int)PanelNum.x + 2),VertexPos[2].y);
	//Console::Print("VertexIndex[3] : %2d : %9.3f\n",Index + ((int)PanelNum.x + 1),VertexPos[3].y);
#if 0
	for (int cnt = 0;cnt<4;cnt++)
	{
		VertexPos[cnt].x -= _Size.x/2;
		VertexPos[cnt].z -= _Size.z/2;
		VertexPos[cnt].z *= -1;
	}
#endif
	VECTOR3 Vec0 = VertexPos[1] - VertexPos[0];
	VECTOR3 Vec1 = VECTOR3(0,0,0);

	// �@���̎擾
	GetNormal(pos.x, pos.z, normal);

	bool flag = false;
	for (int cnt = 0;cnt < 3;cnt++)
	{
		Vec0 = VertexPos[((cnt + 1) % 3)] - VertexPos[(cnt % 3)];
		Vec1 = pos - VertexPos[(cnt % 3)];
		if ((Vec0.z*Vec1.x - Vec0.x*Vec1.z) >= 0)
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
		//Console::SetCursorPos(1,12);
		//Console::Print("��\n");
		return GetHeightPolygon(VertexPos[1],VertexPos[2],VertexPos[0],pos,nullptr);
	}
	else
	{
		//Console::SetCursorPos(1,12);
		//Console::Print("��\n");
		return GetHeightPolygon(VertexPos[3],VertexPos[0],VertexPos[2],pos,nullptr);
	}

	return 0;
}

float CMeshGround::GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal)
{
	//Console::SetCursorPos(1,13);
	//Console::Print("Pos[0] : (%9.3f, %9.3f) : Height(%9.3f)\n",p0.x,p0.z,p0.y);
	//Console::Print("Pos[1] : (%9.3f, %9.3f) : Height(%9.3f)\n",p1.x,p1.z,p1.y);
	//Console::Print("Pos[2] : (%9.3f, %9.3f) : Height(%9.3f)\n",p2.x,p2.z,p2.y);
	VECTOR3 Vec1,Vec0;
	VECTOR3 normal = VECTOR3(0,0,0);
	Vec0 = p1 - p0;
	Vec1 = p2 - p0;
	//Console::Print("Vec[0] : (%9.3f, %9.3f, %9.3f)\n",Vec0.x,Vec0.y,Vec0.z);
	//Console::Print("Vec[1] : (%9.3f, %9.3f, %9.3f)\n",Vec1.x,Vec1.y,Vec1.z);
	VECTOR3::Cross(&normal,Vec0,Vec1);
	normal.Normalize();
	if (normal.y == 0.0f)
	{
		return 0;
	}
	if (Normal != nullptr)
	{
		*Normal = normal;
	}

	float Height = p0.y - (-normal.x*(pos.x - p0.x) + normal.z*(pos.z - p0.z)) / normal.y;
	//Console::Print("Height : %9.3f\n",Height);
	//Console::Print("Normal : (%9.3f, %9.3f, %9.3f)\n",normal.x,normal.y,normal.z);

	return Height;

}

//�摜�f�[�^�ǂݍ���
void CMeshGround::LoadImg(const char * imgFile)
{
	FILE *file;
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;//�w�b�_�[���
	float FieldScl = HeightMag;
	file = fopen(imgFile,"rb");
	if (file != NULL)
	{
		fread(&bmfh,sizeof(BITMAPFILEHEADER),1,file);
		fread(&bmih,sizeof(BITMAPINFOHEADER),1,file);
		if (PanelNum.x <= 0 || PanelNum.y <= 0)
		{
			PanelNum.x = bmih.biWidth - 1;
			PanelNum.y = bmih.biHeight - 1;
		}
   if(PanelNum.x > bmih.biWidth)
   {
    PanelNum.x = bmih.biWidth;
   }
   if(PanelNum.y > bmih.biHeight)
   {
    PanelNum.y = bmih.biHeight;
   }
		IndexNum = (int)(SUM_INDEX(PanelNum.x,PanelNum.y));
		VertexNum = (int)((PanelNum.x + 1)*(PanelNum.y + 1));
		PolygonNum = (int)(((PanelNum.x * 2)*PanelNum.y) + ((PanelNum.y - 1) * 4));
		MapNum = (int)(PanelNum.x*PanelNum.y * 2);

		HeightMap = new float[VertexNum];
		bmih.biWidth = PanelNum.x + 1;
		bmih.biHeight = PanelNum.y + 1;
		BYTE Height;
		if (bmih.biBitCount == 24)
		{

			for (int y = bmih.biHeight - 1; y >= 0; y--)
			{
				for (int x = 0; x < bmih.biWidth; x++)
				{
					fread(&Height,1,1,file);
					fread(&Height,1,1,file);
					fread(&Height,1,1,file);
					if (Height == '\0')
					{

						fread(&Height,1,1,file);
						fread(&Height,1,1,file);
						fread(&Height,1,1,file);
					}
					HeightMap[bmih.biWidth * y + x] = (float)Height - 255;
					HeightMap[bmih.biWidth * y + x] += 255;
					HeightMap[bmih.biWidth * y + x] *= FieldScl;
				}
			}
			ImgSize.x = bmih.biWidth;
			ImgSize.y = bmih.biHeight;
		}
		else
			if (bmih.biBitCount == 8)
			{
				RGBQUAD sRGB[256];

				fread(&sRGB,sizeof(RGBQUAD),256,file);

				for (int y = bmih.biHeight - 1; y >= 0; y--)
				{
					for (int x = 0; x < bmih.biWidth; x++)
					{
						fread(&Height,1,1,file);
						if (Height == '\0')
						{

							fread(&Height,1,1,file);
						}

						HeightMap[bmih.biWidth * y + x] = (float)sRGB[Height].rgbBlue - 255;
						HeightMap[bmih.biWidth * y + x] += 255;
						HeightMap[bmih.biWidth * y + x] *= FieldScl;
					}
				}
				ImgSize.x = bmih.biWidth;
				ImgSize.y = bmih.biHeight;


			}
		fclose(file);

	}
	else{

		//���s������10x10�̕���
		HeightMap = new float[11 * 11];
		memset(HeightMap,0,sizeof(float));
		ImgSize.x = 10;
		ImgSize.y = 10;
		PanelNum.x = ImgSize.x - 1;
		PanelNum.y = ImgSize.y - 1;
	}

}

void CMeshGround::CalculateBufferNormal(void)
{
	// ���_���̎擾
	UINT	numVertexX = static_cast< int >(PanelNum.x) + 1;		// X�������_��
	UINT	numVertexZ = static_cast< int >(PanelNum.y) + 1;		// Z�������_��

	// �@���̐ݒ�
	VECTOR3	vectorNormal[2];		// �@���x�N�g��
	VECTOR3	vectorPlaneX[2];		// X�����x�N�g��
	VECTOR3	vectorPlaneZ[2];		// Z�����x�N�g��
	for (UINT cntZ = 1; cntZ < numVertexZ - 1; ++cntZ)
	{
		for (UINT cntX = 1; cntX < numVertexX - 1; ++cntX)
		{
			vectorPlaneX[0].x = -PanelSize.x;
			vectorPlaneX[0].z = 0.0f;
			vectorPlaneX[0].y = HeightMap[numVertexX * cntZ + (cntX + 1)];
			vectorPlaneZ[0].x = 0.0f;
			vectorPlaneZ[0].z = PanelSize.y;
			vectorPlaneZ[0].y = HeightMap[numVertexX * (cntZ + 1) + cntX];
			vectorPlaneX[1].x = PanelSize.x;
			vectorPlaneX[1].z = 0.0f;
			vectorPlaneX[1].y = HeightMap[numVertexX * cntZ + (cntX - 1)];
			vectorPlaneZ[1].x = 0.0f;
			vectorPlaneZ[1].z = -PanelSize.y;
			vectorPlaneZ[1].y = HeightMap[numVertexX * (cntZ - 1) + cntX];
			vectorPlaneX[0].y -= HeightMap[numVertexX * cntZ + cntX];
			vectorPlaneX[1].y -= HeightMap[numVertexX * cntZ + cntX];
			vectorPlaneZ[0].y -= HeightMap[numVertexX * cntZ + cntX];
			vectorPlaneZ[1].y -= HeightMap[numVertexX * cntZ + cntX];
			vectorPlaneX[0].Normalize();
			vectorPlaneX[1].Normalize();
			vectorPlaneZ[0].Normalize();
			vectorPlaneZ[1].Normalize();
			VECTOR3::Cross(&vectorNormal[0], vectorPlaneX[0], vectorPlaneZ[0]);
			VECTOR3::Cross(&vectorNormal[1], vectorPlaneX[1], vectorPlaneZ[1]);
			vectorNormal[0].Normalize();
			vectorNormal[1].Normalize();
			vectorNormal[0] += vectorNormal[1];
			vectorNormal[0].Normalize();
			BufferNormal[numVertexX * cntZ + (static_cast< int >(PanelNum.x) - 1 - cntX)] = vectorNormal[0];
		}
	}

	// �[�̖@���̐ݒ�
	for (UINT cntZ = 1; cntZ < numVertexZ - 1; ++cntZ)
	{
		BufferNormal[numVertexX * cntZ] = BufferNormal[numVertexX * cntZ + 1];
		BufferNormal[numVertexX * cntZ + numVertexX - 1] = BufferNormal[numVertexX * cntZ + numVertexX - 2];
	}
	for (UINT cntX = 1; cntX < numVertexX - 1; ++cntX)
	{
		BufferNormal[cntX] = BufferNormal[numVertexX + cntX];
		BufferNormal[numVertexX * (numVertexZ - 1) + cntX] = BufferNormal[numVertexX * (numVertexZ - 2) + cntX];
	}
	vectorNormal[0] = BufferNormal[1] + BufferNormal[numVertexX];
	BufferNormal[0] = vectorNormal[0];
	BufferNormal[0].Normalize();
	vectorNormal[0] = BufferNormal[numVertexX - 2] + BufferNormal[numVertexX * 2 - 1];
	BufferNormal[numVertexX - 1] = vectorNormal[0];
	BufferNormal[numVertexX - 1].Normalize();
	vectorNormal[0] = BufferNormal[numVertexX * (numVertexZ - 1) + 1] + BufferNormal[numVertexX * (numVertexZ - 2)];
	BufferNormal[numVertexX * (numVertexZ - 1)] = vectorNormal[0];
	BufferNormal[numVertexX * (numVertexZ - 1)].Normalize();
	vectorNormal[0] = BufferNormal[numVertexX * (numVertexZ - 1) + numVertexX - 2] + BufferNormal[numVertexX * (numVertexZ - 2) + numVertexX - 1];
	BufferNormal[numVertexX * (numVertexZ - 1) + numVertexX - 1] = vectorNormal[0];
	BufferNormal[numVertexX * (numVertexZ - 1) + numVertexX - 1].Normalize();
}

void CMeshGround::GetNormal(float fPosX, float fPosZ, VECTOR3* pOut)
{
	// ���_�̔ԍ����Z�o
	int		nIdxVtxX = static_cast< int >((fPosX + PanelSize.x * static_cast< int >(PanelNum.x) * 0.5f) / PanelSize.x);
	int		nIdxVtxZ = static_cast< int >((fPosZ + PanelSize.y * static_cast< int >(PanelNum.y) * 0.5f) / PanelSize.y);

	// �������Z�o
	float	fRateX;
	float	fRateZ;
	fRateX = (fPosX - PanelSize.x * (nIdxVtxX - static_cast< int >(PanelNum.x) * 0.5f)) / PanelSize.x;
	if (fRateX < 0.0f)
	{
		fRateX = 0.0f;
	}
	else if (fRateX > 1.0f)
	{
		fRateX = 1.0f;
	}
	fRateZ = (fPosZ - PanelSize.y * (nIdxVtxZ - static_cast< int >(PanelNum.y) * 0.5f)) / PanelSize.y;
	if (fRateZ < 0.0f)
	{
		fRateZ = 0.0f;
	}
	else if (fRateZ > 1.0f)
	{
		fRateZ = 1.0f;
	}

	// �@����ݒ�
	VECTOR3	vecNormal(0.0f, 0.0f, 0.0f);
	vecNormal += BufferNormal[(nIdxVtxZ + 0) * (static_cast< int >(PanelNum.x) + 1) + (nIdxVtxX + 0)] * (1.0f - fRateX) * (1.0f - fRateZ);
	vecNormal += BufferNormal[(nIdxVtxZ + 0) * (static_cast< int >(PanelNum.x) + 1) + (nIdxVtxX + 1)] * fRateX * (1.0f - fRateZ);
	vecNormal += BufferNormal[(nIdxVtxZ + 1) * (static_cast< int >(PanelNum.x) + 1) + (nIdxVtxX + 0)] * (1.0f - fRateX) * fRateZ;
	vecNormal += BufferNormal[(nIdxVtxZ + 1) * (static_cast< int >(PanelNum.x) + 1) + (nIdxVtxX + 1)] * fRateX * fRateZ;
	if (vecNormal.y < 0.01f && vecNormal.y > -0.01f)
	{
		if (vecNormal.x < 0.01f && vecNormal.x > -0.01f)
		{
			if (vecNormal.z < 0.01f && vecNormal.z > -0.01f)
			{
				vecNormal.x = vecNormal.z = 0.0f;
				vecNormal.y = 1.0f;
			}
		}
	}
	*pOut = vecNormal;
}