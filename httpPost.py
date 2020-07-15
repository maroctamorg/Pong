import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import requests
import json

print("python httpPost is running.\n")

with open('serverData.json', 'r') as f:
        jPost = json.load(f)
        pR = requests.post('http://192.168.0.24:5000/session', data = jPost, timeout = 0.010)

sys.exit()