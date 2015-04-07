/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

var App = angular.module("dispreader", ["ionic", "dispreader.services"])
.config(function($compileProvider){
        $compileProvider.imgSrcSanitizationWhitelist(/^\s*(https?|ftp|mailto|file|tel):/);
})
.run(function($ionicPlatform) {

  $ionicPlatform.ready(function() {
    if(window.StatusBar) {
      StatusBar.overlaysWebView(false); //Turns off web view overlay.
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})
.directive('slideToggle', function() {
    return {
        restrict: 'A',
        scope:{
            isOpen: "=slideToggle" // 'data-slide-toggle' in our html
        },
        link: function(scope, element, attr) {
            var slideDuration = parseInt(attr.slideToggleDuration, 10) || 200;

            // Watch for when the value bound to isOpen changes
            // When it changes trigger a slideToggle
            scope.$watch('isOpen', function(newIsOpenVal, oldIsOpenVal){
                if(newIsOpenVal !== oldIsOpenVal){
                    element.stop().slideToggle(slideDuration);
                }
            });

        }
    };
})
.controller("AppCtrl", function ($scope, CameraSrv){
    var path = "img/pfm20det.png"

      $scope.getPhoto = function() {
            /*cordova.plugins.DisplayDetector.processImage(path,
                function errorHandler(err) {
                    console.log('Error' + err);
                    $scope.processError=!$scope.processError;
                },
                function callback(data) {
                    console.log('Wrote date ' + data);
                    $scope.lastPhoto = data;
                });*/
          $scope.lastPhoto = path;


       /*   CameraSrv.getPicture().then(function(imageURI) {
            $scope.lastPhoto = imageURI;

          }, function(err) {
            console.err(err);
          }, {
            quality: 75,
            targetWidth: 320,
            targetHeight: 320,
            saveToPhotoAlbum: false,
            destinationType : Camera.DestinationType.DATA_URL
          });*/
        };
    });


//document.addEventListener('deviceready', onDeviceReady, false);
