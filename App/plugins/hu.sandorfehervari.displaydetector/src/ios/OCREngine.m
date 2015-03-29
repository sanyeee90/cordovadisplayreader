//
//  OCREngine.m
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#import "OCREngine.h"
#import <TesseractOCR/TesseractOCR.h>

@implementation OCREngine {
    G8Tesseract *tesseract;

}

- (id)initWithImage: (UIImage*) image
{
    if( self = [super init] )
    {
        tesseract = [[G8Tesseract alloc] initWithLanguage:@"eng"];
        tesseract.charWhitelist = @"0123456789";
        tesseract.maximumRecognitionTime = 2.0;
        tesseract.image = image;
    }
    
    return self;
}

- (NSInteger) getNumberFromImage:(CGRect *)detectionArea {
    tesseract.rect = *(detectionArea);
    [tesseract recognize];

    NSString* recognizedText = [tesseract recognizedText];
    NSLog(@"%@", recognizedText);
    return [recognizedText integerValue];
}

@end
