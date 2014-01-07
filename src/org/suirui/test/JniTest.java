package org.suirui.test;

/**
 * Created by wei.li on 14-1-6.
 */
public class JniTest implements Runnable {
    static int value = 0;
    int id = 0;
    int test = 192;
    Thread trd = null;
    JniTest() {
        trd = new Thread(this, "jnitestthread");
    }

    JniTest(int id) {
        this.id = id;
        trd = new Thread(this, "jnitestthread");
    }

    public void printMessage() {
        System.out.println("Message from JniTest");
    }

    public void printMessage(String msg) {
        System.out.println("Message : " + msg);
    }

    @Override
    public void run() {
        while (true) {
            try {
                synchronized (this){
                    value++;
                }
                Thread.sleep(1000);
                System.out.println("Thread: " + id + "--" + "value: " + value);
            } catch (Exception e){
                e.printStackTrace();
            }
        }
    }
}
