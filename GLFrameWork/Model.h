//=============================================================================
//モデルクラス
//=============================================================================
#ifndef _SCENE_MODEL_H_
#define _SCENE_MODEL_H_

//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Object.h"

//=============================================================================
//モデルデータ構造体
//=============================================================================
struct MODEL_DATA
{
	MODEL_DATA()
	{
		Vtx = Nor = NULL;
		Tex = NULL;
		TriIndexPos = TriIndexNor = TriIndexTex = NULL;
		QuadIndexPos = QuadIndexNor = QuadIndexTex = NULL;
		TriIndexNum = QuadIndexNum = NULL;
		PartsNum = 0;
		Max = Min = Size = VECTOR3(0,0,0);
	};

	VECTOR3* Vtx;
	VECTOR3* Nor;
	VECTOR2* Tex;
	int* TriIndexPos;
	int* TriIndexNor;
	int* TriIndexTex;

	int* QuadIndexPos;
	int* QuadIndexNor;
	int* QuadIndexTex;

	int* TriIndexNum;
	int* QuadIndexNum;
	int PartsNum;
	VECTOR3 Max,Min,Size;

	//三角形ポリゴンのインデックスの合計を返す
	int TriIndexSum(void){
		int sum = 0;
		for (int cnt = 0;cnt < PartsNum;cnt++)
		{
			sum += TriIndexNum[cnt];
		}
		return sum;
	}

	//四角形ポリゴンのインデックスの合計を返す
	int QuadIndexSum(void){
		int sum = 0;
		for (int cnt = 0;cnt < PartsNum;cnt++)
		{
			sum += QuadIndexNum[cnt];
		}
		return sum;
	}
};
//=============================================================================
//モデルクラス
//=============================================================================
class CModel:public CObject
{
public:
	enum
	{
		MIKU = 0,
		MODEL_MAX
	};

	CModel(int priority=3);
	virtual ~CModel(){UnlinkList();}
	void Init(void);
	void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static void Initialize(void);
	static void Finalize(void);

	void LoadAnim(char* filename);
	static CModel* Create(int id,const VECTOR3& pos);
	
	LPSTR GetModelName(void){return FileName;}
	int GetModelID(void){return ModelID;}

	CModel* GetNext(void){return CModel::Next;}
	CModel* GetPrev(void){return CModel::Prev;}

	static CModel* GetTop(void){return CModel::Top;}
	static CModel* GetCur(void){return CModel::Cur;}

	static int GetModelNum(void){return ModelNum;}
	VECTOR3 GetSize(void){ return Size; }

	static CModel* GetModel(int id);

private:
	void LinkList(void);
	void UnlinkList(void);
	static CModel* Top;
	static CModel* Cur;

	static void Scaling(int id,int posCount);

	CModel* Next;
	CModel* Prev;

protected:

	VECTOR3 Size;
	MATERIAL Material;
	float rot;

	static MODEL_DATA ModelData[];
	static bool LoadModel(const char* filename,int id);
	static int ModelNum;
	int ModelID;
	int MotionFrameNum;
	int MotionID;
	LPSTR FileName;

	VECTOR3** MotionPos;
	VECTOR3** MotionRot;
};
#endif