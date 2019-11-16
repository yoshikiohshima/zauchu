/***********************************************************************
<�A�v���P�[�V�����C�x���g�֐�>

 <wtii_evc.c>
					��������

		Squeak�C�x���g�֐�

***********************************************************************/
/*
	���̃t�@�C����SZAB���������X�V���܂��B
	���̃t�@�C���𒼐ڕҏW���鎞�ɂ́A�K�����L���ӎ���������Ă��������B
	�����łȂ��ꍇ�́A
	�@�ESZAB�����삵�Ȃ��Ȃ�
	�@�E���[�U�l�̕ҏW���ʂ�������
	�\��������܂��B

	*******�@���ӎ����@*******
	[1] ���L�̏ꏊ�̂ݕҏW�ł��܂�
	      �E�w�b�_��include  ��1�R�����g�̊�
	    �@�E�e�C�x���g�֐��̓����i�֐�{}�̒��j

		����ȊO�̉ӏ���ҏW���Ȃ��ł��������B
		���ɃR�����g�s�� �L�[���[�h"SZAB"���܂܂�Ă���s���΂�
		�ύX�E�폜���Ȃ��ł��������B

	[2] ���̃t�@�C�����G�f�B�^���Œ��ڕҏW���鎞�A
	�@  SZAB�t�H�[���G�f�B�^�͕K���I�����Ă����Ă��������B

	  �@SZAB�t�H�[���G�f�B�^�͂��̃t�@�C���ɏ������݂܂��B
	  �@�Q�v���O��������{�t�@�C���֏������ނ��ƂɂȂ�܂��̂�
	  �@��ѐ����ۂĂȂ��Ȃ�\��������܂��B

	[3] ���[�U�l�ŊJ�������������֐��i��SZAB�����������֐��ȊO�̊֐��j
	�@�@�́A���̃t�@�C���Ƃ͕ʂ̃\�[�X�t�@�C���֒u���Ă�������
	�@�@���̃t�@�C���ɒu���ƁA�i�u�����ꏊ�ɂ���ẮjSZAB�ɂ��
	�@�@�폜�����\��������܂��B
*/

/*======================================================================
    include
======================================================================*/
#include <zheaders.h>
#include <pimmgr.h>
#include <wtii_res.h>

#include "wtii_C_usr.h"

/* ��1  �ȉ��Ɂi���̃t�H�[���p�́j���Ȃ��Ǝ���include��ǉ����Ă������� */
/* #include "your_header.h" */
/* ��1  �����܂� */


/*======================================================================
    �C�x���g�֐�
	�C�x���g�֐��̐錾��SZAB���������܂��B
	�֐������i{}�̒��j���L�q���������B
	�֐������̓G�f�B�^���Œ��ڕҏW�ł��܂��B
======================================================================*/

/*-- SZAB Part  Squeak -------------------------------*/
/*-- SZAB Event Squeak 101 --*/
void SqueakFormRedisp(APLFORMPTR AplForm, WINDOWHD WnHandle)
{
  FormDefRedisp(AplForm, WnHandle);
  if (squeakStarted) {
    fullDisplayUpdate();
  }
}
/*-- SZAB Event End --*/

/*-- SZAB Part  Squeak -------------------------------*/
/*-- SZAB Event Squeak 102 --*/
void SqueakFormSetUp(APLFORMPTR AplForm, MESSAGE* Message)
{
  if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    SetCtrlProperty(Status, LEFT_X_PID, usValue, 40*4, AplForm->Control);
    SetCtrlProperty(Status, WIDTH_PID,  usValue, 130*4-1, AplForm->Control);
    SetCtrlProperty(Status, HEIGHT_PID, usValue, 16*4-1, AplForm->Control);
    
    /* SetCtrlProperty(KEYPAD00, Dummy_Y_PID, usValue, 195*4, AplForm->Control); */

    SetCtrlProperty(SqueakSqLogo, LEFT_X_PID,  usValue, 304*4, AplForm->Control);
    SetCtrlProperty(KeyState, LEFT_X_PID, usValue, 270*4, AplForm->Control);
    SetCtrlProperty(BlueButton, LEFT_X_PID, usValue, 250*4, AplForm->Control);
  }
  
  formSetUpTime = GetSystemTickCount();
}
/*-- SZAB Event End --*/

/*-- SZAB Part  Squeak -------------------------------*/
/*-- SZAB Event Squeak 103 --*/
void SqueakFormEnd(APLFORMPTR AplForm, MESSAGE* Message)
{
  MESSAGE msg;
  short check;


  GetCtrlProperty(KeyState, VALUE_PID, sValue, check,
		  AplForm->Control, short);

  if (check == CHECK_OFF) {
    EntryRedispFunc(AplForm->FormWnHandle, 0, NULL, NULL);
    msg.CommonMsg.MessageID = ToolCloseID;
    ControlCall(KEYPAD00, AplForm->Control, &msg);
  }

  ioExitBody();
  
}
/*-- SZAB Event End --*/

/*-- SZAB Part  Squeak -------------------------------*/
/*-- SZAB Event Squeak 106 --*/
void SqueakFormEventFilter(APLFORMPTR AplForm, MESSAGE* Message)
{
  if (Message->CommonMsg.MessageID == SystemTickID) {
    if (Message->SystemTick.Count - formSetUpTime > 0) {
      main(AplForm);
    }
  }
}
/*-- SZAB Event End --*/

