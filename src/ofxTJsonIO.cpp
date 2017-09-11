//
//  ofxTJsonIO.cpp
//

#include "ofxTJsonIO.h"

#include <ofLog.h>
#include <ofUtils.h>
#include <stdexcept>

using namespace ofxTCommon;

void JsonUtil::writeJsonToFile(const std::string &filepath,
                               const ofJson &val) {
  ofFile file(filepath, ofFile::WriteOnly);
  file << val.dump(2);
}

void JsonReadable::readFromFile(std::string filepath) {
  ofJson obj = ofLoadJson(filepath);
  if (obj.empty()) {
    //..?
    return;
  }
  readJson(obj);
}

void JsonWritable::writeJsonTo(std::ostream &os) const {
  auto obj = toJson();
  os << obj.dump(2);
}

void JsonWritable::writeJsonTo(std::string filepath) const {
  auto obj = toJson();
  JsonUtil::writeJsonToFile(filepath, obj);
}


std::string JsonWritable::toJsonString() const {
  auto obj = toJson();
  return obj.dump(2);
}

void JsonUtil::assertHasType(const ofJson &val, ofJson::value_t type) {
  if (val.type() != type) {
    throw JsonException("Incorrect value type: " + val.dump());
  }
}

void JsonUtil::assertHasLength(const ofJson &val, ofJson::size_type length) {
  if (val.size() != length) {
    throw JsonException("Incorrect object size: " + val.dump());
  }
}

void JsonUtil::assertIsObject(const ofJson &val) {
  JsonUtil::assertHasType(val, ofJson::value_t::object);
}

void JsonUtil::assertIsArray(const ofJson &val) {
  JsonUtil::assertHasType(val, ofJson::value_t::array);
}

void JsonUtil::assertIsArray(const ofJson &val,
                             ofJson::size_type length) {
  JsonUtil::assertIsArray(val);
  JsonUtil::assertHasLength(val, length);
}

template<>
ofJson JsonUtil::toJson(const ofVec2f& value) {
  return {value.x, value.y};
}

template<>
ofJson JsonUtil::toJson(const ofVec3f& value) {
  return {value.x, value.y, value.z};
}

template<>
ofJson JsonUtil::toJson(const ofVec4f& value) {
  return {value.x, value.y, value.z, value.w};
}

template<>
ofJson JsonUtil::toJson(const glm::vec2& value) {
  return {value.x, value.y};
}

template<>
ofJson JsonUtil::toJson(const glm::vec3& value) {
  return {value.x, value.y, value.z};
}

template<>
ofJson JsonUtil::toJson(const glm::vec4& value) {
  return {value.x, value.y, value.z, value.w};
}

template<>
ofJson JsonUtil::toJson(const ofFloatColor& value) {
  return {value.r, value.g, value.b, value.a};
}

template<>
ofJson JsonUtil::toJson(const ofMatrix4x4& value) {
  return {
    JsonUtil::toJson(value.getRowAsVec4f(0)),
    JsonUtil::toJson(value.getRowAsVec4f(1)),
    JsonUtil::toJson(value.getRowAsVec4f(2)),
    JsonUtil::toJson(value.getRowAsVec4f(3)),
  };
}

template<>
ofVec2f JsonUtil::fromJson<ofVec2f>(const ofJson& value) {
  JsonUtil::assertIsArray(value, 2);
  return ofVec2f(value[0], value[1]);
}

template<>
ofVec3f JsonUtil::fromJson<ofVec3f>(const ofJson& value) {
  JsonUtil::assertIsArray(value, 3);
  return ofVec3f(value[0], value[1], value[2]);
}

template<>
ofVec4f JsonUtil::fromJson<ofVec4f>(const ofJson& value) {
  JsonUtil::assertIsArray(value, 4);
  return ofVec4f(value[0], value[1], value[2], value[3]);
}

template<>
glm::vec2 JsonUtil::fromJson<glm::vec2>(const ofJson& value) {
  return JsonUtil::fromJson<ofVec2f>(value);
}

template<>
glm::vec3 JsonUtil::fromJson<glm::vec3>(const ofJson& value) {
  return JsonUtil::fromJson<ofVec3f>(value);
}

template<>
glm::vec4 JsonUtil::fromJson<glm::vec4>(const ofJson& value) {
  return JsonUtil::fromJson<ofVec4f>(value);
}

template<>
ofMatrix4x4 JsonUtil::fromJson<ofMatrix4x4>(const ofJson& value) {
  JsonUtil::assertIsArray(value, 4);
  auto row0 = JsonUtil::fromJson<ofVec4f>(value[0]);
  auto row1 = JsonUtil::fromJson<ofVec4f>(value[1]);
  auto row2 = JsonUtil::fromJson<ofVec4f>(value[2]);
  auto row3 = JsonUtil::fromJson<ofVec4f>(value[3]);
  return ofMatrix4x4(row0.x, row0.y, row0.z, row0.w,
                     row1.x, row1.y, row1.z, row1.w,
                     row2.x, row2.y, row2.z, row2.w,
                     row3.x, row3.y, row3.z, row3.w);
}

template<>
ofFloatColor JsonUtil::fromJson<ofFloatColor>(const ofJson& value) {
  JsonUtil::assertIsArray(value, 4);
  return ofFloatColor(value[0], value[1], value[2], value[3]);
}

void JsonUtil::mergeInto(ofJson &targetObj, const ofJson &sourceObj) {
  for (auto iter = sourceObj.begin(); iter != sourceObj.end(); ++iter) {
    targetObj[iter.key()] = iter.value();
  }
}

ofJson JsonUtil::merge(const ofJson &obj1, const ofJson &obj2) {
  ofJson out(obj1);
  JsonUtil::mergeInto(out, obj2);
  return out;
}
