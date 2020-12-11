package com.kt.helloAR;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLUtils;
import android.util.Log;

import java.io.IOException;

public class JniInterface {
    private static final String TAG = JniInterface.class.getSimpleName();
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private static long nativeApplication = 0;
    private static AssetManager assetManager;

    /**
     * [private static native] Native Methods
     */
    // public static native String stringFromJNI();
    private static native long createNativeApplication(AssetManager assetManager);
    private static native void onPause(long nativeApplication);
    private static native void onResume(long nativeApplication, Context context, Activity activity);
    private static native void destroyNativeApplication(long nativeApplication);
    private static native void onGlSurfaceCreated(long nativeApplication);
    private static native void onDisplayGeometryChanged(
            long nativeApplication, int displayRotation, int width, int height);
    private static native void onGlSurfaceDrawFrame(
            long nativeApplication, boolean depthColorVisualizationEnabled, boolean useDepthForOcclusion);
    private static native void onTouched(long nativeApplication, float x, float y);
    private static native boolean hasDetectedPlanes(long nativeApplication);
    private static native boolean isDepthSupported(long nativeApplication);
    private static native void onSettingsChange(
            long nativeApplication, boolean isInstantPlacementEnabled);


    /**
     * [public static] Java Methods
     */
    public static void onCreate(Context context) {
        assetManager = context.getAssets();
        nativeApplication = createNativeApplication(assetManager);
    }

    public static void onPause() {
        if (nativeApplication != 0) {
            onPause(nativeApplication);
        }
    }

    public static void onResume(Context context, Activity activity) {
        if (nativeApplication != 0) {
            onResume(nativeApplication, context, activity);
        }
    }

    public static void onDestroy() {
        if (nativeApplication != 0) {
            destroyNativeApplication(nativeApplication);
            nativeApplication = 0;
        }
    }

    /** Allocate OpenGL resources for rendering. */
    public static void onGlSurfaceCreated() {
        if (nativeApplication != 0) {
            onGlSurfaceCreated(nativeApplication);
        }
    }

    /**
     * Called on the OpenGL thread before onGlSurfaceDrawFrame when the view port width, height, or
     * display rotation may have changed.
     */
    public static void onDisplayGeometryChanged(int displayRotation, int width, int height) {
        if (nativeApplication != 0) {
            onDisplayGeometryChanged(nativeApplication, displayRotation, width, height);
        }
    }

    /** Main render loop, called on the OpenGL thread. */
    public static void onGlSurfaceDrawFrame(boolean depthColorVisualizationEnabled, boolean useDepthForOcclusion) {
        if (nativeApplication != 0) {
            onGlSurfaceDrawFrame(nativeApplication, depthColorVisualizationEnabled, useDepthForOcclusion);
        }
    }

    /** OnTouch event, called on the OpenGL thread. */
    public static void onTouched(float x, float y) {
        if (nativeApplication != 0) {
            onTouched(nativeApplication, x, y);
        }
    }

    public static void onSettingsChange(boolean instantPlacementEnabled) {
        if (nativeApplication != 0) {
            onSettingsChange(nativeApplication, instantPlacementEnabled);
        }
    }

    public static boolean isDepthSupported() {
        if (nativeApplication != 0) {
            return isDepthSupported(nativeApplication);
        }
        return false;
    }

    /** Get plane count in current session. Used to disable the "searching for surfaces" snackbar. */
    public static boolean hasDetectedPlanes() {
        if (nativeApplication != 0) {
            return hasDetectedPlanes(nativeApplication);
        }
        return false;
    }

    public static Bitmap loadImage(String imageName) {
        try {
            return BitmapFactory.decodeStream(assetManager.open(imageName));
        } catch (IOException e) {
            Log.e(TAG, "Cannot open image " + imageName);
            return null;
        }
    }

    public static void loadTexture(int target, Bitmap bitmap) {
        GLUtils.texImage2D(target, 0, bitmap, 0);
    }
}
