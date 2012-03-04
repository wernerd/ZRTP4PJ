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
		   $(CC_INC)../../zsrtp/zrtp/src \
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

ciphersossl = zrtp/srtp/crypto/openssl/AesSrtp.o \
    zrtp/srtp/crypto/openssl/hmac.o \
    zrtp/src/libzrtpcpp/crypto/openssl/ZrtpDH.o \
    zrtp/src/libzrtpcpp/crypto/openssl/hmac256.o \
    zrtp/src/libzrtpcpp/crypto/openssl/sha256.o \
    zrtp/src/libzrtpcpp/crypto/openssl/hmac384.o \
    zrtp/src/libzrtpcpp/crypto/openssl/sha384.o \
    zrtp/src/libzrtpcpp/crypto/openssl/AesCFB.o

skeinmac = zrtp/srtp/crypto/skein.o zrtp/srtp/crypto/skein_block.o zrtp/srtp/crypto/skeinApi.o \
    zrtp/srtp/crypto/macSkein.o

twofish = zrtp/src/libzrtpcpp/crypto/twofish.o \
	zrtp/src/libzrtpcpp/crypto/twofish_cfb.o \
	zrtp/src/libzrtpcpp/crypto/TwoCFB.o

# Gcrypt support currently not tested
#ciphersgcrypt = crypto/gcrypt/gcryptAesSrtp.o crypto/gcrypt/gcrypthmac.o \
#          crypto/gcrypt/InitializeGcrypt.o

zrtpobj = zrtp/src/ZrtpCallbackWrapper.o \
    zrtp/src/ZIDFile.o \
    zrtp/src/ZIDRecord.o \
    zrtp/src/ZRtp.o \
    zrtp/src/ZrtpCrc32.o \
    zrtp/src/ZrtpPacketCommit.o \
    zrtp/src/ZrtpPacketConf2Ack.o \
    zrtp/src/ZrtpPacketConfirm.o \
    zrtp/src/ZrtpPacketDHPart.o \
    zrtp/src/ZrtpPacketGoClear.o \
    zrtp/src/ZrtpPacketClearAck.o \
    zrtp/src/ZrtpPacketHelloAck.o \
    zrtp/src/ZrtpPacketHello.o \
    zrtp/src/ZrtpPacketError.o \
    zrtp/src/ZrtpPacketErrorAck.o \
    zrtp/src/ZrtpPacketPingAck.o \
    zrtp/src/ZrtpPacketPing.o \
    zrtp/src/ZrtpPacketSASrelay.o \
    zrtp/src/ZrtpPacketRelayAck.o \
    zrtp/src/ZrtpStateClass.o \
    zrtp/src/ZrtpTextData.o \
    zrtp/src/ZrtpConfigure.o \
    zrtp/src/ZrtpCWrapper.o \
    zrtp/src/Base32.o

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



