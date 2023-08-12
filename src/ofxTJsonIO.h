//
//  ofxTJsonIO.h
//

#pragma once

#include <ofJson.h>
#include <ofRectangle.h>
#include <ofTypes.h>
#include <ofMatrix4x4.h>
#include <string>
#include <type_traits>
#include "ofxTEnums.h"

namespace ofxTCommon {

  class JsonException {
  public:
    JsonException(std::string msg) : message(msg) { }
    std::string message;
  };


  class JsonReadable {
  public:
    virtual void readJson(const ofJson& val) = 0;
    void readFromFile(std::string filepath);
  };

  class JsonWritable {
  public:
    virtual ofJson toJson() const = 0;

    std::string toJsonString() const;
    void writeJsonTo(std::ostream& os) const;
    void writeJsonTo(std::string filepath) const;
  };

  namespace JsonUtil {
    void writeJsonToFile(const std::string& filepath,
                         const ofJson& val);

    void assertHasType(const ofJson& val, ofJson::value_t type);
    void assertHasLength(const ofJson& val, ofJson::size_type length);
    void assertIsArray(const ofJson& val);
    void assertIsArray(const ofJson& val, ofJson::size_type length);
    void assertIsObject(const ofJson& val);

//    template<typename T>
//    ofJson::value_t valueType();
//
//    template<>
//    ofJson::value_t valueType<bool>() {
//      return ofJson::value_t::boolean;
//    }
//
//    template<>
//    ofJson::value_t valueType<int>() {
//      return ofJson::value_t::number_integer;
//    }

    template<typename T>
    ofJson toJson(const T& value) {
      return value;
    }

    template<>
    ofJson toJson(const ofVec2f& value);

    template<>
    ofJson toJson(const ofVec3f& value);

    template<>
    ofJson toJson(const ofVec4f& value);

    template<>
    ofJson toJson(const glm::vec2& value);

    template<>
    ofJson toJson(const glm::vec3& value);

    template<>
    ofJson toJson(const glm::vec4& value);

    template<>
    ofJson toJson(const ofFloatColor& value);

    template<>
    ofJson toJson(const ofMatrix4x4& value);

    template<>
    ofJson toJson(const ofRectangle& value);

    template<typename T,
    typename std::enable_if<std::is_base_of<JsonWritable, T>::value, T>::type>
    ofJson toJson(const T& value) {
      return value.toJson();
    }

    template<typename T, typename Iter>
    ofJson toJsonArray(Iter begin, Iter end) {
      auto arr = ofJson::array();
      for (auto iter = begin; iter != end; iter++) {
        arr.push_back(toJson<T>(*iter));
      }
      return arr;
    }

    template<typename T, typename Iter>
    ofJson toJsonArrayOrNull(Iter begin, Iter end) {
      if (begin == end) {
        return nullptr;
      }
      return toJsonArray<T, Iter>(begin, end);
    }

    template<typename T>
    T fromJson(const ofJson& value) {
      return value;
    }

    template<>
    ofVec2f fromJson<ofVec2f>(const ofJson& value);

    template<>
    ofVec3f fromJson<ofVec3f>(const ofJson& value);

    template<>
    ofVec4f fromJson<ofVec4f>(const ofJson& value);

    template<>
    glm::vec2 fromJson<glm::vec2>(const ofJson& value);

    template<>
    glm::vec3 fromJson<glm::vec3>(const ofJson& value);

    template<>
    glm::vec4 fromJson<glm::vec4>(const ofJson& value);

    template<>
    ofMatrix4x4 fromJson<ofMatrix4x4>(const ofJson& value);

    template<>
    ofFloatColor fromJson<ofFloatColor>(const ofJson& value);

    template<>
    ofRectangle fromJson<ofRectangle>(const ofJson& value);

    template<typename T,
    typename std::enable_if<std::is_default_constructible<T>::value && std::is_base_of<JsonWritable, T>::value, T>::type>
    T fromJson(const ofJson& value) {
      T result;
      result.readJson(value);
      return result;
    }

    template<typename T>
    std::vector<T> fromJsonArray(const ofJson& value) {
      assertHasType(value, ofJson::value_t::array);
      std::vector<T> result;
      for (const auto& val : value) {
        result.push_back(fromJson<T>(val));
      }
      return result;
    }

    template<typename T>
    std::vector<T> fromJsonArrayOrNull(const ofJson& value) {
      if (value.is_null()) {
        return std::vector<T>();
      }
      return fromJsonArray<T>(value);
    }

    template<typename T>
    T fromJsonField(const ofJson& obj,
                    const std::string& key,
                    const T& defaultValue) {
      if (obj.find(key) == obj.end()) {
        return defaultValue;
      }
      ofJson val = obj[key];
      if (val.is_null()) {
        return defaultValue;
      } else {
        return fromJson<T>(val);
      }
    }

    template<typename T>
    T fromJsonField(const ofJson& obj,
                    const std::string& key) {
      T defaultValue;
      return fromJsonField(obj, key, defaultValue);
    }

    template<typename T>
    typename std::enable_if<std::is_enum<T>::value, T>::type
    enumFromJson(const ofJson& value) {
      assertHasType(value, ofJson::value_t::string);
      std::string str = value;
      return ofxTCommon::parseEnum<T>(str);
    }

    template<typename T>
    ofJson enumToJson(const T& value) {
      return ofxTCommon::enumToString(value);
    }

    void mergeInto(ofJson& targetObj, const ofJson& sourceObj);
    ofJson merge(const ofJson& obj1, const ofJson& obj2);
  }
}
