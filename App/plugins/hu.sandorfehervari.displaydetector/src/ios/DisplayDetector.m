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
    
    UIImage* imageToProcess = [UIImage imageNamed:filename];
    
    if(!imageToProcess) {
        NSLog(@"The image cannot be loaded from path %@ !", filename);
        CDVPluginResult* result = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"The given file cannot be loaded!"];
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
        return;
    }

    float retValue = [self getScaleAngle: imageToProcess];
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
-(float) getScaleAngle:(UIImage*) imageFile {
    id <DetectorAlgorithm> algo = [[ScaleDetector alloc] init];
    return [algo applyDetectorAlgorithm:imageFile];
}

@end
