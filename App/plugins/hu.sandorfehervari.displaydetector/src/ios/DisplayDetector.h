//
//  DisplayDetector.h
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.01.19..
//
//

#import <Cordova/CDV.h>

@interface DisplayDetector : CDVPlugin

- (void) processImage:(CDVInvokedUrlCommand *)command;

#pragma mark - Util_methods

-(float) getScaleAngle:(UIImage*) imgURL;

@end
