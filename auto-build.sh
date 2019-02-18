#!/bin/bash
echo "============= start build NET =============="
cd build/linux
source config
make release

echo "============= build NET done =============="