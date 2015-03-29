//
//  DisplayDetector.m
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.01.19..
//
//

#import "DisplayDetector.h"
#import "ScaleDetector.h"
@implementation DisplayDetector

- (void) cordovaGetScaleAngle:(CDVInvokedUrlCommand *)command{
    NSString *filename = [command.arguments objectAtIndex:0];
    NSLog(filename);
    float retValue = [self getScaleAngle: filename];
    NSDictionary *jsonObj = [ [NSDictionary alloc]
                             initWithObjectsAndKeys :
                             [NSNumber numberWithFloat:retValue], @"value",
                             @"true", @"success",
                             nil
                             ];
    CDVPluginResult *pluginResult = [ CDVPluginResult
                                     resultWithStatus    : CDVCommandStatus_OK
                                     messageAsDictionary : jsonObj
                                     ];
    [self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];

}


#pragma mark - Util_Methods


//dummy function
-(float) getScaleAngle:(NSString*) imgURL {
    id <DetectorAlgorithm> algo = [[ScaleDetector alloc] init];
    return [algo applyDetectorAlgorithm:imgURL];
}

@end
