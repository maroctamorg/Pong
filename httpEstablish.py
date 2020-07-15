print("python httpEstablish is running.\n")

import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import requests
import json

gR = requests.get('http://192.168.0.24:5000/', timeout = 0.010)

while (not (gR["sss1wEst"] and gR["sss2wEst"])):
    print("Establishing session!\n")
    gR = requests.get('http://192.168.0.24:5000/', timeout = 0.010)

sys.exit()