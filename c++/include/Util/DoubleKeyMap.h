#pragma once
#include "containers_util.h"
#include "structures.h"
#include "KBDebug/Debug.h"

namespace Kuribrawl {
    template <typename P, typename T1, typename T2>
    concept PairOf = std::is_base_of<std::pair<T1, T2>, P>::value;

    template <template <typename T1, typename T2> typename P, typename T1, typename T2>
    concept RefPairOf = requires (P<T1, T2> p) {
        {p.first} -> std::same_as<T1&>;
        {p.second} -> std::same_as<T2&>;
    };

    template <typename K1, typename K2, typename V, typename _Key, template <typename T1, typename T2> typename _Refpair>
    concept ValidDoubleKeyMapTypeArguments = 
        PairOf<_Key, K1, K2> && RefPairOf<_Refpair, K1, K2>;

    template<typename K1, typename K2, typename V, typename _Key, template <typename Tr1, typename Tr2> typename _Refpair>
        requires ValidDoubleKeyMapTypeArguments<K1, K2, V, _Key, _Refpair>
    class __DoubleKeyMap : public std::map<_Key, V, std::less<>>{
        using base_type = std::map<_Key, V, std::less<>>;

        public:
        //using ref_key = std::pair<std::reference_wrapper<K>, std::reference_wrapper<K>>;
        using ref_key = _Refpair<const K1, const K2>;
        //using const_ref_key = refbipair<const K>;

        using base_type::map;

        template <class C>
        __DoubleKeyMap(std::initializer_list<C> init){
            Debug::log("Construcion");
        }

        ref_key make_key_ref(const K1& k1, const K2& k2){
            return ref_key(k1, k2);
        }

        template<class... Args>
        void try_emplace(const K1& k1, const K2& k2, Args&&... args){
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

        private:
        template <class Iterator>
        struct __tuple_iterator : Iterator{
                using base_it = Iterator;
                auto operator*(){
                    const _Key& k = (*this)->first;
                    const V& v = (*this)->second;
                    return std::tuple<const K1&, const K2&, const V&>(k.first, k.second, v);
                }
        };

        public:
        using tuple_iterator = __tuple_iterator<typename base_type::iterator>;
        using const_tuple_iterator = __tuple_iterator<typename base_type::const_iterator>;

        tuple_iterator tbegin(){
            return (tuple_iterator)base_type::begin();
        }
        
        const_tuple_iterator tbegin() const {
            return (const_tuple_iterator)base_type::begin();
        }

        struct DoubleKeyRange;

        DoubleKeyRange& trange() {
            return (DoubleKeyRange&)(*this);
        }

        const DoubleKeyRange& trange() const {
            return (const DoubleKeyRange&)(*this);
        }
    };

    template <typename K1, typename K2, typename V, typename _Key, template <typename T1, typename T2> typename _Refpair> 
    struct __DoubleKeyMap<K1, K2, V, _Key, _Refpair>::DoubleKeyRange : public __DoubleKeyMap<K1, K2, V, _Key, _Refpair>{
        auto begin(){
            return tbegin();
        }

        const auto begin() const {
            return tbegin();
        }
    };

    template<typename K1, typename K2, typename V, typename _Key = std::pair<K1, K2>, template <typename Tr1, typename Tr2> typename _Refpair = refpair_std>
    class DoubleKeyMap;

    template<typename K1, typename K2, typename V, typename _Key, template <typename Tr1, typename Tr2> typename _Refpair>
        requires(!(std::is_reference_v<K1> && std::is_reference_v<K2>))
    class DoubleKeyMap<K1, K2, V, _Key, _Refpair> : public __DoubleKeyMap<K1, K2, V, _Key, _Refpair> {
        template<class... Args>
        void try_emplace(K1&& k1, K2&& k2, Args&&... args){
            try_emplace(_Key(k1, k2), args...);
        }
    };

    template<typename K1, typename K2, typename V, typename _Key, template <typename Tr1, typename Tr2> typename _Refpair>
        requires(std::is_reference_v<K1> || std::is_reference_v<K2>)
    class DoubleKeyMap<K1, K2, V, _Key, _Refpair> : public __DoubleKeyMap<K1, K2, V, _Key, _Refpair>
    {};

    template <typename K, typename V>
    using DoubleKeyMapST = DoubleKeyMap<K, K, V>;

    template <typename K1, typename K2, typename V>
    using DynamicMatrix = DoubleKeyMap<K1, K2, V>;

    template <typename K, typename V>
    using DynamicMatrixST = DoubleKeyMapST<K, V>;

}

