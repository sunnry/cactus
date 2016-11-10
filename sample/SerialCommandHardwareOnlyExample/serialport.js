function CommandBuffer(){
   this.buffer = [];
}

CommandBuffer.prototype.append = function append(string){
	this.buffer.push(string);
	return this;
}

CommandBuffer.prototype.toString = function toString(){
	return this.buffer.join("");
}

CommandBuffer.prototype.clear = function clear(){
	this.buffer = [];
}

var resCommand = new CommandBuffer();

var SerialPort = require('serialport');

var port = new SerialPort('/dev/ttyAMA0',{baudRate:115200});

port.close(function(err){
  console.log("i am closeing port,",err);
});

port.on('error',function(err){
	console.error("Err:",err);
});

port.on('open',function(){
	console.log('port opened..., Press reset on the arduino');

});

port.on('data',function(data){

	for(var i=0;i<data.length;i++){
		//console.log(data[i]);
		if(data[i]==0xA){
			resCommand.append(String.fromCharCode(data[i]));
			console.log('Data:'+ resCommand);
			resCommand.clear();
		}else{
			//console.log(data);
			resCommand.append(String.fromCharCode(data[i]));
			//resCommand.append(data[i]);	
		}
	}
	

//	console.log('DATA:' + data);
	//console.log(data);
	port.write("CMD1\r");
//	port.write("HELLO\r");
//	port.write("fuck\r");
//	port.write("OFF\r");
});

port.on('close',function(err){
	console.log('serial port closed');
});



