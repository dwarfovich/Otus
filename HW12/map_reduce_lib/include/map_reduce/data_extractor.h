#pragma once

#include <vector>
#include <string>
#include <optional>

inline static constexpr unsigned priceColumnNumber = 9;

inline std::optional<std::size_t> extractUll(const std::string& input, unsigned number)
{
    if (input.empty()) {
        return {};
    }

    std::size_t pos = 0;
    for (unsigned i = 0; i < number && pos != std::string::npos; ++i) {
        pos = input.find(',', pos + 1);
    }

    try {
        if (pos == 0 && number == 0) {
            return std::stoull(input);
        }
        if (pos == std::string::npos) {
            return {};
        }
        auto nextCommaPos = input.find(',', pos + 1);

        return std::stoull(input.substr(pos + 1, nextCommaPos - pos));
    } catch (const std::exception&) {
        return {};
    }
}
