package com.inqognitoo.spring.control;

import com.inqognitoo.spring.service.CounterService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.Collections;

@RestController
public class CounterRestController {

    private static final Logger logger =
            LoggerFactory.getLogger(CounterRestController.class);

    private final CounterService counterService;

    @Autowired
    public CounterRestController(CounterService counterService){
        this.counterService = counterService;
    }

    @RequestMapping(method = RequestMethod.GET,
            value = "/counter",
            produces = "application/json")
    public Object getCounter(){
        // just a get demo message :|
        logger.info("/GET /counter");
        return Collections.singletonMap("counter", counterService.get());
    }
}