angular.module('dispreader.services', [])


.factory('CameraSrv', ['$q', function($q) {

  return {
    getPicture: function(options) {
      var q = $q.defer();

      navigator.camera.getPicture(function(result) {
        // Do any magic you need
        q.resolve(result);
      }, function(err) {
        q.reject(err);
      }, options);

      return q.promise;
    }
  }
}])
.factory('DisplayReaderService', ['$q', function($q) {
  return {
    processImage: function(imageUrl) {
      var q = $q.defer();
      cordova.plugins.DisplayDetector.processImage(imageUrl,
          function(result) {
            q.reject(result);
          }, function(error) {
            q.reject(error);
          });
      return q.promise;
    }
  }
}])
.factory('CameraSrvStub', ['$q', function($q) {

  return {
    getPicture: function(options) {
      var q = $q.defer();
      q.resolve("www/img/pfm20det.png");
      return q.promise;
    }
  }
}])
