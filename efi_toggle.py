#!/usr/bin/python

import sys, string

def efi_toggle(set_image):
    with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
        if set_image == 'A':
            f.write(b'\x07\x00\x42\x00\x00') # "B"
        elif set_image == 'B':
            f.write(b'\x07\x00\x41\x00\x00') # "A"
    with open('/sys/firmware/efi/efivars/UPDATEFLAG-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            f.write(b"\x07\x00\x31\x00\x00") # "1"
    with open('/sys/firmware/efi/efivars/BOOTCOUNT-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            f.write(b"\x07\x00\x30\x00\x00") # "0"
    print('next boot set to image' + set_image + '.')

def get_current():
    current_image = ""
    # Iterate over EFI variable stripping out non printable characters (such as the second byte in each CHAR16)        
    with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'r') as f:
        b = f.read(1)
        while b != "":
            if b in string.printable:
                current_image += b
                b = f.read(1)
    return current_image

if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1].lower() == 'a':
            efi_toggle('A')
        elif sys.argv[1].lower() == 'b':
            efi_toggle('B')
        elif sys.argv[1] == '-c':
            print(get_current())
    else:
        print('usage: efi_toggle.py [A/a/B/b]')
        print('options:')
        print('  -c [--current]         print current toggle definition')

