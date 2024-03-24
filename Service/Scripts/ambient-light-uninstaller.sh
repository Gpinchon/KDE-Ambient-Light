#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
BINDIR="/usr/bin"
SERVICEDIR="/etc/systemd/user"

if test -f "$BINDIR/ambient-light"; then
    echo "Uninstalling Ambient Light service"
    echo "Stopping ambient-light.service"
    systemctl --user stop ambient-light.service
    echo "Disabling ambient-light.service"
    sudo systemctl --global disable ambient-light.service
    echo "Removing $SERVICEDIR/ambient-light.service"
    sudo rm "$SERVICEDIR/ambient-light.service"
    echo "Removing $BINDIR/ambient-light"
    sudo rm "$BINDIR/ambient-light"
    systemctl --user daemon-reload
    echo "Uninstallation done !"
else
    echo "Ambient Light Service not installed !"
fi