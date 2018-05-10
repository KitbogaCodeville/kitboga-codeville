# Written for Python 3.6.x

import json
import os
import random
import string
import subprocess
import time
from urllib import request

from winapi import WinApi

class Client:

    def __init__(self, api_base, api_key):
        self.api_base = api_base
        self.api_key = api_key

        self.client_id = self.generate_id()
        self.current_path = os.path.dirname(os.path.realpath(__file__))

    def generate_id(self):
        client_id = ''.join(random.choices(string.ascii_letters + string.digits, k=random.randint(12, 14)))

        WinApi.show_message('Client ID', client_id, WinApi.BoxType.MB_OK, WinApi.IconType.INFO)

        return client_id

    def get_response(self, client_id):
        with request.urlopen(f'{self.api_base}{client_id}?key={self.api_key}') as response:
            return json.loads(response.read())

    def loop(self):
        while True:
            response = self.get_response(self.client_id)

            for item in response:
                # Check if the server echos back the client id
                if item.get('clientID') == self.client_id:
                    action = item.get('action')

                    if action and item.get('data'):
                        data = json.loads(item['data'])

                        if action == 'messagebox':
                            WinApi.show_message(
                                data.get('title') or '',
                                data.get('message') or '',
                                WinApi.box_type(data.get('type') or ''),
                                data.get('icon') and WinApi.IconType[data['icon'].upper()] or None
                            )

                        elif action == 'showimage':
                            opener = request.build_opener()
                            opener.addheaders = [('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64; x64; rv:60.0) Gecko/20100101 Firefox/60.0')]
                            request.install_opener(opener)

                            request.urlretrieve(data.get('url') or '', 'image.tmp')

                            subprocess.call(f'rundll32 "C:\Program Files\Windows Photo Viewer\PhotoViewer.dll", ImageView_Fullscreen {self.current_path}\\image.tmp')

            time.sleep(20)
