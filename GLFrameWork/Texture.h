#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "main.h"
typedef struct TEX_INFO
{
	TEX_INFO()
	{
		TexID = 0;
		Width = Height = 0;
		InverseH = InverseV = false;
	}
	unsigned int TexID;
	bool InverseH;
	bool InverseV;
	float Width;
	float Height;
}TEX_INFO;


class CTexture
{
public:

	enum
	{
		TEX_MIKU = 0,
		TEX_LIGHT,
		TEX_MAX
	};

	static void Initialize(void);
	static void Finalize(void);

	static unsigned int TexID(int id)	{ return Tex[id].TexID; }
	static bool InverseH(int id)		{ return Tex[id].InverseH; }
	static bool InverseV(int id)		{ return Tex[id].InverseV; }
	static float Width(int id)			{ return Tex[id].Width; }
	static float Height(int id)			{ return Tex[id].Height; }
	static TEX_INFO Texture(int id)	{ return Tex[id];}

private:

	static TEX_INFO Tex[];
	static void LoadTexture(int id,const char* filename);
};
#endif