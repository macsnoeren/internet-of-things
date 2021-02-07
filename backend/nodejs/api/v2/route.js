const express = require('express');
const router  = express.Router();

const wemos       = require('../../logic/wemos.js')

router.get('/log',    wemos.getLog );
router.get('/state',  wemos.getState );
router.post('/event', wemos.postEvent );

module.exports = router;
