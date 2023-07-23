package com.inqognitoo.spring.text;

import java.util.List;

public class Text {

    public static boolean isPalindrome(String text) throws IllegalArgumentException {
        if(text.isEmpty()){
            throw new IllegalArgumentException("Trying to classify an empty string as a palindrome");
        }

        int i = 0;
        int j = text.length() - 1;

        while(i < j){
            if(text.charAt(i) != text.charAt(j)){
                return false;
            }

            i++;
            j--;
        }

        return true;
    }

    public static String quoted(String string){
        return "\"" + string + "\"";
    }

    public static List<String> quoted(List<String> string){
        return string.stream().map(Text::quoted).toList();
    }
}
