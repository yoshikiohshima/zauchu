;;**********************************************************************
; <end_mblk.s>
;					��������
;	Squeak���\�[�X�I�[
;	���̃t�@�C����SZAB�ɂ���Đ����A�X�V�����̂Œ���!!
;;**********************************************************************

	.file	"end_mblk.s"

;;-----------------------------------------------------------
;	section  definition
;------------------------------------------------------------

	.section	"mblk_end",.text

;------------------------------------------------------------
;	include definition
;------------------------------------------------------------

	.include	"mb.seq"
	.include	"apladr.seq"
	.include	"wtii_res.seq"

;;

 .if ROM_APPLICATION
apl_res_end:
	.data.b	BLOCK_SECTION_MARK
	.data.l	WTII_MBLOCK_NEXT	;�����͐�΂Ɏ��̃��\�[�X�擪�������I
	.data.w	0
 .endif

	.end