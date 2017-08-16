
package com.samsung.esrenderengine;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.SynchronousQueue;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class RenderAppView extends GLSurfaceView {
    private static final String TAG = "GLES3JNI";
    private static final boolean DEBUG = true;

    private Renderer mRenderer;

    public RenderAppView(Context context) {
        super(context);
        // Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
        // supporting OpenGL ES 2.0 or later backwards-compatible versions.
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(2);
        mRenderer = new Renderer();
        setRenderer(mRenderer);

        setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                int count = motionEvent.getPointerCount();
                int action = motionEvent.getAction();
                int[] id = new int[count];
                int[] x  = new int[count];
                int[] y  = new int[count];
                for(int i = 0; i < count; i++)
                {
                    id[i] = motionEvent.getPointerId(i);
                    x[i] = (int)motionEvent.getX(i);
                    y[i] = (int)motionEvent.getY(i);
                }
                if(count != 0)
                {
                    Bundle event = new Bundle();
                    event.putInt("action", action);
                    event.putIntArray("id", id);
                    event.putIntArray("x", x);
                    event.putIntArray("y", y);
                    event.putInt("count", count);
                    mRenderer.queueTouchEvent(event);
                }
                else
                    Log.e("java renderappview", "count is zero");

                return true;
            }

        });
    }


    private static class Renderer implements GLSurfaceView.Renderer {
        Queue<Bundle> mEventQueue;

        public void queueTouchEvent(Bundle touchEvent)
        {
            mEventQueue.add(touchEvent);
        }

        public void onDrawFrame(GL10 gl) {
            Bundle event = mEventQueue.poll();
            while(event != null)
            {
                int action = event.getInt("action");
                int[] id = event.getIntArray("id");
                int[] x  = event.getIntArray("x");
                int[] y  = event.getIntArray("y");
                int count= event.getInt("count");
                RenderApp.onTouchEvent(action, id, x, y, count);
                event = mEventQueue.poll();
            }
            RenderApp.draw();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            RenderApp.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            if(mEventQueue == null)
                mEventQueue = new LinkedBlockingQueue<>();


            RenderApp.init();
        }
    }
}
