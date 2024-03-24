#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
BINDIR="/usr/bin"
SERVICEDIR="/etc/systemd/user"

sh ambient-light-uninstaller.sh
echo "Copying ambient-light.service to $SERVICEDIR/ambient-light.service"
sudo cp "$SOURCE/ambient-light.service" "$SERVICEDIR/ambient-light.service"
echo "Copying ambient-light to $BINDIR/ambient-light"
sudo cp "$SOURCE/ambient-light" "$BINDIR/ambient-light"
echo "Relaunching systemd"
systemctl --user daemon-reload
echo "Starting Ambient Light Service"
systemctl --user start ambient-light.service
echo "Enabling Ambient Light Service"
sudo systemctl --global enable ambient-light.service
echo "Installation done !"