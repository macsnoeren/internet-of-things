const express = require('express');
const router  = express.Router();

const auth        = require('../../logic/auth.js')
const wemos       = require('../../logic/wemos.js')

router.post('/login',  auth.getToken);
router.get('/log',    auth.validateToken, wemos.getLog );
router.get('/state',  auth.validateToken, wemos.getState );
//router.post('/event', wemos.postEvent );
router.post('/event/:event', auth.validateToken, wemos.postEvent );

module.exports = router;
