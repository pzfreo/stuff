var mqtt    = require('mqtt');
var msgpack = require("msgpack-lite");
var client = mqtt.connect("mqtt://localhost");
// var mongoose = require("mongoose");
var sleep = require('sleep');
// mongoose.connect('mongodb://localhost/test');
var counter = 0
// var poSchema = mongoose.Schema({
//     date: String,
//     id: {type: String, unique: true},
//     lineItem: Number,
//     quantity: Number,
//     poNumber: String
//    });
// var Purchase = mongoose.model('PurchaseOrder', poSchema);

var uuid = require('uuid');

client.on('connect', function () {
  client.subscribe('/purchase', {qos:1});
});

// client.on('message', function (t,p) {});

client.handleMessage =  function (packet, callback) {
  // payload is msgpack buffer
  if (packet.cmd == "publish") {
    counter++;
    console.log(counter);
    // po = new Purchase(msgpack.decode(packet.payload));
    //console.log(msgpack.decode(packet.payload));
    // po.save();
    sleep.usleep(100000);
  }

  callback();

};
