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

/*-- SZAB Part  MemorySize -------------------------------*/
/*-- SZAB Event MemorySize 0 --*/
void MemorySizeSliderSliderSet(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* SldMsg)
{
  switch (SldMsg->SliderSet.Value) {
  case 1:
    memorySize = ((2048+256)*1024);
    break;
  case 2:
    memorySize = ((4096+512)*1024);
    break;
  default:
    memorySize = ((4096+512)*1024);
    break;
  }
}
/*-- SZAB Event End --*/

/*-- SZAB Part  CloseBtn -------------------------------*/
/*-- SZAB Event CloseBtn 0 --*/
void CloseBtnBtnSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* BtnMsg)
{
  ioExit();
}
/*-- SZAB Event End --*/

/*-- SZAB Part  F1FileList -------------------------------*/
/*-- SZAB Event F1FileList 0 --*/
void F1ListFileSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* FLMsg)
{
  extern char shortImageName[DFNAME_LEN];
  extern char vmPath[DFNAME_LEN];

  strcpy(shortImageName, (const char*)FLMsg->FileSelect.FileName);
  strcpy(vmPath, "F1:");

  Message->CommonMsg.MessageID = SqueakScnID;
  AplForm->IsExit = true;
}
/*-- SZAB Event End --*/

/*-- SZAB Part  F0FileList -------------------------------*/
/*-- SZAB Event F0FileList 0 --*/
void F0ListFileSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* FLMsg)
{
  extern char shortImageName[DFNAME_LEN];
  extern char vmPath[DFNAME_LEN];

  strcpy(shortImageName, (const char*)FLMsg->FileSelect.FileName);
  strcpy(vmPath, "F0:");

  Message->CommonMsg.MessageID = SqueakScnID;
  AplForm->IsExit = true;
}
/*-- SZAB Event End --*/

/*-- SZAB Part  F0List -------------------------------*/
/*-- SZAB Event F0List 102 --*/
void F0ListFormSetUp(APLFORMPTR AplForm, MESSAGE* Message)
{
  unsigned short val = 2;

  SetCtrlProperty(MemorySize, VALUE_PID, usValue, val,
		  AplForm->Control);

  if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    SetCtrlProperty(CloseBtn,   LEFT_X_PID, usValue, 304*4, AplForm->Control);
    
    SetCtrlProperty(MemorySize, LEFT_X_PID, usValue, 224*4, AplForm->Control);
    SetCtrlProperty(MemorySize, TOP_Y_PID,  usValue, 210*4, AplForm->Control);
    SetCtrlProperty(MemorySize, WIDTH_PID,  usValue, 96*4-1, AplForm->Control);
    SetCtrlProperty(MemorySize, HEIGHT_PID, usValue, 29*4-1, AplForm->Control);
    
    /* F0FileList, F1FileList �� Program ����͉�ʈʒu��ύX�ł��Ȃ� ;_; */
  }
}
/*-- SZAB Event End --*/

/*-- SZAB Part  F0List -------------------------------*/
/*-- SZAB Event F0List 103 --*/
void F0ListFormEnd(APLFORMPTR AplForm, MESSAGE* Message)
{
  USHORT MemValue;
  int availMem;

  /** set memory from MemorySlider **/
  GetCtrlProperty(MemorySize, VALUE_PID, usValue, MemValue, 
		  AplForm->Control, USHORT);

  availMem = GetMaxAvailHeapSize();

  switch (MemValue) {
  case 1:
    if (availMem > 8192*1024)
     memorySize = ((4096+512)*1024);
    else if (availMem < 2048*1024)
     memorySize = ((512)*1024);
    else
     memorySize = ((2048+256)*1024);
    break;
  case 2:
  default:
    if (availMem > 8192*1024)
     memorySize = ((8192+4096+512)*1024);
    else if (availMem < 2048*1024)
     memorySize = ((1024+512)*1024);
    else
     memorySize = ((4096+512)*1024);
    break;
  }

  if(availMem <= memorySize)
    memorySize = availMem - 100*1024;



}
/*-- SZAB Event End --*/

