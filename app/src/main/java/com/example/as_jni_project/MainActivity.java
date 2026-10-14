package com.example.as_jni_project;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Looper;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import com.example.as_jni_project.databinding.ActivityMainBinding;

import java.io.File;
import java.security.PublicKey;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {


    private ActivityMainBinding binding;
    public int number = 10;

    public static String num = "abc";

    private int add(int a,int b){
        System.out.println("aaaaaa :" + a  +"," + b);
        return a + b;
    }

    public int[] aar = {1,2,3,4,5,6,7,8,9,10};

    public String[] content = {"a","b","c","d","e"};

    private YiGePlay yiGePlay;

    private final ActivityResultLauncher<String[]> requestPermissionLauncher =
            registerForActivityResult(
                    new ActivityResultContracts.RequestMultiplePermissions(),
                    result -> {
                        boolean granted = true;
                        for (Map.Entry<String, Boolean> e : result.entrySet()) {
                            if (!e.getValue()) granted = false;
                        }
                        if (granted) {
                            loadFiles();
                        } else {
                            Toast.makeText(this, "存储权限被拒绝", Toast.LENGTH_SHORT).show();
                        }
                    }
            );

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        yiGePlay = new YiGePlay();
        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        System.out.println("number修改前:"+ number);
        changeNumber();
        System.out.println("number修改后:"+ number);
        System.out.println("num修改前:"+ num);
        changeStaticNum();
        System.out.println("num修改后:"+ num);
        addMethod();
        printfArr(aar);
        for (int i = 0; i < aar.length; i++) {
            System.out.println("第"+i+"个 值是:" + aar[i]);
        }

        changeContent(content);
        for (int i = 0; i < content.length; i++) {
            System.out.println("字符串第"+i+"个 值是:" + content[i]);
        }
        Student student = new Student();
        student.setAge(10);
        student.setName("abc");
        getStudent(student);

        System.out.println("java student : " + student.getName() + "," + student.getAge());

        initObject();

        binding.sampleText.setOnClickListener(v -> initDynamic());
        binding.sampleJump.setOnClickListener(v -> naitveThread() );
        checkAndRequest();
        System.out.println("读取文件 555555");
        yiGePlay.setYiGePlayListener(() -> {
            System.out.println("读取文件 准备播放2222");
            runOnUiThread(() -> Toast.makeText(this,"准备好了",Toast.LENGTH_LONG).show());
            yiGePlay.onStartPlay();
        });


    }

    private void checkAndRequest() {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.R){
            // Android11及以上：申请所有文件访问权限
            if(Environment.isExternalStorageManager()){
                loadFiles();
            }else{
                //跳转到系统设置页，用户手动打开开关
                Intent intent = new Intent(android.provider.Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                Uri uri = Uri.parse("package:" + getPackageName());
                intent.setData(uri);
                startActivity(intent);
                Toast.makeText(this,"请手动开启【访问所有文件】权限",Toast.LENGTH_LONG).show();
            }
        }else{
            // <= Android10 老的动态权限
            List<String> permissions = new ArrayList<>();
            if (Build.VERSION.SDK_INT <= 28) {
                permissions.add(Manifest.permission.READ_EXTERNAL_STORAGE);
                permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            } else {
                permissions.add(Manifest.permission.READ_EXTERNAL_STORAGE);
            }
            List<String> need = new ArrayList<>();
            for (String p : permissions) {
                if (ContextCompat.checkSelfPermission(this, p) != PackageManager.PERMISSION_GRANTED) {
                    need.add(p);
                }
            }
            if (need.isEmpty()) {
                loadFiles();
            } else {
                requestPermissionLauncher.launch(need.toArray(new String[0]));
            }
        }
    }


    private void loadFiles() {
        System.out.println("读取文件 准备播放");
        File file = new File(Environment.getExternalStorageDirectory(),"Movies/demo.mp4");
        yiGePlay.setSource(file.getAbsolutePath());
        System.out.println("读取文件 111111");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        yiGePlay.onReleasePlay();
    }

    @Override
    protected void onResume() {
        super.onResume();
        System.out.println("读取文件 66666");
        yiGePlay.prepare();
    }

    @Override
    protected void onStop() {
        super.onStop();
        yiGePlay.onStopPlay();
    }

    public void updateActivityUI() {
        if (Looper.getMainLooper() == Looper.myLooper()) { // TODO C++ 用主线程调用到此函数 ---->  主线程
            new AlertDialog.Builder(MainActivity.this)
                    .setTitle("UI")
                    .setMessage("updateActivityUI Activity UI ...")
                    .setPositiveButton("老夫知道了", null)
                    .show();
        } else {  // TODO  C++ 用异步线程调用到此函数 ---->  异步线程
            Log.d("Derry", "updateActivityUI 所属于子线程，只能打印日志了..");

            runOnUiThread(new Runnable() { // 哪怕是异步线程  UI操作 正常下去 runOnUiThread
                @Override
                public void run() {

                    // 可以在子线程里面 操作UI
                    new AlertDialog.Builder(MainActivity.this)
                            .setTitle("updateActivityUI")
                            .setMessage("所属于子线程，只能打印日志了..")
                            .setPositiveButton("老夫知道了", null)
                            .show();
                }
            });
        }
    }


    /**
     * A native method that is implemented by the 'as_jni_project' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native void changeNumber();
    public static native void changeStaticNum();

    public native void addMethod();
    public native void printfArr(int[] aar);

    public native void changeContent(String[] aar);

    public native void initObject();
    public native void getStudent(Student student);

    public native void initDynamic();
    public native void naitveThread();


}
