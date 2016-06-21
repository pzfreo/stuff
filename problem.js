var mqtt    = require('mqtt');
var sleep = require('sleep');
var client = mqtt.connect("mqtt://localhost");

var counter = 0


client.on('connect', function () {
  client.subscribe('/purchase', {qos:1});
});


client.handleMessage =  function (packet, callback) {
  if (packet.cmd == "publish") {
    counter++;
    console.log(counter);
    sleep.usleep(100000);
  }
  callback();
};
