print("python httpEstablish is running.\n")

import sys

print(sys.version)
print('\n')

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import time
import requests
import json


while (True):
    time.sleep(1)
    print("Establishing session!\n")
    gR = requests.get('http://192.168.0.35:5000/establish', timeout = 0.010)
    response = gR.json()
    print(response)
    print(response['sss1wEst'] and response['sss2wEst'])
    if (response['sss1wEst'] and response['sss2wEst']):
        with open("serverData.json", "w") as serverData:
            json.dump(response, serverData)
        break