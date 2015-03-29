//
//  OCREngine.h
//  Analog Display Reader
//
//  Created by Fehervari Sandor on 2015. 03. 29..
//
//

#import <Foundation/Foundation.h>

@interface OCREngine : NSObject

- (id)initWithImage: (UIImage*) image;
- (NSInteger) getNumberFromImage:(CGRect*) detectionArea;

@end
