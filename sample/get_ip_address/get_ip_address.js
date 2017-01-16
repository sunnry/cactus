/*
usages:

var ipmodule = require('./get_ip_address.js');
var address = ipmodule.get_ip_address('eth0');
console.log(address);
*/

var os = require('os');

function get_ip_address(iface){

	var ifaces = os.networkInterfaces();
	var address = null;

	for(var dev in ifaces){
		if(dev.toLowerCase().indexOf(iface) != -1){
			ifaces[dev].forEach(function(details){
				if(details.family.toUpperCase() === "IPV4"){
					//console.log(details.address);
					address = details.address;
				}
			})
		}
	}

	return address;
}

module.exports.get_ip_address = get_ip_address;
