/*
       Licensed to the Apache Software Foundation (ASF) under one
       or more contributor license agreements.  See the NOTICE file
       distributed with this work for additional information
       regarding copyright ownership.  The ASF licenses this file
       to you under the Apache License, Version 2.0 (the
       "License"); you may not use this file except in compliance
       with the License.  You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing,
       software distributed under the License is distributed on an
       "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
       KIND, either express or implied.  See the License for the
       specific language governing permissions and limitations
       under the License.
 */

package hu.sandorfehervari.analogdisplayreader;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaWebView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.net.URI;

public class DisplayDetector extends CordovaPlugin
{
    public static final String TAG = "DisplayDetector";
    private static final String APP_PREFERENCES = "DisplayDetectorPref";

    private static final String ASSET_TESSDATA_PATH = "tessdata/eng.traineddata";

    private static final String TESSDATA_PATH = "TessDataPath";
    private static final String TESSDATA_FILE = "tessdata/eng.traineddata";
    private String tessDataFile = "";

    static
    {
        System.loadLibrary("opencv_java");
        System.loadLibrary("lept");
        System.loadLibrary("tess");
        System.loadLibrary("DisplayReaderJNI");
    }

    public DisplayDetector() {
    }


    /**
     * Sets the context of the Command. This can then be used to do things like
     * get file paths associated with the Activity.
     *
     * @param cordova The context of the main Activity.
     * @param webView The CordovaWebView Cordova is running in.
     */
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {

        super.initialize(cordova, webView);

        SharedPreferences sharedPrefs = cordova.getActivity().getSharedPreferences(APP_PREFERENCES, 0);
        tessDataFile = sharedPrefs.getString(TESSDATA_PATH, "");

        if ("".equals(tessDataFile) || !isFileAlreadyCopied(tessDataFile)) {
            tessDataFile = copyDataToStorage();
            SharedPreferences.Editor editor = sharedPrefs.edit();
            editor.putString(TESSDATA_PATH, tessDataFile);
            editor.commit();
        }

    }

    private String copyDataToStorage() {
        Context Context = cordova.getActivity().getApplicationContext();
        String destinationFile = Context.getFilesDir().getPath() + File.separator;
        File outputFile = new File(destinationFile + TESSDATA_FILE);
        InputStream IS = null;
        OutputStream OS = null;
        try {
            IS = Context.getAssets().open(ASSET_TESSDATA_PATH);
            outputFile.getParentFile().mkdirs();
            OS = new FileOutputStream(outputFile, false);
            CopyStream(IS, OS);
            OS.flush();
        } catch (IOException ex) {
            Log.d(TAG, ex.getMessage());
            ex.getStackTrace();
            Log.e(TAG, "Failed to copy the file from the asset folder.");
            return "";
        } finally {
            try {
                IS.close();
            } catch (Exception ex) {
                Log.e(TAG, "exception happened when tried to close the inputStream.");
            }
            try {
                OS.close();
            } catch (Exception ex) {
                Log.e(TAG, "exception happened when tried to close the outputStream.");
            }
        }
        return destinationFile;
    }

    private boolean isFileAlreadyCopied(String path) {
        return new File(path + TESSDATA_FILE).exists();
    }

    private void CopyStream(InputStream Input, OutputStream Output) throws IOException {
        byte[] buffer = new byte[5120];
        int length = Input.read(buffer);
        while (length > 0) {
            Output.write(buffer, 0, length);
            length = Input.read(buffer);
        }
    }

    /**
     * Executes the request and returns PluginResult.
     *
     * @param action            The action to execute.
     * @param args              JSONArry of arguments for the plugin.
     * @param callbackContext   The callback id used when calling back into JavaScript.
     * @return                  True if the action was valid, false if not.
     */
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {

        if (action.equals("processImage")) {
            Log.i(TAG, args.toString());
            String fileName = args.getString(1);
            File fileToProcess = null;
            if (tessDataFile.isEmpty()) {
                callbackContext.error("failed initialize the detector plugin");
                return true;
            }
            try {
                String normalizedFilePath = getNormalizedFilePath(fileName);
                int imageOrientation = ExifInterface.ORIENTATION_UNDEFINED;
                if (normalizedFilePath.toLowerCase().endsWith(".jpg") || normalizedFilePath.toLowerCase().endsWith(".jpeg")) {
                    imageOrientation = getOrientation(normalizedFilePath);
                }


                fileToProcess = new File(normalizedFilePath);
                if (!fileToProcess.exists()) {
                    callbackContext.error("failed to load the input image.");
                    return true;
                }
                JSONObject result = new JSONObject();
                result.put("value", new DisplayDetectorJNI().processImage(args.getString(0), tessDataFile, imageOrientation));

                result.put("success", true);
                callbackContext.success(result);
            } catch (AssertionError ex) {
                callbackContext.error("the filename was null!");
                return true;
            }
            return true;
        } else {
            return false;
        }
    }

    private String getNormalizedFilePath(String inputFilePath) {
        String retValue = inputFilePath;
        assert inputFilePath != null;
        if (inputFilePath.startsWith("file://")) {
            retValue = Uri.parse(inputFilePath).getPath();
        }
        return retValue;
    }

    private int getOrientation(String inputImage) {
        ExifInterface exif = null;
        try {
            exif = new ExifInterface(inputImage);
        } catch (IOException e) {
            Log.e(TAG, "Failed to red exif information from image.");
            e.printStackTrace();
        }
        int orientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION,
                ExifInterface.ORIENTATION_UNDEFINED);
        return orientation;
    }


}
