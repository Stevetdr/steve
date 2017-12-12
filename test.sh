#!/bin/bash
action=$1
sub_cmd=$2
#VERBOSE=-v
VERBOSE=
# device=/dev/ttyUSB0

function HELP {
    echo
    echo " Non hai digitato nessun comando ne comando+parametro !!!"
    echo
    echo "prova con lista oppure lista UNIX"

    exit
}

[[ "$action" = "" ]] && HELP    # non messo nessuna variabile, esegue HELP a seguire
                                # [[a=b]] da true se a=b altrimenti False

#-------------------------------------------------------------------------------------
if [[ "$action" =~ "ciao" ]]; then  #match a sinistra per la lunghezza di ciao o altro
    echo "------"
    echo " CIAO "
    echo "------"
    echo
    echo "Hai digitato il testo: " $action
    exit
fi
#-------------------------------------------------------------------------------------
if [[ "$action" =~ "lista" ]]; then
    if [[ "$SUB_CMD" = "" ]]; then
        echo "kkkk" #sub_cmd=UNIX
        exit
    echo "-------------------"
    echo "LISTA la directory "
    echo "-------------------"
    echo
    ls -la | grep "$2"
    exit
#[[ $a == z* ]]    # Vero se $a inizia con una "z" (corrispondenza di modello).
fi

#-------------------------------------------------------------------------------------
#monitor
if [[ "$action" =~ "m" ]]; then
    [[ "$device" = "" ]] && SYNTAX          # da chiarire!!!!
    echo "------"
    echo "MONITOR"
    echo "------"
    echo
    echo platformio device monitor -p $device
#    platformio device monitor -p $device
fi
#-------------------------------------------------------------------------------------
  CLEAN='platformio run --target clean'
  BUILD='platformio run --environment nano'
 UPLOAD='platformio run --target upload --upload-port /dev/ttyUSB0'
MONITOR='platformio device monitor -p /dev/ttyUSB0'