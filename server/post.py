import requests
import argparse

parser = argparse.ArgumentParser(description='Post a game to PONG 9000!')
parser.add_argument('-t', metavar="title", required=False, help="What title do you want in the game browser? Under 50 chars.")
parser.add_argument("hostname", type=str, help="The same hostname coded into your arduino; from which you're sharing. Under 50 chars.")
parser.add_argument('gamefile', type=str, help="Filename containing the game code")

args = parser.parse_args()
d = vars(args)

d['gamefile'] = "../games/" + d['gamefile']
if d['t'] is None: 
    d['t'] = d['hostname'] + "'s game"
    if len(d['t']) > 50: 
        raise ValueError("Your host name is too long! Keep it under 50 characters.")

else:
    if len(d['t']) > 50: raise ValueError("Your game name is too long! Under 50 pls.")
    if len(d['hostname']) > 50: raise ValueError("Your hostname is too long! Length of under 50 characters!")

body = {}
body['host'] = d['hostname']
body['game_name'] = d['t']
code = ""

with open(d['gamefile'], 'r') as f:
    for line in f: code += line

body['game_code'] = code
url = "https://608dev-2.net/sandbox/sc/team070/request_handler/request_handler.py"

print("Posting...")
resp = requests.post(url, data = body)

print("Done!")
if "Something is not working in your code" in resp.text:
    print(resp.text)
    print("------------------------")
    print("Something is broken above!")

