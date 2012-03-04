## General

This file contains the sources and build files to create the GNU ZRTP
modules for pjproject's ZRTP support. In additon this file also contains
a SRTP implementation that support different key lengths. The distribution
also contains all files to build the GNU ZRTP library, thus no special 
download is necessary.

The directory structure and the build process follows the well known and
established process of pjproject's third_party build process. 

The structure is:

    build/zsrtp/             # contains the Makefile
    example/                 # a modified simple_pjsua.c
    zsrtp/                   # Contains transport_zrtp
    |-- include
    |   `-- crypto           # *.h files for PJSIP ZRTP transport and SRTP
    |-- srtp                 # SRTP source for PJSIP
    `-- zrtp                 # GNU ZRTP sources, cloned via 'getzrtp.sh'


## Building

The only prerequisits the build ZRTP for PJ are:

- openSSL development environment
- a C and C++ compiler (tested with gcc and g++)
- installed and build pjproject - tested with 1.8.5 and 1.10 and the latest SVN trunk
- to use ZRTP together with PJSUA you may need to apply a patch to add a specific
  callback mechanism. The patch is quite small and should work without any
  problems. After applying the patch just rebuild pjsip / pjsua.
  *NOTE:* Since 17-Jun-2011 this callback function is part of PJSIP's SVN repository
  and is available in the SVN branch 1.x. If you use this branch and a
  recent SVN version then _do not apply the patch_ and just use ZRTP4PJ.

You may clone this directory or get the pre-packaged tar file (see
Download). If ou clone this repository just change to the cloned repository
and skip the steps to copy and unpack the tar file.

Copy the ZSRTP4PJ.tar to your pjproject's third_party directory, for example:

    cp ZSRTP4PJ.tar ~/development/pjproject/third_party

Unpack the tar file:

    cd ~/development/pjproject/third_party
    tar xvf ZSRTP4PJ.tar

The tar file and the cloned repository does not contain the ZRTP and its
associated SRTP sources. To get these sources change to the _zsrtp_ directory
and get the sources.

    cd zsrtp
    sh getzrtp.sh
    cd ..

The shell scripts clones the ZRTP source repository into the _zrtp_
directoy. If this directory already exists then the script updates the sources
to get the latest version.

Before you can build the project you need to adjust a path setting in the
Makefile. Change to the correct build directory and open the Makefile file
with your preferred text edito. Adjust the setting of the variable PJDIR to
your environment. Store the makefile and run make dep and make.

    cd build/zsrtp
    make dep
    make

If make does not report errors (some warnings are displayed) the build was 
successful and the static library was copied to 

    ~/development/pjproject/third_party/lib

Now the ZRTP for PJ is ready to use.


## Building an application

Create a makefile that follows the known pjproject pattern for
makefiles. The following annotated example shows the important parts of
the example makefile (see example directory):

    # Modify this to point to your pjproject location.
    PJBASE = ~/development/pjproject

    # include pjproject's standard build.mak
    include $(PJBASE)/build.mak

    # include the ZRTP specific build.mak. The ZRTP build process creates
    # this build.mak. It modifies some variable to include the ZRTP library
    # and the ZRTP include path
    include $(PJBASE)/third_party/build/zsrtp/build.mak

    # Make sure to use the C++ compiler as defined by $(PJ_CXX). This is
    # necessary because GNU ZRTP uses C++
    CC      = $(PJ_CXX)
    LDFLAGS = $(PJ_LDFLAGS)
    LDLIBS  = $(PJ_LDLIBS)
    CFLAGS  = $(PJ_CFLAGS)
    CPPFLAGS= ${CFLAGS}

    # Here we create a modified version of pjproject's simple_pjsua.
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

After you adapted your makefile just run `make` to create the application

## Some documentation

The source code contains a lot of inline documentation and is ready for doxygen.
The makefile in `build/zsrtp` contains a `doc` make target to produce the documentation files,
just call `make doc`. Please generate them and you have all documentation ready for browsing.

The directory `example` contains a slightly modified version of `simple_pjsua.c` together
with the makefile to build it. You probably need to adjust the makefile to reflect your
development environment and adapt `simple_pjsua.c` to your SIP environment. This
modified version shows how to setup the ZRTP callback structures, register ZRTP callbacks,
and how to initialize and start ZRTP.
