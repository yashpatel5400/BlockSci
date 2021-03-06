//
//  cluster_manager.hpp
//  blocksci
//
//  Created by Harry Kalodner on 7/6/17.
//
//

#ifndef cluster_manager_hpp
#define cluster_manager_hpp

#include "cluster.hpp"

#include <blocksci/file_mapper.hpp>
#include <blocksci/address/address_info.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/range/counting_range.hpp>

#include <stdio.h>

class Cluster;

template<blocksci::AddressType::Enum type>
struct ScriptClusterIndexFile : public blocksci::FixedSizeFileMapper<uint32_t> {
    using blocksci::FixedSizeFileMapper<uint32_t>::FixedSizeFileMapper;
};

class ClusterExpander;

struct TaggedCluster {
    Cluster cluster;
    std::vector<TaggedAddress> taggedAddresses;
    
    TaggedCluster(const Cluster &cluster_, std::vector<TaggedAddress> &&taggedAddresses_) : cluster(cluster_), taggedAddresses(taggedAddresses_) {}
};

class ClusterManager {
    blocksci::FixedSizeFileMapper<uint32_t> clusterOffsetFile;
    blocksci::FixedSizeFileMapper<blocksci::Address> clusterAddressesFile;
    
    using ScriptClusterIndexTuple = blocksci::internal::to_script_type<ScriptClusterIndexFile, blocksci::AddressInfoList>::type;
    
    ScriptClusterIndexTuple scriptClusterIndexFiles;
    
public:
    ClusterManager(std::string baseDirectory);
    
    Cluster getCluster(const blocksci::Address &address) const;
    
    template<blocksci::AddressType::Enum type>
    uint32_t getClusterNum(uint32_t addressNum) const {
        auto &file = std::get<ScriptClusterIndexFile<type>>(scriptClusterIndexFiles);
        return *file.getData(addressNum);
    }
    
    uint32_t getClusterNum(const blocksci::Address &address) const;
    
    uint32_t getClusterSize(uint32_t clusterNum) const;
    boost::iterator_range<const blocksci::Address *> getClusterAddresses(uint32_t clusterNum) const;
    
    uint32_t clusterCount() const;
    
    boost::transformed_range<ClusterExpander, const boost::iterator_range<boost::iterators::counting_iterator<unsigned int>>> getClusters() const;
    
    std::vector<uint32_t> getClusterSizes() const;
    
    std::vector<TaggedCluster> taggedClusters(const std::unordered_map<blocksci::Address, std::string> &tags);
};

class ClusterExpander
{
public:
    typedef const Cluster result_type;
    
    const ClusterManager &manager;
    ClusterExpander(const ClusterManager &manager_) : manager(manager_) {}
    
    result_type operator()(uint32_t clusterNum) const {
        return Cluster(clusterNum, manager);
    }
};

#endif /* cluster_manager_hpp */
