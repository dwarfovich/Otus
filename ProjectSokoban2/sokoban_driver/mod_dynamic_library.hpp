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
    ModDynamicLibrary(const boost::filesystem::path& modDllPath)
        : library_ { modDllPath, boost::dll::load_mode::append_decorations }
        , mod_ { library_.get_alias<ModCreatorFunction>(methodName_)() }
    {
    }

    sokoban::Mod& mod() { return *mod_; }

private: // data
    inline static const std::string methodName_ = "createMod";

    boost::dll::shared_library    library_;
    std::unique_ptr<sokoban::Mod> mod_ = nullptr;
};

ModDynamicLibrary loadModDll(const std::filesystem::path& modFolderPath)
{
    boost::filesystem::path modDllPath = modFolderPath.string() + "/mod";
    return ModDynamicLibrary { modDllPath };
}