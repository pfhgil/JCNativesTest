package org.example;

public class SingletonExample
{
    private static SingletonExample instance;

    private int foo;
    private String bar;

    private SingletonExample() {}

    public static SingletonExample getInstance()
    {
        return instance = (instance == null ? new SingletonExample() : instance);
    }

    @Override
    public String toString()
    {
        return "foo: " + foo + ", bar: " + bar;
    }

    public int getFoo() { return foo; }
    public void setFoo(int foo) { this.foo = foo; }

    public String getBar() { return bar; }
    public void setBar(String bar) { this.bar = bar; }
}
