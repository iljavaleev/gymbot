#include "bot/database/UserStorage.hpp"


template<typename K, typename V> 
std::optional<V> Storage<K, V>::get(K key)
{
    if (auto res = Storage::storage.find(key); res != storage.end())
        return Storage::storage.at(key);
    return std::nullopt;
}


template<typename K, typename V> 
void Storage<K, V>::set(K key, V value)
{
    if (auto res = Storage::storage.find(key); res != storage.end())
    {
        Storage::storage[key] = value;
        return;
    }
        
    if (Storage::storage.size() > 100)
        Storage::storage.clear();

    Storage::storage.insert({key, value});
}