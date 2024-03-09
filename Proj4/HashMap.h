//
//  HashMap.hpp
//  Proj4
//
//  Created by Nathan Wei on 3/7/24.
//

#ifndef HashMap_hpp
#define HashMap_hpp
#include <iostream>
#include <vector>
#include <list>

template <typename T>
class HashMap
{
public:
 HashMap(double max_load = 0.75) : maxLoadFactor(max_load), numItems(0), numBuckets(10)
 {
     buckets.resize(numBuckets);
 }
// constructor
 ~HashMap() {}
// destructor; deletes all of the items in the hashmap
 int size() const
 {
     return numItems;
 }
// return the number of associations in the hashmap
 // The insert method associates one item (key) with another (value).
 // If no association currently exists with that key, this method inserts
 // a new association into the hashmap with that key/value pair. If there is
 // already an association with that key in the hashmap, then the item
 // associated with that key is replaced by the second parameter (value).
 // Thus, the hashmap must contain no duplicate keys.
 void insert(const std::string& key, const T& value)
 {
     size_t index = std::hash<std::string>()(key) % numBuckets;
     auto& bucket = buckets[index];
     auto it = bucket.begin();
     for (; it != bucket.end(); it++)
     {
         if (it->key == key)
         {
             it->value = value;
             return;
         }
     }
     bucket.push_back({key, value});
     numItems++;
     double loadFactor = static_cast<double>(numItems) /numBuckets;
     if (loadFactor > maxLoadFactor)
         rehash();
 }
 // Defines the bracket operator for HashMap, so you can use your map like this:
 // your_map["david"] = 2.99;
 // If the key does not exist in the hashmap, this will create a new entry in
 // the hashmap and map it to the default value of type T (0 for builtin types).
 // It returns a reference to the newly created value in the map.
 T& operator[](const std::string& key)
 {
     T* p = find(key);
     if (p == nullptr)
     {
         insert(key, T());
         p = find(key);
     }
     return *p;
 }
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value within the map.
 const T* find(const std::string& key) const
{
    size_t index = std::hash<std::string>()(key) % numBuckets;
    auto& bucket = buckets[index];
    if (bucket.size() == 0)
        return nullptr;
    else
    {
        for (auto it = bucket.begin(); it != bucket.end(); it++)
        {
            if (it->key == key)
                return &it->value;
        }
    }
    return nullptr;
}
 // If no association exists with the given key, return nullptr; otherwise,
 // return a pointer to the value associated with that key. This pointer can be
 // used to examine that value or modify it directly within the map.
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
private:
    struct KeyValuePair {
        std::string key;
        T value;
        KeyValuePair(const std::string& k, const T& v) : key(k), value(v) {}
    };

    std::vector<std::list<KeyValuePair>> buckets;
    int numItems;
    int numBuckets;
    double maxLoadFactor;
    void rehash() {
        int newSize = numBuckets * 2;
        numBuckets = newSize;
        std::vector<std::list<KeyValuePair>> newBuckets(newSize);
        for (const auto& bucket : buckets)
        {
             for (const auto& kvPair : bucket)
             {
                 int i = std::hash<std::string>()(kvPair.key) % newSize;
                 newBuckets[i].push_back(kvPair);
             }
         }
         buckets = std::move(newBuckets);
     }
};
#endif /* HashMap_hpp */
