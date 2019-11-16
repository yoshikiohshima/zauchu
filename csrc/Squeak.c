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

