/***********************************************************************
 SH3										<$BJ,N`(B>
		
 							Time-stamp: <99/02/08 10:44:38 yasui>
 <lcdzdm.h>
								$B%7%c!<%W3t<02q<R(B
								Sharp Corporation
								$B7HBS%7%9%F%`;v6HIt(B  $BBh#15;=QIt(B
					personal intelligent systems Div. engineering Dept.1
										$B0B0f(B $BE4Li(B
										T.Yasui
		Zaurus Draw $B%^%M!<%8%c(B

 ver. 0		1997/03/18		created by T.Yasui for Opal
 ver. 1		<date>		<$BJQ99<T;aL>(B>
		<$B$3$N%P!<%8%g%s$NFbMF(B>
***********************************************************************/
#ifndef	_LCDZDM_H_DEFINED

#include <zdrawlib.h>
#include <pinklib.h>

/* --------------------------------------------------------------------
   $B!|(B $BHs8x3+(BAPI
   $B!}(B $B8x3+(BAPI - (ZDRAW$B9=B$BN;HMQ(B)
   $B!{(B $B8x3+(BAPI - (ZDRAW$B9=B$BNL$;HMQ(B)
--------------------------------------------------------------------- */

/***********************************************************************
  $B!Z=i4|2=![(B
  $B!|(B(01) LcdInitDM()
***********************************************************************/
/* $B=i4|2=%l%Y%k(B */
typedef UCHAR INITLEV;
#define	ZDRAW_ALL_RESET	(0x00)	/* $B%*!<%k%j%;%C%H(B */
#define	ZDRAW_RESET		(0x01)	/* $B%j%;%C%H(B */
#define	ZDRAW_OFF		(0x02)	/* $B%*%U(B */
#define	ZDRAW_ON		(0x03)	/* $B%*%s(B */
#define	ZDRAW_RESET_SEM	(0x04)	/* $B%j%;%C%H(B ($B%;%^%U%)MQ(B) */

/***********************************************************************
  $B!ZDL>oJ8;zIA2h![(B
  $B!}(B(02) Disp1CharDM()
  $B!}(B(03) DispNCharDM()
***********************************************************************/
/* $BJ8;zIA2h3+;O:BI80LCV$N;XDj(B */
typedef	USHORT	STARTPOINT;
#define	STP_LEFT_LOWER	(0x00)	/* $B:82<(B */
#define	STP_LEFT_UPPER	(0x01)	/* $B:8>e(B */
#define	STP_RIGHT_LOWER	(0x02)	/* $B1&2<(B */
#define	STP_RIGHT_UPPER	(0x03)	/* $B1&>e(B */

/* $BJ8;zO"B3IA2hJ}8~$N;XDj(B */
typedef	USHORT	DISPANGLE;
#define	DSPA_NORMAL		(0x00)	/* $B:8"*1&(B */
#define	DSPA_REVERSE	(0x01)	/* $B1&"*:8(B */
#define	DSPA_TOP_DOWN	(0x02)	/* $B>e"*2<(B */
#define	DSPA_BOTTOM_UP	(0x03)	/* $B2<"*>e(B */

/* $BJ8;z$N%5%$%:(B (zdrawlib.h $B$KDj5A(B) */
typedef	UCHAR	FONTSIZE;

/*  $BJ8;z$N<oN`(B */
typedef	ULONG	FONTTYPE;
#define	FT_FJNM	'FJNM'		/* $B9qFb(B Fixed$B%U%)%s%H(B(System FONT) */
#define	FT_FJZ2	'FJZ2'		/* $B9qFb(B Fixed$B%U%)%s%H(B(PI-2 FONT) */
#define	FT_FJZ1	'FJZ1'		/* $B9qFb(B Fixed$B%U%)%s%H(B(Zaurus FONT) */
#define	FT_FJTK	'FJTK'		/* $B9qFb(B Fixed$B%U%)%s%H(B(Tokei FONT) */
#define	FT_FJCL	'FJCL'		/* $B9qFb(B Fixed$B%U%)%s%H(B(Calendar FONT) */
#define	FT_FJJT	'FJJT'		/* $B9qFb(B Fixed$B%U%)%s%H(B(Jiten FONT) */
#define	FT_FE05	'FE05'		/* $B3$302#(B5dot Fixed$B%U%)%s%H(B */
#define	FT_FE03	'FE03'		/* $B3$302#(B3dot Fixed$B%U%)%s%H(B */
#define	FT_PE05	'PE05'		/* $B3$302#(B5dot Proportional$B%U%)%s%H(B */
#define	FT_PE03	'PE03'		/* $B3$302#(B3dot Proportional$B%U%)%s%H(B */
#define FT_FJKY	'FJKY'		/* $B?7%+%l%s%@!<%U%)%s%H(B(Koyomi) */
#define FT_FJDN	'FJDN'		/* $B?7EEBn%U%)%s%H(B(Dentaku) */

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

/* $BJ8;z=$>~>pJs(B */
typedef	USHORT	FONTSTYLE;
#define	FS_NORMAL		(0x00)		/* $BDL>o%9%?%$%k(B */
#define	FS_BOLD			(0x01)		/* $B%\!<%k%I(B */
#define	FS_ITALIC		(0x02)		/* $B%$%?%j%C%/(B */
#define	FS_UNDERLINE	(0x04)		/* $B2<@~IU$-(B */
#define	FS_SHADOW		(0x08)		/* $B1FJ8;z(B($B;HMQIT2D(B) */

/* $BJ8;z2sE>3QEY(B */
typedef	USHORT	FONTANGLE;
#define	FA_NO_ROTATION	(0x0000)	/* $B2sE>$J$7(B*/
#define	FA_180_ROTATION	(0x0001)	/* 180$BEY2sE>(B */
#define	FA_R90_ROTATION	(0x0002)	/* $B1&(B90$BEY2sE>(B */
#define	FA_L90_ROTATION	(0x0003)	/* $B:8(B90$BEY2sE>(B */

/* $BJ8;z$N7A>u(B */
typedef struct FONTINFO {
	FONTSIZE	FontSize;		/* $BJ8;z%5%$%:(B */
	FONTTYPE	FontType;		/* $B%U%)%s%H%?%$%W(B($BL>A0#4J8;z(B) */
	FONTSTYLE	FontStyle;		/* $BJ8;z=$>~>pJs(B */
	FONTANGLE	FontAngle;		/* $BJ8;z2sE>3QEY(B */
} FONTINFO;

typedef struct DISP1CHR {
	short		Pitch;			/* $BJ8;z4V3V(B (QP) */
	USHORT		SJISCode;		/* shift JIS code */
	STARTPOINT	StartPoint;		/* $B3+;OE@>pJs(B */
	DISPANGLE	DispAngle;		/* $BO"B3IA2hJ}8~(B */
	FONTINFO	FontForm;		/* $BJ8;z7A>u(B */
} DISP1CHR;

typedef struct DISPNCHR {
	short		Pitch;			/* $BJ8;z4V3V(B (QP) */
	USHORT		Len;			/* $B%G!<%?%P%$%H?t(B($B#N#U#L#L$O4^$^$:(B) */
	UCHAR		*Str;			/* $B%G!<%?@hF,%"%I%l%9(B */
	STARTPOINT	StartPoint;		/* $B3+;OE@>pJs(B */
	DISPANGLE	DispAngle;		/* $BO"B3IA2hJ}8~(B */
	FONTINFO	FontForm;		/* $BJ8;z7A>u(B */
} DISPNCHR;

/***********************************************************************
  $B!ZE@IA2h![(B
  $B!}(B(04) DispPointDM()
***********************************************************************/
typedef struct DISPPOINT {
	DOTSIZE		DotSize;		/* $BE@$NBg$-$5(B(QP) */
	DOTSHAPE	DotShape;		/* $BE@7A>u(B */
} DISPPOINT;

/***********************************************************************
  $B!Z@~IA2h![(B
  $B!}(B(05) DrawLineDM()
  $B!}(B(06) DrawContLineDM()
  $B!}(B(07) BoxLineDM()
***********************************************************************/
/* $B@~$N%Q%?!<%s%G!<%?(B */
typedef	long	LINEPTD;

typedef struct DRAWLINE {
	DOTSIZE		DotSize;		/* $BE@$NBg$-$5(B(QP) */
	DOTSHAPE	DotShape;		/* $BE@7A>u(B */
	LINEPAT		LinePt;			/* $B@~7A>u(B */
	LINEPTD		LinePtd;		/* $B@~$N%Q%?!<%s%G!<%?(B */
} DRAWLINE;

typedef struct BOXLINE {
	DOTSIZE		DotSize;		/* $BE@$NBg$-$5(B(QP) */
	DOTSHAPE	DotShape;		/* $BE@7A>u(B */
	LINEPAT		LinePt;			/* $B@~7A>u(B */
	LINEPTD		LinePtd;		/* $B@~$N%Q%?!<%s%G!<%?(B */
} BOXLINE;

/***********************************************************************
  $B!ZEI$j$D$V$7![(B
  $B!}(B(08) LcdClsDM()
  $B!}(B(09) AreaClearDM()
  $B!}(B(10) AreaDarkDM()
  $B!}(B(11) BoxPaintDM()
***********************************************************************/
typedef struct BOXPAINT {
	PAINTPAT	PaintPt;		/* $B%Z%$%s%H%Q%?!<%s(B */
	UPOINT		PaintOfs;		/* $B%Z%$%s%H%Q%?!<%s3+;O:BI8%*%U%;%C%H(B */
} BOXPAINT;


/***********************************************************************
  $B!Z%"%H%j%S%e!<%HIU$-J8;zIA2h![(B
  $B!}(B(12) DispStyledTextDM()
***********************************************************************/
/* $B%"%H%j%S%e!<%HIU$-J8;zNsI=<($NI=<(B0@-(B */
typedef USHORT STR_ATR;

/*
   STR_ATR : 
   $B(#(!(!(!(!(((!(!(!(!(!(!(((!(!(!(!(!(((!(!(!(!(!(!($(B
   $B("(BFType(3)$B("(B  FColor(5) $B("(B FSize(3) $B("(B  FStyle(5) $B("(B
   $B(&(!(!(!(!(*(!(!(!(!(!(!(*(!(!(!(!(!(*(!(!(!(!(!(!(%(B
   *()$BFb$NCM$O%S%C%H?t!#(B

   FType  : $BJ8;z$N<oN`(B
   FColor : $BJ8;z$N?'(B
   FSize  : $BJ8;z$N%5%$%:(B
   FStyle : $BJ8;z=$>~>pJs(B
*/

/* $BI=<(B0@-$K4X$9$kJ8;z$N<oN`(B */
#define	STRA_MINCHO		(0x0000)
/*	Reserve				(0x2000) */
/*	Reserve				(0x4000) */
/*	Reserve				(0x6000) */
/*	Reserve				(0x8000) */
/*	Reserve				(0xc000) */
/*	Reserve				(0xe000) */

#define STRA_TYPE_MASK	(0xe000)

/* $BI=<(B0@-$K4X$9$kJ8;z$N?'(B ( $BL$Dj(B ) */
#define	STRA_BLACK		(0x0000)
#define	STRA_RED		(0x0100)
#define	STRA_GREEN		(0x0200)
#define	STRA_BLUE		(0x0300)
#define	STRA_WHITE		(0x0400)

#define STRA_COLOR_MASK	(0x1f00)

/* $BI=<(B0@-$K4X$9$kJ8;z$N%5%$%:(B */
#define	STRA_40QP		(0x0000)		/* $B9b$5#1#0%I%C%HJ8;z(B */
#define	STRA_48QP		(0x0020)		/* $B9b$5#1#2%I%C%HJ8;z(B */
#define	STRA_64QP		(0x0040)		/* $B9b$5#1#6%I%C%HJ8;z(B */
#define	STRA_96QP		(0x0060)		/* $B9b$5#2#4%I%C%HJ8;z(B */
/*	Reserve				(0x0080) */
/*	Reserve				(0x00a0) */
/*	Reserve				(0x00c0) */
/*	Reserve				(0x00e0) */

#define STRA_SIZE_MASK	(0x00e0)

/* $BI=<(B0@-$K4X$9$kJ8;z=$>~>pJs(B */
#define	STRA_PLANE		(0x0000)		/* $B=$>~$J$7(B */
#define	STRA_BOLD		(0x0001)		/* $B%\!<%k%I;XDj(B */
#define	STRA_ITALIC		(0x0002)		/* $B%$%?%j%C%/;XDj(B */
#define	STRA_UNDERLINE	(0x0004)		/* $B%"%s%@!<%i%$%s;XDj(B */
/*	Reserve				(0x0008) */
/*	Reserve				(0x0010) */

#define	STRA_STYLE_MASK	(0x001f)

#define	STRA_DEFAULT	(STRA_MINCHO|STRA_BLACK|STRA_48QP|STRA_PLANE)

/* $B%"%H%j%S%e!<%HIU$-J8;zNsI=<($NJ8;zNs%G!<%?(B */
typedef USHORT	STR_CHAR;

/* $B%"%H%j%S%e!<%HIU$-J8;zNsI=<($NI=<(J}K!$K4X$9$k%S%C%H>pJs(B */
typedef	UCHAR STR_INFO;
#define	WORD_WRAP_ON	(0x01)	/* $B%o!<%I%i%C%W5!G=(B ON */
#define	SYMBOL_09_ON	(0x02)	/* TAB(0x09)$B%7%s%\%kI=<(5!G=(B ON */
#define	SYMBOL_0C_ON	(0x04)	/* $B2~JG(B(0x0c)$B%7%s%\%kI=<(5!G=(B ON */
#define	SYMBOL_0D_ON	(0x08)	/* $B2~9T(B(0x0d)$B%7%s%\%kI=<(5!G=(B ON */
#define	CLEAR_OFF		(0x10)	/* $BJ8;zI=<(2DG=NN0h%/%j%"(B OFF */

typedef struct STR_TAB_TBL {
    USHORT		nTabs;			/* $B#T#A#B0LCV@_Dj?t!J(B0$B!A(BMAX_TABS$B!K(B*/
    USHORT		TabPos[8];		/* $B#T#A#B0LCV(B (QP) */
} STR_TAB_TBL;
#define	MAX_TABS	(8)			/* TabPos[MAX_TABS] */

typedef struct DISPSTEXT {
	short		Pitch;			/* $BJ8;z4V3V(B (QP) */
	USHORT		Len;			/* $BI=<(J8;z?t(B */
	STR_CHAR	*Str;			/* $BI=<(J8;zNs(B */
	STR_ATR		*AttrPtr;		/* $BI=<(B0@-(B */
	STR_TAB_TBL	*TabTblPtr;		/* TAB $B0LCV(B */
	USHORT		Indent;			/* $B%$%s%G%s%H0LCV(B */
	USHORT		SelSAdr;		/* $B%;%l%/%H3+;O%*%U%;%C%H(B */
	USHORT		SelEAdr;		/* $B%;%l%/%H=*N;%*%U%;%C%H(B */
	COLOR		SelRevC;		/* $B%;%l%/%HH?E>?'(B */
	STR_INFO	Info;			/* $BI=<(J}K!$K4X$9$k>pJs(B */
    USHORT		Height;			/* $B:GBgI=<(9b$5(B (QP) */
    USHORT		Width;			/* $BI=<(I}(B (QP) */
} DISPSTEXT;

/***********************************************************************
  $B!Z%"%$%3%sIA2h![(B
  $B!}(B(13) DispIconDM()
***********************************************************************/
typedef struct DISPICN {
	ICONDATA	*IcnData;
	ICONMODE	IcnMode;
} DISPICN;

/***********************************************************************
  $B!Z%$%a!<%8IA2h![(B
  $B!|(B(14) DispImageDM()
  $B!}(B(15) DispZPMImageDM()
***********************************************************************/
typedef union IMGPTR {
	void	 	*ImgData;
	IMAGEDATA	*ImgDataW;		/* $B%G!<%?%5%$%:(B 2byte */
	IMAGEDATA_L	*ImgDataL;		/* $B%G!<%?%5%$%:(B 4byte$B3HD%(B (PI-2) */
} IMGPTR;

typedef struct DISPIMG {
	IMGPTR		ImgPtr;
} DISPIMG;

typedef struct DISPZPMIMG {
	USHORT		*ZPMImgData;
} DISPZPMIMG;

/***********************************************************************
  $B!ZNN0h$NB`Hr(B/$BI|5"![(B
  $B!}(B(16) DispPushDM()	-	$B<B:]8x3+$9$k(BAPI$B$O!"(BPutImageToBufferL()
  $B!}(B(17) DispPopDM()	-	$B<B:]8x3+$9$k(BAPI$B$O!"(BDispImageDM()
***********************************************************************/
typedef UCHAR DPBUFTYPE;	/* $B%P%C%U%!%?%$%W;XDj(B */

#define	DP_NEW_HEAP			(0x00)		/* $B?75,%R!<%W(B (size 2byte) */
#define	DP_NEW_OFFSCRN		(0x01)		/* $B?75,%*%U%9%/%j!<%s(B */
#define	DP_PTR				(0x02)		/* $B;XDj%P%C%U%!(B (size 2byte) */
#define	DP_EXIST_OFFSCRN	(0x03)		/* $B;XDj%*%U%9%/%j!<%s(B */
#define	DP_NEW_HEAP_L		(0x04)		/* $B?75,%R!<%W(B (size 4byte) */
#define	DP_PTR_L			(0x05)		/* $B;XDj%P%C%U%!(B (size 4byte) */

typedef struct DPEXISTOFS {
	WINDOWHD	OSHandle;
	RECTANGLE	OSRect;
	char		dumy1;		/* for h2equ bug $B2sHr(B */
	char		dumy2;		/* for h2equ bug $B2sHr(B */
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
	IMGTYPE		DataType;		/* IMAGE_16BIT $B$N$_%5%]!<%H(B */
	UCHAR		reserved0;
	USHORT		reserved1;		/* $BI,$:(B0$B$K$9$k;v!*(B */
	ULONG		reserved2;		/* $BI,$:(B0$B$K$9$k;v!*(B */
} DISPPUSHPOP;

/***********************************************************************
  $B!ZNN0h$N0\F0![(B
  $B!}(B(18) BoxMoveDM()
  $B!}(B(19) ScrollUpDM()
  $B!}(B(20) ScrollDownDM()
  $B!}(B(21) SmoothScrollDM()	+++ PI-2 $B$GDI2C(B
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
  $B!Z=D%Q%?!<%sIA2h![(B
  $B!}(B(22) DispNVPatternDM()
  $B!}(B(23) ReadPointDM()
***********************************************************************/
typedef struct DISPVPTN {
	USHORT		VPtn;
} DISPVPTN;

typedef struct DISPNVPTN {
	USHORT		*NVPtnPtr;
	USHORT		Len;
} DISPNVPTN	;

/***********************************************************************
  $B!ZE@$NFI$_$@$7![(B
  $B!}(B(22) DispNVPatternDM()
  $B!}(B(23) ReadPointDM()
***********************************************************************/
typedef struct READPOINT {
	USHORT		ReadC16;			/* 16$B%S%C%H%+%i!<3JG<@h(B */
} READPOINT;

/***********************************************************************
  $B!Z%+!<%=%k$K4X$9$k=hM}![(B
  $B!|(B CursorBlinkDM()
  $B!{(B CursorMoveDM()
  $B!{(B CursorDispDM()
  $B!{(B GetCursorInfoDM()
***********************************************************************/
/* $B%+!<%=%k%?%$%W(B */
typedef USHORT CRSTYPE;
#define	WN_CRS_OFF			(0x00)	/* $B%+!<%=%k(BOFF $B>uBV(B */
#define	WN_CRS_ON			(0x01)	/* $B%+!<%=%k(BON $B>uBV(B */
#define	WN_CRS_BLINK		(0x08)	/* $B%V%j%s%/;XDj(B */
#define	WN_CRS_ONOFF_MASK	(0x07)	/* $B%+!<%=%k%?%$%W>pJs%^%9%/MQ(B */
#define	WN_CRS_SIZE_MASK	(0xf0)	/* $B%+!<%=%k%5%$%:%^%9%/MQ(B */

typedef struct CURSOR {
	WINDOWHD	WnHandle;
	POINT		Start;			/* $B:82<:BI8$N;XDj(B */
	CRSTYPE		CrsType;
	COLOR		Fg;
	COLOR		Bg;
} CURSOR;

/***********************************************************************
  $B!Z%$%s%/IA2h![(B
  $B!|(B DispPinkDM()
  $B!|(B Disp1BundleDM()
***********************************************************************/

/***********************************************************************
  $B!Z?'>pJs<hF@![(B
  $B!{(B GetRealColorDM()
  $B!{(B GetGuiNoDM()
***********************************************************************/
typedef struct GETCOLOR {
	GUICOLOR	GCNo;
	COLOR		*RealCPtr;
	GCTABLE		GCTblNo;
} GETCOLOR;

/***********************************************************************
  $B!Z2hLL%5%$%:>pJs<hF@![(B
  $B!{(B GetScreenSizeDM()
***********************************************************************/

/***********************************************************************
  $B!ZJ8;z%G!<%?>pJs<hF@![(B
  $B!{(B ChrFontDM()
  $B!{(B GetFontDM()
***********************************************************************/
typedef struct CHRFONT {
	USHORT		SJISCode;		/* IN  : shift JIS code */
	FONTINFO	*FontFormPtr;	/* IN  : $B%U%)%s%H7A>u@hF,%"%I%l%9(B */
	USHORT		*CgPtr;			/* OUT : $B#C#G%G!<%?@hF,(B(ROM)$B%"%I%l%9(B */
	UCHAR		HLoops;			/* OUT : $B=D%k!<%WCM(B */
	UCHAR		HDot;			/* OUT : $B=D%I%C%H?t(B */
	USHORT		WLoops;			/* OUT : $B2#I}%k!<%WCM(B */
	USHORT		LastWLoopsMask;	/* OUT : $B:G2<2#%k!<%W%^%9%/(B */
} CHRFONT;

typedef struct GETFONT {
	USHORT		SJISCode;		/* IN  : shift JIS code */
	FONTINFO	*FontFormPtr;	/* IN  : $B%U%)%s%H7A>u@hF,%"%I%l%9(B */
	USHORT		*CgBufPtr;		/* IN  : $B#C#G%G!<%?3JG<@hF,%"%I%l%9(B */
	UCHAR		Height;			/* OUT : $B9b$5(B(dot) */
	UCHAR		Width;			/* OUT : $BI}(B(dot) */
	USHORT		*CgPtr;			/* OUT : $B#C#G%G!<%?@hF,(B(ROM)$B%"%I%l%9(B */
} GETFONT;

/***********************************************************************
  $B!Z%@%$%"%0![(B
  $B!|(BLCDDiagDM()
***********************************************************************/

/***********************************************************************
  $B!Z%P%C%/%i%$%H![(B
  $B!{(BSetLightDM()
  $B!{(BGetLightDM()
***********************************************************************/

/***********************************************************************
  $B!Z#V#R#A#M@hF,%"%I%l%9<hF@(B/$BD>@\IA2h;XDj![(B
  $B!|(B EnableAccessVRAM()
  $B!|(B DisableAccessVRAM()
  $B!|(B GetVramTop()
***********************************************************************/
typedef UCHAR ACSVRAM;
#define	VRAM_DIRECT_ENABLE	(0x01)	/* VRAM $BD>@\IA2h2DG=>uBV(B */

/***********************************************************************
  $B!Z(BMI-100$B@lMQ(B($BGr9u(B/$B%0%l!<(B)$B![(B
  $B!|(B GetVramTop()
  $B!|(B SetLcdMode()
  $B!|(B GetLcdMode()
  $B!|(B GetGrayColor()
  $B!|(B SetLcdContrastDM()
  $B!|(B GetLcdContrastDM()
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

/* $B#L#C#D%3%s%s%H%i%9%HCM$NDj5A(B */
#define	LCD_CONTRAST_MIN		(0x00)
#define LCD_CONTRAST_MAX		(0x3f)
#define	LCD_CONTRAST_DEFAULT	(0x0f)

/***********************************************************************
  $B!Z%0%i%G!<%7%g%sIA2h![(B
***********************************************************************/
typedef struct GRDRAW {
    COLOR	startcolor;
	COLOR	endcolor;
	COLOR	edge1;
	COLOR	edge2;
	USHORT	pitch;
} GRDRAW;

/***********************************************************************
  $B!Z3Q4];M3QIA2h![(B
***********************************************************************/
typedef struct DRDRAW {
    COLOR	fill;
	COLOR	inner;
	COLOR	edge;
} DRDRAW;

/***********************************************************************
  $BIA2h;XDj#A#P#I6&DL9=B$BN(B ($B!}0u$N(BAPI$B$G;HMQ(B)
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
	UCHAR		FuncNo;		/* $B%U%!%s%/%7%g%s(BNo. ($BL$;HMQ(B) */
	WINDOWHD	WnHandle;	/* $B%&%#%s%I%&%O%s%I%k(B */
	CDSPMODE	Mode;		/* $BI=<(%b!<%I(B */
 	POINT   	Start; 		/* $B%&%#%s%I%&Fb3+;OE@:BI8(B */
	POINT		End;        /* $B%&%#%s%I%&Fb=*N;E@:BI8(B */
	RECTANGLE	Mask;		/* $B%U%l!<%`%^%9%/NN0h(B */
	COLOR		Fg;			/* $B%U%)%"%0%i%&%s%I%+%i!<(B */
	COLOR		Bg;			/* $B%P%C%/%0%i%&%s%I%+%i!<(B */
	DISP_PARAM	DspParam;	/* $B3F<oIA2h%Q%i%a!<%?(B */
} ZDRAW;

/***********************************************************************
  $B%U%!%s%/%7%g%s%J%s%PDj5A(B
***********************************************************************/
#define	TOP_FUNC				(0x40)

#define	LCD_INIT_ZDM			(0x40)	/* $B=i4|2=(B */
#define	DISP_1_CHAR_ZDM			(0x41)	/* $B#1J8;zI=<((B */
#define	DISP_N_CHAR_ZDM			(0x42)	/* $B#NJ8;zI=<((B */
#define	DISP_POINT_ZDM			(0x43)	/* $B#1E@I=<((B */
#define	DRAW_LINE_ZDM			(0x44)	/* $B@~$rIA$/(B */
#define	DRAW_CONT_LINE_ZDM		(0x45)	/* $BO"B3IA2hMQ$N@~$rIA$/(B */
#define	BOX_LINE_ZDM			(0x46)	/* $BD9J}7A$rIA$/(B */
#define	CLS_LCD_ZDM				(0x47)	/* $BA42hLL%/%j%"(B */
#define	AREA_CLEAR_ZDM			(0x48)	/* $B;XDjHO0O$N%/%j%"(B */
#define	AREA_DARK_ZDM			(0x49)	/* $B;XDjHO0O$N1F%/%j%"(B */
#define	BOX_PAINT_ZDM			(0x4a)	/* $B6k7ANN0h$N%Z%$%s%H(B */
#define	DISP_STYLED_TEXT_ZDM	(0x4b)	/* $B%"%H%j%S%e!<%HIU(B $B#NJ8;zI=<((B */
#define	DISP_ICON_ZDM			(0x4c)	/* $B%"%$%3%sI=<((B */
#define	DISP_IMAGE_ZDM			(0x4d)	/* $B%$%a!<%8I=<((B */
#define	DISP_ZPMIMAGE_ZDM		(0x4e)	/* ZPM$B%$%a!<%8I=<((B */
#define	DISP_PUSH_ZDM			(0x4f)	/* $B6k7ANN0h$NB`Hr(B */
#define	DISP_POP_ZDM			(0x50)	/* $B6k7ANN0h$NI|5"(B */
#define	BOX_MOVE_ZDM			(0x51)	/* $B6k7A$N0\F0(B */
#define	SCROLL_UP_ZDM			(0x52)	/* $B>e%9%/%m!<%k(B */
#define	SCROLL_DOWN_ZDM			(0x53)	/* $B2<%9%/%m!<%k(B */
#define	DISP_V_PATTERN_ZDM		(0x54)	/* $B=D(B16$B%I%C%H%Q%?!<%sI=<((B */
#define	DISP_N_V_PATTERN_ZDM	(0x55)	/* $BJ#?t=D(B16$B%I%C%H%Q%?!<%sI=<((B */
#define	READ_POINT_ZDM			(0x56)
/* $B!Z(BPI-2$B!A![(B */
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

/* $B!Z(BMI-100$B!A![(B*/
#define	SET_LCD_MODE_ZDM		(0x73)
#define	GET_LCD_MODE_ZDM		(0x74)
#define	GET_GRAY_COLOR_ZDM		(0x75)
#define	SET_LCD_CONTRAST_ZDM	(0x76)
#define	GET_LCD_CONTRAST_ZDM	(0x77)

/* $B!Z(BMI-310$B!A![(B */
#define	ROUND_BOX_PAINT_ZDM		(0x78)

/* $B!Z(BPI-2$B!A![(B */
#define	IS_POWER_SAVE_ZDM		(0x79)

#define	DRAW_GRADATION			(0x80)
#define	DRAW_ROUND_AREA			(0x81)

#define	LAST_FUNC		DRAW_ROUND_AREA

/* $B!ZI=<(%D!<%k![(B */
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

void CursorBlinkDM(void);			/* PI-2$B$G%9%Z%C%/JQ99(B */
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
void LCDDiagDM(UCHAR,int,int,int);	/* PI-2$B$G%9%Z%C%/JQ99(B */
void SetLightDM(UCHAR,UCHAR);
UCHAR GetLightDM(void);
ACSVRAM EnableAccessVRAM(void);
void DisableAccessVRAM(ACSVRAM);

/* $B!Z(BMI-100$B!A![(B*/
UCHAR *GetVramTop(void);
BOOLEAN SetLcdMode(LCDMODE,RDSPMODE);
LCDMODE GetLcdMode(void);
void GetGrayColor(UCHAR,COLOR *);
void SetLcdContrastDM(char);
char GetLcdContrastDM(void);

/* $B!Z(BPI-2$B!A![(B */
BOOLEAN IsPowerSave(void);

/* $B!ZI=<(%D!<%k4XO"![(B */
void CharSizeDM(FONTINFO *,UCHAR,USHORT *,USHORT *);
USHORT GetWidthDM(DISPNCHR *);
void GetLengthFromWidthDM(DISPNCHR *,USHORT,USHORT *,USHORT *);
UCHAR GetStringSizeDM(DISPSTEXT*,POINT *,POINT *);

void CharSizeDMFromDPI(DPI,FONTINFO *,UCHAR,USHORT *,USHORT *);
void CharSizeFromDPI(DPI,UCHAR,USHORT *,USHORT *);
void CharSizeExt(WINDOWHD,UCHAR,USHORT *,USHORT *);

#define	_LCDZDM_H_DEFINED
#endif

