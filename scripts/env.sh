#!/bin/bash

OS=$(lsb_release -a | grep "Distributor ID:" | cut -d ":" -f 2 | sed -e "s/ //g" -e "s/\t//g")

if [ $OS = "Ubuntu" ]
then
    echo "Ubuntu reconhecido! Instalando dependências..."
    echo
    sudo apt-get install -y gcc make build-essential flex bison indent
else
    echo "SO não reconhecido pela script"
fi