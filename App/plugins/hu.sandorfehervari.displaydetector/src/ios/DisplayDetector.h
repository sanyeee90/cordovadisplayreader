//
//  DisplayDetector.h
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.01.19..
//
//

#import <Cordova/CDV.h>

@interface DisplayDetector : CDVPlugin

- (void) cordovaGetScaleAngle:(CDVInvokedUrlCommand *)command;

#pragma mark - Util_methods

-(float) getScaleAngle:(NSString*) imgURL;

@end
