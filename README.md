#EFI-boot-toggle

EFI program allowing for A/B split booting based on a linux-accessible EFI variable.

Standard use case is OTA-update implementations, where you have this program invoked by startup.nsh, and then have a boot_a.nsh and boot_b.nsh for each image.

EFI variable "BOOTTOGGLE" can be made accessible in Linux through `efivarfs` interface, which you can then set to "A" or "B" to boot from an updated image in-place, while allowing fallback for failed updates.
