var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io').listen(server);

app.get('/',function(req,res){
	res.sendFile(__dirname +'/index.html');
});

io.sockets.on('connection',function(socket){
	console.log("one client connected");
	socket.on('client has ready',function(data){
		console.log('client has ready');
	});
	socket.emit('io server ready');
});


server.listen(3000,function(){
	console.log('listening on *:3000');
});
