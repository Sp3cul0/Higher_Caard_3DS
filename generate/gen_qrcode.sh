#!/bin/bash

if ! command -v qrencode &> /dev/null; then
    echo "qrencode non trouvé, installation..."
    sudo dnf install qrencode -y
fi

IP=$(ip addr show | grep "inet " | grep -v 127.0.0.1 | awk '{print $2}' | cut -d'/' -f1 | head -1)

if [ -z "$IP" ]; then
    echo "Impossible de détecter l'IP locale."
    exit 1
fi

PORT=8080
FICHIER="Higher_Caard.cia"
URL="http://$IP:$PORT/$FICHIER"
OUTPUT="$HOME/Projects/3DSProject/qrcode.png"

echo "IP détectée : $IP"
echo "URL générée : $URL"

qrencode -o "$OUTPUT" -s 10 "$URL"

echo "QR code généré : $OUTPUT"
echo "Scanne-le avec FBI sur ta 3DS !"

if command -v eog &> /dev/null; then
    eog "$OUTPUT" &
elif command -v feh &> /dev/null; then
    feh "$OUTPUT" &
elif command -v xdg-open &> /dev/null; then
    xdg-open "$OUTPUT" &
fi