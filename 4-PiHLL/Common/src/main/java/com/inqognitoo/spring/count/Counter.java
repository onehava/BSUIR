package com.inqognitoo.spring.count;

// almost atomic but synchronized :|
public class Counter {

    // using wrap to demonstrate a 'problem' with synchronization :|
    private Long count = 0L;

    public long get(){
        return count;
    }

    public synchronized void increment(){
        ++count;
    }
}
