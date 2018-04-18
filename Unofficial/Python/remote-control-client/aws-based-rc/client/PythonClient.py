#Written for Python 3.6.x

import urllib.request
import json
import ctypes
import random
import string
import time
import os
from subprocess import call
dir_path = os.path.dirname(os.path.realpath(__file__))

API_BASE = 'your_api_url/'
API_KEY = 'your_api_key'


#Different Types Of Boxes
#Ok Only
MB_OK = 0x0
#Ok and Cancel
MB_OKCXL = 0x01
#Yes, No and Cancel
MB_YESNOCXL = 0x03
#Yes and No
MB_YESNO = 0x04
#Help Modifier - Largely useless
MB_HELP = 0x4000
#Icons
#Exclamation Point
ICON_EXCLAIM = 0x30
#Lowercase i in circle
ICON_INFO = 0x40
#Question Mark
ICON_QUESTION = 0x20
#"Stop" Cross
ICON_STOP = 0x10

btypes_map = {"okonly": MB_OK, "okcancel": MB_OKCXL, "yesnocancel": MB_YESNOCXL, "yesno": MB_YESNO}
bicons_map = {"exclaim": ICON_EXCLAIM, "info": ICON_INFO, "question": ICON_QUESTION, "stop": ICON_STOP}

def Mbox(title, content, type=MB_OK, icon=None):
    if icon == None:
        result = ctypes.windll.user32.MessageBoxW(0, content, title, type)
    else:
        result = ctypes.windll.user32.MessageBoxW(0, content, title, type | icon)
    return result

def generateClientID():
    cID = ""
    choices = [list(string.ascii_letters), list(string.digits)]
    for i in range(0, random.randint(12,14)):
        cID = cID + random.choice(random.choice(choices))
    Mbox("Client ID", cID, MB_OK, ICON_INFO)
    return cID

cID = generateClientID()

def runner(cID):
    with urllib.request.urlopen(API_BASE+cID+"?key="+API_KEY) as response:
        jsonR = response.read()
        jsonR = json.loads(jsonR)
        return jsonR

#Mbox("Wirus Alert", "I am wirus push ok to accept wirus to you network. you need fixation network security", MB_OK, ICON_EXCLAIM)

while True:
    result = runner(cID)
    for act in result:
        if act['clientID'] == cID:
            #Good the clientID Matches
            if act['action'] == "messagebox":
                rData = act['data']
                rData = json.loads(rData)
                try:
                    Mbox(rData['title'], rData['message'], btypes_map[rData['type']], bicons_map[rData['icon_type']])
                except KeyError:
                    Mbox(rData['title'], rData['message'], btypes_map["okonly"])
            elif act['action'] == "showimage":
                rData = act['data']
                rData = json.loads(rData)
                try:
                    opener = urllib.request.build_opener()
                    opener.addheaders = [('User-agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:60.0) Gecko/20100101 Firefox/60.0')]
                    urllib.request.install_opener(opener)
                    urllib.request.urlretrieve(rData['url'], 'tmp.image')
                    #Lazy - Run the windows photo viewer of the just dl-d image.
                    call('rundll32 "C:\Program Files\Windows Photo Viewer\PhotoViewer.dll", ImageView_Fullscreen {}\\tmp.image'.format(dir_path))
                except:
                    pass
            else:
                pass
                #NO IDEA WHAT THIS ACTION IS!
        else:
            pass
            #Ignore the message, it's for someone else, shouldn't be encountered but edge case.
    time.sleep(20)
