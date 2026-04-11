#!/bin/bash

cd ~/Projects/3DS_Visit_Caard
pwd

echo "Nettoyage..."
make clean

echo "Build..."
make

if [ $? -eq 0 ]; then
    echo "Build réussi ! -> 3DSProject.3dsx"
else
    echo "Erreur de build."
    exit 1
fi