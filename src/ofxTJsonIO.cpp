//
//  ofxTJsonIO.cpp
//

#include "ofxTJsonIO.h"

#include <ofLog.h>
#include <ofUtils.h>
#include <ofTypes.h>
#include <stdexcept>

using namespace ofxTCommon;

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
  ofSavePrettyJson(filepath, obj);
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
