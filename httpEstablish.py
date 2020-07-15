print("python httpEstablish is running.\n")

import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import time
import requests
import json

gR = requests.get('http://192.168.0.35:5000/', timeout = 0.010)
response = gR.json()

while (not (response['sss1wEst'] and response['sss2wEst'])):
    time.sleep(1)
    print("Establishing session!\n")
    gR = requests.get('http://192.168.0.35:5000/', timeout = 0.010)

sys.exit()