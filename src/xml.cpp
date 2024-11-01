#include "badgecpp/xml.hpp"
#include <cstddef>
#include <sstream>

namespace badge {
    Xml::Xml(std::string name, Attrs attr, Contents content) : name_(std::move(name)), attr_(std::move(attr)), content_(std::move(content)) {}
    Xml &Xml::setName(std::string name) {
        name_ = std::move(name);
        return *this;
    }
    Xml &Xml::addAttr(std::string key, std::string value) {
        attr_.emplace_back(std::move(key), std::move(value));
        return *this;
    }
    void Xml::render(std::ostream &os) const {
        const bool hasTag = !name_.empty();
        if (hasTag) {
            os << '<' << name_;
            for (const auto &[key, value]: attr_) {
                os << ' ' << key << "=\"";
                escape_xml(os, value);
                os << '"';
            }
        }
        if (isSubEmpty()) {
            if (hasTag) os << "/>";
        } else {
            if (hasTag) os << '>';
            for (size_t i = 0; i < content_.size(); ++i) {
                if (std::holds_alternative<std::string>(content_[i])) {
                    escape_xml(os, std::get<std::string>(content_[i]));
                } else {
                    std::get<0>(content_[i])->render(os);
                }
            }
            if (hasTag) os << "</" << name_ << '>';
        }
    }
    std::string Xml::render() const {
        std::ostringstream oss;
        render(oss);
        return oss.str();
    }

    bool Xml::isEmpty() const {
        if (!name_.empty()) return false;
        return isSubEmpty();
    }
    bool Xml::isSubEmpty() const {
        if (content_.empty()) return true;
        for (const auto &item: content_) {
            if (std::holds_alternative<std::string>(item)) {
                if (!std::get<std::string>(item).empty()) return false;
            } else {
                if (!std::get<0>(item)->isEmpty()) return false;
            }
        }
        return true;
    }

    void Xml::escape_xml(std::ostream &os, const std::string &str) {
        for (const char &ch: str) {
            switch (ch) {
                    // clang-format off
                    case '&':  os << "&amp;";  break;
                    case '<':  os << "&lt;";   break;
                    case '>':  os << "&gt;";   break;
                    case '"':  os << "&quot;"; break;
                    case '\'': os << "&apos;"; break;
                    default:   os << ch;       break;
                    // clang-format on
            }
        }
    }
}// namespace badge