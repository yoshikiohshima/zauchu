#########################################################################
#	Squeak用 Makefile
#########################################################################

TARGET	=	C:\ohshima\zau-chu\ICRUISE\2000.1

include ../makefiles/makedef

# プロセスが使用するグローバル変数
LIBS	= $(DEVDIR)/lib/sysglob.obj

#[SRCS]---BEGIN
SRCS	=	prochead.s	\
			aplhead.s	\
			frm_wtii.s	\
			ctl_wtii.s	\
			icn_wtii.s	\
			img_wtii.s	\
			str_wtii.s	\
			mnu_wtii.s	\
			end_mblk.s	\
			wtiimain.c	\
			wtii_evc.c
#[SRCS]---END

#[DIFF_TARGET]+++BEGIN
#[OBJS]---BEGIN
OBJS	=	prochead.obj	\
			aplhead.obj		\
			frm_wtii.obj	\
			ctl_wtii.obj	\
			icn_wtii.obj	\
			img_wtii.obj	\
			str_wtii.obj	\
			mnu_wtii.obj	\
			end_mblk.obj	\
			wtiimain.obj	\
			wtii_evc.obj
#[OBJS]---END
#[DIFF_TARGET]+++END

#[INCS]---BEGIN
INCS	=
#[INCS]---END

$(TARGET):	wtii_res.seq $(OBJS) $(LIBS) $(TARGET).lnk $(CRIMLIBS)
	$(LK) $(LKFLAGS) $(OBJS) $(LIBS) $(LKFLAGS2) -o $@
#	mkloadinfo $(TARGET).map
#	dli $(TARGET).load $(TARGET).dli
#	makezac $(TARGET) $(TARGET).ZAC

depend:
	mkdepsh $(DEPENDFLAGS) -- $(CFLAGS) -- $(SRCS)

release:
	relfiles $(TARGET) $(SRCS) $(INCS)

chrel:
	chrel $(TARGET)

tags:
	etags $(SRCS) $(INCS)

wtii_res.seq:	wtii_res.h
	h2equ wtii_res.h $(H2EQUFLAGS)
	cpown wtii_res.h wtii_res.seq

# DO NOT DELETE THIS LINE

