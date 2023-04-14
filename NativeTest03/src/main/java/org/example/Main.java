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

    public static class PoolAllocator
    {
        private long pointer = 0x0;
        private long byteSize;
        private long maxRegionByteSize;

        public PoolAllocator(long byteSize, long maxRegionSize)
        {
            this.byteSize = byteSize;
            this.maxRegionByteSize = maxRegionSize;
            pointer = create(this.byteSize, this.maxRegionByteSize);
        }

        private native long create(long byteSize, long maxRegionSize);

        public long getPointer() { return pointer; }
    }

    public static class Vector3f
    {
        private long pointer = 0x0;
        private long poolAllocatorPointer = 0x0;

        public Vector3f(long poolAllocatorPointer)
        {
           pointer = create(poolAllocatorPointer);

           this.poolAllocatorPointer = poolAllocatorPointer;

           n_x_y_z(pointer, 0f, 0f, 0f);
        }

        public Vector3f(long poolAllocatorPointer, float x, float y, float z)
        {
            pointer = create(poolAllocatorPointer);

            this.poolAllocatorPointer = poolAllocatorPointer;

            n_x_y_z(pointer, x, y, z);
        }

        public Vector3f(long poolAllocatorPointer, float v)
        {
            pointer = create(poolAllocatorPointer);

            this.poolAllocatorPointer = poolAllocatorPointer;

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

        public void memFree(long poolAllocatorPointer) { n_memFree(poolAllocatorPointer, pointer); }

        private native long create(long poolAllocatorPointer);

        private native void n_memFree(long poolAllocatorPointer, long pointer);

        private native float n_x(long pointer);
        private native void n_x(long pointer, float val);

        private native float n_y(long pointer);
        private native void n_y(long pointer, float val);

        private native float n_z(long pointer);

        private native void n_z(long pointer, float val);

        private native void n_x_y_z(long pointer, float x, float y, float z);

        public long getPointer() { return pointer; }
    }

    public static void main(String[] args)
    {

        System.out.println("Hello world!");

        final int size = 1000;

        PoolAllocator firstPoolAllocator = new PoolAllocator(96, 12);
        System.out.println("fptr: " + firstPoolAllocator.getPointer());
        PoolAllocator secondPoolAllocator = new PoolAllocator(1024 * 1024, 12);
        System.out.println("sptr: " + secondPoolAllocator.getPointer());

        Vector3f firstVec = new Vector3f(firstPoolAllocator.getPointer(), 10);
        Vector3f secondVec = new Vector3f(secondPoolAllocator.getPointer(), 20);

        firstVec.memFree(secondPoolAllocator.getPointer());
        secondVec.memFree(firstPoolAllocator.getPointer());

        firstVec.memFree(firstPoolAllocator.getPointer());
        secondVec.memFree(secondPoolAllocator.getPointer());
                /*
        List<Vector3f> vectors = new ArrayList<>();
        for(int i = 0; i < size; i++) {
            Vector3f vec = new Vector3f(10, 10);
            vec.memFree();
            //System.out.println(vec);
            vectors.add(vec);
        }

                 */

        SingletonExample.getInstance().setBar("hello world!");
        SingletonExample.getInstance().setFoo(18);

        System.out.println(SingletonExample.getInstance());

        System.out.println("filled");

        Scanner s = new Scanner(System.in);

        s.next();

        NativeControl.freeVec3Allocator(secondPoolAllocator.getPointer());

        s.next();
    }
}