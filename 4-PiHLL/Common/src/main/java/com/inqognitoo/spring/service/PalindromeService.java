package com.inqognitoo.spring.service;

import com.inqognitoo.spring.memory.PalindromeCache;
import com.inqognitoo.spring.persistance.PalindromeRepository;
import com.inqognitoo.spring.text.Text;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class PalindromeService {

    private static final Logger logger =
            LoggerFactory.getLogger(PalindromeService.class);

    private final PalindromeCache cache;
    private final PalindromeRepository repository;

    @Autowired
    public PalindromeService(PalindromeCache cache, PalindromeRepository repository){
        this.cache = cache;
        this.repository = repository;
    }

    public boolean test(String string){
        Optional<Boolean> cached = cache.get(string);

        if(cached.isPresent()){
            logger.info("Response for request '" + string + "' has been found in cache");
            return cached.get();
        }

        Optional<Boolean> stored = repository.get(string);
        boolean response;

        if(stored.isPresent()){
            logger.info("Response for request '" + string + "' has been found in redis");
            response = stored.get();
        }
        else {
            logger.info("Response for request '" + string + "' can't be found, make it");
            response = Text.isPalindrome(string);
            repository.put(string, response);
        }

        cache.put(string, response);

        return response;
    }

    public List<Boolean> test(List<String> strings){
        return strings.stream().map(this::test).toList();
    }
}