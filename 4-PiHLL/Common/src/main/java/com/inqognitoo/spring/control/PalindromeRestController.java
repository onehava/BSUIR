package com.inqognitoo.spring.control;

import com.inqognitoo.spring.service.CounterService;
import com.inqognitoo.spring.service.PalindromeService;
import com.inqognitoo.spring.statistics.Statistics;
import com.inqognitoo.spring.text.Text;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletableFuture;

@RestController
public class PalindromeRestController {

    private static final Logger logger =
            LoggerFactory.getLogger(PalindromeRestController.class);

    private final PalindromeService palindromeService;
    private final CounterService counterService;

    @Autowired
    public PalindromeRestController(PalindromeService palindromeService, CounterService counterService){
        this.palindromeService = palindromeService;
        this.counterService = counterService;
    }

    @RequestMapping(method = RequestMethod.GET,
            value = "/palindrome",
            produces = "application/json")
    public Object isPalindrome(@RequestParam(value = "string") String string){
        // just a get demo message :|
        logger.info("GET /palindrome " + Text.quoted(string));
        counterService.increase();
        return Collections.singletonMap("isPalindrome", palindromeService.test(string));
    }

    @RequestMapping(method = RequestMethod.POST,
            value = "/palindromes",
            produces = "application/json")
    public Object isPalindrome(@RequestBody List<String> strings){
        // just a post demo message :|
        logger.info("POST /palindromes " + Text.quoted(strings));
        counterService.increase();
        List<Boolean> booleans = palindromeService.test(strings);
        return Map.of(
            "isPalindrome", booleans,
            "averageLength", Statistics.averageLength(strings),
            "averageResult", Statistics.averageBoolean(booleans)
        );
    }

    @RequestMapping(method = RequestMethod.GET,
            value = "/async/palindrome",
            produces = "application/json")
    public Object isPalindromeAsync(@RequestParam(value = "string") String string){
        // just a get demo message :|
        logger.info("GET /async/palindrome " + Text.quoted(string));
        CompletableFuture.runAsync(() -> {
            counterService.increase();
            palindromeService.test(string);
        });
        return Collections.singletonMap("key", string);
    }
}