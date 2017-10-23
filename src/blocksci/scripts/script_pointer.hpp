//
//  script_pointer.hpp
//  blocksci
//
//  Created by Harry Kalodner on 9/19/17.
//
//

#ifndef script_pointer_hpp
#define script_pointer_hpp

#include "script_type.hpp"
#include "script_variant.hpp"
#include "scriptsfwd.hpp"

#include <memory>

namespace blocksci {
    struct Address;
    
    
    struct ScriptPointer {
        uint32_t scriptNum;
        ScriptType::Enum type;
        
        ScriptPointer() {}
        ScriptPointer(uint32_t scriptNum_, ScriptType::Enum type_) : scriptNum(scriptNum_), type(type_) {}
        ScriptPointer(const Address &address);
        
        bool operator==(const ScriptPointer &other) const {
            return scriptNum == other.scriptNum && type == other.type;
        }
        
        std::string toString() const;
        
        ScriptVariant getScript(const ScriptAccess &access) const;
        
        #ifndef BLOCKSCI_WITHOUT_SINGLETON
        ScriptVariant getScript() const;
        #endif
    };
    
    std::ostream &operator<<(std::ostream &os, const ScriptPointer &scriptPointer);
}

namespace std {
    template <>
    struct hash<blocksci::ScriptPointer> {
        typedef blocksci::ScriptPointer argument_type;
        typedef size_t  result_type;
        result_type operator()(const argument_type &b) const {
            return (static_cast<size_t>(b.scriptNum) << 32) + static_cast<size_t>(b.type);
        }
    };
}


#endif /* script_pointer_hpp */