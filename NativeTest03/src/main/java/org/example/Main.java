package org.example;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main
{
    static
    {
        //System.load("iostream");
        System.loadLibrary("libtest00");
    }

    public static class TransformComponent
    {

    }

    public static class Vector3f
    {
        private long pointer = 0x0;

        public Vector3f()
        {
           pointer = create();

           n_x_y_z(pointer, 0f, 0f, 0f);
        }

        public Vector3f(float x, float y, float z)
        {
            pointer = create();

            n_x_y_z(pointer, x, y, z);
        }

        public Vector3f(float v)
        {
            pointer = create();

            n_x_y_z(pointer, v, v, v);
        }

        @Override
        public String toString()
        {
            return x() + ", "  + y() + ", " + z();
        }

        public float x() { return n_x(pointer); }
        public void x(float val) { n_x(pointer, val); }

        public float y() { return n_y(pointer); }
        public void y(float val) { n_y(pointer, val); }

        public float z() { return n_z(pointer); }
        public void z(float val) { n_z(pointer, val); }

        public native void doCrazy();

        public void memFree() { n_memFree(pointer); }

        private native long create();

        private native void n_memFree(long pointer);

        private native float n_x(long pointer);
        private native void n_x(long pointer, float val);

        private native float n_y(long pointer);
        private native void n_y(long pointer, float val);

        private native float n_z(long pointer);

        private native void n_z(long pointer, float val);

        private native void n_x_y_z(long pointer, float x, float y, float z);

        public long getPointer() { return pointer; }
    }

    public static class Vector3f_j
    {
        public float x;
        public float y;
        public float z;

        public Vector3f_j()
        {

        }

        public Vector3f_j(float x, float y, float z)
        {
           this.x = x;
           this.y = y;
           this.z = z;
        }

        public Vector3f_j(float v)
        {
            x = y = z = v;
        }

        public void doCrazy()
        {
            int k = 0;
            for(int i = 0; i < 100000; i++)
            {
                k += i;
            }
        }

        @Override
        public String toString()
        {
            return x + ", "  + y + ", " + z;
        }
    }


    public static void main(String[] args)
    {

        System.out.println("Hello world!");

        final int size = 10;

        List<Vector3f> vectors = new ArrayList<>();
        for(int i = 0; i < 10; i++) {
            Vector3f vec = new Vector3f(10);
            System.out.println(vec);
            vectors.add(vec);
        }

        for(int i = 0; i < 10; i++) {
            Vector3f vec = vectors.get(i);
            vec.memFree();
            System.out.println(vec);
        }

        for(int i = 0; i < 10; i++) {
            vectors.set(i, new Vector3f());
            Vector3f vec = vectors.get(i);
            System.out.println(vec);
        }

        //Scanner s = new Scanner(System.in);
        //s.next();

        /*
        List<Vector3f_j> vectors = new ArrayList<>();

        for(int i = 0; i < 100000; i++) {
            Vector3f_j v = new Vector3f_j();
            v.doCrazy();
            vectors.add(v);
        }

         */
    }
}