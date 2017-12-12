
#!/bin/bash
var=$1

#VERBOSE=-v
VERBOSE=


function HELP {
    echo
    echo " Non hai digitato nessun comando per la ricerca"
    echo
    echo " Prova con h.sh pippo"

    exit
}


[[ "$var" = "" ]] && HELP    # non messo nessuna variabile, esegue HELP a seguire
                                #

echo "Ricerca su log di :" $1
history | grep $1
exit
