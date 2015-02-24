//=============================================================================
//
// UI�}�l�[�W�� [UI.cpp]
//
// Auther : Takahiro Kikushima
//          Masato Masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "UI.h"
#include "life.h"
#include "LoadGauge.h"
#include "LoadString.h"
#include "MiniMap.h"
#include "Report.h"
#include "NumberManager.h"
#include "Number2D.h"
#include "Input/VC.h"
#include "Time.h"
#include "Polygon2D.h"
#include "ManagerGL.h"

#include "CPlayer.h"

namespace{

	// ���U�Q�[�W
	const VECTOR3		GAUGE_POS = VECTOR3(90.0f, 100.0f, 0.0f);
	const VECTOR2		GAUGE_SIZE = VECTOR2(230.0f,40.0f);
	const float			GAUGE_STR_OFFSET = 50.0f;
	const COLOR			GAUGE_COLOR = COLOR(0.0f, 0.7f, 1.0f, 1.0f);
	// ���U������
	const VECTOR3		GAUGE_STR_POS = VECTOR3(GAUGE_POS.x + GAUGE_STR_OFFSET, GAUGE_POS.y, 0.0f);
	const VECTOR2		GAUGE_STR_SIZE = VECTOR2(130.0f, GAUGE_SIZE.y);
	// ���U�Q�[�W�̃x�[�X
	const float			GAUGE_BASE_ADD = 4.0f;
	const VECTOR3		GAUGE_BASE_POS = VECTOR3(GAUGE_POS.x - (GAUGE_BASE_ADD * 0.5f), GAUGE_POS.y - (GAUGE_BASE_ADD * 0.5f), 0.0f);
	const VECTOR2		GAUGE_BASE_SIZE = VECTOR2(GAUGE_SIZE.x + GAUGE_BASE_ADD, GAUGE_SIZE.y + GAUGE_BASE_ADD);
	const COLOR			GAUGE_BASE_COLOR = COLOR(0.0f, 0.0f, 0.0f, 0.8f);
	// ���U�Q�[�W�A�C�R��
	const float			ICON_SIZE = 70.0f;
	const VECTOR3		ICON_POS = VECTOR3(10.0f + ICON_SIZE / 2, GAUGE_POS.y + (ICON_SIZE / 2) - 15.0f, 0.0f);
	// ���ѕ\
	const float			REPORT_ADD_Y = 30.0f;
	const VECTOR3		REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f, (SCREEN_HEIGHT*0.5f) + REPORT_ADD_Y, 0.0f);
	const VECTOR3		REPORT_BASE_POS = VECTOR3(SCREEN_WIDTH*0.5f, (SCREEN_HEIGHT*0.5f), 0.0f);
	const COLOR			REPORT_BG_COLOR = COLOR(0.0f, 0.05f, 0.0f, 0.6f);
	// ���C�t
	const VECTOR3		LIFE_POS = VECTOR3(10.0f,0.0f,0.0f);
	const VECTOR2		LIFE_SIZE = VECTOR2(100.0f, 100.0f);
	//�^�C�}�[����
	const VECTOR3 TIME_POS = VECTOR3(570, 90, 0);
	const VECTOR2 TIME_SIZE = VECTOR2(65, 65);
	// �^�C�}�[�t���[��
	const VECTOR3 TIME_F_POS = VECTOR3(SCREEN_WIDTH*0.5f, 80, 0);
	const float TIME_F_SCALE = 0.9f;
	const VECTOR2 TIME_F_SIZE = VECTOR2(256.0f * TIME_F_SCALE, 170.0f * TIME_F_SCALE);
//	const COLOR TIME_F_COLOR = COLOR(0.2f, 0.65f, 0.2f, 1.0f);
	const COLOR TIME_F_COLOR = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	// �J�E���g���鐔��
	const float   COUNTER_SCAL = 2.5f;
	const VECTOR2 COUNTER_SIZE = VECTOR2(128.0f*COUNTER_SCAL, 100.0f*COUNTER_SCAL); // 128 100
	const VECTOR3 COUNTER_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
	// ����
	const float STRING_SCAL = 1.5f;
	const VECTOR2 STRING_SIZE = VECTOR2(430 * STRING_SCAL, 150 * STRING_SCAL);
	const VECTOR3 STRING_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
}

//=============================================================================
// constructor
//=============================================================================
CUI::CUI()
{
	life = nullptr;
	miniMap = nullptr;
	loadGauge = nullptr;
	loadGaugeBase = nullptr;
	loadString = nullptr;
	reportBg = nullptr;
	report = nullptr;
	numberManager = nullptr;
	Time = nullptr;
	counter = nullptr;
	string = nullptr;

	myID = 0;
}

//=============================================================================
// destructor
//=============================================================================
CUI::~CUI()
{
}

//=============================================================================
// init
//=============================================================================
void CUI::Init(void)
{
	//�^�C�}�[����
	Time = CTime::Create(TIME_POS, TIME_SIZE);
	Time->SetUpdateFlag(false);

	//�^�C�}�[�̘g
	timeFrame = CPolygon2D::Create(TIME_F_POS, TIME_F_SIZE);
	timeFrame->SetTex(CTexture::Texture(TEX_TIMEFRAME));
	timeFrame->SetColor(TIME_F_COLOR);

	//���C�t����
	life = CLife::Create(LIFE_POS, LIFE_SIZE);

	// ���U�Q�[�W�̉��n
	loadGaugeBase = CLoadGauge::Create(GAUGE_BASE_POS, GAUGE_BASE_SIZE);
	loadGaugeBase->SetDefaultColor(GAUGE_BASE_COLOR);

	// ���U�Q�[�W
	loadGauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	loadGauge->SetDefaultColor(GAUGE_COLOR);

	// �u���U���v�̕���
	loadString = CLoadString::Create(GAUGE_STR_POS,GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_RELOAD));
	loadString->DrawEnable();

	// �e�A�C�R��
	CPolygon2D* canonIcon = nullptr;
	canonIcon = CPolygon2D::Create(ICON_POS,VECTOR2(ICON_SIZE,ICON_SIZE));
	canonIcon->SetTex(CTexture::Texture(TEX_GAUGE_ICON));

	// ���ѕ\�̔w�i
	reportBg = CReport::Create(REPORT_BASE_POS, VECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	reportBg->SetColor(REPORT_BG_COLOR);

	// ���ѕ\
	const float scl = 0.8f;
	report = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH * scl,SCREEN_HEIGHT * scl));
	report->SetTex(CTexture::Texture(TEX_REPORT));// ���ѕ\�̐��l

	// �ԍ��}�l�[�W���[
	numberManager = CNumberManager::Create();

	// ���ѕ\�B��
	reportBg->SetDrawFlag(false);
	report->SetDrawFlag(false);
	numberManager->SetDrawFlag(false);

	//�~�j�}�b�v�̏�����
	miniMap = new CMiniMap;
	miniMap->Init();

	// �J�E���g���鐔��
	counter = CNumber2D::Create(COUNTER_POS, COUNTER_SIZE);
	counter->SetNumber(3);
	counter->SetTex(CTexture::Texture(TEX_NUMBER));
	counter->SetDrawFlag(false);

	// �X�^�[�g����
	string = CPolygon2D::Create(STRING_POS, STRING_SIZE);
	string->SetTex(CTexture::Texture(TEX_START));
	string->SetDrawFlag(false);
}

//=============================================================================
// Release
//=============================================================================
void CUI::Uninit(void)
{
	SafeDelete(miniMap);

	SafeRelease(numberManager);

	SafeRelease(reportBg);

	SafeRelease(report);

	SafeRelease(life);

	SafeRelease(loadGauge);

	SafeRelease(loadString);

	SafeRelease(Time);

	SafeRelease(counter);

	player = nullptr;
}

//=============================================================================
// update
//=============================================================================
void CUI::Update(void)
{
	// �v���C���[������Ώ���
	if (player != nullptr){

		// ���U�Q�[�W
		const float currentTimer = (float)player[myID]->ReloadTimer();
		const float maxTimer = (float)PLAYER_RELOAD_TIME;
		const float rate = currentTimer / maxTimer;

		// ������%�ł�����
		loadGauge->SetRate(rate);
		if (rate >= 1.0f){
			loadString->DrawDisable();
		}
		if (rate <= 0.0f){
			loadString->DrawEnable();
		}
		// �~�j�}�b�v�ɕ\��
		for (int cnt = 0;cnt < PLAYER_MAX;cnt++)
		{
			miniMap->SetPlayer(cnt,player[cnt]->Pos(),player[cnt]->Rot().y);
		}

		miniMap->Update();

		// ���C�t���v���C���[����擾���ĕ\��
		int lifeP = player[myID]->PlayerLife();
		life->SetLife(lifeP);

		if (counter->GetDrawFlag() == true){
			const float subScale = 0.012f;
			const VECTOR2 counterSub = VECTOR2(COUNTER_SIZE.x * subScale, COUNTER_SIZE.y * subScale);
			counter->SubSize(counterSub);
		}
	}

	if (Time->GetUpdateFlag() == true){
		// ���ѕ\��ON/OFF
		if(VC::Instance()->Press(COMMAND_SCORE)){
			reportBg->SetDrawFlag(true);
			report->SetDrawFlag(true);
			numberManager->SetDrawFlag(true);

			// kill / death
			for (int playerCnt = 0; playerCnt < PLAYER_MAX; ++playerCnt){
				numberManager->SetNumber((CNumberManager::TYPE)playerCnt,
					CManager::userInfo[playerCnt].kill,
					CManager::userInfo[playerCnt].death);
			}
		} else {
			reportBg->SetDrawFlag(false);
			report->SetDrawFlag(false);
			numberManager->SetDrawFlag(false);
		}
	}
}

//=============================================================================
// SetNumber
//-----------------------------------------------------------------------------
//  type  :  �^�C�v, kill  :  KILL��, DEATH  :  DEATH��
//=============================================================================
void CUI::SetNumber(CNumberManager::TYPE type, int kill, int death)
{
	numberManager->SetNumber(type, kill, death);
}

//=============================================================================
// SetNumber
//-----------------------------------------------------------------------------
//  type  :  �^�C�v, kill  :  KILL��, DEATH  :  DEATH��
//=============================================================================
void CUI::SetNumber(int num)
{
	counter->SetNumber(num); 
}

//=============================================================================
// SetNumberDrawFlag
//-----------------------------------------------------------------------------
//  bool : ON/OFF
//=============================================================================
void CUI::SetNumberDrawFlag(bool flag)
{
	counter->SetDrawFlag(flag);
}

//=============================================================================
// SetTimeUpdateFlag
//-----------------------------------------------------------------------------
//  bool : ON/OFF
//=============================================================================
void CUI::SetTimeUpdateFlag(bool flag)
{
	Time->SetUpdateFlag(flag);
}

//=============================================================================
// SetStringTexture
//-----------------------------------------------------------------------------
//	�D���ȃe�N�X�`��������
//=============================================================================
void CUI::SetStringTexture(TEX_INFO tex)
{
	string->SetTex(tex);
}

//=============================================================================
// SetStringSizeRefresh
//-----------------------------------------------------------------------------
//	�T�C�Y��߂���
//=============================================================================
void CUI::SetCountSizeRefresh(void)
{
	counter->SetSize(COUNTER_SIZE);
}

//=============================================================================
// SetDrawFlag
//-----------------------------------------------------------------------------
//	�`�悫���[
//=============================================================================
void CUI::SetStringDrawFlag(bool draw)
{
	string->SetDrawFlag(draw);
}

//=============================================================================
// GetTime
//-----------------------------------------------------------------------------
// �^�C�}�[�擾
//=============================================================================
int		CUI::GetTime(void)
{
	return Time->GetTime();
}

//=============================================================================
// SetTime
//-----------------------------------------------------------------------------
// �^�C���ݒ�
//=============================================================================
void	CUI::SetTime(int time)
{
	Time->SetTime(time);
}

//=============================================================================
// SubTime
//-----------------------------------------------------------------------------
// �^�C�}�[���Z����
//=============================================================================
void	CUI::SubTime()
{
	Time->SubTime();
}


// end of file