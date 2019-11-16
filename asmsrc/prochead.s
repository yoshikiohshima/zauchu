;;**********************************************************************
; <prochead.s>
;					��������
;
;	Squeak�̃v���Z�X�̃R�[�h���w�b�_
;
;;**********************************************************************

	.file		"prochead.s"

	.include	"apmlib.seq"
	.include	"apladr.seq"
	.include	"wtii_res.seq"

	.section	"header",.text>WTII_CODE_TOP

	.import	___ghsbegin_shadow	;text�Z�N�V�����̍Ō�(�����J���f��)
	.import	___ghsbegin_data	;data�Z�N�V�����̐擪(�����J���f��)
	.import	___ghsend_data		;data�Z�N�V�����̍Ō�(�����J���f��)
	.import	___ghsbegin_bss		;data�Z�N�V�����̐擪(�����J���f��)
	.import	___ghsend_bss		;data�Z�N�V�����̍Ō�(�����J���f��)

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
	.data.l	___ghsbegin_shadow		;�e�L�X�g�̃G���h�A�h���X
text_offset:
	.data.l	0
 .endif
data_address:
	.data.l	APL_DATA_ADR			;�f�[�^�̘_���A�h���X
data_end:
	.data.l	___ghsend_data			;�f�[�^�̃G���h�A�h���X
data_offset:
	.data.l	___ghsbegin_shadow		;�������f�[�^��ROM��̃A�h���X
bss_address:
	.data.l	___ghsbegin_bss			;�[���������f�[�^�̘_���A�h���X
bss_end:
	.data.l	___ghsend_bss			;�[���������f�[�^�̃G���h�A�h���X
entry:
	.data.l	APL_LAUNCH_ADR			;�v���Z�X�̃G���g���A�h���X

	.end
