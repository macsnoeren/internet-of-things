const config  = require('../config.json');

var Module = module.exports;
Module.getToken  = getToken;
Module.validateToken = validateToken;

function getToken(req, res) {
    const user     = req.body.user; // POST
    const password = req.body.password; // POST

    console.log("getToken: " + user + ", " + password);

    if ( user == "maurice" && password == "iwsn") {
        res.status(200).json({token: "123456"});

    } else {
        res.status(401).json({status: "sorry!"});
    }
}

function validateToken(req, res, next) {
    const token = req.query.token;
    
    if ( token == 123456 ) {
        next();
        
    } else {
        res.status(401).json({status: "Not authorized!"});
    }
}
