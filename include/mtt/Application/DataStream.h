#pragma once

#include <vector>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <QtCore/QDataStream>

#include <mtt/Utilities/UID.h>

namespace mtt
{
  class DataStream : public QDataStream
  {
  public:
    inline DataStream(const QByteArray& a);
    inline DataStream(QByteArray* a, QIODevice::OpenMode mode);
    inline DataStream(QIODevice* d);
    inline DataStream();
    DataStream(const DataStream&) = delete;
    DataStream& operator = (const DataStream&) = delete;
    virtual ~DataStream() noexcept = default;

    inline DataStream& operator<<(int8_t i);
    inline DataStream& operator<<(uint8_t i);
    inline DataStream& operator<<(int16_t i);
    inline DataStream& operator<<(uint16_t i);
    inline DataStream& operator<<(int32_t i);
    inline DataStream& operator<<(uint32_t i);
    inline DataStream& operator<<(int64_t i);
    inline DataStream& operator<<(uint64_t i);
    inline DataStream& operator<<(std::nullptr_t ptr);
    inline DataStream& operator<<(bool i);
    inline DataStream& operator<<(float f);
    inline DataStream& operator<<(double f);
    inline DataStream& operator<<(const char* s);
    inline DataStream& operator<<(const glm::vec2& value);
    inline DataStream& operator<<(const glm::vec3& value);
    inline DataStream& operator<<(const glm::vec4& value);
    inline DataStream& operator<<(const glm::quat& value);
    inline DataStream& operator<<(const glm::mat3& value);
    inline DataStream& operator<<(const glm::mat4& value);
    inline DataStream& operator<<(const UID& id);
    template<typename ElementType>
    inline DataStream& operator<<(const std::vector<ElementType>& data);

    inline DataStream& operator>>(int8_t& i);
    inline DataStream& operator>>(uint8_t& i);
    inline DataStream& operator>>(int16_t& i);
    inline DataStream& operator>>(uint16_t& i);
    inline DataStream& operator>>(int32_t& i);
    inline DataStream& operator>>(uint32_t& i);
    inline DataStream& operator>>(int64_t& i);
    inline DataStream& operator>>(uint64_t& i);
    inline DataStream& operator>>(std::nullptr_t& ptr);
    inline DataStream& operator>>(bool& i);
    inline DataStream& operator>>(float& f);
    inline DataStream& operator>>(double& f);
    inline DataStream& operator>>(char*& s);
    inline DataStream& operator>>(glm::vec2& value);
    inline DataStream& operator>>(glm::vec3& value);
    inline DataStream& operator>>(glm::vec4& value);
    inline DataStream& operator>>(glm::mat3& value);
    inline DataStream& operator>>(glm::mat4& value);
    inline DataStream& operator>>(glm::quat& value);
    inline DataStream& operator>>(UID& id);
    template<typename ElementType>
    inline DataStream& operator>>(std::vector<ElementType>& target);

    inline int8_t readInt8();
    inline uint8_t readUint8();
    inline int16_t readInt16();
    inline uint16_t readUint16();
    inline int32_t readInt32();
    inline uint32_t readUint32();
    inline int64_t readInt64();
    inline uint64_t readUint64();
    inline bool readBool();
    inline float readFloat();
    inline double readDouble();
    inline glm::vec2 readVec2();
    inline glm::vec3 readVec3();
    inline glm::vec4 readVec4();
    inline glm::mat3 readMat3();
    inline glm::mat4 readMat4();
    inline glm::quat readQuat();
    inline UID readUID();
  };

  inline DataStream::DataStream(const QByteArray& a) :
    QDataStream(a)
  {
  }

  inline DataStream::DataStream(QByteArray* a, QIODevice::OpenMode mode) :
    QDataStream(a, mode)
  {
  }

  inline DataStream::DataStream(QIODevice* d) :
    QDataStream(d)
  {
  }

  inline DataStream::DataStream()
  {
  }

  inline DataStream& DataStream::operator<<(int8_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(uint8_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(int16_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(uint16_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(int32_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(uint32_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(int64_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(uint64_t i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(std::nullptr_t ptr)
  {
    QDataStream::operator<<(ptr);
    return *this;
  }

  inline DataStream& DataStream::operator<<(bool i)
  {
    QDataStream::operator<<(i);
    return *this;
  }

  inline DataStream& DataStream::operator<<(float f)
  {
    QDataStream::operator<<(f);
    return *this;
  }

  inline DataStream& DataStream::operator<<(double f)
  {
    QDataStream::operator<<(f);
    return *this;
  }

  inline DataStream& DataStream::operator<<(const char* s)
  {
    QDataStream::operator<<(s);
    return *this;
  }

  inline DataStream& DataStream::operator<<(const glm::vec2& value)
  {
    return operator<<(value.x) << value.y;
  }

  inline DataStream& DataStream::operator<<(const glm::vec3& value)
  {
    return operator<<(value.x) << value.y << value.z;
  }

  inline DataStream& DataStream::operator<<(const glm::vec4& value)
  {
    return operator<<(value.x) << value.y << value.z << value.w;
  }

  inline DataStream& DataStream::operator<<(const glm::quat& value)
  {
    return operator<<(value.x) << value.y << value.z << value.w;
  }

  inline DataStream& DataStream::operator<<(const glm::mat3& value)
  {
    return operator<<(value[0]) << value[1] << value[2];
  }

  inline DataStream& DataStream::operator<<(const glm::mat4& value)
  {
    return operator<<(value[0]) << value[1] << value[2] << value[3];
  }

  inline DataStream& DataStream::operator<<(const UID& id)
  {
    return operator<<(id.value());
  }

  template<typename ElementType>
  inline DataStream& DataStream::operator<<(
                                          const std::vector<ElementType>& data)
  {
    uint64_t dataLength = (uint64_t) data.size();
    operator<<(dataLength);
    for(const ElementType& element : data)
    {
      operator<<(element);
    }
    return *this;
  }

  inline DataStream& DataStream::operator>>(int8_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(uint8_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(int16_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(uint16_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(int32_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(uint32_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(int64_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(uint64_t& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(std::nullptr_t& ptr)
  {
    QDataStream::operator>>(ptr);
    return *this;
  }

  inline DataStream& DataStream::operator>>(bool& i)
  {
    QDataStream::operator>>(i);
    return *this;
  }

  inline DataStream& DataStream::operator>>(float& f)
  {
    QDataStream::operator>>(f);
    return *this;
  }

  inline DataStream& DataStream::operator>>(double& f)
  {
    QDataStream::operator>>(f);
    return *this;
  }

  inline DataStream& DataStream::operator>>(char*& s)
  {
    QDataStream::operator>>(s);
    return *this;
  }

  inline DataStream& DataStream::operator>>(glm::vec2& value)
  {
    return operator>>(value.x) >> value.y;
  }

  inline DataStream& DataStream::operator>>(glm::vec3& value)
  {
    return operator>>(value.x) >> value.y >> value.z;
  }

  inline DataStream& DataStream::operator>>(glm::vec4& value)
  {
    return operator>>(value.x) >> value.y >> value.z >> value.w;
  }

  inline DataStream& DataStream::operator>>(glm::mat3& value)
  {
    return operator>>(value[0]) >> value[1] >> value[2];
  }

  inline DataStream& DataStream::operator>>(glm::mat4& value)
  {
    return operator>>(value[0]) >> value[1] >> value[2] >> value[3];
  }

  inline DataStream& DataStream::operator>>(glm::quat& value)
  {
    return operator>>(value.x) >> value.y >> value.z >> value.w;
  }

  inline DataStream& DataStream::operator>>(UID& id)
  {
    UID::ValueType value;
    operator>>(value);
    id = UID(value);
    return *this;
  }

  template<typename ElementType>
  inline DataStream& DataStream::operator>>(std::vector<ElementType>& target)
  {
    uint64_t dataLength;
    operator>>(dataLength);
    if(dataLength == 0) target.clear();
    else
    {
      target.resize(dataLength);
      for(ElementType& element : target) operator>>(element);
    }
    return *this;
  }

  inline int8_t DataStream::readInt8()
  {
    int8_t value(0);
    operator >> (value);
    return value;
  }

  inline uint8_t DataStream::readUint8()
  {
    uint8_t value(0);
    operator >> (value);
    return value;
  }

  inline int16_t DataStream::readInt16()
  {
    int16_t value(0);
    operator >> (value);
    return value;
  }

  inline uint16_t DataStream::readUint16()
  {
    uint16_t value(0);
    operator >> (value);
    return value;
  }

  inline int32_t DataStream::readInt32()
  {
    int32_t value(0);
    operator >> (value);
    return value;
  }

  inline uint32_t DataStream::readUint32()
  {
    uint32_t value(0);
    operator >> (value);
    return value;
  }

  inline int64_t DataStream::readInt64()
  {
    int64_t value(0);
    operator >> (value);
    return value;
  }

  inline uint64_t DataStream::readUint64()
  {
    uint64_t value(0);
    operator >> (value);
    return value;
  }

  inline bool DataStream::readBool()
  {
    bool value(false);
    operator >> (value);
    return value;
  }

  inline float DataStream::readFloat()
  {
    float value(0);
    operator >> (value);
    return value;
  }

  inline double DataStream::readDouble()
  {
    double value(0);
    operator >> (value);
    return value;
  }

  inline glm::vec2 DataStream::readVec2()
  {
    glm::vec2 value(0);
    operator >> (value);
    return value;
  }

  inline glm::vec3 DataStream::readVec3()
  {
    glm::vec3 value(0);
    operator >> (value);
    return value;
  }

  inline glm::vec4 DataStream::readVec4()
  {
    glm::vec4 value(0);
    operator >> (value);
    return value;
  }

  inline glm::mat3 DataStream::readMat3()
  {
    glm::mat3 value(0);
    operator >> (value);
    return value;
  }

  inline glm::mat4 DataStream::readMat4()
  {
    glm::mat4 value(0);
    operator >> (value);
    return value;
  }

  inline glm::quat DataStream::readQuat()
  {
    glm::quat value(1,0,0,0);
    operator >> (value);
    return value;
  }

  inline UID DataStream::readUID()
  {
    UID value;
    operator >> (value);
    return value;
  }
}