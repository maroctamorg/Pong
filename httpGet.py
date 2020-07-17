print("python httpGet is running.\n")

import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import requests
import json

gR = requests.get('http://192.168.0.35:5000/data', timeout = 0.010)
jGet = gR.json()
print('jGet: ')
print(jGet)
print('\n\n\n')

with open('serverData.json', 'w') as f:
    json.dump(jGet, f)