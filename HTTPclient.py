import requests
import json

done = False

while(not done):
    print("python HTTPclient is running.\n")

    gR = requests.get('http://192.168.0.24:5000')
    jGet = gR.content

    if(jGet["done"]):
        done = true

    with open('serverData.json', 'w') as f:
        f.seek(0)
        json.dump(jGet, f)
        jPost = json.load(f)
        pR = requests.post('http://192.168.0.24:5000', data = jPost)

exit()
