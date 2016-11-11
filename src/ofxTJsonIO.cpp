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
ofJson JsonUtil::toJson(const ofFloatColor& value) {
  return {value.r, value.g, value.b, value.a};
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
