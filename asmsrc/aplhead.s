;;**********************************************************************
; 						<アプリヘッダ>
;
; <aplhead.s>
;					おおしま
;
;	Squeakのアプリヘッダ(APMが参照する)
;	このファイルはSZABによって生成、更新されるので注意!!
;;**********************************************************************
;-----------------------------------------------------------------------
;	ファイル名宣言
;-----------------------------------------------------------------------

	.file		"aplhead.s"

;-----------------------------------------------------------------------
;	インクルード宣言
;-----------------------------------------------------------------------

	.include	"apmlib.seq"	;アプリヘッダで使う定数定義
	.include	"apladr.seq"	;配置アドレス定義
	.include	"wtii_res.seq"	;リソース定義
	.include	"file.seq"
	.include	"keycode.seq"
	.include	"eventmsg.seq"

;-----------------------------------------------------------------------
;	セクション宣言
;-----------------------------------------------------------------------

	.section	"mblock",.text>WTII_MBLOCK_TOP

;-----------------------------------------------------------------------
;	外部ラベルの参照
;-----------------------------------------------------------------------

	.import		_PIMMain

;-----------------------------------------------------------------------
;	ファイルヘッダ部分
;-----------------------------------------------------------------------
 .if ROM_APPLICATION
block_top:
	.data.b	BLOCK_HEADER_MARK	;ブロックヘッダ
	.sdata	"WTII    APL"	;ファイル名(8bytes+'APL")
	.data.b	IN_ROM_BLOCK		;ブロック属性
	.data.w	0			;ブロック時間
	.data.w	0			;ブロック年月日
	.data.l	block_end-block_top	;ブロックサイズ
	.data.w	0			;ブロックID
	.data.l	block_end-block_top
	.data.l	block_end-block_top
	.data.l	0
	.data.l	0
 .endif
;ファイル先頭
file_top:
	.data.w	FILE_HEADER_ID
	.data.b	FILE_ID_APPLI
	.data.b	0

;-----------------------------------------------------------------------
;	アプリヘッダ本体部分
;-----------------------------------------------------------------------

	.data.l	"WTII"			;アプリケーション識別名(Hiから配置)
	.data.w	CRIMSON_APL_HEADER_VER	;アプリヘッダバージョン
;#AplVersion
	.data.w	0			;アプリケーションバージョン
	.data.w	0			;予約
;#AplType
	.data.w	NEED_PROCEND_MSG			;アプリタイプ
;	.data.w	STICKY			;アプリタイプ(常駐アプリ)

;#AplStackSize
	.data.l	APL_DEF_STACK_SIZE			;@1 アプリが使用するスタックサイズ

;-----------------------------------------------------------------------
;	以下はプロセス起動情報 - アプリのロード形態により変わる
;	この領域は12bytesの大きさの共用体になっている
;-----------------------------------------------------------------------
 .if ROM_APPLICATION
	.import		_wtiiModuleHead
;ROMに配置された場合
	.data.l	_wtiiModuleHead		;アプリモジュールトップアドレス
	.res.b	12			;パディング(サイズに注意...)

;ファイルの場合
 .else
	.sdataz	"F0:WTII.BIN"		;アプリモジュールファイル名
	.res.b	4
 .endif
;-----------------------------------------------------------------------
;	プロセス起動直後に送信するパラメータ情報
;-----------------------------------------------------------------------
;コールバックエントリ
;#AplCallBackEntry
	.data.l	0x00801000			;AplEntry
;APLRES型のパラメータ			;InitAplRes
	.data.w	wtiiPIMInfoID			;ResID
	.data.w	"00"				;ResFNo
;long型のパラメータ			;InitParam
	.data.l	0

;-----------------------------------------------------------------------
;	アプリケーション起動キー情報
;-----------------------------------------------------------------------
;起動キー属性マスク
;APMは無条件にこの値でキー属性をマスクしてから、起動キー属性と比べる
;#ExecKeyMask
	.data.b	0
	.res.b	1	;########

;起動キー情報(２個)
;#ExecKeyInfo
	.data.w	0			;@1 キーコード
	.data.b	0			;アトリビュート
	.data.b	0			;キーサウンド
	.data.w	0			;キーコード
	.data.b	0			;アトリビュート
	.data.b	0			;キーサウンド

	.res.b	2	;########

;-----------------------------------------------------------------------
;	Sleep状態で受けたいイベント情報
;	値はeventmsg.seqのメッセージマスクビットを使用する
;-----------------------------------------------------------------------
;#EventMessage
	.data.l	0			;アクティブ状態で受けたいイベント
	.data.l	0			;非アクティブ状態で受けたいイベント
	.data.l	0			;OUT状態で受けたいイベント

;-----------------------------------------------------------------------
;	使用するヒープの数
;-----------------------------------------------------------------------
;#HeapNumMark
	.data.w	40			;デフォルトは０

;-----------------------------------------------------------------------
;	ブロックエンド
;-----------------------------------------------------------------------
	.align	h'10

block_end:

	.end
