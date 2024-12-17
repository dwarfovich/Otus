#pragma once

#include "sokoban_core/mod.hpp"

#include <boost/filesystem.hpp>
#include <boost/dll/import.hpp>

#include <memory>
#include <filesystem>

class ModDynamicLibrary
{
private: // types
    using ModCreatorFunction = std::unique_ptr<sokoban::Mod>();

public:
    ModDynamicLibrary(const std::filesystem::path& modFolderPath, const std::string& modFileName)
        : library_ { boost::filesystem::path { modFolderPath } / modFileName, boost::dll::load_mode::append_decorations }
        , mod_ { library_.get_alias<ModCreatorFunction>(methodName_)() }
        , modFolderPath_ { modFolderPath } 
    {

    }

    const std::filesystem::path&  modFolderPath() const { return modFolderPath_;}
    sokoban::Mod& mod() { return *mod_; }

private: // data
    inline static const std::string methodName_ = "createMod";

    boost::dll::shared_library    library_;
    std::unique_ptr<sokoban::Mod> mod_ = nullptr;
    std::filesystem::path modFolderPath_;
};

ModDynamicLibrary loadModDynamicLibrary(const std::filesystem::path& modFolderPath)
{
    static const std::string modFileName = "mod";
    
    return ModDynamicLibrary { modFolderPath, modFileName };
}