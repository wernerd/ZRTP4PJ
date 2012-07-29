# Adapt this path to your pjproject path

export PJDIR := ~/devhome/pjproject.git

include $(PJDIR)/build.mak
include $(PJDIR)/build/common.mak

export LIBDIR := ../../lib

RULES_MAK := $(PJDIR)/build/rules.mak

export ZSRTP_LIB := ../../lib/libzsrtp-$(TARGET_NAME)$(LIBEXT)

###############################################################################
# Gather all flags.
#
export _CFLAGS 	:= $(CC_INC). $(CC_INC)../../zsrtp/include \
		   $(CC_INC)../../zsrtp/zrtp \
		   $(CC_INC)../../zsrtp/zrtp/zrtp \
		   $(CC_INC)../../zsrtp/zrtp/zrtp/libzrtpcpp \
		   $(CC_INC)../../zsrtp/zrtp/srtp \
		   $(CC_INC)../../zsrtp/zrtp/srtp/crypto \
		   $(CC_INC)$(PJDIR)/pjlib/include \
		   $(CC_INC)$(PJDIR)/pjlib-util/include \
		   $(CC_INC)$(PJDIR)/pjmedia/include \
		   $(CC_CFLAGS) $(OS_CFLAGS) $(HOST_CFLAGS) $(M_CFLAGS) \
		   $(CFLAGS)  -fno-strict-aliasing
export _CXXFLAGS:= $(_CFLAGS) $(CC_CXXFLAGS) $(OS_CXXFLAGS) $(M_CXXFLAGS) \
		   $(HOST_CXXFLAGS) $(CXXFLAGS)
export _LDFLAGS := $(CC_LDFLAGS) $(OS_LDFLAGS) $(M_LDFLAGS) $(HOST_LDFLAGS) \
		   $(LDFLAGS) 

ciphersossl = zrtp/srtp/crypto/openssl/SrtpSymCrypto.o \
    zrtp/srtp/crypto/openssl/hmac.o \
    zrtp/zrtp/crypto/openssl/zrtpDH.o \
    zrtp/zrtp/crypto/openssl/hmac256.o \
    zrtp/zrtp/crypto/openssl/sha256.o \
    zrtp/zrtp/crypto/openssl/hmac384.o \
    zrtp/zrtp/crypto/openssl/sha384.o \
    zrtp/zrtp/crypto/openssl/aesCFB.o

skeinmac = zrtp/cryptcommon/skein.o zrtp/cryptcommon/skein_block.o zrtp/cryptcommon/skeinApi.o \
    zrtp/cryptcommon/macSkein.o

twofish = zrtp/cryptcommon/twofish.o \
	zrtp/cryptcommon/twofish_cfb.o \
	zrtp/zrtp/crypto/twoCFB.o

# Gcrypt support currently not tested
#ciphersgcrypt = crypto/gcrypt/gcryptAesSrtp.o crypto/gcrypt/gcrypthmac.o \
#          crypto/gcrypt/InitializeGcrypt.o

zrtpobj = zrtp/zrtp/ZrtpCallbackWrapper.o \
    zrtp/zrtp/ZIDCacheFile.o \
    zrtp/zrtp/ZIDRecordFile.o \
    zrtp/zrtp/ZRtp.o \
    zrtp/zrtp/ZrtpCrc32.o \
    zrtp/zrtp/ZrtpPacketCommit.o \
    zrtp/zrtp/ZrtpPacketConf2Ack.o \
    zrtp/zrtp/ZrtpPacketConfirm.o \
    zrtp/zrtp/ZrtpPacketDHPart.o \
    zrtp/zrtp/ZrtpPacketGoClear.o \
    zrtp/zrtp/ZrtpPacketClearAck.o \
    zrtp/zrtp/ZrtpPacketHelloAck.o \
    zrtp/zrtp/ZrtpPacketHello.o \
    zrtp/zrtp/ZrtpPacketError.o \
    zrtp/zrtp/ZrtpPacketErrorAck.o \
    zrtp/zrtp/ZrtpPacketPingAck.o \
    zrtp/zrtp/ZrtpPacketPing.o \
    zrtp/zrtp/ZrtpPacketSASrelay.o \
    zrtp/zrtp/ZrtpPacketRelayAck.o \
    zrtp/zrtp/ZrtpStateClass.o \
    zrtp/zrtp/ZrtpTextData.o \
    zrtp/zrtp/ZrtpConfigure.o \
    zrtp/zrtp/ZrtpCWrapper.o \
    zrtp/zrtp/Base32.o

srtpobj = srtp/ZsrtpCWrapper.o zrtp/srtp/CryptoContext.o zrtp/srtp/CryptoContextCtrl.o

transportobj = transport_zrtp.o

cryptobj =  $(ciphersossl) $(skeinmac) $(twofish)

export ZSRTP_SRCDIR = ../../zsrtp
export ZSRTP_OBJS = $(zrtpobj) $(cryptobj) $(srtpobj) $(transportobj)
export ZSRTP_CFLAGS = $(_CFLAGS)
export ZSRTP_CXXFLAGS = $(_CXXFLAGS)

#path := `pwd`

export CC_OUT CC AR RANLIB HOST_MV HOST_RM HOST_RMDIR HOST_MKDIR OBJEXT LD LDOUT
###############################################################################
# Main entry
#
# $(TARGET) is defined in os-$(OS_NAME).mak file in current directory.
#
TARGETS := libzsrtp

all: $(TARGETS)

doc:
	doxygen docs/doxygen.cfg

dep: depend
distclean: realclean

.PHONY: dep depend libsrtp clean realclean distclean

libzsrtp:
	rm -f build.mak
	$(MAKE) -f $(RULES_MAK) APP=ZSRTP app=libzsrtp $(ZSRTP_LIB)
	echo 'export PJ_CFLAGS := $(PJ_CFLAGS)' -I`pwd`/../../zsrtp/include >> build.mak
	echo 'export PJ_LDFLAGS := $(PJ_LDFLAGS)' -L`pwd`/../../lib >> build.mak
	echo  export PJ_LDLIBS := -lzsrtp-$(TARGET_NAME) $(PJ_LDLIBS) >> build.mak

clean print_lib:
	$(MAKE) -f $(RULES_MAK) APP=ZSRTP app=libzsrtp $@

realclean:
	$(subst @@,$(subst /,$(HOST_PSEP),.ilbc-$(TARGET_NAME).depend),$(HOST_RMR))
	$(MAKE) -f $(RULES_MAK) APP=ZSRTP app=libzsrtp $@

depend:
	$(MAKE) -f $(RULES_MAK) APP=ZSRTP app=libzsrtp $@



