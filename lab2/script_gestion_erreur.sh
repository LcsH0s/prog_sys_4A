#!/bin/bash

NB_USERS=$(cat /etc/passwd | wc -l)

# le fichier ''/etc/password'' n’existe pas (c’est ''/etc/passwd'')

if [ $NB_USERS -gt 0 ]; then
    echo ''$NB_USERS sont déclarés sur votre système''
else
    echo ''une erreur a dû se produire''
fi
