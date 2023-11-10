#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Veuillez fournir un argument."
  exit 1
fi

arg=$1

if [ -d "$arg" ]; then
  echo "$arg : est un répertoire"
elif [ -f "$arg" ]; then
  echo "$arg : est un fichier"
else
  echo "ERROR : $arg n'est ni un répertoire ni un fichier"
fi
