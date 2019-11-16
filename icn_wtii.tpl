;;**********************************************************************
; <icn_wtii.s>
;					おおしま
;	Squeakアイコンリソース
;	このファイルはSZABによって生成、更新されるので注意!!
;;**********************************************************************

	.file		"icn_wtii.s"

	.include	"adlcmn.seq"
	.include	"wtii_res.seq"

	.section	"mblock",.text>WTII_MBLOCK_TOP
;
;	ブロックヘッダ
;
 .if ROM_APPLICATION
block_top:
	.data.b	BLOCK_HEADER_MARK
	.sdata	"WTIIIC00JPN"			; リソースファイル名(11bytes)
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
;	ファイルヘッダ
;
file_top:
	.data.b	H'0ff
	.data.b	0
	.data.b	FILE_ID_MSG			;file id
	.data.b	RES_FVER_CRIMSON		;file version
;
;	IDテーブルヘッダ
;
res_tbl_top1:
	.data.w	id_top-file_top		; message tbl ptr

;id_no
	.data.w	40000			; 先頭 id no
	.data.w	40000+id_max_no-1	; 終了 id no
;
;	IDテーブル
;
id_top:
	.data.l	_wtiiIcn-file_top
id_end:

id_max_no	.equ	1
;
;	データ本体
;

;ローンチャで表示されるアプリアイコン
_wtiiIcn:
	.data.w	data_end-_wtiiIcn-2
;


data_end:

;
;	エンド
;

	.align	h'10

block_end:

	.end
