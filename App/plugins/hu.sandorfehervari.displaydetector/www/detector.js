var cordova = require('cordova'),
	exec = require('cordova/exec');

function getScaleAngle(imagePath) {
	var retval = null;
	cordova.exec(
	      	// Register the callback handler
	      	function callback(data) {
	          console.log('Wrote date ' + data);
	          retval = data;
	      	},
	      	// Register the errorHandler
	      	function errorHandler(err) {
	      		console.log('Error' + err);
	      	},
	      	// Define what class to route messages to
	      	'DisplayDetector',
	      	// Execute this method on the above class
	      	'codrovaGetScaleAngle',
	      	// An array containing one String (our newly created Date String).
	      	[ "valami" ]
	    );
	return retval;
}