;;**********************************************************************
; <str_wtii.s>
;					‚¨‚¨‚µ‚Ü
;	Squeak
;;**********************************************************************

	.file		"str_wtii.s"

	.include	"adlcmn.seq"
	.include	"wtii_res.seq"
	.include	"string.seq"

	.section	"mblock",.text>WTII_MBLOCK_TOP
;
;
 .if ROM_APPLICATION
block_top:
	.data.b	BLOCK_HEADER_MARK
	.sdata	"WTIIDM00JPN"
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
;
file_top:
	.data.b	H'0ff
	.data.b	0
	.data.b	FILE_ID_MSG			;file id
	.data.b	RES_FVER_CRIMSON		;file version
;
;
res_tbl_top1:
	.data.w	id_top-file_top	; message tbl ptr

;id_no
	.data.w	40000
	.data.w	40000+id_max_no-1
;
;
id_top:
	.data.l	wtiiStr-file_top
id_end:

id_max_no	.equ	1
;
;
wtiiStr:
	.data.w	data_end-wtiiStr-2
;
	.sdataz	"Squeak"
;


data_end:

;
;

	.align	h'10

block_end:

	.end
