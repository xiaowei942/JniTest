package org.suirui.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import org.suirui.test.R;

public class MyActivity extends Activity implements View.OnClickListener{
    /**
     * Called when the activity is first created.
     */
    Button btn_test;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        btn_test = (Button)findViewById(R.id.btn_test);
        btn_test.setOnClickListener(this);
    }

    @Override
    public void onClick(View view) {
        int ret = JniMethodInit();
        if(view.getId() == R.id.btn_test) {
            if (ret==1) {
                DoTest();
                Toast.makeText(getApplicationContext(), "Test return: " + ret, Toast.LENGTH_LONG).show();
            } else {
                Toast.makeText(getApplicationContext(), "Test return: " + ret, Toast.LENGTH_LONG).show();
            }
        }
    }

    static {
        System.loadLibrary("jnitest");
    }
    public native int JniMethodInit();
    public native void DoTest();
}
