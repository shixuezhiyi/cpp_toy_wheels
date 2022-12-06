#ifndef SERIALIZE_SERIALIZABLE_H
#define SERIALIZE_SERIALIZABLE_H

class DataStream;

class Serializable
{
public:
    virtual void serialize( DataStream&  stream) const = 0;

    virtual bool deserialize(DataStream & stream) = 0;
};

#define SERIALIZE(...)                              \
    void serialize(DataStream & stream) const       \
    {                                               \
        char type = DataStream::CUSTOM;             \
        stream.write((char *)&type, sizeof(char));  \
        stream.writeArgs(__VA_ARGS__);             \
    }                                               \
                                                    \
    bool deserialize(DataStream & stream)           \
    {                                               \
        char type;                                  \
        stream.read(&type, sizeof(char));           \
        if (type != DataStream::CUSTOM)             \
        {                                           \
            return false;                           \
        }                                           \
        stream.readArgs(__VA_ARGS__);              \
        return true;                                \
    };
#endif //SERIALIZE_SERIALIZABLE_H
