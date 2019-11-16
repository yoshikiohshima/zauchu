/***********************************************************************
<アプリリソース定義>

  <wtii_res.h>
					おおしま
		Squeakリソース定義
***********************************************************************/

/* FormIDStart */
/* FormIDEnd */

#ifdef ADL
/* ADL ControlIDStart */
#define	wtiiDelFListID	40000
#define	wtiiPIMInfoID		40001
#define	wtiiPIMExecID		40002
#define	wtiiFormTableID	40003
/* 自分でリソースを追加した場合はこれより上に入れてください */
/* ADL FileInfoLabelStart */
#define	WTIIPIMFInfo1ID		40005
/* ADL FileInfoLabelEnd */
/* ADL ControlIDEnd */
/* ADL StrResIDStart */
#define	wtiiStrID			40000
/* ADL StrResIDEnd */
/* ADL IconResIDStart */
#define	wtiiIcnID			40000
/* ADL IconResIDEnd */
/* ADL ImageResIDStart */
/* ADL ImageResIDEnd */
/* ADL MenuResIDStart */
/* ADL MenuResIDEnd */
/* ADL AttrResIDStart */
/* ADL AttrResIDEnd */
/* ADL MenuIDStart */
/* ADL MenuIDEnd */

#define	WTIIDefScreenScnID 	40000	
/* ADL ScreenIDStart */
/* ADL ScreenIDEnd */
#else
/* ControlIDStart */
#define	wtiiDelFListID	(USHORT)40000
#define	wtiiPIMInfoID		(USHORT)40001
#define	wtiiPIMExecID		(USHORT)40002
#define	wtiiFormTableID	(USHORT)40003
#define	WTIIAdlResID		(USHORT)40004
/* 自分でリソースを追加した場合はこれより上に入れてください */
/* FileInfoLabelStart */
#define	WTIIPIMFInfo1ID		(USHORT)40005
/* FileInfoLabelEnd */
/* ControlIDEnd */
/* StrResIDStart */
#define	wtiiStrID			(USHORT)40000
/* StrResIDEnd */
/* IconResIDStart */
#define	wtiiIcnID			(USHORT)40000
/* IconResIDEnd */
/* ImageResIDStart */
/* ImageResIDEnd */
/* MenuResIDStart */
/* MenuResIDEnd */
/* AttrResIDStart */
/* AttrResIDEnd */
/* MenuIDStart */
/* MenuIDEnd */

#define	wtiiDefScreenID 	(USHORT)40000
/* ScreenIDStart */
/* ScreenIDEnd */
#endif ADL


/* ROMアプリケーションなら1、ダウンロードアプリなら0にする */
/* AplType_TopAdr */
#define	ROM_APPLICATION	0
#define	WTII_CODE_TOP	DLAPL_CODE_TOP
#define	WTII_MBLOCK_TOP	DLAPL_MBLOCK_TOP

