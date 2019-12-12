package com.clericyi.pbexport;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;

import org.tensorflow.contrib.android.TensorFlowInferenceInterface;

public class MainActivity extends AppCompatActivity {

    private TensorFlowInferenceInterface mInferenceInterface;

    private static final String OUTPUT_NODE = "add";
    private final String TAG = this.getClass().getName();

    TextView textView;
    EditText v1, v2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        onChangeListening();
    }

    private void onChangeListening() {
        v1.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {
                if (checkContent()) {
                    useTensorPB();
                }
            }
        });

        v2.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {
                if (checkContent()) {
                    useTensorPB();
                }
            }
        });
    }

    private boolean checkContent() {
        try {
            if (v1.getText().toString().length() > 0 && v2.getText().toString().length() > 0)
                return true;
        } catch (Exception e) {
            return false;
        }
        return false;
    }


    /**
     * 初始化控件
     */
    private void initView() {
        v1 = findViewById(R.id.v1);
        v2 = findViewById(R.id.v2);
        textView = findViewById(R.id.result);


    }

    /**
     * 使用Tensorflow模型
     */
    private void useTensorPB() {
        float[] output = new float[1];
        String MODEL_FILE = "file:///android_asset/model.pb";
        mInferenceInterface = new TensorFlowInferenceInterface(getAssets(), MODEL_FILE);
        mInferenceInterface.feed("v1", new float[]{Float.valueOf(v1.getText().toString())});
        mInferenceInterface.feed("v2", new float[]{Float.valueOf(v2.getText().toString())});

        mInferenceInterface.run(new String[]{OUTPUT_NODE});
        mInferenceInterface.fetch(OUTPUT_NODE, output);
        textView.setText(String.valueOf(output[0]));
        Log.e(TAG, "output:" + output[0]);
    }
}
