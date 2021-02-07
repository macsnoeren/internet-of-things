const express = require('express');
const router  = express.Router();
const mqtt    = require('mqtt');

const config  = require('../config.json');
const wemos   = require('./wemos');

const mqttClient = mqtt.connect(config.mqtt.broker);

var Module = module.exports;
Module.publishEvent = publishEvent;

mqttClient.on('connect', () => {
	console.log('Connected to MQTT!');

	mqttClient.subscribe("iwsn-wemos", function (err) {
		if (err) {
		  console.log("MQTT Error: " + err.message);
		}
	});

	mqttClient.subscribe("iwsn-wemos-event", function (err) {
		if (err) {
			console.log("MQTT Error: " + err.message);
		}
	});
});

mqttClient.on('message', function (topic, message) {
	message = JSON.parse(message.toString());
	topic   = topic.toString();

	if ( topic == "iwsn-wemos" && message.looptiming ) {
        wemos.addLog(topic, message);
    }

	if ( topic == "iwsn-wemos" && message.state ) {
        wemos.addState(topic, message);
    }
});

function publishEvent(event) {
    mqttClient.publish("iwsn-wemos-event", JSON.stringify({event: event}));
}
