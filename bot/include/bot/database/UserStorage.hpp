#ifndef USER_STORAGE_HPP
#define USER_STORAGE_HPP

#include <optional>
#include <unordered_map>

template<typename K, typename V>
struct Storage
{
    std::unordered_map<K, V> storage;
    std::optional<V> get(K key);
    void set(K key, V value);
};

#endif