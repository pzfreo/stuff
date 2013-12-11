This is some various bits of code I wrote at CommsHack. Unfortunately I was only there 
for about 90 minutes so I didn't complete it. 

Basically it all depends on having a mosquitto / mqtt broker locally.

Weatherscript.py periodically grabs weather data from openweather and reformats it into a slightly simpler json
before posting to /weather/{country}/{city}/

smslistener.js is a webhook for calls from Twilio. It publishes the GET URL to /sms

smsrepub is a python script that turns the url params into json and republishes to /smsjson

sms.py takes any messages from /phone/{number} and sms-es via twilio

subscribe.py wasn't finished. This is basically designed to read sms messages and weather data and then republish interesting events to /phone.

