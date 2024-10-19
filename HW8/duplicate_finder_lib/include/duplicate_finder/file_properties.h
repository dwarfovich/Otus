#pragma once

#include <filesystem>
#include <cassert>

struct FileProperties
{
public:
    FileProperties(const std::filesystem::path& path, std::uintmax_t size) : path_ { path }, size_ { size } {}

    const std::filesystem::path& path() const noexcept { return path_; }
    std::uintmax_t               size() const noexcept { return size_; }

    bool operator==(const FileProperties& rhs) const noexcept
    {
        assert(path_ == rhs.path_ ? size_ == rhs.size_ : true);
        return path_ == rhs.path_;
    }

private:
    std::filesystem::path path_;
    std::uintmax_t        size_ = 0 ;
};