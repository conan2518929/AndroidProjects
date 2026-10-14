package com.example.as_jni_project;

public class YiGePlay {

    // Used to load the 'as_jni_project' library on application startup.
    static {
        System.loadLibrary("as_jni_project");
    }
    private String source = "";

    private YiGePlayListener yiGePlayListener;

    public void setYiGePlayListener(YiGePlayListener yiGePlayListener) {
        this.yiGePlayListener = yiGePlayListener;
    }


    public void setSource(String source){
        this.source = source;
    }

    public void prepare(){
        System.out.println("传递的 source ： " + source);
        onNativeSetSource(source);
    }

    public void onStartPlay(){
        onNativeStartPlay();
    }

    public void onStopPlay(){
        onNativeStopPlay();
    }


    public void onReleasePlay(){
        onNativeReleasePlay();
    }

    void onPrapare(){
        yiGePlayListener.onPrapare();
    }

    public interface YiGePlayListener{
        void onPrapare();
    }

    public native void onNativeSetSource(String source);
    public native void onNativeStartPlay();
    public native void onNativeStopPlay();
    public native void onNativeReleasePlay();

}
