#ifndef _TEXTURE_H_
#define _TEXTURE_H_

typedef struct TEX_INFO
{
 TEX_INFO()
 {
  TexID = 0;
  Width = Height = 0;
  InverseH = InverseV = false;
 }
 unsigned int TexID;//テクスチャのID(バインドするのはこれ)
 bool InverseH;//左右反転フラグ
 bool InverseV;//上下反転フラグ
 float Width;//幅
 float Height;//高さ

}TEX_INFO;

enum
{

	TEX_MIKU = 0,
	TEX_TITLELOGO,
	TEX_RESULT_LOGO,
	TEX_TEAM_LOGO,
	TEX_CONNECTION,
	TEX_TEST,
	TEX_YOUJO_RED,
	TEX_YOUJO_BLUE,
	TEX_YOUJO_WATER,
	TEX_YOUJO_ORANGE,
	TEX_YOUJO_WHITE,
	TEX_YOUJO_YELLOW,
	TEX_NETWARK_YOUJO_BG,
	TEX_NETWARK_YOUJO_BLUE,
	TEX_NETWARK_YOUJO_CPU,
	TEX_NETWARK_YOUJO_ENTER,
	TEX_NETWARK_YOUJO_LOGO,
	TEX_NETWARK_YOUJO_ORANGE,
	TEX_NETWARK_YOUJO_READY,
	TEX_NETWARK_YOUJO_RED,
	TEX_NETWARK_YOUJO_RINCHAN,
	TEX_NETWARK_YOUJO_WATER,
	TEX_NETWARK_YOUJO_WHITE,
	TEX_NETWARK_YOUJO_HOST,
	TEX_NETWARK_YOUJO_PLAYER,
	TEX_LIGHT,
	TEX_FIELD,
	TEX_SKY,
	TEX_LIFE,
	TEX_RESULT_TEXT,
	TEX_ROCK,
	TEX_BULLET,
	TEX_BALLISTIC,
	TEX_LANDING,
	TEX_GAUGE_ICON,
	TEX_NUMBER,
	TEX_PAUSE,
	TEX_PLAYER1,
	TEX_PLAYER2,
	TEX_PLAYER3,
	TEX_PLAYER4,
	TEX_PLAYER5,
	TEX_PLAYER6,
	TEX_PLAYER_YOUJO_RED,
	TEX_PLAYER_YOUJO_BLUE,
	TEX_PLAYER_YOUJO_WATER,
	TEX_PLAYER_YOUJO_ORANGE,
	TEX_PLAYER_YOUJO_WHITE,
	TEX_PLAYER_YOUJO_YELLOW,
	TEX_RELOAD,
	TEX_REPORT,
	TEX_PUSH_ENTER,
	TEX_WALL,
	TEX_MINIMAP,
	TEX_MINIMAP_ARROW,
	TEX_EXPLOSION,
	TEX_SMOKE,
	TEX_SAND_CLOUD,
	TEX_MINIMAP_BG,
	TEX_NARITADA,
	TEX_TIMEFRAME,
	TEX_SHADOW,
	TEX_START,
	TEX_END,
	TEX_MAX
};

class CTexture
{
public:

 

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
 static void LoadTga(int id,const char* filename);
 static void LoadDDS(int id,const char* filename);
};
#endif