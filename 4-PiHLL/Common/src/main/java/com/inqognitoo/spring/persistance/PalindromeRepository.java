package com.inqognitoo.spring.persistance;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.ValueOperations;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public class PalindromeRepository {

    private final ValueOperations<String, Boolean> operations;

    @Autowired
    public PalindromeRepository(ValueOperations<String, Boolean> operations){
        this.operations = operations;
    }

    public Optional<Boolean> get(String key){
        return Optional.ofNullable(operations.get(key));
    }

    public void put(String key, boolean val){
        operations.set(key, val);
    }
}