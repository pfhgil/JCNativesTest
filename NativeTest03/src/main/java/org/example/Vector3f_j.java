package org.example;

public class Vector3f_j
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
