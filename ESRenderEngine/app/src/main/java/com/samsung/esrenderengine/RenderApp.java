
package com.samsung.esrenderengine;

// Wrapper for native library

public class RenderApp {

     static {
          System.loadLibrary("RenderEngineJNIGlue");
     }

     public static native void init();
     public static native void resize(int width, int height);
     public static native void draw();
}
