#!/bin/bash
### BEGIN INIT INFO
# Provides:          efi-ota
# Required-Start:
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Finalize software update, assuring EFI-side program of successful boot.
# Description:  
### END INIT INFO

UPDATEFLAG=/sys/firmware/efi/efivarfs/UPDATEFLAG-6302d008-7f9b-4f30-87ac-60c9fef5da4e
BOOTCOUNT=/sys/firmware/efi/efivarfs/BOOTCOUNT-6302d008-7f9b-4f30-87ac-60c9fef5da4e

mount -t efivarfs none /sys/firmware/efi/efivars;

if [[ $(cat $UPDATEFLAG) == *"1"* ]]; then
    echo -n -e "\x07\x00\x30\x00\x00" > $UPDATEFLAG     # "0"
    echo -n -e "\x07\x00\x30\x00\x00" > $BOOTCOUNT      # "0"
fi

exit 0