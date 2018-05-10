# A file for you to run the client with ease.
from optparse import OptionParser
import os
import sys

from client import Client

parser = OptionParser()
parser.add_option('-k', '--key', dest='api_key', help='The Api Key', metavar='api_key', default=None)
parser.add_option('-b', '--base', dest='api_base', help='Base url of the api', metavar='base_url', default=None)

(options, args) = parser.parse_args()

API_KEY = os.environ.get('AWS_CONTROL_API_KEY') or options.api_key
API_BASE = os.environ.get('AWS_CONTROL_API_BASE') or options.api_base

if not API_KEY or not API_BASE:
    print('Invalid API_KEY/API_BASE set, please use `file.py -k <KEY> -b <BASE>` or set the `AWS_CONTROL_API_KEY/AWS_CONTROL_API_BASE` env vars!')
    sys.exit(0)

client = Client(API_BASE, API_KEY)

# loop blocks until it's exited
client.loop()

print('Exiting program...')
