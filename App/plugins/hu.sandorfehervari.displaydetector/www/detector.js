var cordova = require('cordova'),
	exec = require('cordova/exec');

function DisplayDetector() {

}

DisplayDetector.prototype.processImage = function(imagePath, successCallback, errorCallback) {
	var retval = null;
	cordova.exec(
		// Register the callback handler
		successCallback,
		// Register the errorHandler
		errorCallback,
		// Define what class to route messages to
		'DisplayDetector',
		// Execute this method on the above class
		'processImage',
		// An array containing one String (our newly created Date String).
		[ imagePath ]
	);
	return retval;
}
module.exports = new DisplayDetector();