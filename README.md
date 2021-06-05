# Nuodai The Filesystem

This is C++ filesystem implementation attempt.

# Configuration

to compile and run this amazig piece of flawless software you need

install pkg-config, g++, make libfuse3, fuse3, jsoncpp, libcurl, libssl (openssl)

Go to folder, and:

`make clean && make`

you'll end up with mkfs and mount files there

use 

`mkfs <desired config name>`

and follow instructions

then 

`mount <mountpoint>`

For now you must name your config config.bin


# Testing

To test you need to install libcppunit 
and then 

`make clean&&make&&make test`

And not segfault in the proces :)