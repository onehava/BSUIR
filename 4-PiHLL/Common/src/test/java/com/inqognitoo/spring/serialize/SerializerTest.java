package com.inqognitoo.spring.serialize;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.data.redis.serializer.SerializationException;

public class SerializerTest {

    // your mock could be here :)
    private static final BooleanRedisSerializer serializer = new BooleanRedisSerializer();

    private static final byte[] TRUE_BYTES = "true".getBytes();
    private static final byte[] FALSE_BYTES = "false".getBytes();

    @Test
    void testSerializeNull(){
        Assertions.assertThrows(SerializationException.class, () -> serializer.serialize(null));
    }

    @Test
    void testDeserializeNull(){
        Assertions.assertNull(serializer.deserialize(null));
    }

    @Test
    void testSerialize(){
        Assertions.assertArrayEquals(serializer.serialize(true), TRUE_BYTES);
        Assertions.assertArrayEquals(serializer.serialize(false), FALSE_BYTES);
    }

    @Test
    void testDeserializeValid(){
        Assertions.assertEquals(Boolean.TRUE, serializer.deserialize(TRUE_BYTES));
        Assertions.assertEquals(Boolean.FALSE, serializer.deserialize(FALSE_BYTES));
    }

    @Test
    void testDeserializeInvalid(){
        Assertions.assertNull(serializer.deserialize("True".getBytes()));
        Assertions.assertNull(serializer.deserialize("False".getBytes()));
        Assertions.assertNull(serializer.deserialize("Spring".getBytes()));
    }
}
