var http = require('http');
var mqtt = require('mqtt')

client = mqtt.createClient(1883, 'localhost');

http.createServer(function (req, res) {
  console.log(req.headers);
  console.log(req.method);
  console.log(req.url);
  client.publish('/sms', req.url);

}).listen(8080);
