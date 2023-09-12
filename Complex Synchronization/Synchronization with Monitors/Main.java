public class Main {
    public static void main(String[] args) {
        System.out.println("Create a new buffer whose capacity is 100.\n");
        BoundedBuffer myBuffer = new BoundedBuffer(100);

        MyThread thread1 = new MyThread(1, myBuffer);
        MyThread thread2 = new MyThread(2, myBuffer);
        MyThread thread3 = new MyThread(3, myBuffer);

        thread1.start();
        thread2.start();
        thread3.start();
    }
}