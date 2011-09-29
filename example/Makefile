# Modify this to point to the PJSIP location.
# PJBASE=/path/to/pjsip
PJBASE=~/devhome/pjproject.git

include $(PJBASE)/build.mak

# Include the transport specific build.mak
# The ZRTP4PJ build process creates a build.mak that sets or
# extends some pjsib defined make variables
#
# include /path/to/ZRTP4PJ/build/zsrtp/build.mak
include ~/devhome/ZRTP4PJ/build/zsrtp/build.mak

# Make sure you use the CXX (C++) compiler/linker because
# ZRTP is a C++ implementation and PJSIP uses a C-to-C++ wrapper
# to use it.
CC      = $(PJ_CXX)
LDFLAGS = $(PJ_LDFLAGS)
LDLIBS  = $(PJ_LDLIBS)
CFLAGS  = $(PJ_CFLAGS)
CPPFLAGS= ${CFLAGS}

# If your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all:  simple_pjsua # streamutilzrtp

streamutilzrtp: streamutilzrtp.c
	$(CC) -o $@ $< \
	$(CPPFLAGS) \
	$(LDFLAGS) \
	$(LDLIBS)

simple_pjsua: simple_pjsua.c
	$(CC) -o $@ $< \
	$(CPPFLAGS) \
	$(LDFLAGS) \
	$(LDLIBS)

clean:
	rm -f streamutilzrtp streamutilzrtp.o simple_pjsua simple_pjsua.o
