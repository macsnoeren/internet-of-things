const mongoose    = require('mongoose');

const config      = require('../config.json');
const ThingsLog   = require('../models/thingslog');
const ThingsState = require('../models/thingsstate');
const websocket   = require('./websocket');

var Module = module.exports;
Module.addLog = addLog;
Module.addState = addState;

mongoose.connect(config.mongodb.host, {
    useNewUrlParser: true,
    useUnifiedTopology: true,

}).then(db => {
    console.log("Connected to MongoDB!");

}).catch(error => {
    console.warn(`Unable to connect to MongoDB: ${error.toString()}`);
    throw error;
});

function addLog(topic, message) {
    const record = new ThingsLog({
        topic: topic,
        thing: message.id,
        looptiming: message.looptiming
    });

    record.save()
    .then( (data) => {
        console.log("New log is saved!");

    }).catch( (error) => {
        console.log("Error during saving the log: " + error);
    });

    websocket.sendLogMessage("LOG: + " + JSON.stringify(message));
}

function addState(topic, message) {
    const record = new ThingsState({
        topic: topic,
        thing: message.id,
        state: message.state
    });

    record.save()
    .then( (data) => {
        console.log("New state is saved!");

    }).catch( (error) => {
        console.log("Error during saving the state: " + error);
    });

    websocket.sendLogMessage("STATE: + " + JSON.stringify(message));
}
