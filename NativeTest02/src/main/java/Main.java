public class Main
{
    public native void print();

    static
    {
        System.loadLibrary("hello");
    }
    public static void main(String[] args) {
        //System.out.println("Hello world!");

        new Main().print();
    }
}