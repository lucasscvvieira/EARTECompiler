#!/bin/bash

DISTRO=$(awk -F= '$1=="ID" { print $2 ;}' /etc/os-release)

if [ $DISTRO = "ubuntu" ]; then
    echo "Ubuntu reconhecido! Instalando dependências..."
    echo
    sudo apt-get install -y gcc make build-essential flex bison indent
else
    echo "SO não reconhecido pela script"
fi
