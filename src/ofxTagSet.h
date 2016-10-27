//
//  ofxTagSet.h
//

#pragma once

#include <initializer_list>
#include <unordered_map>
#include <unordered_set>

namespace ofxTCommon {

  template<typename T>
  class TagSet {
  public:
    using Storage = std::unordered_map<T, bool>;
    using iterator = typename Storage::iterator;
    using const_iterator = typename Storage::const_iterator;

    TagSet() {}

    template<typename Init>
    TagSet(Init init)
    : _tags(init) { }

    TagSet(std::initializer_list<T> tags) {
      enable(tags);
    }

    void setParent(const TagSet<T>* parent) {
      _parent = parent;
    }

    const TagSet<T>* getParent() const {
      return _parent;
    }

    bool operator[](const T& tag) const {
      auto iter = _tags.find(tag);
      if (iter != _tags.end()) {
        return iter->second;
      }
      return _parent && (*_parent)[tag];
    }

    void enable(const T& tag) {
      put(tag, true);
    }

    template<typename Iter>
    void enable(Iter begin, Iter end) {
      put(begin, end, true);
    }

    template<typename Src>
    void enable(Src tags) {
      enable(tags.begin(), tags.end());
    }

    void disable(const T& tag) {
      put(tag, false);
    }

    template<typename Iter>
    void disable(Iter begin, Iter end) {
      disable(begin, end, false);
    }

    template<typename Src>
    void disable(Src tags) {
      disable(tags.begin(), tags.end());
    }

    void clear(const T& tag) {
      _tags.erase(tag);
    }

    template<typename Iter>
    void clear(Iter begin, Iter end) {
      for (auto iter = begin; iter != end; iter++) {
        clear(*iter);
      }
    }

    template<typename Src>
    void clear(Src tags) {
      clear(tags.begin(), tags.end());
    }

    void clear() { _tags.clear(); }

    iterator begin() { return _tags.begin(); }
    iterator end() { return _tags.end(); }
    const_iterator begin() const { return _tags.begin(); }
    const_iterator end() const { return _tags.end(); }
    bool empty() const { return _tags.empty(); }

    void put(const T& tag, bool value) {
      _tags.insert(std::make_pair(tag, value));
    }

    template<typename Iter>
    void put(Iter begin, Iter end, bool value) {
      for (auto iter = begin; iter != end; iter++) {
        put(*iter, value);
      }
    }

    template<typename Src>
    void put(Src tags, bool value) {
      put(tags.begin(), tags.end(), value);
    }

    std::unordered_set<T> getEffectiveTags() const {
      std::unordered_set<T> results;
      updateEffectiveTags(&results);
      return results;
    }

  private:
    void updateEffectiveTags(std::unordered_set<T>* results) const {
      if (_parent) {
        _parent->updateEffectiveTags(results);
      }
      for (const auto& entry : _tags) {
        if (entry.second) {
          results->insert(entry.first);
        } else {
          results->erase(entry.first);
        }
      }
    }

    Storage _tags;
    const TagSet<T>* _parent;
  };
}

template<typename T>
using ofxTagSet = ofxTCommon::TagSet<T>;

