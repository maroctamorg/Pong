import sys

sys.path.append('/Library/Frameworks/Python.framework/Versions/3.8/lib/python3.8/site-packages')

import requests
import json

print("python httpPost is running.\n")

with open('serverData.json', 'r') as f:
        jPost = json.load(f)
        print(jPost)
        pR = requests.post('http://192.168.0.35:5000/data', json = jPost, timeout = 0.010)