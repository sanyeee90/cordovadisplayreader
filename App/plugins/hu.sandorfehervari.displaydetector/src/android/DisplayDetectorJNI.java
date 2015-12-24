package hu.sandorfehervari.analogdisplayreader;

/**
 * Created by fehervarisandor on 2015. 04. 04..
 */
public class DisplayDetectorJNI {

    public native float processImage(String imagePath, String tesseractDir, int imageOrientation);
}
