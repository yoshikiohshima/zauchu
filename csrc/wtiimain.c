/****************************************************************
 <wtiimain.c>
					おおしま

		SqueakPIMクラスメソッド

*****************************************************************/
#include <zheaders.h>
#include <wtii.h>
#include "wtii_C_usr.h"

//======================================================================
//  		PIMクラスメソッド
//======================================================================
BOOLEAN
PIMAplConstruct(PIMINFO *PIMInfo,USHORT InitLevel)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

	// アプリケーションプロセス起動時に作業用メモリの確保などの処理
	// が必要な場合は、ここに記述して下さい。

	return true;
}
//----------------------------------------------------------------------
void
PIMAplEnd(PIMINFO *PIMInfo,MESSAGE *Message)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

	// 注意！ - この関数はデフォルトではコールされません。
	// 必要な場合は、IDEの[ツール]-[アプリケーション設定]の
	// アプリケーションタイプタブで、[プロセス終了メッセージ要求]
	// をチェックして下さい。

	// アプリケーションプロセス終了時に処理が必要な場合は、ここに
	// 記述して下さい。
   ioExitBody();

}
#if useDefaultFunction
//----------------------------------------------------------------------
BOOLEAN
PIMAplSetup(PIMINFO *PIMInfo,MESSAGE *Message,USHORT InitLevel)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

#ifdef VGA_140DPI_APL
//この部分は、VGA用に作成したMOREソフトをVGA画面ではないザウルスで
//実行しようとしたときに警告を出し、プロセス起動をしないためのコードです
{
	MESSAGE msg;
	UCHAR	ModelName[MODEL_NAME_MAX];
	ULONG	Version;
	UPOINT	ScreenSize;
	DPI	DPISize;

	/* システム情報を取ってくる */
	GetSystemInfo(ModelName,&Version);

	if((Version>>16) < MODEL_NUMBER_MIEX1){
		/* MI-EX1以前の機種の場合（実行できない） */
		/* 警告メッセージを表示 */
		msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
		AlertExecStr(&msg,(STRING)"この機種では実行できません",
				ALB_CENTER, 0);
		/* エラーとしてプロセス終了する */
		SetFailMessage(Message);
		return false;
	}else{
		/* MI-EX1（を含む）以降の機種の場合 */
		/* 画面サイズが640*480ドット以上なら動作するようにする */
		GetScreenSizeAndDPI(&ScreenSize, &DPISize);
		if((ScreenSize.x < 640) || (ScreenSize.y < 480)){
			/* VGA（以上）ではないので実行できない */
			/* 警告メッセージを表示 */
			msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
			AlertExecStr(&msg,(STRING)"この機種では実行できません",
				ALB_CENTER, 0);
			/* エラーとしてプロセス終了する */
			SetFailMessage(Message);
			return false;
		}
	}
}
#endif /* VGA_140DPI_APL */


	// アプリケーションプロセス起動時に初期化やチェックなどの処理
	// が必要な場合は、ここに記述して下さい。


	return true;
}
//----------------------------------------------------------------------
#endif
void
PIMAplOutMsg(PIMINFO *PIMInfo,MESSAGE *Message)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

	// アプリケーションがアウト中に受信したメッセージに対して処理
	// が必要な場合は、ここに記述して下さい。

}
//----------------------------------------------------------------------
BOOLEAN
PIMExecConstruct(PIMEXECWORK *PIMExecWork,MESSAGE *Message)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

	// アプリケーションがEXEC中に使用する作業用メモリの確保などが
	// 必要な場合は、ここに記述して下さい。

	return true;
}
//----------------------------------------------------------------------
void
PIMExecEnd(PIMEXECWORK *PIMExecWork,MESSAGE *Message)
{
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

	// アプリケーションがアウト状態に移行するときに処理
	// が必要な場合は、ここに記述して下さい。

	// アウト状態を持たないアプリケーションの場合は、以下の２行の
	// コメントを外して下さい。
	// アウト状態に移行するときにプロセス終了する処理
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
	// 関数名を変更する場合は、IDEの[ツール]-[アプリケーション設定]
	// のPIMクラスメソッドタブに表示される関数名も変更して下さい。

#ifdef VGA_140DPI_APL
	WIN_WORK_EXT WinWorkExt;

	/* PIMクラスのウィンドウオープンをやめる */
	PIMExecWork->WhichWindow = PIM_WIN_NO_OPEN;

	/* VGAウィンドウを開き、PIMクラスがそのウィンドウを使用するように */
	/* 設定する */
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


	// アプリケーションがアウト状態からEXEC状態に移行するときに処理
	// が必要な場合は、ここに記述して下さい。


	return true;
}
#endif


BOOLEAN
PIMExecSetup(PIMEXECWORK *PIMExecWork, MESSAGE *Message)
{
  WIN_WORK_EXT WinWorkExt;
  /* PIMクラスのウィンドウオープンをやめる*/
  PIMExecWork->WhichWindow = PIM_WIN_NO_OPEN;
  /* VGAウィンドウを開き、PIMクラスがそのウィンドウを使用するように設定する*/
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
  
  /* システム情報を取ってくる*/
  GetSystemInfo(ModelName,&Version);
  if((Version>>16) < MODEL_NUMBER_MIEX1) {
    /* MI-EX1以前の機種の場合（実行できない）*/
    /* 警告メッセージを表示*/
    msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
    AlertExecStr(&msg,(STRING)"This binary is not appropriate for this machine",
		 ALB_CENTER, 0);
    /* エラーとしてプロセス終了する*/
    SetFailMessage(Message);
    return false;
  } else {
    /* MI-EX1（を含む）以降の機種の場合*/
    GetScreenSizeAndDPI(&ScreenSize, &DPISize);
    if ((ScreenSize.x == 640) && (ScreenSize.y == 480)) {
      zaurusScreenSize = ZAURUS_SCREEN_VGA;
    } else if ((ScreenSize.x == 320) && (ScreenSize.y == 240)) {
      zaurusScreenSize = ZAURUS_SCREEN_QVGA;
    } else {
      /* 警告メッセージを表示*/
      msg.CommonMsg.MessageID = AlboxWaitDispOpenID;
      AlertExecStr(&msg,(STRING)"This binary is not appropriate for this machine",
		   ALB_CENTER, 0);
      /* エラーとしてプロセス終了する*/
      SetFailMessage(Message);
      return false;
    }
  }
  return true;
}
