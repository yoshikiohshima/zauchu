/****************************************************************
 <wtiimain.c>
					��������

		SqueakPIM�N���X���\�b�h

*****************************************************************/
#include <zheaders.h>
#include <wtii.h>
#include "wtii_C_usr.h"

//======================================================================
//  		PIM�N���X���\�b�h
//======================================================================
BOOLEAN
PIMAplConstruct(PIMINFO *PIMInfo,USHORT InitLevel)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

	// �A�v���P�[�V�����v���Z�X�N�����ɍ�Ɨp�������̊m�ۂȂǂ̏���
	// ���K�v�ȏꍇ�́A�����ɋL�q���ĉ������B

	return true;
}
//----------------------------------------------------------------------
void
PIMAplEnd(PIMINFO *PIMInfo,MESSAGE *Message)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

	// ���ӁI - ���̊֐��̓f�t�H���g�ł̓R�[������܂���B
	// �K�v�ȏꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]��
	// �A�v���P�[�V�����^�C�v�^�u�ŁA[�v���Z�X�I�����b�Z�[�W�v��]
	// ���`�F�b�N���ĉ������B

	// �A�v���P�[�V�����v���Z�X�I�����ɏ������K�v�ȏꍇ�́A������
	// �L�q���ĉ������B
   ioExitBody();

}
#if useDefaultFunction
//----------------------------------------------------------------------
BOOLEAN
PIMAplSetup(PIMINFO *PIMInfo,MESSAGE *Message,USHORT InitLevel)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

#ifdef VGA_140DPI_APL
//���̕����́AVGA�p�ɍ쐬����MORE�\�t�g��VGA��ʂł͂Ȃ��U�E���X��
//���s���悤�Ƃ����Ƃ��Ɍx�����o���A�v���Z�X�N�������Ȃ����߂̃R�[�h�ł�
{
	MESSAGE msg;
	UCHAR	ModelName[MODEL_NAME_MAX];
	ULONG	Version;
	UPOINT	ScreenSize;
	DPI	DPISize;

	/* �V�X�e����������Ă��� */
	GetSystemInfo(ModelName,&Version);

	if((Version>>16) < MODEL_NUMBER_MIEX1){
		/* MI-EX1�ȑO�̋@��̏ꍇ�i���s�ł��Ȃ��j */
		/* �x�����b�Z�[�W��\�� */
		msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
		AlertExecStr(&msg,(STRING)"���̋@��ł͎��s�ł��܂���",
				ALB_CENTER, 0);
		/* �G���[�Ƃ��ăv���Z�X�I������ */
		SetFailMessage(Message);
		return false;
	}else{
		/* MI-EX1�i���܂ށj�ȍ~�̋@��̏ꍇ */
		/* ��ʃT�C�Y��640*480�h�b�g�ȏ�Ȃ瓮�삷��悤�ɂ��� */
		GetScreenSizeAndDPI(&ScreenSize, &DPISize);
		if((ScreenSize.x < 640) || (ScreenSize.y < 480)){
			/* VGA�i�ȏ�j�ł͂Ȃ��̂Ŏ��s�ł��Ȃ� */
			/* �x�����b�Z�[�W��\�� */
			msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
			AlertExecStr(&msg,(STRING)"���̋@��ł͎��s�ł��܂���",
				ALB_CENTER, 0);
			/* �G���[�Ƃ��ăv���Z�X�I������ */
			SetFailMessage(Message);
			return false;
		}
	}
}
#endif /* VGA_140DPI_APL */


	// �A�v���P�[�V�����v���Z�X�N�����ɏ�������`�F�b�N�Ȃǂ̏���
	// ���K�v�ȏꍇ�́A�����ɋL�q���ĉ������B


	return true;
}
//----------------------------------------------------------------------
#endif
void
PIMAplOutMsg(PIMINFO *PIMInfo,MESSAGE *Message)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

	// �A�v���P�[�V�������A�E�g���Ɏ�M�������b�Z�[�W�ɑ΂��ď���
	// ���K�v�ȏꍇ�́A�����ɋL�q���ĉ������B

}
//----------------------------------------------------------------------
BOOLEAN
PIMExecConstruct(PIMEXECWORK *PIMExecWork,MESSAGE *Message)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

	// �A�v���P�[�V������EXEC���Ɏg�p�����Ɨp�������̊m�ۂȂǂ�
	// �K�v�ȏꍇ�́A�����ɋL�q���ĉ������B

	return true;
}
//----------------------------------------------------------------------
void
PIMExecEnd(PIMEXECWORK *PIMExecWork,MESSAGE *Message)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

	// �A�v���P�[�V�������A�E�g��ԂɈڍs����Ƃ��ɏ���
	// ���K�v�ȏꍇ�́A�����ɋL�q���ĉ������B

	// �A�E�g��Ԃ������Ȃ��A�v���P�[�V�����̏ꍇ�́A�ȉ��̂Q�s��
	// �R�����g���O���ĉ������B
	// �A�E�g��ԂɈڍs����Ƃ��Ƀv���Z�X�I�����鏈��
	// Message->CommonMsg.MessageID = AppliProcEndReqID;
	// Message->AppliProcEndReq.AplFilter = AF_THIS_APPLI;
  sqZaurusCameraRelease(1);
  sqZaurusCameraRelease(2);

}

#ifdef useDefaultFunction
//----------------------------------------------------------------------
BOOLEAN
PIMExecSetup(PIMEXECWORK *PIMExecWork,MESSAGE *Message)
{
	// �֐�����ύX����ꍇ�́AIDE��[�c�[��]-[�A�v���P�[�V�����ݒ�]
	// ��PIM�N���X���\�b�h�^�u�ɕ\�������֐������ύX���ĉ������B

#ifdef VGA_140DPI_APL
	WIN_WORK_EXT WinWorkExt;

	/* PIM�N���X�̃E�B���h�E�I�[�v������߂� */
	PIMExecWork->WhichWindow = PIM_WIN_NO_OPEN;

	/* VGA�E�B���h�E���J���APIM�N���X�����̃E�B���h�E���g�p����悤�� */
	/* �ݒ肷�� */
	WinWorkExt.userID = APL_WIN;
	WinWorkExt.CrLocPoint.x = 0;
	WinWorkExt.CrLocPoint.y = 0;
	WinWorkExt.CrSize.x = 640 * 4 - 1;
	WinWorkExt.CrSize.y = 480 * 4 - 1;
	WinWorkExt.WinType = WN_NO_FRAME;
	WinWorkExt.PileMode = BOTTOM;
	WinWorkExt.DestWinNo = 0;
	WinWorkExt.OpenPosition = 0;
	WinWorkExt.AutoAdjMode = NONE;
	GetSystemColor(&(WinWorkExt.Color), SYS_BACK_COLOR);
	PIMExecWork->WnHandle = OpenNormWindowVGA(&WinWorkExt);
#endif /* VGA_140DPI_APL */


	// �A�v���P�[�V�������A�E�g��Ԃ���EXEC��ԂɈڍs����Ƃ��ɏ���
	// ���K�v�ȏꍇ�́A�����ɋL�q���ĉ������B


	return true;
}
#endif


BOOLEAN
PIMExecSetup(PIMEXECWORK *PIMExecWork, MESSAGE *Message)
{
  WIN_WORK_EXT WinWorkExt;
  /* PIM�N���X�̃E�B���h�E�I�[�v������߂�*/
  PIMExecWork->WhichWindow = PIM_WIN_NO_OPEN;
  /* VGA�E�B���h�E���J���APIM�N���X�����̃E�B���h�E���g�p����悤�ɐݒ肷��*/
  WinWorkExt.userID = APL_WIN;
  WinWorkExt.CrLocPoint.x = 0;
  WinWorkExt.CrLocPoint.y = 0;
  WinWorkExt.WinType = WN_NO_FRAME;
  WinWorkExt.PileMode = BOTTOM;
  WinWorkExt.DestWinNo = 0;
  WinWorkExt.OpenPosition = 0;
  WinWorkExt.AutoAdjMode = NONE;
  if (zaurusScreenSize == ZAURUS_SCREEN_VGA) {
    WinWorkExt.CrSize.x = 640 * 4 - 1;
    WinWorkExt.CrSize.y = 480 * 4 - 11;
    GetSystemColor(&(WinWorkExt.Color), SYS_BACK_COLOR);
    PIMExecWork->WnHandle = OpenNormWindowVGA(&WinWorkExt);
  } else if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    WinWorkExt.CrSize.x = 320 * 4 - 1;
    WinWorkExt.CrSize.y = 240 * 4 - 11;
    /* GetSystemColor(&(WinWorkExt.Color), SYS_BACK_COLOR); */
    PIMExecWork->WnHandle = OpenNormWindow(&WinWorkExt);
  }

  return true;
}

BOOLEAN
PIMAplSetup(PIMINFO *PIMInfo, MESSAGE *Message, USHORT InitLevel)
{
  MESSAGE msg;
  UCHAR ModelName[MODEL_NAME_MAX];
  ULONG Version;
  UPOINT ScreenSize;
  DPI DPISize;

  zaurusScreenSize = ZAURUS_SCREEN_QVGA;
  
  /* �V�X�e����������Ă���*/
  GetSystemInfo(ModelName,&Version);
  if((Version>>16) < MODEL_NUMBER_MIEX1) {
    /* MI-EX1�ȑO�̋@��̏ꍇ�i���s�ł��Ȃ��j*/
    /* �x�����b�Z�[�W��\��*/
    msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
    AlertExecStr(&msg,(STRING)"This binary is not appropriate for this machine",
		 ALB_CENTER, 0);
    /* �G���[�Ƃ��ăv���Z�X�I������*/
    SetFailMessage(Message);
    return false;
  } else {
    /* MI-EX1�i���܂ށj�ȍ~�̋@��̏ꍇ*/
    GetScreenSizeAndDPI(&ScreenSize, &DPISize);
    if ((ScreenSize.x == 640) && (ScreenSize.y == 480)) {
      zaurusScreenSize = ZAURUS_SCREEN_VGA;
    } else if ((ScreenSize.x == 320) && (ScreenSize.y == 240)) {
      zaurusScreenSize = ZAURUS_SCREEN_QVGA;
    } else {
      /* �x�����b�Z�[�W��\��*/
      msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
      AlertExecStr(&msg,(STRING)"This binary is not appropriate for this machine",
		   ALB_CENTER, 0);
      /* �G���[�Ƃ��ăv���Z�X�I������*/
      SetFailMessage(Message);
      return false;
    }
  }
  return true;
}
