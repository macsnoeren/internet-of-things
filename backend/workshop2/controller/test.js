const config  = require('../config.json');

var Module = module.exports;
Module.test = test;

function test (req, res, next) {
    res.status(200).json({status: "Dit is een test!"});
}


