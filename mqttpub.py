import msgpack
import json
import paho.mqtt.publish as publish
from datetime import datetime
import sys

reload(sys)
sys.setdefaultencoding('utf8')



total = 50001

list = []
for i in xrange(0, total):
    dict = {"date":"24/7/2012","lineItem":"11111","quantity":"1","customerNumber":"1","poNumber":"PO1"}
    #dict = {"count": i}
    bytes = bytearray(msgpack.packb(dict))
    list.append({'topic':'/purchase', 'payload': bytes, 'qos':0})


start = datetime.now()

publish.multiple(list, hostname = 'localhost', port = 1883);

end = datetime.now();
delta = end - start
print delta.seconds, delta.microseconds
print  1000000* total/ ((delta.seconds/1000000)+ delta.microseconds)
# dict = {"date":"24/7/2012","lineItem":"11111","quantity":"1","customerNumber":"1","poNumber":"PO1"}
# print msgpack.packb(dict)
