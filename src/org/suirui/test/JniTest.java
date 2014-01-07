package org.suirui.test;

/**
 * Created by wei.li on 14-1-6.
 */
public class JniTest extends Object {
    JniTest() {
    }

    public void printMessage(int num) {
        System.out.println("Thread : " + num);
    }

    public void printMessage(String msg) {
        System.out.println("Message : " + msg);
    }
}
