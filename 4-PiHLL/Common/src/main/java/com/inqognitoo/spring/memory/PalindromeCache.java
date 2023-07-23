package com.inqognitoo.spring.memory;

import org.springframework.stereotype.Component;

import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;

@Component
public class PalindromeCache {

    private final Map<String, Boolean> cache;

    public PalindromeCache(){
        this.cache = new ConcurrentHashMap<>();
    }

    public Optional<Boolean> get(String key){
        Boolean value = cache.get(key);
        return Optional.ofNullable(value);
    }

    public void put(String key, boolean val){
        cache.put(key, val);
    }
}
