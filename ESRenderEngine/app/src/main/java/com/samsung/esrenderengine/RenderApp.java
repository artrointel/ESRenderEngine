
package com.samsung.esrenderengine;

// Wrapper for native library

import android.view.MotionEvent;
import android.view.View;

public class RenderApp {

     static {
          System.loadLibrary("RenderEngineJNIGlue");
     }

     public static native void init();
     public static native void resize(int width, int height);
     public static native void draw();

     public static native void onTouchEvent(int action, int[] id, int[] x, int[] y, int count);
}
