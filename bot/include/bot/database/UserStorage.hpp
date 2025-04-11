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

template<typename K, typename V> 
std::optional<V> Storage<K, V>::get(K key)
{
    if (auto res = storage.find(key); res != storage.end())
        return storage.at(key);
    return std::nullopt;
}


template<typename K, typename V> 
void Storage<K, V>::set(K key, V value)
{
    if (auto res = storage.find(key); res != storage.end())
    {
        storage[key] = value;
        return;
    }
        
    if (storage.size() > 100)
        storage.clear();

    storage.insert({key, value});
}

#endif