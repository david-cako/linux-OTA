#!/bin/bash
### BEGIN INIT INFO
# Provides:          efi-boot-toggle
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
    echo "0" > $UPDATEFLAG
    echo "0" > $BOOTCOUNT
fi

exit 0