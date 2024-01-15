#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

cd $SOURCE/build/Service && chmod +x *.sh && ./ambient-light-installer.sh