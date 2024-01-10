#!/bin/bash

SOURCE="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

if compgen -G "/usr/bin/ambient-light" > /dev/null;
then
    echo "Uninstalling Ambient Light service"
    echo "Stopping ambient-light.service"
    systemctl stop ambient-light.service;
    echo "Removing /etc/systemd/system/ambient-light.service"
    sudo sh -c "rm /etc/systemd/system/ambient-light.service"
    echo "Removing /usr/bin/ambient-light"
    sudo sh -c "rm /usr/bin/ambient-light"
    systemctl daemon-reload
    echo "Uninstallation done !"
fi