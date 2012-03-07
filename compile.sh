#!/bin/sh
echo "now compiling server"
gcc -o server-tcp server-tcp.c

echo "now compiling client"
gcc -o client-tcp client-tcp.c


