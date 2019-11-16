;;**********************************************************************
; 					<�R���g���[����`���\�[�X>
;
; <ctl_wtii.s>
;					��������
;	Squeak�R���g���[����`���\�[�X
;	���̃t�@�C����SZAB�ɂ���Đ����A�X�V�����̂Œ���!!
;;**********************************************************************

	.file		"ctl_wtii.s"

	.include	"adlcmn.seq"
	.include	"wtii_res.seq"
	.include	"string.seq"
	.include	"c_grad.seq"
	.include	"c_mvfrm.seq"
	.include	"c_mddlog.seq"

	.section	"mblock",.text>WTII_MBLOCK_TOP
;
;	�u���b�N�w�b�_
;
 .if ROM_APPLICATION
block_top:
	.data.b	BLOCK_HEADER_MARK
	.sdata	"WTIIST00JPN"			; ���\�[�X�t�@�C����(11bytes)
	.data.b	B'00000001			; block atr
	.data.w	0				; block time
	.data.w 0				; block date
	.data.l	block_end-block_top		; block size
	.data.w	0				; block id
	.data.l	block_end-block_top		; free ptr top
	.data.l	block_end-block_top		; free ptr bottom
	.data.l	0
	.data.l	0
 .endif
;
;	�t�@�C���w�b�_
;
file_top:
	.data.b	H'0ff
	.data.b	0
	.data.b	FILE_ID_MSG			;file id
	.data.b	RES_FVER_CRIMSON		;file version
;
;	ID�e�[�u���w�b�_
;
res_tbl_top1:
	.data.w	id_top-file_top	; message tbl ptr

;id_no
	.data.w	40000			; �擪 id no
	.data.w	40000+id_max_no-1	; �I�� id no
;
;	ID�e�[�u��
;
id_top:
	.data.l	wtiiDelFList-file_top
	.data.l	wtiiPIMInfo-file_top
	.data.l	wtiiPIMExec-file_top
	.data.l wtiiFormTable-file_top
	.data.l WTIIAdlRes-file_top
;���\�[�X�������Œǉ�����Ƃ��͂������ɓ���Ă��������B
;#FileInfoLabelTblStart
	.data.l	WTIIPIMFInfo1-file_top
;#FileInfoLabelTblEnd
id_end:

id_max_no	.equ	6
;
;	�f�[�^�{��
;
;�폜�t�@�C�����X�g
wtiiDelFList:
	.data.w	wtiiPIMInfo-wtiiDelFList-2
;
;#DelFileListStart
	.sdata	"WTII.APL,P"
	.data.b	0x0d
	.sdata	"WTII.BIN,P"
	.data.b	0x0d
	.sdata	"WTIIST00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIFM00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIIC00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIIM00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIMN00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIDM00.JPN,P"
	.data.b	0x0d
	.sdata	"WTIIADL0.ZAD,P"
	.data.b	0
;#DelFileListEnd

;PIMINFO - OUT��ԂŎg�p����A�v���P�[�V�������
wtiiPIMInfo:
	.data.w	wtiiPIMExec-wtiiPIMInfo-2	; data length
;
	.res.b	4				;AplName
	.data.w	wtiiPIMExecID			;PIMExecRes.ResID
	.data.w	"00"				;PIMExecRes.ResFNo
	.data.w	WTIIPIMFInfo1ID			;PIMFInfoRes.ResID
	.data.w	"00"				;PIMFInfoRes.ResFNo
;#MaxHeapNum
	.data.w	40			;MaxHeapNum
;#BoxNum
	.data.w	0			;BoxNum
	.data.l	0				;PIMFInfo
	.data.l	0				;PIMAplWork
;#PIMAplConst
	_c_proc	_PIMAplConstruct			;PIMAplConstruct
;#PIMAplEnd
	_c_proc	_PIMAplEnd			;PIMAplEnd
;#PIMAplSetup
	_c_proc	_PIMAplSetup			;PIMAplSetup
;#PIMAplOutMsg
	_c_proc	_PIMAplOutMsg			;PIMAplOutMsg
	.data.l	0				;SysEnv
;#PIMFInfoSetup
	_noproc					;PIMFInfo

;PIMEXECWORK - EXEC��Ԃł̃A�v���P�[�V�������
wtiiPIMExec:
	.data.w	wtiiFormTable-wtiiPIMExec-2
;
	.res.b	4				;AplName
	.data.b	0				;WnHandle
	.res.b	1

	;@2 �ŏ��ɕ\�������ʔԍ���ύX�������ꍇ�͂��������������܂��B
;#FirstScreenIDMark
	.data.w	F0ListScnID			;@2 DefScreenID

	.data.l	0				;IsActive
	.data.w	wtiiFormTableID			;PIMFormTRes.ResID
	.data.w	"00"				;PIMFormTRes.ResFNo

	;@2 �A�v���P�[�V�����̃��C���E�B���h�E�̃��\�[�X�������ւ������ꍇ��
	;   ���������������܂��B�X�y�[�X�̏ꍇ�̓f�t�H���g���g�p���܂��B
;#MainScreenResFName
	.sdataz	"        "			;@2 PIMWindowRes.ResFile
	.res.b	1
;#MainScreenResID
	.data.w	0			;@2 DefScreenID

	.data.l	0				;SysEnv
	.data.l	0				;PIMInfo
	.data.l	0				;PIMExecWork
;#PIMExecConst
	_c_proc	_PIMExecConstruct			;PIMExecConst
;#PIMExecEnd
	_c_proc	_PIMExecEnd			;PIMExecEnd
;#PIMExecSetup
	_c_proc	_PIMExecSetup			;PIMExecSetup

;�t�H�[���e�[�u��
wtiiFormTable:
	.data.w	WTIIAdlRes-wtiiFormTable-2
;
;#FormTblStart
	.data.w	F0ListScnID		;ScreenNo
	.data.w	F0ListID		;FormResID
	_noproc				;Func
	.data.w	SqueakScnID		;ScreenNo
	.data.w	SqueakID		;FormResID
	_noproc				;Func
;#FormTblEnd

	.data.w	0				;�I���R�[�h

WTIIAdlRes:
;#AdlResLabel
	.data.w	WTIIPIMFInfo1-WTIIAdlRes-2
;#ADLStackSize
	.data.l	1024			;adl stack size

;#BoxFileInfoStart
;�t�@�C����񏉊������\�[�X
WTIIPIMFInfo1:
	.data.w	data_end-WTIIPIMFInfo1-2
;
	.data.l	0			;FInfo
;
	.data.w	0		;FInfoResID
	.data.w	0		;ItemResID
	.data.w	0		;IdxInfoResID
;
;#BoxFileInfoEnd
;

data_end:

;
;	�G���h
;

	.align	h'10

block_end:

	.end

