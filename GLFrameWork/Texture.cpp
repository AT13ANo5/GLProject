#include "Texture.h"
#include "main.h"
#include <stdio.h>
#include<string>
TEX_INFO CTexture::Tex[TEX_MAX];

static std::string TexFile[TEX_MAX] = 
{
	"data/texture/miku.tga",
	"data/texture/titleLogo.tga",
	"data/texture/resultLogo.tga",
	"data/texture/TeamLogo.dds",
	"data/texture/Connection.dds",
	"data/texture/Maro.tga",
	"data/texture/youjo_red.dds",
	"data/texture/youjo_blue.dds",
	"data/texture/youjo_green.dds",
	"data/texture/youjo_orange.dds",
	"data/texture/youjo_white.dds",
	"data/texture/youjo_yellow.dds",
	"data/texture/youjo_network_bg.dds",
	"data/texture/youjo_network_blue.dds",
	"data/texture/youjo_network_cpu.dds",
	"data/texture/youjo_network_enterpng.dds",
	"data/texture/youjo_network_Logo.dds",
	"data/texture/youjo_network_orange.dds",
	"data/texture/youjo_network_ready.dds",
	"data/texture/youjo_network_red.dds",
	"data/texture/youjo_network_rinchan.dds",
	"data/texture/youjo_network_water.dds",
	"data/texture/youjo_network_white.dds",
	"data/texture/Light.tga",
 	"data/texture/field000.dds",
	"data/texture/sky000.dds",
 	"data/texture/Life.dds",
	"data/texture/resultText.tga",
	"data/texture/rock.dds",
	"data/texture/bullet100.tga",
	"data/texture/ballistic.tga",
	"data/texture/gaugeIcon.tga",
	"data/texture/number011.tga",
	"data/texture/pause.tga",
	"data/texture/player1.tga",
	"data/texture/player2.tga",
	"data/texture/player3.tga",
	"data/texture/player4.tga",
	"data/texture/player5.tga",
	"data/texture/player6.tga",
	"data/texture/reload.tga",
	"data/texture/pause.tga",
	"data/texture/pushEnter.tga",
	"data/texture/wall100.dds",
	"data/texture/MiniMap.dds",
	"data/texture/MiniMapArrow.dds",
	"data/texture/explosion.tga",
	"data/texture/spark.tga",
	"data/texture/sandCloud.tga",
	"data/texture/minimapBG.dds",
	"data/texture/naritada.dds",
	"data/texture/timer.tga",
};

void CTexture::Initialize(void)
{
	for (int cnt = 0;cnt < TEX_MAX;cnt++)
	{
		std::string::iterator it = TexFile[cnt].end()-3;
		char ex[4] = { *it,*(it + 1),*(it + 2) ,'\0'};
		if (strcmp(ex,"tga")==0)
		{
			LoadTga(cnt,TexFile[cnt].c_str());
		}
		else if (strcmp(ex,"dds") == 0)
		{
			LoadDDS(cnt,TexFile[cnt].c_str());
		}
		//LoadTexture(cnt,TexFile[cnt].c_str());
	}
}

void CTexture::Finalize(void)
{
	for (int cnt = 0;cnt < TEX_MAX;cnt++)
	{
		glDeleteTextures(1,&Tex[cnt].TexID);
	}
}

void CTexture::LoadTga(int id,const char* filename)
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

//----------------------
//		DDSヘッダー用構造体
//----------------------

//DDPIXELFORMAT
struct DDPIXELFORMAT
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwRGBAlphaBitMask;
};

//DDCAPS2
struct DDCAPS2
{
	DWORD dwCaps1;
	DWORD dwCaps2;
	DWORD Reserved[2];
};

//DDSURFACEDESC2
struct DDSURFACEDESC2
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD dwPitchOrLinearSize;
	DWORD dwDepth;
	DWORD dwMipMapCount;
	DWORD dwReserved1[11];
	DDPIXELFORMAT ddpfPixelFormat;
	DDCAPS2  ddsCaps;
	DWORD dwReserved2;
};

void CTexture::LoadDDS(int id,const char* filename)
{

	BYTE *pixel = NULL;											//ピクセルデータ
	int width;
	int height;

	FILE* file;
	fopen_s(&file,filename,"rb");
	//ファイルオープン
	if (file == NULL) { return; }

	//マジックワード
	const int magic_number = 4;								//マジックナンバー
	char magic_word[magic_number + 1] = { '\0' };
	fread(magic_word,sizeof(char),magic_number,file);

	//サーフェイスフォーマットヘッダー
	DDSURFACEDESC2 dds_header;
	char suf_header[sizeof(DDSURFACEDESC2)] = { '\0' };

	fread(suf_header,sizeof(char),sizeof(DDSURFACEDESC2),file);
	memcpy(&dds_header,suf_header,sizeof(DDSURFACEDESC2));


	//画像の高さと幅の保存	
	width = dds_header.dwWidth;
	height = dds_header.dwHeight;

	//データのサイズを設定
	int const RGBA_SIZE = 4;
	int pixel_size = dds_header.dwWidth * dds_header.dwHeight * RGBA_SIZE;

	//メインサーフェイスデータ
	pixel = new BYTE[pixel_size];
	fread(pixel,sizeof(BYTE),pixel_size,file);

	fclose(file);	//ファイルクローズ
	bool flag =false;
	if (!(dds_header.ddpfPixelFormat.dwRGBAlphaBitMask & 0xff000000))
	{
		flag = true;
	}
	
	

	//DDS画像がBGRAの順にデータが並んでいるのでRGBAの順に並び替える
	BYTE r,g,b,a;
	for (int i = 0;i<pixel_size;i += 4){
		b = pixel[i];
		g = pixel[i + 1];
		r = pixel[i + 2];
		(flag) ? a = 255 : a = pixel[i + 3];
		pixel[i] = r;
		pixel[i + 1] = g;
		pixel[i + 2] = b;
		pixel[i + 3] = a;
	}

	glGenTextures(1,&Tex[id].TexID);//テクスチャ名を生成
	glBindTexture(GL_TEXTURE_2D,Tex[id].TexID);//ロック
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//リピート設定(横)
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//リピート設定(縦)
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//拡大フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//縮小フィルタリング設定
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixel);
	glBindTexture(GL_TEXTURE_2D,0);//アンロック

	Tex[id].Width = (float)dds_header.dwWidth;
	Tex[id].Height = (float)dds_header.dwHeight;
	Tex[id].InverseV = true;
	Tex[id].InverseH = false;

	delete[] pixel;
}