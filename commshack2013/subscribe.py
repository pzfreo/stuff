
import mosquitto
import json

subscriptions = []

def on_sms(mosq,obj,msg):
  if (msg.topic.find("/smsjson")):
    sms(msg)
  if (msg.topic.find("/weather")): 
    weather(msg)

def sms(msg):
  data = json.loads(msg)
  text = data['Body']
  frm = data['From']  
  print text, frm
  

def weather(msg):
   data = json.loads()

   

client = mosquitto.Mosquitto("sms-subscribe")
client.connect("localhost")
client.subscribe("/smsjson/")
client.on_message = on_sms
client.loop_forever()

