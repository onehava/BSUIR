package com.inqognitoo.spring.errors;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;

@ControllerAdvice
public class ControllerAdvisor {

    private static final Logger logger =
            LoggerFactory.getLogger(ControllerAdvisor.class);

    @ExceptionHandler(IllegalArgumentException.class)
    public ResponseEntity<ErrorMessage> argumentHandle(IllegalArgumentException exception){
        String msg = "Bad request: " + exception.getMessage();
        logger.info(msg);
        ErrorMessage message = new ErrorMessage(400, msg);
        return new ResponseEntity<>(message, HttpStatus.BAD_REQUEST);
    }

    @ExceptionHandler(Exception.class)
    public ResponseEntity<ErrorMessage> unexpectedHandle(Exception exception){
        String msg = "Internal error: " + exception.getMessage();
        logger.error(msg);
        ErrorMessage message = new ErrorMessage(500, "Internal server error");
        return new ResponseEntity<>(message, HttpStatus.INTERNAL_SERVER_ERROR);
    }
}