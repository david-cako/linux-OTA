#!/usr/bin/python
import urllib2, json, string
from hashlib import md5

UPDATE_SERVER = "URL"
CURRENT_RELEASE = os.environ("VARIABLE")
BOOT_PATH = "/mnt/flash/"

class Update(Object):
    def __init__():
        self.current_image = ""
        # Iterate over EFI variable stripping out non printable characters (such as the second byte in each CHAR16)        
        with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'r') as f:
            b = f.read(1)
            while b != "":
                if b in string.printable:
                    self.current_image += b
                b = f.read(1)

    def download_update():
        # check current image and write to opposite image
        if self.current_image == "a":
            output_path = BOOT_PATH + "bzImage_b.efi"
        elif self.current_image == "b":
            output_path = BOOT_PATH + "bzImage_a.efi"            
        r = urllib2.urlopen(UPDATE_SERVER)
        release_data = json.loads(r.read())
        if release_data['datestamp'] > CURRENT_RELEASE:
            update = requests.get(release_data['update_url'], stream=True)
            with open(output_path, 'w') as f:
                for chunk in r.iter_content(chunk_size=1024):
                    if chunk:
                        f.write(chunk)
        if md5(output_path).hexdigest() == release_data['md5']:
            self.efi_toggle()

    def efi_toggle():
        with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            if self.current_image == "a":
                f.write("")

get_current_image()




