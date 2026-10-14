package com.example.as_jni_project;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity2 extends AppCompatActivity {

    public native void nativeFun5(); // env 1000H
    public static native void nativeFun6(); // env 1000H

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        nativeFun5(); // main线程调用的
        nativeFun6(); // main线程调用的
    }
}
