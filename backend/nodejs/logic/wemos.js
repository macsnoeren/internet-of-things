const express = require('express');
const router  = express.Router();

const config  = require('../config.json');
const mqtt    = require('./mqtt');
const websocket = require('./websocket');
const thingsState = require('../models/thingsstate.js');
const thingsLog   = require('../models/thingslog.js');

var Module = module.exports;
Module.addLog    = addLog;
Module.addState  = addState;
Module.getLog    = getLog;
Module.getState  = getState;
Module.postEvent = postEvent;

function addLog (topic, data) {
    const record = new thingsLog({
        topic: topic,
        thing: data.id,
        looptiming: data.looptiming
    });

    record.save()
    .then( (d) => {
        console.log("New log saved!");

    }).catch( (error) => {
        console.log("Error during saving the log:" + error);
    });

    websocket.sendLogMessage("LOG: " + JSON.stringify(data));
}

function addState (topic, data) {
    const record = new thingsState({
        topic: topic,
        thing: data.id,
        state: data.state
    });

    record.save()
    .then( (d) => {
        console.log("New state saved!");

    }).catch( (error) => {
        console.log("Error during saving the state:" + error);
    });

    websocket.sendLogMessage("STATE: " + JSON.stringify(data));
}

function getLog (req, res) {
	const limit = parseInt(req.query.limit) || -1;
	const skip  = parseInt(req.query.skip)  || -1;

	var query = thingsLog.find({});

	if(limit > 0) {
		query = query.limit(limit);
	}

	if(skip > 0) {
		query = query.skip(skip);
	}

	query.then(result => {
		res.status(200).json(result);
	});
};

function getState (req, res) {
	const limit = parseInt(req.query.limit) || -1;
	const skip  = parseInt(req.query.skip)  || -1;

	var query = thingsState.find({});

	if(limit > 0) {
		query = query.limit(limit);
	}

	if(skip > 0) {
		query = query.skip(skip);
	}

	query.then(result => {
		res.status(200).json(result);
	});
};

function postEvent (req, res) {
	const event = req.query.event;
	mqtt.publishEvent(event);
	res.status(200).json({status: "Published event", event: event});
};
