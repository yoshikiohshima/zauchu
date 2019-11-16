/***********************************************************************
 SH3										<分類>
		
 							Time-stamp: <99/02/08 10:44:38 yasui>
 <lcdzdm.h>
								シャープ株式会社
								Sharp Corporation
								携帯システム事業部  第１技術部
					personal intelligent systems Div. engineering Dept.1
										安井 鉄也
										T.Yasui
		Zaurus Draw マネージャ

 ver. 0		1997/03/18		created by T.Yasui for Opal
 ver. 1		<date>		<変更者氏名>
		<このバージョンの内容>
***********************************************************************/
#ifndef	_LCDZDM_H_DEFINED

#include <zdrawlib.h>
#include <pinklib.h>

/* --------------------------------------------------------------------
   ● 非公開API
   ◎ 公開API - (ZDRAW構造体使用)
   ○ 公開API - (ZDRAW構造体未使用)
--------------------------------------------------------------------- */

/***********************************************************************
  【初期化】
  ●(01) LcdInitDM()
***********************************************************************/
/* 初期化レベル */
typedef UCHAR INITLEV;
#define	ZDRAW_ALL_RESET	(0x00)	/* オールリセット */
#define	ZDRAW_RESET		(0x01)	/* リセット */
#define	ZDRAW_OFF		(0x02)	/* オフ */
#define	ZDRAW_ON		(0x03)	/* オン */
#define	ZDRAW_RESET_SEM	(0x04)	/* リセット (セマフォ用) */

/***********************************************************************
  【通常文字描画】
  ◎(02) Disp1CharDM()
  ◎(03) DispNCharDM()
***********************************************************************/
/* 文字描画開始座標位置の指定 */
typedef	USHORT	STARTPOINT;
#define	STP_LEFT_LOWER	(0x00)	/* 左下 */
#define	STP_LEFT_UPPER	(0x01)	/* 左上 */
#define	STP_RIGHT_LOWER	(0x02)	/* 右下 */
#define	STP_RIGHT_UPPER	(0x03)	/* 右上 */

/* 文字連続描画方向の指定 */
typedef	USHORT	DISPANGLE;
#define	DSPA_NORMAL		(0x00)	/* 左→右 */
#define	DSPA_REVERSE	(0x01)	/* 右→左 */
#define	DSPA_TOP_DOWN	(0x02)	/* 上→下 */
#define	DSPA_BOTTOM_UP	(0x03)	/* 下→上 */

/* 文字のサイズ (zdrawlib.h に定義) */
typedef	UCHAR	FONTSIZE;

/*  文字の種類 */
typedef	ULONG	FONTTYPE;
#define	FT_FJNM	'FJNM'		/* 国内 Fixedフォント(System FONT) */
#define	FT_FJZ2	'FJZ2'		/* 国内 Fixedフォント(PI-2 FONT) */
#define	FT_FJZ1	'FJZ1'		/* 国内 Fixedフォント(Zaurus FONT) */
#define	FT_FJTK	'FJTK'		/* 国内 Fixedフォント(Tokei FONT) */
#define	FT_FJCL	'FJCL'		/* 国内 Fixedフォント(Calendar FONT) */
#define	FT_FJJT	'FJJT'		/* 国内 Fixedフォント(Jiten FONT) */
#define	FT_FE05	'FE05'		/* 海外横5dot Fixedフォント */
#define	FT_FE03	'FE03'		/* 海外横3dot Fixedフォント */
#define	FT_PE05	'PE05'		/* 海外横5dot Proportionalフォント */
#define	FT_PE03	'PE03'		/* 海外横3dot Proportionalフォント */
#define FT_FJKY	'FJKY'		/* 新カレンダーフォント(Koyomi) */
#define FT_FJDN	'FJDN'		/* 新電卓フォント(Dentaku) */

#pragma H2EQU_ASMONLY .macro	_FJNMFont
#pragma H2EQU_ASMONLY	.data.l	"FJNM"
#pragma H2EQU_ASMONLY .endm
#pragma H2EQU_ASMONLY .macro	_FE05Font
#pragma H2EQU_ASMONLY	.data.l	"FE05"
#pragma H2EQU_ASMONLY .endm
#pragma H2EQU_ASMONLY .macro	_FE03Font
#pragma H2EQU_ASMONLY	.data.l	"FE03"
#pragma H2EQU_ASMONLY .endm
#pragma H2EQU_ASMONLY .macro	_PE05Font
#pragma H2EQU_ASMONLY	.data.l	"PE05"
#pragma H2EQU_ASMONLY .endm
#pragma H2EQU_ASMONLY .macro	_PE03Font
#pragma H2EQU_ASMONLY	.data.l	"PE03"
#pragma H2EQU_ASMONLY .endm

/* 文字修飾情報 */
typedef	USHORT	FONTSTYLE;
#define	FS_NORMAL		(0x00)		/* 通常スタイル */
#define	FS_BOLD			(0x01)		/* ボールド */
#define	FS_ITALIC		(0x02)		/* イタリック */
#define	FS_UNDERLINE	(0x04)		/* 下線付き */
#define	FS_SHADOW		(0x08)		/* 影文字(使用不可) */

/* 文字回転角度 */
typedef	USHORT	FONTANGLE;
#define	FA_NO_ROTATION	(0x0000)	/* 回転なし*/
#define	FA_180_ROTATION	(0x0001)	/* 180度回転 */
#define	FA_R90_ROTATION	(0x0002)	/* 右90度回転 */
#define	FA_L90_ROTATION	(0x0003)	/* 左90度回転 */

/* 文字の形状 */
typedef struct FONTINFO {
	FONTSIZE	FontSize;		/* 文字サイズ */
	FONTTYPE	FontType;		/* フォントタイプ(名前４文字) */
	FONTSTYLE	FontStyle;		/* 文字修飾情報 */
	FONTANGLE	FontAngle;		/* 文字回転角度 */
} FONTINFO;

typedef struct DISP1CHR {
	short		Pitch;			/* 文字間隔 (QP) */
	USHORT		SJISCode;		/* shift JIS code */
	STARTPOINT	StartPoint;		/* 開始点情報 */
	DISPANGLE	DispAngle;		/* 連続描画方向 */
	FONTINFO	FontForm;		/* 文字形状 */
} DISP1CHR;

typedef struct DISPNCHR {
	short		Pitch;			/* 文字間隔 (QP) */
	USHORT		Len;			/* データバイト数(ＮＵＬＬは含まず) */
	UCHAR		*Str;			/* データ先頭アドレス */
	STARTPOINT	StartPoint;		/* 開始点情報 */
	DISPANGLE	DispAngle;		/* 連続描画方向 */
	FONTINFO	FontForm;		/* 文字形状 */
} DISPNCHR;

/***********************************************************************
  【点描画】
  ◎(04) DispPointDM()
***********************************************************************/
typedef struct DISPPOINT {
	DOTSIZE		DotSize;		/* 点の大きさ(QP) */
	DOTSHAPE	DotShape;		/* 点形状 */
} DISPPOINT;

/***********************************************************************
  【線描画】
  ◎(05) DrawLineDM()
  ◎(06) DrawContLineDM()
  ◎(07) BoxLineDM()
***********************************************************************/
/* 線のパターンデータ */
typedef	long	LINEPTD;

typedef struct DRAWLINE {
	DOTSIZE		DotSize;		/* 点の大きさ(QP) */
	DOTSHAPE	DotShape;		/* 点形状 */
	LINEPAT		LinePt;			/* 線形状 */
	LINEPTD		LinePtd;		/* 線のパターンデータ */
} DRAWLINE;

typedef struct BOXLINE {
	DOTSIZE		DotSize;		/* 点の大きさ(QP) */
	DOTSHAPE	DotShape;		/* 点形状 */
	LINEPAT		LinePt;			/* 線形状 */
	LINEPTD		LinePtd;		/* 線のパターンデータ */
} BOXLINE;

/***********************************************************************
  【塗りつぶし】
  ◎(08) LcdClsDM()
  ◎(09) AreaClearDM()
  ◎(10) AreaDarkDM()
  ◎(11) BoxPaintDM()
***********************************************************************/
typedef struct BOXPAINT {
	PAINTPAT	PaintPt;		/* ペイントパターン */
	UPOINT		PaintOfs;		/* ペイントパターン開始座標オフセット */
} BOXPAINT;


/***********************************************************************
  【アトリビュート付き文字描画】
  ◎(12) DispStyledTextDM()
***********************************************************************/
/* アトリビュート付き文字列表示の表示属性 */
typedef USHORT STR_ATR;

/*
   STR_ATR : 
   ┌────┬──────┬─────┬──────┐
   │FType(3)│  FColor(5) │ FSize(3) │  FStyle(5) │
   └────┴──────┴─────┴──────┘
   *()内の値はビット数。

   FType  : 文字の種類
   FColor : 文字の色
   FSize  : 文字のサイズ
   FStyle : 文字修飾情報
*/

/* 表示属性に関する文字の種類 */
#define	STRA_MINCHO		(0x0000)
/*	Reserve				(0x2000) */
/*	Reserve				(0x4000) */
/*	Reserve				(0x6000) */
/*	Reserve				(0x8000) */
/*	Reserve				(0xc000) */
/*	Reserve				(0xe000) */

#define STRA_TYPE_MASK	(0xe000)

/* 表示属性に関する文字の色 ( 未定 ) */
#define	STRA_BLACK		(0x0000)
#define	STRA_RED		(0x0100)
#define	STRA_GREEN		(0x0200)
#define	STRA_BLUE		(0x0300)
#define	STRA_WHITE		(0x0400)

#define STRA_COLOR_MASK	(0x1f00)

/* 表示属性に関する文字のサイズ */
#define	STRA_40QP		(0x0000)		/* 高さ１０ドット文字 */
#define	STRA_48QP		(0x0020)		/* 高さ１２ドット文字 */
#define	STRA_64QP		(0x0040)		/* 高さ１６ドット文字 */
#define	STRA_96QP		(0x0060)		/* 高さ２４ドット文字 */
/*	Reserve				(0x0080) */
/*	Reserve				(0x00a0) */
/*	Reserve				(0x00c0) */
/*	Reserve				(0x00e0) */

#define STRA_SIZE_MASK	(0x00e0)

/* 表示属性に関する文字修飾情報 */
#define	STRA_PLANE		(0x0000)		/* 修飾なし */
#define	STRA_BOLD		(0x0001)		/* ボールド指定 */
#define	STRA_ITALIC		(0x0002)		/* イタリック指定 */
#define	STRA_UNDERLINE	(0x0004)		/* アンダーライン指定 */
/*	Reserve				(0x0008) */
/*	Reserve				(0x0010) */

#define	STRA_STYLE_MASK	(0x001f)

#define	STRA_DEFAULT	(STRA_MINCHO|STRA_BLACK|STRA_48QP|STRA_PLANE)

/* アトリビュート付き文字列表示の文字列データ */
typedef USHORT	STR_CHAR;

/* アトリビュート付き文字列表示の表示方法に関するビット情報 */
typedef	UCHAR STR_INFO;
#define	WORD_WRAP_ON	(0x01)	/* ワードラップ機能 ON */
#define	SYMBOL_09_ON	(0x02)	/* TAB(0x09)シンボル表示機能 ON */
#define	SYMBOL_0C_ON	(0x04)	/* 改頁(0x0c)シンボル表示機能 ON */
#define	SYMBOL_0D_ON	(0x08)	/* 改行(0x0d)シンボル表示機能 ON */
#define	CLEAR_OFF		(0x10)	/* 文字表示可能領域クリア OFF */

typedef struct STR_TAB_TBL {
    USHORT		nTabs;			/* ＴＡＢ位置設定数（0〜MAX_TABS）*/
    USHORT		TabPos[8];		/* ＴＡＢ位置 (QP) */
} STR_TAB_TBL;
#define	MAX_TABS	(8)			/* TabPos[MAX_TABS] */

typedef struct DISPSTEXT {
	short		Pitch;			/* 文字間隔 (QP) */
	USHORT		Len;			/* 表示文字数 */
	STR_CHAR	*Str;			/* 表示文字列 */
	STR_ATR		*AttrPtr;		/* 表示属性 */
	STR_TAB_TBL	*TabTblPtr;		/* TAB 位置 */
	USHORT		Indent;			/* インデント位置 */
	USHORT		SelSAdr;		/* セレクト開始オフセット */
	USHORT		SelEAdr;		/* セレクト終了オフセット */
	COLOR		SelRevC;		/* セレクト反転色 */
	STR_INFO	Info;			/* 表示方法に関する情報 */
    USHORT		Height;			/* 最大表示高さ (QP) */
    USHORT		Width;			/* 表示幅 (QP) */
} DISPSTEXT;

/***********************************************************************
  【アイコン描画】
  ◎(13) DispIconDM()
***********************************************************************/
typedef struct DISPICN {
	ICONDATA	*IcnData;
	ICONMODE	IcnMode;
} DISPICN;

/***********************************************************************
  【イメージ描画】
  ●(14) DispImageDM()
  ◎(15) DispZPMImageDM()
***********************************************************************/
typedef union IMGPTR {
	void	 	*ImgData;
	IMAGEDATA	*ImgDataW;		/* データサイズ 2byte */
	IMAGEDATA_L	*ImgDataL;		/* データサイズ 4byte拡張 (PI-2) */
} IMGPTR;

typedef struct DISPIMG {
	IMGPTR		ImgPtr;
} DISPIMG;

typedef struct DISPZPMIMG {
	USHORT		*ZPMImgData;
} DISPZPMIMG;

/***********************************************************************
  【領域の退避/復帰】
  ◎(16) DispPushDM()	-	実際公開するAPIは、PutImageToBufferL()
  ◎(17) DispPopDM()	-	実際公開するAPIは、DispImageDM()
***********************************************************************/
typedef UCHAR DPBUFTYPE;	/* バッファタイプ指定 */

#define	DP_NEW_HEAP			(0x00)		/* 新規ヒープ (size 2byte) */
#define	DP_NEW_OFFSCRN		(0x01)		/* 新規オフスクリーン */
#define	DP_PTR				(0x02)		/* 指定バッファ (size 2byte) */
#define	DP_EXIST_OFFSCRN	(0x03)		/* 指定オフスクリーン */
#define	DP_NEW_HEAP_L		(0x04)		/* 新規ヒープ (size 4byte) */
#define	DP_PTR_L			(0x05)		/* 指定バッファ (size 4byte) */

typedef struct DPEXISTOFS {
	WINDOWHD	OSHandle;
	RECTANGLE	OSRect;
	char		dumy1;		/* for h2equ bug 回避 */
	char		dumy2;		/* for h2equ bug 回避 */
} DPEXISTOFS;

typedef struct DPPTR {
	IMGPTR		ImgPtr;
	ULONG		ImgSize;	/* USHORT ImgSize; */
} DPPTR;

typedef	union DPBUFINFO {
	DPPTR		Ptr;
	DPEXISTOFS	ExistOS;
} DPBUFINFO;

typedef struct DISPPUSHPOP {
	DPBUFTYPE	BufType;
	DPBUFINFO	BufInfo;
	IMGTYPE		DataType;		/* IMAGE_16BIT のみサポート */
	UCHAR		reserved0;
	USHORT		reserved1;		/* 必ず0にする事！ */
	ULONG		reserved2;		/* 必ず0にする事！ */
} DISPPUSHPOP;

/***********************************************************************
  【領域の移動】
  ◎(18) BoxMoveDM()
  ◎(19) ScrollUpDM()
  ◎(20) ScrollDownDM()
  ◎(21) SmoothScrollDM()	+++ PI-2 で追加
***********************************************************************/
typedef UCHAR BMCLRINFO;
#define	BM_CLEAR	1

typedef struct BOXMOVE {
	short		MoveX;
	short		MoveY;
	BMCLRINFO	ClrInfo;
} BOXMOVE;

typedef struct SCROLL {
	USHORT 		Distance;
	BMCLRINFO	ClrInfo;
} SCROLL;

typedef struct SMSCROLL {
	SMSPTN		pattern;
	SMSDIRCT	direction;
	short		distance;
	USHORT		time;
	USHORT		*pixmapUD;
	USHORT		*pixmapLR;
	ULONG		reserved1;
} SMSCROLL;

/***********************************************************************
  【縦パターン描画】
  ◎(22) DispNVPatternDM()
  ◎(23) ReadPointDM()
***********************************************************************/
typedef struct DISPVPTN {
	USHORT		VPtn;
} DISPVPTN;

typedef struct DISPNVPTN {
	USHORT		*NVPtnPtr;
	USHORT		Len;
} DISPNVPTN	;

/***********************************************************************
  【点の読みだし】
  ◎(22) DispNVPatternDM()
  ◎(23) ReadPointDM()
***********************************************************************/
typedef struct READPOINT {
	USHORT		ReadC16;			/* 16ビットカラー格納先 */
} READPOINT;

/***********************************************************************
  【カーソルに関する処理】
  ● CursorBlinkDM()
  ○ CursorMoveDM()
  ○ CursorDispDM()
  ○ GetCursorInfoDM()
***********************************************************************/
/* カーソルタイプ */
typedef USHORT CRSTYPE;
#define	WN_CRS_OFF			(0x00)	/* カーソルOFF 状態 */
#define	WN_CRS_ON			(0x01)	/* カーソルON 状態 */
#define	WN_CRS_BLINK		(0x08)	/* ブリンク指定 */
#define	WN_CRS_ONOFF_MASK	(0x07)	/* カーソルタイプ情報マスク用 */
#define	WN_CRS_SIZE_MASK	(0xf0)	/* カーソルサイズマスク用 */

typedef struct CURSOR {
	WINDOWHD	WnHandle;
	POINT		Start;			/* 左下座標の指定 */
	CRSTYPE		CrsType;
	COLOR		Fg;
	COLOR		Bg;
} CURSOR;

/***********************************************************************
  【インク描画】
  ● DispPinkDM()
  ● Disp1BundleDM()
***********************************************************************/

/***********************************************************************
  【色情報取得】
  ○ GetRealColorDM()
  ○ GetGuiNoDM()
***********************************************************************/
typedef struct GETCOLOR {
	GUICOLOR	GCNo;
	COLOR		*RealCPtr;
	GCTABLE		GCTblNo;
} GETCOLOR;

/***********************************************************************
  【画面サイズ情報取得】
  ○ GetScreenSizeDM()
***********************************************************************/

/***********************************************************************
  【文字データ情報取得】
  ○ ChrFontDM()
  ○ GetFontDM()
***********************************************************************/
typedef struct CHRFONT {
	USHORT		SJISCode;		/* IN  : shift JIS code */
	FONTINFO	*FontFormPtr;	/* IN  : フォント形状先頭アドレス */
	USHORT		*CgPtr;			/* OUT : ＣＧデータ先頭(ROM)アドレス */
	UCHAR		HLoops;			/* OUT : 縦ループ値 */
	UCHAR		HDot;			/* OUT : 縦ドット数 */
	USHORT		WLoops;			/* OUT : 横幅ループ値 */
	USHORT		LastWLoopsMask;	/* OUT : 最下横ループマスク */
} CHRFONT;

typedef struct GETFONT {
	USHORT		SJISCode;		/* IN  : shift JIS code */
	FONTINFO	*FontFormPtr;	/* IN  : フォント形状先頭アドレス */
	USHORT		*CgBufPtr;		/* IN  : ＣＧデータ格納先頭アドレス */
	UCHAR		Height;			/* OUT : 高さ(dot) */
	UCHAR		Width;			/* OUT : 幅(dot) */
	USHORT		*CgPtr;			/* OUT : ＣＧデータ先頭(ROM)アドレス */
} GETFONT;

/***********************************************************************
  【ダイアグ】
  ●LCDDiagDM()
***********************************************************************/

/***********************************************************************
  【バックライト】
  ○SetLightDM()
  ○GetLightDM()
***********************************************************************/

/***********************************************************************
  【ＶＲＡＭ先頭アドレス取得/直接描画指定】
  ● EnableAccessVRAM()
  ● DisableAccessVRAM()
  ● GetVramTop()
***********************************************************************/
typedef UCHAR ACSVRAM;
#define	VRAM_DIRECT_ENABLE	(0x01)	/* VRAM 直接描画可能状態 */

/***********************************************************************
  【MI-100専用(白黒/グレー)】
  ● GetVramTop()
  ● SetLcdMode()
  ● GetLcdMode()
  ● GetGrayColor()
  ● SetLcdContrastDM()
  ● GetLcdContrastDM()
***********************************************************************/
typedef UCHAR LCDMODE;
#define	LMD_GRAY1		(0x00)
#define	LMD_GRAY4		(0x01)
#define	LMD_COLOR16		(0x02)

typedef UCHAR RDSPMODE;
#define	RMD_NODISP		(0x00)
#define	RMD_KEEP		(0x01)
#define	RMD_CLEAR		(0x02)
#define	RMD_ALLREDISP	(0x03)
#define	GRAY1_MASK		(0x01)
#define	GRAY4_MASK		(0x0f)

/* ＬＣＤコンントラスト値の定義 */
#define	LCD_CONTRAST_MIN		(0x00)
#define LCD_CONTRAST_MAX		(0x3f)
#define	LCD_CONTRAST_DEFAULT	(0x0f)

/***********************************************************************
  【グラデーション描画】
***********************************************************************/
typedef struct GRDRAW {
    COLOR	startcolor;
	COLOR	endcolor;
	COLOR	edge1;
	COLOR	edge2;
	USHORT	pitch;
} GRDRAW;

/***********************************************************************
  【角丸四角描画】
***********************************************************************/
typedef struct DRDRAW {
    COLOR	fill;
	COLOR	inner;
	COLOR	edge;
} DRDRAW;

/***********************************************************************
  描画指定ＡＰＩ共通構造体 (◎印のAPIで使用)
***********************************************************************/
typedef	union DISP_PARAM {
	DISP1CHR	Dsp1C;
	DISPNCHR	DspNC;
	DISPSTEXT	DspST;
	DISPPOINT	DspP;
	DRAWLINE	DrwL;
	DRAWLINE	DrwCL;
	BOXLINE		BoxL;
	BOXPAINT	BoxP;
	DISPICN		DspIcn;
	DISPIMG		DspImg;
	DISPZPMIMG	DspZPMImg;
	DISPPUSHPOP	DspPP;
	BOXMOVE		BoxMv;
	SCROLL		Scroll;
	DISPVPTN	DspVPtn;
	DISPNVPTN	DspNVPtn;
	READPOINT	ReadP;
	SMSCROLL	SMScroll;
	GRDRAW		GrDraw;
	DRDRAW		DrDraw;
} DISP_PARAM;

typedef struct ZDRAW {
	UCHAR		FuncNo;		/* ファンクションNo. (未使用) */
	WINDOWHD	WnHandle;	/* ウィンドウハンドル */
	CDSPMODE	Mode;		/* 表示モード */
 	POINT   	Start; 		/* ウィンドウ内開始点座標 */
	POINT		End;        /* ウィンドウ内終了点座標 */
	RECTANGLE	Mask;		/* フレームマスク領域 */
	COLOR		Fg;			/* フォアグラウンドカラー */
	COLOR		Bg;			/* バックグラウンドカラー */
	DISP_PARAM	DspParam;	/* 各種描画パラメータ */
} ZDRAW;

/***********************************************************************
  ファンクションナンバ定義
***********************************************************************/
#define	TOP_FUNC				(0x40)

#define	LCD_INIT_ZDM			(0x40)	/* 初期化 */
#define	DISP_1_CHAR_ZDM			(0x41)	/* １文字表示 */
#define	DISP_N_CHAR_ZDM			(0x42)	/* Ｎ文字表示 */
#define	DISP_POINT_ZDM			(0x43)	/* １点表示 */
#define	DRAW_LINE_ZDM			(0x44)	/* 線を描く */
#define	DRAW_CONT_LINE_ZDM		(0x45)	/* 連続描画用の線を描く */
#define	BOX_LINE_ZDM			(0x46)	/* 長方形を描く */
#define	CLS_LCD_ZDM				(0x47)	/* 全画面クリア */
#define	AREA_CLEAR_ZDM			(0x48)	/* 指定範囲のクリア */
#define	AREA_DARK_ZDM			(0x49)	/* 指定範囲の影クリア */
#define	BOX_PAINT_ZDM			(0x4a)	/* 矩形領域のペイント */
#define	DISP_STYLED_TEXT_ZDM	(0x4b)	/* アトリビュート付 Ｎ文字表示 */
#define	DISP_ICON_ZDM			(0x4c)	/* アイコン表示 */
#define	DISP_IMAGE_ZDM			(0x4d)	/* イメージ表示 */
#define	DISP_ZPMIMAGE_ZDM		(0x4e)	/* ZPMイメージ表示 */
#define	DISP_PUSH_ZDM			(0x4f)	/* 矩形領域の退避 */
#define	DISP_POP_ZDM			(0x50)	/* 矩形領域の復帰 */
#define	BOX_MOVE_ZDM			(0x51)	/* 矩形の移動 */
#define	SCROLL_UP_ZDM			(0x52)	/* 上スクロール */
#define	SCROLL_DOWN_ZDM			(0x53)	/* 下スクロール */
#define	DISP_V_PATTERN_ZDM		(0x54)	/* 縦16ドットパターン表示 */
#define	DISP_N_V_PATTERN_ZDM	(0x55)	/* 複数縦16ドットパターン表示 */
#define	READ_POINT_ZDM			(0x56)
/* 【PI-2〜】 */
#define	SMOOTH_SCROLL_ZDM		(0x57)

#define	CURSOR_BLINK_ZDM		(0x60)
#define	CURSOR_MOVE_ZDM			(0x61)
#define	CURSOR_DISP_ZDM			(0x62)
#define	GET_CURSOR_INFO_ZDM		(0x63)
#define	DISP_PINK_ZDM			(0x64)
#define	DISP_1BUNDLE_ZDM		(0x65)
#define	GET_REAL_COLOR_ZDM		(0x66)
#define	GET_GUI_NO_ZDM			(0x67)
#define	GET_SCREEN_SIZE_ZDM		(0x68)
#define	CHR_FONT_ZDM			(0x69)
#define	GET_FONT_ZDM			(0x6a)
#define	LCD_DIAG_ZDM			(0x6b)
#define	SET_LIGHT_ZDM			(0x6c)
#define	GET_LIGHT_ZDM			(0x6d)
#define	ENABLE_ACCESS_VRAM_ZDM	(0x70)
#define	DISABLE_ACCESS_VRAM_ZDM	(0x71)
#define	GET_VRAM_TOP_ZDM		(0x72)

/* 【MI-100〜】*/
#define	SET_LCD_MODE_ZDM		(0x73)
#define	GET_LCD_MODE_ZDM		(0x74)
#define	GET_GRAY_COLOR_ZDM		(0x75)
#define	SET_LCD_CONTRAST_ZDM	(0x76)
#define	GET_LCD_CONTRAST_ZDM	(0x77)

/* 【MI-310〜】 */
#define	ROUND_BOX_PAINT_ZDM		(0x78)

/* 【PI-2〜】 */
#define	IS_POWER_SAVE_ZDM		(0x79)

#define	DRAW_GRADATION			(0x80)
#define	DRAW_ROUND_AREA			(0x81)

#define	LAST_FUNC		DRAW_ROUND_AREA

/* 【表示ツール】 */
#define	CHAR_SIZE_ZDM				(0x40)
#define	GET_WIDTH_ZDM				(0x41)
#define	GET_LENGTH_FROM_WIDTH_ZDM	(0x42)
#define	GET_STRING_SIZE_ZDM			(0x43)
#define	CHAR_SIZE_ZDM_FROM_DPI		(0x48)
#define	CHAR_SIZE_EXT				(0x50)
#define	CHAR_SIZE_FROM_DPI			(0x58)

void LcdInitDM(INITLEV);
void Disp1CharDM(ZDRAW *);
void DispNCharDM(ZDRAW *);
void DispPointDM(ZDRAW *);
void DrawLineDM(ZDRAW *);
void DrawContLineDM(ZDRAW *);
void BoxLineDM(ZDRAW *);
void LcdClsDM(ZDRAW *);
void AreaClearDM(ZDRAW *);
void AreaDarkDM(ZDRAW *);
void BoxPaintDM(ZDRAW *);
void DispStyledTextDM(ZDRAW *);
void DispIconDM(ZDRAW *);
void DispImageDM( ZDRAW *);
void DispZPMImageDM(ZDRAW *);
void DispPushDM(ZDRAW *);
void DispPopDM(ZDRAW *);
void BoxMoveDM(ZDRAW *);
void ScrollUpDM(ZDRAW *);
void ScrollDownDM(ZDRAW *);
void DispVPatternDM(ZDRAW *);
void DispNVPatternDM(ZDRAW *);
void ReadPointDM(ZDRAW *);
void SmoothScrollDM(ZDRAW *);
void DrawGradiation(ZDRAW *);
void DrawRoundArea(ZDRAW *);

void CursorBlinkDM(void);			/* PI-2でスペック変更 */
void CursorMoveDM(CURSOR *);
void CursorDispDM(CURSOR *);
void GetCursorInfoDM(CURSOR *);
void DispPinkDM(PINK_PM	*,int);
void Disp1BundleDM(PINK_PM *);
void GetRealColorDM(GETCOLOR *);
BOOLEAN GetGuiNoDM(GETCOLOR *);
void GetScreenSizeDM(UPOINT *);
void ChrFontDM(CHRFONT *);
void GetFontDM(GETFONT *);
void LCDDiagDM(UCHAR,int,int,int);	/* PI-2でスペック変更 */
void SetLightDM(UCHAR,UCHAR);
UCHAR GetLightDM(void);
ACSVRAM EnableAccessVRAM(void);
void DisableAccessVRAM(ACSVRAM);

/* 【MI-100〜】*/
UCHAR *GetVramTop(void);
BOOLEAN SetLcdMode(LCDMODE,RDSPMODE);
LCDMODE GetLcdMode(void);
void GetGrayColor(UCHAR,COLOR *);
void SetLcdContrastDM(char);
char GetLcdContrastDM(void);

/* 【PI-2〜】 */
BOOLEAN IsPowerSave(void);

/* 【表示ツール関連】 */
void CharSizeDM(FONTINFO *,UCHAR,USHORT *,USHORT *);
USHORT GetWidthDM(DISPNCHR *);
void GetLengthFromWidthDM(DISPNCHR *,USHORT,USHORT *,USHORT *);
UCHAR GetStringSizeDM(DISPSTEXT*,POINT *,POINT *);

void CharSizeDMFromDPI(DPI,FONTINFO *,UCHAR,USHORT *,USHORT *);
void CharSizeFromDPI(DPI,UCHAR,USHORT *,USHORT *);
void CharSizeExt(WINDOWHD,UCHAR,USHORT *,USHORT *);

#define	_LCDZDM_H_DEFINED
#endif

