const config  = require('../config.json');
const mqtt    = require('./mqtt');

var Module = module.exports;
Module.postEvent = postEvent;

function postEvent (req, res) {
    //const event = req.query.event;
    const event = req.params.event;
	mqtt.publishEvent(event);
	res.status(200).json({status: "Published event", event: event});
};


