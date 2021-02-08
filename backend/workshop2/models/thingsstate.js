const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const ThingsStateSchema = new Schema({
	topic: {
		type: String,
		minlength: [ 3, 'topic contains at least 3 characters'],
        maxlength: [ 20, 'topic contains at maximum 20 characters'],
		required: true
	},
	thing: {
		type: String,
		minlength: [ 3, 'thing contains at least 3 characters'],
        maxlength: [ 20, 'thing contains at maximum 20 characters'],
		required: true
	},
	state: {
		type: String,
		minlength: [ 3, 'state contains at least 3 characters'],
        maxlength: [ 20, 'state contains at maximum 20 characters'],
		required: true
	}
});

const ThingsState = mongoose.model('ThingsState', ThingsStateSchema);
module.exports = ThingsState;