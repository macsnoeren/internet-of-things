const mqtt    = require('mqtt');

const config  = require('../config.json');
const db      = require('../controller/mongodb');

var Module = module.exports;
Module.publishEvent = publishEvent;

const mqttClient = mqtt.connect(config.mqtt.broker);

mqttClient.on('connect', () => {
	console.log('Connected to MQTT!');

	mqttClient.subscribe(config.mqtt.topic_log, function (err) {
		if (err) {
		  console.log("MQTT Error: " + err.message);
		}
	});

	mqttClient.subscribe(config.mqtt.topic_event, function (err) {
		if (err) {
			console.log("MQTT Error: " + err.message);
		}
	});
});

mqttClient.on('message', function (topic, message) {
	try {
		message = JSON.parse(message.toString());
		topic   = topic.toString();

		console.log("MQTT message (" + topic + "): " + JSON.stringify(message));

		if ( topic == "iwsn-wemos" && message.looptiming >= 0 ) {
			db.addLog(topic, message);
		}

		if ( topic == "iwsn-wemos" && message.state ) {
			db.addState(topic, message);
		}
	} catch ( error ) {
		console.log("MQTT could not parse message");
	}
});

function publishEvent(event) {
    mqttClient.publish("iwsn-wemos-event", JSON.stringify({event: event}));
}

