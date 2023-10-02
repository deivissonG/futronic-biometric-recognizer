#!/usr/bin/env bash

if [ ! -d "/usr/local/bin" ]; then
    mkdir -p /usr/local/bin
fi

echo "Copying files to /usr/local/bin"
cp -f ./bozorth3 /usr/local/bin
chmod +rx /usr/local/bin/bozorth3
echo "bozorth3 copied"
cp -f ./mindtct /usr/local/bin
chmod +rx /usr/local/bin/mindtct
echo "mindtct copied"
cp -f ./cwsq /usr/local/bin
chmod +rx /usr/local/bin/cwsq
echo "cwsq copied"

if [ ! -d "/usr/local/lib" ]; then
    mkdir -p /usr/local/lib
fi
echo "Copying files to /usr/local/lib"
cp -f ./libfbr.so /usr/local/lib
chmod +rx /usr/local/lib/libfbr.so
echo "libfbr.so copied"
cp -f ./libScanAPI.so /usr/local/lib
chmod +rx /usr/local/lib/libScanAPI.so
echo "libScanAPI.so copied"