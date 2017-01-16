var exec = require('child_process').exec;

gs = exec('raspivid -t 9999999 -h 720 -w 1080 -fps 25 -hf -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse !  rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=192.168.0.103 port=5000');

gs.stdout.on('data',function(data){
	console.log('stdout:' + data);
});

gs.on('exit',function(code){
	console.log('exit sub process:' + code);
});
