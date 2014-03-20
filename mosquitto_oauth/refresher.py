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



def on_message(mosq,obj,msg):
    print "on_message"
    print msg.payload
    if (msg.topic == "/r"):
        print msg.payload
        
        print binascii.hexlify(bytearray(msg.payload))
        arr = msg.payload.split('\xFF')
        clientid = arr[0]
        print arr[0]
        refreshtoken = arr[1].split("\0")[0]
        token = refresh_token(refreshtoken).encode('ascii')
        print token
        client.publish("/c/"+clientid, token+"\0");
    return
        

def refresh_token(token):
    h = httplib2.Http( disable_ssl_certificate_validation=True)


    data = dict(grant_type="refresh_token", refresh_token=token, client_id=clientid, client_secret=clientsecret)
    resp, content = h.request(token_url,
        "POST", body=urlencode(data),
        headers={'content-type':'application/x-www-form-urlencoded'} )
    print resp            
    if (resp['status'] == "200"):
        response=json.loads(content)
        return response['access_token']
    else:
        return "failed"
    

client = mosquitto.Mosquitto("refresher")
client.username_pw_set("admin", "admin")
client.connect("localhost")
client.subscribe("/r")
client.on_message = on_message
client.loop_forever()