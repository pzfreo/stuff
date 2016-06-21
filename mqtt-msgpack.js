var mqtt    = require('mqtt');
var msgpack = require("msgpack-lite");
var client = mqtt.connect("mqtt://localhost");
var mongoose = require("mongoose");
var sleep = require('sleep');
var uuid = require('uuid');
mongoose.connect('mongodb://localhost/test');
var counter = 0
var time  = 0;
var poSchema = mongoose.Schema({
    date: String,
    id: {type: String, unique: true},
    lineItem: Number,
    quantity: Number,
    poNumber: String
   });
var Purchase = mongoose.model('PurchaseOrder', poSchema);

var uuid = require('uuid');

client.on('connect', function () {
  client.subscribe('/purchase', {qos:0});
});

// client.on('message', function (t,p) {});

client.handleMessage =  function (packet, callback) {
  // payload is msgpack buffer
  if (packet.cmd == "publish") {
    if (counter == 0) {
      time = new Date().getTime();
    }
    counter++;
    if (counter>=10000) {
      nw = new Date().getTime();
      delta = nw - time;
      console.log(10000000/delta);
      counter = 0;
    }

    var t = msgpack.decode(packet.payload);
    t.id = uuid.v4()
    po = new Purchase(t);
    //console.log(msgpack.decode(packet.payload));
    po.save();

  }

  callback();

};
