#!/bin/bash
echo "============= start build NET =============="
cd build/linux
sourcr config
make release

echo "============= build NET done =============="