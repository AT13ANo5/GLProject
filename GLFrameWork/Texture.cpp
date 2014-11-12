#include "Texture.h"
#include <stdio.h>

TEX_INFO CTexture::Tex[CTexture::TEX_MAX];

const char* TexFile[CTexture::TEX_MAX] = 
{
	"data/texture/miku.tga",
	"data/texture/Light.tga",
};

void CTexture::Initialize(void)
{
	for (int cnt = 0;cnt < CTexture::TEX_MAX;cnt++)
	{
		LoadTexture(cnt,TexFile[cnt]);
	}
}

void CTexture::Finalize(void)
{
	for (int cnt = 0;cnt < TEX_MAX;cnt++)
	{
		glDeleteTextures(1,&Tex[cnt].TexID);
	}
}

void CTexture::LoadTexture(int id,const char* filename)
{
	unsigned char* Image = nullptr;
	unsigned short st = 0;
	unsigned short bit = 0;
	bool RightToLeft = false;
	bool TopToBottom = false;
	unsigned short width,height;
	FILE* pFile = fopen(filename,"rb");
	if (pFile != NULL)
	{

		fseek(pFile,12,SEEK_SET);
		fread(&width,1,2,pFile);
		fread(&height,1,2,pFile);
		fread(&bit,1,1,pFile);
		fread(&st,1,1,pFile);
		Tex[id].InverseH = (st & 0x10) ? true : false;
		Tex[id].InverseV = (st & 0x20) ? true : false;
		Image = new unsigned char[width*height * 4];

		for (int cnt = 0;cnt<width*height;cnt++)
		{
			fread(&Image[2 + 4 * cnt],1,1,pFile);
			fread(&Image[1 + 4 * cnt],1,1,pFile);
			fread(&Image[0 + 4 * cnt],1,1,pFile);
			if (bit == 32)
			{
				fread(&Image[3 + 4 * cnt],1,1,pFile);
			}
			else
			{
				if (Image[4 * cnt] == 255 && Image[1 + 4 * cnt] == 255 && Image[2 + 4 * cnt] == 255)
				{
					Image[3 + 4 * cnt] = 0;
				}
				else
				{
					Image[3 + 4 * cnt] = 255;
				}
			}

		}
		fclose(pFile);
	}
	else
	{
		return;
	}

	glGenTextures(1,&Tex[id].TexID);//テクスチャ名を生成
	glBindTexture(GL_TEXTURE_2D,Tex[id].TexID);//ロック
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//リピート設定(横)
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//リピート設定(縦)
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//拡大フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//縮小フィルタリング設定
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,&Image[0]);
	glBindTexture(GL_TEXTURE_2D,0);//アンロック

	Tex[id].Width = width;
	Tex[id].Height = height;

	delete[] Image;
}