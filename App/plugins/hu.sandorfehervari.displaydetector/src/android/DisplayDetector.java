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

import android.os.Bundle;
import com.oracle.tools.packager.Log;
import org.apache.cordova.*;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.logging.Logger;

public class DisplayDetector extends CordovaPlugin
{
    public static final String TAG = "DisplayDetector";



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
    }

    /**
     * Executes the request and returns PluginResult.
     *
     * @param action            The action to execute.
     * @param args              JSONArry of arguments for the plugin.rvarisandor/Szakdolgozat/App/platforms/android/ant-build
    [move] Moving 1 file to /Users/fehervarisandor/Szakdolgozat/App/platforms/android/CordovaLib/ant-build

    debug:

    BUILD SUCCESSFUL
     * @param callbackContext   The callback id used when calling back into JavaScript.
     * @return                  True if the action was valid, false if not.
     */
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("processImage")) {
            JSONObject result = new JSONObject();
            result.put("value", 32.0f);
            result.put("success", true);
            callbackContext.success(result);
        }
        else {
            return false;
        }
        return true;
    }
}
