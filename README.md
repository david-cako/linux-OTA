#EFI-OTA

Simple EFI program allowing for safe OTA updates using A/B-split booting.

EFI variable `BOOTTOGGLE` is made accessible in Linux through `efivarfs` interface, which is then set to "A" or 
"B" to boot from an updated image in-place, while allowing fallback for failed updates.  An init script is used 
to set `UPDATEFLAG` and `BOOTCOUNT` to zero on successful boot.

Compiles on Linux using EDK.  Couldn't get EDK working on macOS.

#### efi actions:
- if `UPDATEFLAG` == 0:
    - boot `BOOTTOGGLE`
- if `UPDATEFLAG` == 1:
    - if `BOOTCOUNT` == 0 (not yet attempted):
        - `BOOTCOUNT` = 1
        - boot `BOOTTOGGLE`
    - if `BOOTCOUNT` == 1 (boot from new image unsucessful):
        - `BOOTTOGGLE` = opposite script
        - `UPDATEFLAG` = 0
        - `BOOTCOUNT` = 0
        - boot modified `BOOTTOGGLE`

#### linux init script actions:
- if `UPDATEFLAG` == 0:
    - do nothing
- if `UPDATEFLAG` == 1 (boot must have been successful):
    - `UPDATEFLAG` = 0
    - `BOOTCOUNT` = 0

- `BOOTTOGGLE` is set during update
