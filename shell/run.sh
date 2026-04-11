#!/bin/bash

PROJET=~/Projects/3DS_Visit_Caard
FICHIER="$PROJET/build/3ds/Higher_Caard.3dsx"

if [ ! -f "$FICHIER" ]; then
    echo "3DSProject.3dsx introuvable, build en cours..."
    cd "$PROJET" && make
    if [ $? -ne 0 ]; then
        echo "Erreur de build."
        exit 1
    fi
fi

echo "Lancement dans Azahar..."
flatpak run \
    --env=__NV_PRIME_RENDER_OFFLOAD=1 \
    --env=__GLX_VENDOR_LIBRARY_NAME=nvidia \
    org.azahar_emu.Azahar "$FICHIER"
