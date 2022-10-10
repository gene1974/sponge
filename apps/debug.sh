#!/bin/sh
echo Begin compiling...
g++ webget.cc ../libsponge/util/util.cc ../libsponge/util/address.cc ../libsponge/util/socket.cc ../libsponge/util/buffer.cc ../libsponge/util/file_descriptor.cc ../libsponge/util/eventloop.cc -std=c++17
echo Compiled.
./a.out cs144.keithw.org /hello
