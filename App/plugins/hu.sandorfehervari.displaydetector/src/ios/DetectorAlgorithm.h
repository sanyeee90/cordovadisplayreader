//
//  DetectorAlgorithm.h
//  Analog Display Reader
//
//  Created by Sándor Fehérvári on 2015.02.12..
//
//

#import <Foundation/Foundation.h>

@protocol DetectorAlgorithm <NSObject>

-(float) applyDetectorAlgorithm:(UIImage*) image;

@end
