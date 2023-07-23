package com.inqognitoo.spring.statistics;

import java.util.List;

public class Statistics {

    public static double averageLength(List<String> strings){
        return strings.stream()
                .mapToLong(String::length).average().orElse(0);
    }

    public static double averageBoolean(List<Boolean> booleans){
        return (double) booleans.stream()
                .filter((v) -> v).count() / booleans.size();
    }
}