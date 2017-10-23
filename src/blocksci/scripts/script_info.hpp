//
//  script_info.hpp
//  blocksci
//
//  Created by Harry Kalodner on 9/6/17.
//
//

#ifndef script_info_hpp
#define script_info_hpp

#include "script_type.hpp"
#include <blocksci/util.hpp>

#include <boost/variant/variant_fwd.hpp>

#include <string_view>

namespace blocksci {
    using namespace std::string_view_literals;
    
    struct PubkeyData;
    struct ScriptHashData;
    struct MultisigData;
    struct NonstandardScriptData;
    struct RawData;
    
    template <typename T>
    struct FixedSize;
    
    template <typename ...T>
    struct Indexed;
    
    template <ScriptType::Enum>
    struct ScriptInfo;
    
    template <>
    struct ScriptInfo<ScriptType::Enum::PUBKEY> {
        static constexpr std::string_view name = "pubkey_script"sv;
        static constexpr bool deduped = true;
        static constexpr bool spendable = true;
        using storage = FixedSize<PubkeyData>;
    };
    
    template <>
    struct ScriptInfo<ScriptType::Enum::SCRIPTHASH> {
        static constexpr std::string_view name = "scripthash_script"sv;
        static constexpr bool deduped = true;
        static constexpr bool spendable = true;
        using storage = FixedSize<ScriptHashData>;
    };
    
    template <>
    struct ScriptInfo<ScriptType::Enum::MULTISIG> {
        static constexpr std::string_view name = "multisig_script"sv;
        static constexpr bool deduped = true;
        static constexpr bool spendable = true;
        using storage = Indexed<MultisigData>;
    };
    
    template <>
    struct ScriptInfo<ScriptType::Enum::NONSTANDARD> {
        static constexpr std::string_view name = "nonstandard_script"sv;
        static constexpr bool deduped = false;
        static constexpr bool spendable = true;
        using storage = Indexed<NonstandardScriptData,NonstandardScriptData>;
    };
    
    template <>
    struct ScriptInfo<ScriptType::Enum::NULL_DATA> {
        static constexpr std::string_view name = "null_data_script"sv;
        static constexpr bool deduped = false;
        static constexpr bool spendable = false;
        using storage = Indexed<RawData>;
    };
    using ScriptInfoList = array_to_tuple_t<ScriptType::Enum, ScriptType::size, ScriptType::all>;
    
    template <template<auto> class K>
    using to_script_tuple_t = apply_template_t<K, ScriptInfoList>;
    
    template <template<auto> class K>
    using to_script_variant_t = to_variadic_t<to_script_tuple_t<K>, boost::variant>;
    
    template<ScriptType::Enum type>
    struct SpendableFunctor {
        static constexpr bool f() {
            return ScriptInfo<type>::spendable;
        }
    };
    
    static constexpr auto spendableTable = blocksci::make_static_table<ScriptType, SpendableFunctor>();
    
    constexpr bool isSpendable(ScriptType::Enum t) {
        
        auto index = static_cast<size_t>(t);
        if (index >= ScriptType::size)
        {
            throw std::invalid_argument("combination of enum values is not valid");
        }
        return spendableTable[index];
    }
    
    template<ScriptType::Enum type>
    struct DedupedFunctor {
        static constexpr bool f() {
            return ScriptInfo<type>::deduped;
        }
    };
    
    static constexpr auto dedupedTable = blocksci::make_static_table<ScriptType, DedupedFunctor>();
    
    constexpr bool isDeduped(ScriptType::Enum t) {
        auto index = static_cast<size_t>(t);
        if (index >= ScriptType::size)
        {
            throw std::invalid_argument("combination of enum values is not valid");
        }
        return dedupedTable[index];
    }
    
    template<ScriptType::Enum type>
    struct ScriptNameFunctor {
        constexpr static std::string_view f() {
            return ScriptInfo<type>::name;
        }
    };
    
    static constexpr auto scriptNameTable = make_static_table<ScriptType, ScriptNameFunctor>();
    
    constexpr std::string_view scriptName(ScriptType::Enum type) {
        auto index = static_cast<size_t>(type);
        if (index >= ScriptType::size)
        {
            throw std::invalid_argument("combination of enum values is not valid");
        }
        return scriptNameTable[index];
    }
}

#endif /* script_info_hpp */