import mosquitto
import json

subscribers = [ { 'phone': '447740199729', 'area':'GB-Portsmouth' }]

latest =  {'GB-Nowhere':""} ;

def on_message(mosq,obj,msg):
	topic = msg.topic.split('/');
	print topic
	if (topic[1]=="weather"):
		weather(msg)
	elif (topic[1]=="smsjson"):
		sms(msg)

def sms(msg):
  data = "";
  frm = ""; 
  try:
    data = json.loads(msg.payload)
    frm = data['From'][0][1:] # REMOVE THE PLUS  
  except Exception as inst:
    print "can't read phone number"
    return
  
  try:
    text = data['Body'][0]
    print text, frm
    area = text.split(' ')
    country = area[0]
    city = area[1]
    areatoadd = country+'-'+city
    sub = { 'phone': frm, 'area': areatoadd}
    print "added subscriber "+json.dumps(sub)
    subscribers.append(sub)
    if (areatoadd in latest):
      client.publish("/phone/"+frm, "subscription successful")
      client.publish("/phone/"+frm, json.dumps(latest[areatoadd]))
  except Exception as inst:
    print inst
    client.publish("/phone/"+frm, "sorry subscription failed - maybe you used the wrong format?")


def updateandsend(key, data):
	latest[key] = data;
	for sub in subscribers:
		if (sub['area']==key):
			print 'sending to '+sub['phone']+': '+json.dumps(data)
			client.publish("/phone/"+sub['phone'],json.dumps(data))


def weather(msg):
   topic = msg.topic.split('/')
   country = topic[2]
   city = topic[3]
   print country, city
   data = json.loads(msg.payload)
   key = country+'-'+city;
   if (key in latest):
	if  (latest[key] == data):
		print 'here'
	else:
		updateandsend(key, data)		
   else: 
	updateandsend(key, data)


  
   

client = mosquitto.Mosquitto("sms-subscribe")
client.connect("localhost")
client.subscribe("/#")
client.on_message = on_message
client.loop_forever()

