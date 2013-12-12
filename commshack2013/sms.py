import mosquitto
import httplib2
from urllib import urlencode
import json
from twilio.rest import TwilioRestClient
import os


def on_message(mosq,obj,msg):
   print "received"
   print msg.topic[7:]
   text(msg.topic[7:],msg.payload)

def text(number,message):
  # Download the twilio-python library from http://twilio.com/docs/libraries
 
  # Find these values at https://twilio.com/user/account

  account_sid = os.getenv('TWILIO_SID')
  auth_token = os.getenv('TWILIO_TOKEN')
  phone_number = os.getenv('TWILIO_NUMBER')
  client = TwilioRestClient(account_sid, auth_token)
 
  message = client.messages.create(to=number, from_=phone_number,
                                     body=message)


client = mosquitto.Mosquitto("sms-sender")
client.connect("localhost")
client.subscribe("/phone/#")
client.on_message = on_message
client.loop_forever()

