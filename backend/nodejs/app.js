const http = require('http');

var server = http.createServer(function(rew, res) {
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.write('Hello World');
    res.end();
});
server.listen(3000);
console.log("Server is running at http://localhost:3000");