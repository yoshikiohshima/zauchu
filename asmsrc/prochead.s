;;**********************************************************************
; <prochead.s>
;					おおしま
;
;	Squeakのプロセスのコード情報ヘッダ
;
;;**********************************************************************

	.file		"prochead.s"

	.include	"apmlib.seq"
	.include	"apladr.seq"
	.include	"wtii_res.seq"

	.section	"header",.text>WTII_CODE_TOP

	.import	___ghsbegin_shadow	;textセクションの最後(リンカが吐く)
	.import	___ghsbegin_data	;dataセクションの先頭(リンカが吐く)
	.import	___ghsend_data		;dataセクションの最後(リンカが吐く)
	.import	___ghsbegin_bss		;dataセクションの先頭(リンカが吐く)
	.import	___ghsend_bss		;dataセクションの最後(リンカが吐く)

 .if ROM_APPLICATION
	.export		_wtiiModuleHead
 .endif

	.align	4

 .if ROM_APPLICATION
_wtiiModuleHead:
 .else
text_address:
	.data.l	WTII_CODE_TOP
text_end:
	.data.l	___ghsbegin_shadow		;テキストのエンドアドレス
text_offset:
	.data.l	0
 .endif
data_address:
	.data.l	APL_DATA_ADR			;データの論理アドレス
data_end:
	.data.l	___ghsend_data			;データのエンドアドレス
data_offset:
	.data.l	___ghsbegin_shadow		;初期化データのROM上のアドレス
bss_address:
	.data.l	___ghsbegin_bss			;ゼロ初期化データの論理アドレス
bss_end:
	.data.l	___ghsend_bss			;ゼロ初期化データのエンドアドレス
entry:
	.data.l	APL_LAUNCH_ADR			;プロセスのエントリアドレス

	.end
