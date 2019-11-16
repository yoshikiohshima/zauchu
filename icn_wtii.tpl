;;**********************************************************************
; <icn_wtii.s>
;					��������
;	Squeak�A�C�R�����\�[�X
;	���̃t�@�C����SZAB�ɂ���Đ����A�X�V�����̂Œ���!!
;;**********************************************************************

	.file		"icn_wtii.s"

	.include	"adlcmn.seq"
	.include	"wtii_res.seq"

	.section	"mblock",.text>WTII_MBLOCK_TOP
;
;	�u���b�N�w�b�_
;
 .if ROM_APPLICATION
block_top:
	.data.b	BLOCK_HEADER_MARK
	.sdata	"WTIIIC00JPN"			; ���\�[�X�t�@�C����(11bytes)
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
	.data.l	_wtiiIcn-file_top
id_end:

id_max_no	.equ	1
;
;	�f�[�^�{��
;

;���[���`���ŕ\�������A�v���A�C�R��
_wtiiIcn:
	.data.w	data_end-_wtiiIcn-2
;


data_end:

;
;	�G���h
;

	.align	h'10

block_end:

	.end
