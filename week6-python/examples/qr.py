import os
import qrcode

img = qrcode.make("https://www.linkedin.com/in/dansvendsen/")
img.save("qr.png", "PNG")
os.system("open qr.png")