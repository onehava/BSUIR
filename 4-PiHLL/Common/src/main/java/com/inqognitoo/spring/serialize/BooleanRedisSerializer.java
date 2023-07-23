package com.inqognitoo.spring.serialize;

import org.springframework.data.redis.serializer.RedisSerializer;
import org.springframework.data.redis.serializer.SerializationException;

public class BooleanRedisSerializer implements RedisSerializer<Boolean> {

    @Override
    public byte[] serialize(Boolean primitive) throws SerializationException {
        if(primitive == null){
            throw new SerializationException("try to serialize null value");
        }

        String represent = String.valueOf(primitive);
        return represent.getBytes();
    }

    @Override
    public Boolean deserialize(byte[] bytes) throws SerializationException {
        if(bytes == null){
            return null;
        }

        String represent = new String(bytes);
        return switch(represent){
            case "true" -> true;
            case "false" -> false;
            default -> null;
        };
    }
}