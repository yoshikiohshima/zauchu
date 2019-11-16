/***********************************************************************
<アプリケーションイベント関数>

 <wtii_evc.c>
					おおしま

		Squeakイベント関数

***********************************************************************/
/*
	このファイルはSZABが生成し更新します。
	このファイルを直接編集する時には、必ず下記注意事項を守ってください。
	そうでない場合は、
	　・SZABが動作しなくなる
	　・ユーザ様の編集結果が失われる
	可能性があります。

	*******　注意事項　*******
	[1] 下記の場所のみ編集できます
	      ・ヘッダのinclude  ★1コメントの間
	    　・各イベント関数の内部（関数{}の中）

		これ以外の箇所を編集しないでください。
		特にコメント行で キーワード"SZAB"が含まれている行を絶対に
		変更・削除しないでください。

	[2] このファイルをエディタ等で直接編集する時、
	　  SZABフォームエディタは必ず終了しておいてください。

	  　SZABフォームエディタはこのファイルに書き込みます。
	  　２プログラムから本ファイルへ書き込むことになりますので
	  　一貫性が保てなくなる可能性があります。

	[3] ユーザ様で開発いただいた関数（＝SZABが生成した関数以外の関数）
	　　は、このファイルとは別のソースファイルへ置いてください
	　　このファイルに置くと、（置いた場所によっては）SZABにより
	　　削除される可能性があります。
*/

/*======================================================================
    include
======================================================================*/
#include <zheaders.h>
#include <pimmgr.h>
#include <wtii_res.h>

#include "wtii_C_usr.h"

/* ★1  以下に（このフォーム用の）あなた独自のincludeを追加してください */
/* #include "your_header.h" */
/* ★1  ここまで */


/*======================================================================
    イベント関数
	イベント関数の宣言はSZABが生成します。
	関数内部（{}の中）を記述ください。
	関数内部はエディタ等で直接編集できます。
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
    
    /* F0FileList, F1FileList は Program からは画面位置を変更できない ;_; */
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

