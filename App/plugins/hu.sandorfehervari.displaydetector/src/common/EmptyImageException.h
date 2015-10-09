//
//  EmptyImageException.hpp
//  Csucsaramlasmero
//
//  Created by Fehervari Sandor on 2015. 10. 06..
//  Copyright © 2015. sandorfehervari. All rights reserved.
//

#ifndef EmptyImageException_hpp
#define EmptyImageException_hpp

#include <stdio.h>

class AlgorithmException {
    
private:
    const int errorCode;
public:
    AlgorithmException(const int errorCode):errorCode(errorCode) { }
    int getErrorCode() { return errorCode; }
    
};

#endif /* EmptyImageException_hpp */
