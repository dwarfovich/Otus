#pragma once

#include <filesystem>

struct FileProperties
{
public:
    FileProperties(const std::filesystem::path& path, std::uintmax_t size) : path_{path}, size_{size}{}

    const std::filesystem::path& path() const noexcept {return path_;}
    uintmax_t                    size() const noexcept { return size_ ;}

    bool operator==(const FileProperties& rhs) const noexcept {
        // TODO: implement.
        return true;
    }
    private:
    std::filesystem::path path_;
    std::uintmax_t   size_ {0};
};