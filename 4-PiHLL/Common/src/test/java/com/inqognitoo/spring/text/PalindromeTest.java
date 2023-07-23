package com.inqognitoo.spring.text;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class PalindromeTest {

    @Test
    void testEmptyString(){
        Assertions.assertThrows(IllegalArgumentException.class, ()-> Text.isPalindrome(""));
    }

    @Test
    void testPalindromeTrue(){
        Assertions.assertTrue(Text.isPalindrome("A"));
        Assertions.assertTrue(Text.isPalindrome("AA"));
        Assertions.assertTrue(Text.isPalindrome("ABA"));
        Assertions.assertTrue(Text.isPalindrome("ABBA"));
        Assertions.assertTrue(Text.isPalindrome("AB BA"));
    }

    @Test
    void testPalindromeFalse(){
        Assertions.assertFalse(Text.isPalindrome("AB"));
        Assertions.assertFalse(Text.isPalindrome("ABB"));
        Assertions.assertFalse(Text.isPalindrome("AABB"));
        Assertions.assertFalse(Text.isPalindrome("AB AB"));
        Assertions.assertFalse(Text.isPalindrome("AB  AB"));
    }
}