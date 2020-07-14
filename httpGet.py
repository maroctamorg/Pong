print("python httpGet is running.\n")

import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import requests
import json

gR = requests.get('http://192.168.0.24:5000', timeout = 0.010)
jGet = gR.content

with open('serverData.json', 'w') as f:
    f.seek(0)
    json.dump(jGet, f)

sys.exit()