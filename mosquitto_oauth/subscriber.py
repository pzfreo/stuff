import mosquitto
import httplib2
import urlparse 
import json
from urllib import urlencode
import binascii
import time

clientid="oefnUnEFx9tyLr9MwHykI8x0Vqga"
clientsecret="qtElvfENMRf72pyQEKrIRToZoMUa"
token_url="http://localhost:9763/oauth2/token"
refresh = "257f8273e0afc89c89f0ebb5a6d84ae"

def on_message(mosq,obj,msg):
    print msg.payload
    return
        
h = httplib2.Http( disable_ssl_certificate_validation=True)

data = dict(grant_type="refresh_token", refresh_token=refresh, client_id=clientid, client_secret=clientsecret)
resp, content = h.request(token_url,
	"POST", body=urlencode(data),
	headers={'content-type':'application/x-www-form-urlencoded'} )
response=json.loads(content)
bearer =  response['access_token']
print "received new bearer: "+bearer

client = mosquitto.Mosquitto("subscriber")
client.username_pw_set(bearer, "dummy")
client.connect("localhost")
client.subscribe("/pzf/ypr")
client.on_message = on_message
client.loop_forever()