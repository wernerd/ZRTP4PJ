## How to run zrtptest ##

_zrtptest_ is a really simple programm to test the ZRTP stack for PJSIP. 
It only accepts a few command line options to set a remote IP address, a
local port and a masqurading option. To run _zrtptest_ locally you must
start it in different directories to have seperate ZID cache files.

In the following examples `$TESTPATH` is the path to _zrtptest_. In a first
command line window setup a first _zrtptest_:

    mkdir first
    cd first
    $TESTPATH/zrtptest --remote 127.0.0.1:4002

This starts _zrtptest_ and seets the remote address to the local loopback, 
port number 4002. The program listens to port number 4000 by default.

In a second command line window set up a second _zrtptest_:

    mkdir second
    cd second
    $$TESTPATH/zrtptest --remote 127.0.0.1:4000 --local-port=4002

This starts the test program which listens to port 4002 and sends to local
loopback, port 4000.

After start both programs listen to command input:

    Commands:
      s     Send one data packet
      q     Quit

Type `s` to send one data packet, it's a simple, constructed RTP look-alike
packet. The test program initializes the ZRTP stack in _auto-enable_ mode thus
ZRTP negotiations starts as soon as it sees a RTP data flow. After ZRTP completes
its negotiation the ZRTP transport encrypts all other packets.
