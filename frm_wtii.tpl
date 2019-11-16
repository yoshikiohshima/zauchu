;;**********************************************************************
; <frm_wtii.s>
;					��������
;	Squeak�t�H�[�����\�[�X
;	���̃t�@�C����SZAB�ɂ���Đ����A�X�V�����̂Œ���!!
;;**********************************************************************

	.file		"frm_wtii.s"

	.include	"adlcmn.seq"
	.include	"wtii_res.seq"
 .if ADL
	.include	"wtii_eva.seq"
 .endif
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
	.sdata	"WTIIFM00JPN"			; ���\�[�X�t�@�C����(11bytes)
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
	.data.w	id_top-file_top		; message tbl ptr

;id_no
	.data.w	40000			; �擪 id no
	.data.w	40000+id_max_no-1	; �I�� id no
;
;	ID�e�[�u��
;
id_top:
id_end:

id_max_no	.equ	0
;
;	�f�[�^�{��
;

;


data_end:

;
;	�G���h
;

	.align	h'10

block_end:

	.end
