
import mosquitto
import httplib2
import urlparse 
import json
from twilio.rest import TwilioRestClient

def on_message(mosq,obj,msg):
   repub(msg.payload)

def repub(message):
   data = urlparse.parse_qs(message)
   client.publish("/smsjson",json.dumps(data))

client = mosquitto.Mosquitto("sms-repub")
client.connect("localhost")
client.subscribe("/sms/#")
client.on_message = on_message
client.loop_forever()

