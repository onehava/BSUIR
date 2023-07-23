package com.inqognitoo.spring.service;

import com.inqognitoo.spring.count.Counter;
import org.springframework.stereotype.Service;

@Service
public class CounterService {

    private final Counter counter;

    public CounterService(){
        this.counter = new Counter();
    }

    public void increase(){
        counter.increment();
    }

    public long get(){
        return counter.get();
    }
}