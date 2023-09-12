import java.util.Random;

public class MyThread extends Thread {
    private final int Thread_ID;
    private final BoundedBuffer mybuffer;

    public MyThread(int Thread_ID, BoundedBuffer buffer) {
        super();
        this.Thread_ID = Thread_ID;
        this.mybuffer = buffer;
    }

    public void run() {
        super.run();
        for (int i = 0; i < 30; i++) {
            try {
                Random rand = new Random();
                String insertStr = String.valueOf(Thread_ID) + "-" + String.valueOf(rand.nextInt(90000) + 10000);
                mybuffer.deposit(insertStr);
                System.out.println("Insert string " + insertStr + ".\n");
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
            
        }
    }
}