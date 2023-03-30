#pragma once
#include "containers_util.h"
#include "structures.h"

namespace Kuribrawl {
    template<typename K1, typename K2, typename V, typename _Key = std::pair<K1, K2>, template <typename Tr1, typename Tr2> typename _Refpair = Kuribrawl::refpair>
    class DoubleKeyMap : public std::map<_Key, V, std::less<>>{
        using base_type = std::map<_Key, V, std::less<>>;

        public:
        //using ref_key = std::pair<std::reference_wrapper<K>, std::reference_wrapper<K>>;
        using ref_key = _Refpair<const K1, const K2>;
        //using const_ref_key = refbipair<const K>;

        _Key make_key(const K1& k1, const K2& k2){
            return _Key(k1, k2);
        }
        _Key make_key(K1&& k1, K2&& k2){
            return _Key(k1, k2);
        }
        /*
        const_ref_key make_key_ref(const K& k1, const K& k2){
            std::cout << "\nmake_key_ref\n";
            return const_ref_key(k1, k2);
        }*/
        ref_key make_key_ref(const K1& k1, const K2& k2){
            return ref_key(k1, k2);
        }

        template<class... Args>
        void try_emplace(const K1& k1, const K2& k2, Args&&... args){
            base_type::try_emplace(_Key(k1, k2), args...);
        }
        template<class... Args>
        void try_emplace(K1&& k1, K2&& k2, Args&&... args){
            base_type::try_emplace(_Key(k1, k2), args...);
        }

        base_type::iterator find(const K1& k1, const K2& k2){
            return base_type::find(make_key_ref(k1, k2));
        }

        base_type::const_iterator find(const K1& k1, const K2& k2) const {
            return base_type::find(make_key_ref(k1, k2));
        }

        template <typename T1, typename T2>
        base_type::iterator find(const T1& k1, const T2& k2){
            return base_type::find(_Refpair(k1, k2));
        }

        V get(const K1& k1, const K2& k2) const {
            auto it = base_type::find(make_key_ref(k1, k2));
            if (it != base_type::end()){
                return it->second;
            } 
            return V{};
        }

        template <typename T1, typename T2>
        V get(const T1& k1, const T2& k2){
            auto it = base_type::find(_Refpair(k1, k2));
            if (it != base_type::end()){
                return it->second;
            } 
            return V{};
        }
    };

    template <typename K, typename V>
    using DoubleKeyMapST = DoubleKeyMap<K, K, V>;

    template <typename K1, typename K2, typename V>
    using DynamicMatrix = DoubleKeyMap<K1, K2, V>;

    template <typename K, typename V>
    using DynamicMatrixST = DoubleKeyMapST<K, V>;

}

