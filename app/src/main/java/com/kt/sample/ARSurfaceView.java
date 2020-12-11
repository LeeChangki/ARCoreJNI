package com.kt.sample;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;

import com.kt.helloAR.JniInterface;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class ARSurfaceView extends GLSurfaceView {
    private static final String TAG = ARSurfaceView.class.getSimpleName();
    public static boolean viewportChanged = false;
    private Activity mActivity;

    private void setSurface() {
        setPreserveEGLContextOnPause(true);
        setEGLContextClientVersion(2);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0); // Alpha used for plane bl
        setRenderer(new glRenderer());
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        setWillNotDraw(false);
    }

    public ARSurfaceView(Context context) {
        super(context);
        setSurface();
        mActivity = (Activity)context;
    }

    public ARSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setSurface();
        mActivity = (Activity)context;
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        super.surfaceDestroyed(holder);
    }

    private class glRenderer implements Renderer {
        private int viewportWidth = 0;
        private int viewportHeight = 0;

        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);    // gray
            JniInterface.onGlSurfaceCreated();
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int width, int height) {
            viewportWidth = width;
            viewportHeight = height;
            viewportChanged = true;
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
            // Synchronized to avoid racing onDestroy.
            synchronized (this) {
                if (viewportChanged) {
                    int displayRotation = mActivity.getWindowManager().getDefaultDisplay().getRotation();
                    JniInterface.onDisplayGeometryChanged(displayRotation, viewportWidth, viewportHeight);
                    viewportChanged = false;
                }
                JniInterface.onGlSurfaceDrawFrame(false, false);
            }
        }
    }
}
