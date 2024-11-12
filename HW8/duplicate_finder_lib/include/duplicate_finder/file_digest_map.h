#pragma once

#include "digester.h"
#include "file_properties_vector.h"
#include "file_properties_hasher.h"
#include "file_properties_iter_comparator.h"

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include <stdexcept>

class FileDigestMap
{
public:
    using FilesConstIterator = std::vector<FileProperties>::const_iterator;

    void setDigester(const std::shared_ptr<Digester>& digester){
        //std::lock_guard lock { mutex_ };

        digester_ = digester;
    }

    void setFiles(std::vector<FileProperties>&& files)
    {
        //std::lock_guard lock { mutex_ };

        files_ = std::move(files);
        populateFileMap();
    }

    bool find(const file_properties::ConstIterator& inputIter) { 

        //std::shared_lock lock {mutex_};

        auto iter = fileMap_.find(inputIter->size());
        if(iter == fileMap_.cend()){
            throw std::logic_error("Cann't find iter for size " + std::to_string(inputIter->size()));
        }
        auto setIter = iter->second.find(inputIter);

    }

private: // methods
    void populateFileMap()
    {
        fileMap_.clear();
        for (auto iter = files_.cbegin(); iter != files_.cend(); ++iter) {
            fileMap_[iter->size()].insert(iter);
        }
    }

private: // data
    std::vector<FileProperties> files_;

    using FileSet =
        std::unordered_multiset<FilesConstIterator, FilePropertiesIterHasher, FilePropertiesIterComparator>;
    std::unordered_map<std::uintmax_t, FileSet> fileMap_;
    std::shared_ptr<Digester>                   digester_;
};