#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

cmake -S $SOURCE -B $SOURCE/build && cmake --build  $SOURCE/build --config Release