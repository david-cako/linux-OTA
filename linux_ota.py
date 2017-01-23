#!/usr/bin/python
import urllib2, json, string, logging, os, sys
from hashlib import md5

UPDATE_URL = "URL"
CURRENT_RELEASE = os.environ["VARIABLE"]
BOOT_PATH = "/mnt/flash/"

logging.basicConfig(filename='/var/log/linux-ota.log', level=logging.INFO)

class Update(object):
    def __init__(self, update_url):
        self.update_url = update_url
        self.current_image = ""
        # Iterate over EFI variable stripping out non printable characters (such as the second byte in each CHAR16)        
        with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'r') as f:
            b = f.read(1)
            while b != "":
                if b in string.printable:
                    self.current_image += b
                b = f.read(1)

    def fetch_update(self):
        # check current image and write to opposite image.  will probably need to add tar extraction and initramfs.
        if self.current_image == "A":
            output_path = BOOT_PATH + "bzImage_b.efi"
        elif self.current_image == "B":
            output_path = BOOT_PATH + "bzImage_a.efi"            
        logging.info('checking for update...')
        try:
            r = urllib2.urlopen(self.update_url)
        except urllib2.URLError:
            logging.error('unable to contact update server.')
            sys.exit('unable to contact update server.')
        release_data = json.loads(r.read())
        if release_data['datestamp'] > CURRENT_RELEASE:
            logging.info('new release available: ' + release_data['datestamp'])
            update = requests.get(release_data['update_url'], stream=True)
            with open(output_path, 'w') as f:
                for chunk in r.iter_content(chunk_size=1024):
                    if chunk:
                        f.write(chunk)
        else:
            logging.info('no new release found.')
            sys.exit();
        update_md5 = md5(output_path).hexdigest()
        if update_md5 == release_data['md5']:
            logging.info('update successfully downloaded; md5: ' + update_md5)
            self.efi_toggle()
        else:
            logging.error('update could not be downloaded; md5 mismatch')
            sys.exit('update could not be downloaded; md5 mismatch')

    def efi_toggle():
        with open('/sys/firmware/efi/efivars/BOOTTOGGLE-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            if self.current_image == 'A':
                f.write(b'\x07\x00\x42\x00\x00') # "B"
                self.current_image = 'B'
            elif self.current_image == 'B':
                f.write(b'\x07\x00\x41\x00\x00') # "A"
                self.current_image = 'A'
        with open('/sys/firmware/efi/efivars/UPDATEFLAG-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            f.write(b"\x07\x00\x31\x00\x00") # "1"
        with open('/sys/firmware/efi/efivars/BOOTCOUNT-6302d008-7f9b-4f30-87ac-60c9fef5da4e', 'w') as f:
            f.write(b"\x07\x00\x30\x00\x00") # "0"
        logging.info('update complete.  next boot set to image' + self.current_image + '.')
        sys.exit()

if __name__ == '__main__':
    update = Update(UPDATE_URL)
    update.fetch_update()




