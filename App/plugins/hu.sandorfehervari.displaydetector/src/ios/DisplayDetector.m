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

- (void) processImage:(CDVInvokedUrlCommand *)command{
    NSString *filename = [command.arguments objectAtIndex:0];
    NSLog(filename);
    id <DetectorAlgorithm> algo = [[ScaleDetector alloc] init];
    float retValue = [algo applyDetectorAlgorithm:filename];
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

@end
