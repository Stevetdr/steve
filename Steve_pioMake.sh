
#!/bin/bash
#
#  Programma rinominato in alias come "pios"
#
action=$1
# VERBOSE=-v
# VERBOSE=
# device=/dev/ttyUSB0
# device=$2


# thisDir="$(test -L "$PWD" && readlink "$PWD" || echo "$PWD")"     # risolve anche eventuali LINK presenti sullo script
# thisDir=$(cd $(dirname "$thisDir"); pwd -P)/$(basename "$thisDir")        # GET AbsolutePath
# Project_DIR=${thisDir%/.*}
# if [[ -d "$Project_DIR/src" ]]; then
#     echo $Project_DIR
# else
#     echo "$Project_DIR non contiene la directory src"
#     exit
# fi

if [[ "$action" = "" ]]; then
    echo "immettere:"
    echo "azione: (anche più di uno senza spazi intermedi)"
    echo "      b - build"
    echo "      c - clear"
    echo "      s - read serial"
    echo "      u - upload"
    echo
    # echo "device:"
    # echo "      Es. /dev/ttyUSB0"
    echo
    exit
fi


# function creaLink() {
#     ln -s "/home/pi/Loreto/GIT-REPO/LnProtocol/Arduino/LnLibraries/LnFunctions"      "${Project_DIR}/lib/LnFunctions"
#     ln -s "/home/pi/Loreto/GIT-REPO/LnProtocol/Arduino/LnLibraries/RS485_protocol"   "${Project_DIR}/lib/RS485_protocol"
#     ln -s "/home/pi/Loreto/platformio.ini"                                           "${Project_DIR}/platformio.ini"
# }

# function deleteLink() {
#     rm -f "${Project_DIR}/lib/LnFunctions"
#     rm -f "${Project_DIR}/lib/RS485_protocol"
#     rm -f "${Project_DIR}/platformio.ini"
# }



#cleam
if [[ "$action" =~ "c" ]]; then
    echo "------"
    echo "CLEAN"
    echo "------"
    echo
    platformio run --target clean
fi

# creaLink
#build
if [[ "$action" =~ "b" ]]; then
    echo "------"
    echo "BUILD"
    echo "------"
    echo
    platformio run  --environment nano
fi

#upload
if [[ "$action" =~ "u" ]]; then
    echo "------"
    echo "UPLOAD"
    echo "------"
    echo
    platformio run --target upload  --environment nano
fi

# deleteLink


#monitor
if [[ "$action" =~ "s" ]]; then
    echo "------"
    echo "MONITOR"
    echo "------"
    echo
    platformio device monitor --environment nano
fi