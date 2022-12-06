#ifndef SERIALIZE_DATASTREAM_H
#define SERIALIZE_DATASTREAM_H

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include "Serializable.h"

class DataStream
{
public:
    void load(const std::string& fileName)
    {
        std::ifstream fin(fileName);
        std::stringstream ss;
        ss << fin.rdbuf();
        const std::string & str = ss.str();
        m_buf.clear();
        reserve(str.size());
        write(str.data(), str.size());
    }

    void save(const std::string& fileName)
    {
        std::ofstream out(fileName);
        out.write(m_buf.data(), m_buf.size());
        out.flush();
        out.close();
    }

    enum ByteOrder
    {
        BigEndian = 0,
        LittleEndian
    };
    enum DataType
    {
        BOOL = 0,
        CHAR,
        INT32,
        INT64,
        FLOAT,
        DOUBLE,
        STRING,
        VECTOR,
        LIST,
        MAP,
        SET,
        CUSTOM
    };

    DataStream() : m_pos(0), m_byteOrder(checkByteOrder())
    {
    };

    ~DataStream() = default;

    void reset()
    {
        m_pos = 0;
    }

    void clear()
    {
        m_buf.clear();
        reset();
    }

    bool read(char *data, int len)
    {
        std::memcpy(data, (char *) &m_buf[m_pos], len);
        m_pos += len;
        return true;
    }

    bool read(bool &value)
    {
        if (m_buf[m_pos] != DataType::BOOL)
        {
            return false;
        }
        ++m_pos;
        value = m_buf[m_pos];
        ++m_pos;
        return true;
    }

    bool read(char &value)
    {
        if (m_buf[m_pos] != DataType::CHAR)
        {
            return false;
        }
        ++m_pos;
        value = m_buf[m_pos];
        ++m_pos;
        return true;
    }



    template<typename T, typename ...Args>
    bool readArgs(T &head, Args &... args)
    {
        bool readRes = read(head);
        if constexpr(sizeof...(args) > 0)
            return readArgs(args...) && readRes;
        else
            return readRes;
    }

    bool read(int32_t &value)
    {
        if (m_buf[m_pos] != DataType::INT32)
        {
            return false;
        }
        ++m_pos;
        value = *((int32_t *) (&m_buf[m_pos]));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(int32_t);
            std::reverse(first, last);
        }
        m_pos += 4;
        return true;
    }

    bool read(int64_t &value)
    {
        if (m_buf[m_pos] != DataType::INT64)
        {
            return false;
        }
        ++m_pos;
        value = *((int64_t *) (&m_buf[m_pos]));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(int64_t);
            std::reverse(first, last);
        }
        m_pos += 8;
        return true;
    }

    bool read(float &value)
    {
        if (m_buf[m_pos] != DataType::FLOAT)
        {
            return false;
        }
        ++m_pos;
        value = *((float *) (&m_buf[m_pos]));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(float);
            std::reverse(first, last);
        }
        m_pos += 4;
        return true;
    }

    bool read(double &value)
    {
        if (m_buf[m_pos] != DataType::DOUBLE)
        {
            return false;
        }
        ++m_pos;
        value = *((double *) (&m_buf[m_pos]));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(double);
            std::reverse(first, last);
        }
        m_pos += 8;
        return true;
    }

    bool read(std::string &value)
    {
        if (m_buf[m_pos] != DataType::STRING)
        {
            return false;
        }
        ++m_pos;
        int len;
        read(len);
        if (len < 0)
        {
            return false;
        }
        value.assign((char *) &(m_buf[m_pos]), len);
        m_pos += len;
        return true;
    }

    template<typename T>
    bool read(std::vector<T> &value)
    {
        value.clear();
        if (m_buf[m_pos] != DataType::VECTOR)
        {
            return false;
        }
        ++m_pos;
        int len;
        read(len);
        for (int i = 0; i < len; i++)
        {
            T v;
            read(v);
            value.push_back(v);
        }
        return true;
    }

    template<typename T>
    bool read(std::list<T> &value)
    {
        value.clear();
        if (m_buf[m_pos] != DataType::LIST)
        {
            return false;
        }
        ++m_pos;
        int len;
        read(len);
        for (int i = 0; i < len; i++)
        {
            T v;
            read(v);
            value.push_back(v);
        }
        return true;
    }

    template<typename K, typename V>
    bool read(std::map<K, V> &value)
    {
        value.clear();
        if (m_buf[m_pos] != DataType::MAP)
        {
            return false;
        }
        ++m_pos;
        int len;
        read(len);
        for (int i = 0; i < len; i++)
        {
            K k;
            read(k);

            V v;
            read(v);
            value[k] = v;
        }
        return true;
    }

    template<typename T>
    bool read(std::set<T> &value)
    {
        value.clear();
        if (m_buf[m_pos] != DataType::SET)
        {
            return false;
        }
        ++m_pos;
        int len;
        read(len);
        for (int i = 0; i < len; i++)
        {
            T v;
            read(v);
            value.insert(v);
        }
        return true;
    }

    bool read(Serializable &serializable)
    {
        serializable.deserialize(*this);
    }


    template<typename T>
    void write(const std::vector<T> &value)
    {
        char type = DataType::VECTOR;
        write((char *) &type, sizeof(char));
        int len = value.size();
        write(len);
        for (int i = 0; i < len; i++)
            write(value[i]);

    }

    template<typename T>
    void write(const std::list<T> &value)
    {
        char type = DataType::LIST;
        write((char *) &type, sizeof(char));
        int len = value.size();
        write(len);
        for (auto it = value.begin(); it != value.end(); it++)
        {
            write((*it));
        }
    }

    template<typename K, typename V>
    void write(const std::map<K, V> &value)
    {
        char type = DataType::MAP;
        write((char *) &type, sizeof(char));
        int len = value.size();
        write(len);
        for (auto it = value.begin(); it != value.end(); it++)
        {
            write(it->first);
            write(it->second);
        }
    }

    template<typename T>
    void write(const std::set<T> &value)
    {
        char type = DataType::SET;
        write((char *) &type, sizeof(char));
        int len = value.size();
        write(len);
        for (auto it = value.begin(); it != value.end(); it++)
        {
            write(*it);
        }
    }


    void write(const char *data, int len)
    {
        reserve(len);
        auto size = m_buf.size();
        m_buf.resize(size + len);
        std::memcpy(&m_buf[size], data, len);
    }

    void write(const char *value)
    {
        char type = DataType::STRING;
        write((char *) &type, sizeof(char));
        int len = strlen(value);
        write(len);
        write(value, len);
    }

    void write(const std::string &value)
    {
        char type = DataType::STRING;
        write((char *) &type, sizeof(char));
        int len = value.size();
        write(len);
        write(value.data(), len);
    }

    void write(float value)
    {
        char type = DataType::FLOAT;
        write((char *) &type, sizeof(char));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(float);
            std::reverse(first, last);
        }
        write((char *) &value, sizeof(float));
    }

    void write(double value)
    {
        char type = DataType::DOUBLE;
        write((char *) &type, sizeof(char));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(double);
            std::reverse(first, last);
        }
        write((char *) &value, sizeof(double));
    }

    void write(int64_t value)
    {
        char type = DataType::INT64;
        write((char *) &type, sizeof(char));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(int64_t);
            std::reverse(first, last);
        }
        write((char *) &value, sizeof(int64_t));
    }

    void write(int32_t value)
    {
        char type = DataType::INT32;
        write((char *) &type, sizeof(char));
        if (m_byteOrder == ByteOrder::BigEndian)
        {
            char *first = (char *) &value;
            char *last = first + sizeof(int32_t);
            std::reverse(first, last);
        }


        write((char *) &value, sizeof(int32_t));
    }

    void write(bool value)
    {
        char type = DataType::BOOL;
        write((char *) &type, sizeof(char));
        write((char *) &value, sizeof(bool));
    }

    void write(char value)
    {
        char type = DataType::CHAR;
        write((char *) &type, sizeof(char));
        write((char *) &value, sizeof(char));
    }

    void write(const Serializable &serializable)
    {
        serializable.serialize(*this);
    }


    template<typename T, typename ...Args>
    void writeArgs(const T &head, const Args &... args)
    {
        write(head);
        if constexpr (sizeof...(args) > 0)
            writeArgs(args...);
    }


    template<typename T>
    DataStream &operator<<(const T &v)
    {
        write(v);
        return *this;
    }


    template<typename T>
    DataStream &operator>>(T &v)
    {
        read(v);
        return *this;
    }

private:
    void reserve(int len)
    {
        auto size = m_buf.size();
        auto cap = m_buf.capacity();
        if (size + len > cap)
        {
            while (size + len > cap)
            {
                if (cap == 0)
                {
                    cap = 1;
                } else
                {
                    cap *= 2;
                }
            }
            m_buf.reserve(cap);
        }
    }

    static ByteOrder checkByteOrder()
    {
        int n = 0x12345678;
        char str[4];
        memcpy(str, &n, sizeof(int));
        if (str[0] == 0x12)
        {
            return ByteOrder::BigEndian;
        } else
        {
            return ByteOrder::LittleEndian;
        }
    }

private:
    std::vector<char> m_buf;
    int m_pos = 0;
    ByteOrder m_byteOrder;
};

#endif //SERIALIZE_DATASTREAM_H
