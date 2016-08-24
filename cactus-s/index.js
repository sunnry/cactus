var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io').listen(server);

app.get('/',function(req,res){
	res.sendFile(__dirname +'/index.html');
});

io.sockets.on('connection',function(socket){
	console.log("connected with one android client");

	//this part is used to send message to android client
	socket.on('Client Ready',function(data){
		console.log('client has ready');
		socket.emit('Server Ready');
	});

	//this part is used to receive message from left control pad
	socket.on('Left ControlPad Ready',function(data){
		console.log('left control pad has ready');
		socket.emit('Server Ready');
		socket.on('xPower',function(data){
			console.log('xPower:'+ data);
		});
		socket.on('yPower',function(data){
			console.log('yPower:' + data);
		});
	});

	//this part is used to receive message from right control pad
	socket.on('Right ControlPad Ready',function(data){
		console.log('right control pad has ready');
		socket.emit('Server Ready');
		socket.on('zPower',function(data){
			console.log('zPower:' + data);
		});
	});

});


server.listen(3000,function(){
	console.log('listening on *:3000');
});
