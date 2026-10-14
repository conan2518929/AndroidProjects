package com.example.as_jni_project;

import androidx.annotation.NonNull;

public class Person {

    public Student student;


    public void setStudent(Student student) {
        this.student = student;
    }

    @NonNull
    @Override
    public String toString() {
        return "Person{" +
                "student=" + student +
                '}';
    }
}
