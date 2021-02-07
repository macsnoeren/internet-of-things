const express = require('express');
const parser  = require('body-parser');
const mongoose = require('mongoose');

const config      = require('./config');
const thingsLog   = require('./models/thingslog');
const thingsState = require('./models/thingsstate');
const apiv2       = require('./api/v2/route');         
const websocket   = require('./logic/websocket');
const app = express();

mongoose.connect(config.mongodb.host, {
    useNewUrlParser: true,
    useUnifiedTopology: true,

}).then(db => {
    console.log("Connected to MongoDB!");

}).catch(error => {
    console.warn(`Unable to connect to MongoDB: ${error.toString()}`);
    throw error;
});

app.use(parser.urlencoded({
	extended: true
}));

app.use(parser.json());

app.set('config', config);

app.all('*', function(req, res, next) {
	console.log(`[${new Date().toISOString()}] [${req.method}] ${req.url} has been invoked!`)
	next();
});

app.get('/', express.static('public'));

app.use('/api/v2', apiv2);

const http = app.listen(8080, function() {
	console.log("Server started...");
});

// Adding websocket to the application
const io = require('socket.io')(http);
websocket.setup( io );
