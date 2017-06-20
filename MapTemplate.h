/********************************************************************

	Filename: 	MAPTEMPLATE.H

	Description:MAPTEMPLATE

	Version:	1.0
	Created:	3:3:2014   11:09
	Revison:	none
	Compiler:	gcc vc

	Author:		(wufan), love19862003@163.com

	Organization:
*********************************************************************/
#ifndef __MapTemplate_h__
#define __MapTemplate_h__
#include <map>
#include <memory>
#include <unordered_map>
#include <functional>
#include <algorithm>
namespace ShareSpace {
  namespace Utility {

    template<typename Key, typename Value >
    class ObjectMap {
    public:
      typedef Key       key_type;
      typedef Value     value_type;
      typedef typename std::pair<key_type, value_type> pair_type;
      typedef typename std::map<key_type, value_type> map_type;
      typedef typename map_type::iterator iterator_type;
      typedef typename map_type::const_iterator const_iterator_type;

      void setOptional(const key_type& k, const value_type& v) {
        optional_key = k;
        optional_value = v;
      }
      bool isOptionalKey(const key_type& k) {
        return optional_key == k;
      }
      bool isOptionalValue(const value_type& v)  {
        return optional_value == v;
      }

      bool addData(const key_type& key, const value_type& v) {
        return m_map.insert(std::pair<key_type, value_type>(key, v)).second;
      }
      void replaceData(const key_type& key, const value_type& v) {
        m_map[key] = v;
      }
      bool hasData(const key_type& key) const {
        return m_map.count(key) > 0;
      }
      const key_type& getKey(const value_type& v) const {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp){
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second )){
            return  it->first;
          }
        }
        return optional_key;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp) const{
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }

     const value_type& getData(const std::function<bool(const pair_type&)>& fun) const{
        const_iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return optional_value;
      }

      value_type& getData(const std::function<bool(const pair_type&)>& fun){
        iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
         return optional_value;
      }

      const value_type& getData(const key_type& key) const {
        const_iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return optional_value;
      }
      value_type& getData(const key_type& key) {
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return optional_value;
      }

      void eraseData(const key_type& key) {
        m_map.erase(key);
      }

      size_t eraseData(const std::function<bool(const pair_type&)>& fun){
        size_t count = 0;
        iterator_type it = m_map.begin();
        for (it ; it !=  m_map.end();){
          if (fun(*it)){
            it = m_map.erase(it);
            ++count;
          }else{
            ++it;
          }
        }
        return count;
      }

      size_t size() {
        return m_map.size();
      }
      void clear() {
        return m_map.clear();
      }
      void forEach(const std::function<void(const pair_type&)> fun) {
        for(auto& pair : m_map) { fun(pair); }
      }
      void forEach(const std::function<void(const pair_type&)> fun) const{
        for(auto& pair : m_map){ fun(pair); }
      }

    private:
      value_type optional_value;
      key_type optional_key;
      map_type m_map;

    };


    template<typename Key, typename Value>
    class ObjPtrMap {
    public:
      typedef Key key_type;
      typedef typename std::shared_ptr<Value> value_type;
      typedef typename std::pair<key_type, value_type> pair_type;
      typedef typename std::map<key_type, value_type> map_type;
      typedef typename map_type::iterator iterator_type;
      typedef typename map_type::const_iterator const_iterator_type;

      void setOptional(const key_type& k) {
        optional_key = k;
      }

      bool isOptionalKey(const key_type& k) {
        return optional_key == k;
      }

      bool addData(const key_type& key, const value_type& v) {
        return m_map.insert(std::make_pair(key, v)).second;
      }
      void replaceData(const key_type& key, const value_type& v) {
        m_map[key] = v;
      }
      bool hasData(const key_type& key) {
        return m_map.count(key) > 0;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp) const{
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp){
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) const {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      value_type getData(const key_type& key) {
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return value_type(nullptr);
      }
      value_type getData(const key_type& key)const {
        const_iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type getData(const std::function<bool(const pair_type&)>& fun) const{
        const_iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type getData(const std::function<bool(const pair_type&)>& fun){
        iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if (it != m_map.end()){
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type eraseData(const key_type& key) {
        value_type ptr(nullptr);
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          ptr = it->second;
          m_map.erase(it);
        }
        return ptr;
      }
      size_t eraseData(const std::function<bool(const pair_type&)>& fun){
        size_t count = 0;
        iterator_type it = m_map.begin();
        for(it; it != m_map.end();){
          if(fun(*it)){
            it = m_map.erase(it);
            ++count;
          } else{
            ++it;
          }
        }
        return count;
      }
      size_t size() const{
        return m_map.size();
      }
      void clear() {
        return m_map.clear();
      }

      void forEach(const std::function<void(const pair_type&)> fun) {
        for(auto& pair : m_map) { fun(pair); }
      }
      void forEach(const std::function<void(const pair_type&)> fun) const{
        for(auto& pair : m_map){ fun(pair); }
      }
    private:
      key_type optional_key;
      map_type m_map;
    };

    template<typename Key, typename Value >
    class ObjectHashMap {
    public:
      typedef Key       key_type;
      typedef Value     value_type;
      typedef typename std::pair<key_type, value_type> pair_type;
      typedef typename std::unordered_map<key_type, value_type> map_type;
      typedef typename map_type::iterator iterator_type;
      typedef typename map_type::const_iterator const_iterator_type;

      void setOptional(const key_type& k, const value_type& v) {
        optional_key = k;
        optional_value = v;
      }
      bool isOptionalKey(const key_type& k) {
        return optional_key == k;
      }
      bool isOptionalValue(const value_type& v) {
        return optional_value == v;
      }

      bool addData(const key_type& key, const value_type& v) {
        return m_map.insert(std::pair<key_type, value_type>(key, v)).second;
      }
      void replaceData(const key_type& key, const value_type& v) {
        m_map[key] = v;
      }
      bool hasData(const key_type& key) const {
        return m_map.count(key) > 0;
      }
      const key_type& getKey(const std::function<bool(const value_type&)>& comp) const{
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) const {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }

      const value_type& getData(const key_type& key) const {
        const_iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return optional_value;
      }
      value_type& getData(const key_type& key) {
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return optional_value;
      }

      const value_type& getData(const std::function<bool(const pair_type&)>& fun) const{
        const_iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return optional_value;
      }

      value_type& getData(const std::function<bool(const pair_type&)>& fun){
        iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return optional_value;
      }

      void eraseData(const key_type& key) {
        m_map.erase(key);
      }
      size_t eraseData(const std::function<bool(const pair_type&)>& fun){
        size_t count = 0;
        iterator_type it = m_map.begin();
        for(it; it != m_map.end();){
          if(fun(*it)){
            it = m_map.erase(it);
            ++count;
          } else{
            ++it;
          }
        }
        return count;
      }
      size_t size() {
        return m_map.size();
      }
      void clear() {
        return m_map.clear();
      }

      void forEach(const std::function<void(const pair_type&)> fun) {
        for(auto& pair : m_map) { fun(pair); }
      }

      void forEach(const std::function<void(const pair_type&)> fun) const{
        for(auto& pair : m_map){ fun(pair); }
      }

    private:
      value_type optional_value;
      key_type optional_key;
      map_type m_map;

    };

    template<typename Key, typename Value>
    class ObjPtrHashMap {
    public:
      typedef Key key_type;
      typedef typename std::shared_ptr<Value> value_type;
      typedef typename std::pair<key_type, value_type> pair_type;
      typedef typename std::unordered_map<key_type, value_type> map_type;
      typedef typename map_type::iterator iterator_type;
      typedef typename map_type::const_iterator const_iterator_type;

      void setOptional(const key_type& k) {
        optional_key = k;
      }

      bool isOptionalKey(const key_type& k) {
        return optional_key == k;
      }

      bool addData(const key_type& key, const value_type& v) {
        return m_map.insert(std::make_pair(key, v)).second;
      }
      void replaceData(const key_type& key, const value_type& v) {
        m_map[key] = v;
      }
      bool hasData(const key_type& key) {
        return m_map.count(key) > 0;
      }
      const key_type& getKey(const std::function<bool(const value_type&)>& comp) const{
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }

      const key_type& getKey(const std::function<bool(const value_type&)>& comp) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it){
          if(comp(it->second)){
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) const {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      const key_type& getKey(const value_type& v) {
        for(auto it = m_map.begin(); it != m_map.end(); ++it) {
          if(it->second == v) {
            return  it->first;
          }
        }
        return optional_key;
      }
      value_type getData(const key_type& key) {
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return value_type(nullptr);
      }
      value_type getData(const key_type& key) const{
        const_iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type getData(const std::function<bool(const pair_type&)>& fun) const{
        const_iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type getData(const std::function<bool(const pair_type&)>& fun){
        iterator_type it = std::find_if(m_map.begin(), m_map.end(), fun);
        if(it != m_map.end()){
          return it->second;
        }
        return value_type(nullptr);
      }

      value_type eraseData(const key_type& key) {
        value_type ptr(nullptr);
        iterator_type it = m_map.find(key);
        if(it != m_map.end()) {
          ptr = it->second;
          m_map.erase(it);
        }
        return ptr;
      }
      size_t eraseData(const std::function<bool(const pair_type&)>& fun){
        size_t count = 0;
        iterator_type it = m_map.begin();
        for(it; it != m_map.end();){
          if(fun(*it)){
            it = m_map.erase(it);
            ++count;
          } else{
            ++it;
          }
        }
        return count;
      }
      size_t size() {
        return m_map.size();
      }
      void clear() {
        return m_map.clear();
      }

      void forEach(const std::function<void(const pair_type&)> fun) {
        for(auto& pair : m_map) { fun(pair); }
      }
      void forEach(const std::function<void(const pair_type&)> fun) const{
        for(auto& pair : m_map){ fun(pair); }
      }
    private:
      key_type optional_key;
      map_type m_map;
    };


  }
}
#endif // __MapTemplate_h__
