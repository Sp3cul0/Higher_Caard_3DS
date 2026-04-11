#!/bin/bash

echo "Serveur HTTP démarré sur le port 8080..."
echo "Appuie sur Ctrl+C pour arrêter."
python3 -m http.server 8080 --directory ~/Projects/3DSProject/