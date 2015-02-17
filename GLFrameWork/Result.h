#ifndef _RESULT_H_
#define _RESULT_H_
#include "Scene.h"

class CModel;
class CPolygon2D;
class CMeshGround;
class CMeshSphere;
class CResultUIManager;


class CResult :public CScene
{
public:

  CResult();
  ~CResult();

  typedef enum{
    PHASE_RESULTSHEET = 1,
    PHASE_END,
    PHASE_MAX
  } PHASE;

  void Init(void);
  void Uninit(void);
  void Update(void);

private:
  static const int kROCKMAX = 4;
  static const float  RADIUS_SKY;   // ‹ó‚Ì”¼Œa

  CModel*       Player[PLAYER_MAX];
  CModel*       Barrel[PLAYER_MAX];
  CModel*       Rock[kROCKMAX];
  CMeshGround*  Ground;       // ’nŒ`
  CMeshSphere*  Sky;          // ‹ó
  CResultUIManager* UIManager;
	PHASE         Phase;
};
#endif