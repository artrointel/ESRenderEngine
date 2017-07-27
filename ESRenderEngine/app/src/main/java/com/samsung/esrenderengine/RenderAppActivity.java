package com.samsung.esrenderengine;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class RenderAppActivity extends Activity {

    RenderAppView mView;

    // Rendering Overrides
    @Override protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        mView = new RenderAppView(getApplication());
        setContentView(mView);


    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();

    }
}
