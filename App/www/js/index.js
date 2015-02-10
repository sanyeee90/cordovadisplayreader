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
.controller("AppCtrl", function ($scope, CameraSrv){
      $scope.getPhoto = function() {
cordova.exec(
                                              // Register the callback handler
                                              function callback(data) {
                                              console.log('Wrote date ' + data);
                                              $scope.lastPhoto = data;
                                              },
                                              // Register the errorHandler
                                              function errorHandler(err) {
                                              console.log('Error' + err);
                                              },
                                              // Define what class to route messages to
                                              'DisplayDetector',
                                              // Execute this method on the above class
                                              'cordovaGetScaleAngle',
                                              // An array containing one String (our newly created Date String).
                                              [ "valami" ]
                                              );


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
