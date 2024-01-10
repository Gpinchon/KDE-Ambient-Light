#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

sh ambient-light-uninstaller.sh
echo "Copying ambient-light.service to /etc/systemd/system/ambient-light.service"
sudo sh -c "cp '$SOURCE'/ambient-light.service /etc/systemd/system/ambient-light.service"
echo "Copying ambient-light to /usr/bin/ambient-light"
sudo sh -c "cp '$SOURCE'/ambient-light /usr/bin/ambient-light"
echo "Relaunching systemd"
systemctl daemon-reload
echo "Starting Ambient Light service"
systemctl start ambient-light.service
echo "Enabling Ambient Light service"
systemctl enable ambient-light.service
echo "Installation done !"