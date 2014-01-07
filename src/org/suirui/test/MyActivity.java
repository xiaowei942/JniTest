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
    int id = 0;
    int test = 128;
    Button btn_test;
    Button btn_setenv;
    Button btn_thread;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        btn_test = (Button)findViewById(R.id.btn_test);
        btn_test.setOnClickListener(this);
        btn_setenv = (Button)findViewById(R.id.btn_setenv);
        btn_setenv.setOnClickListener(this);
        btn_thread = (Button)findViewById(R.id.btn_thread);
        btn_thread.setOnClickListener(this);
    }

    public void printMessage() {
        System.out.println("Message from MyActivity");
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.btn_thread:
            {
                Toast.makeText(getApplicationContext(), "create thread", Toast.LENGTH_LONG).show();
                id++;
                JniTest jt = new JniTest(id);
                jt.trd.start();
                break;
            }
            case R.id.btn_setenv:
            {
                Toast.makeText(getApplicationContext(), "setenv", Toast.LENGTH_LONG).show();
                SetEnv();
                break;
            }
            case R.id.btn_test:
            {
                DoTest();
                break;
            }
            default:
                break;
        }
    }

    static {
        System.loadLibrary("jnitest");
    }
    public native void DoTest();
    public native void SetEnv();
}
