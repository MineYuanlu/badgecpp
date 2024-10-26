/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_XML__HPP_GUARD
#define BADGECPP_XML__HPP_GUARD
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
namespace badge {

    /// @brief XML节点
    class Xml {
    public:
        /// @brief 节点名称类型
        using Name = std::string;
        /// @brief 属性类型
        using Attr = std::pair<std::string, std::string>;
        /// @brief 属性集合
        using Attrs = std::vector<Attr>;
        /// @brief 节点内容类型
        using Content = std::variant<std::shared_ptr<Xml>, std::string>;
        /// @brief 节点内容集合
        using Contents = std::vector<Content>;

    private:
        std::string name_{};///< 节点名称
        Attrs attr_{};      ///< 节点属性
        Contents content_{};///< 节点内容


    public:
        /// @brief 空构造, 将不会被渲染进最终结果
        Xml() = default;
        /// @brief 全指定构造
        /// @param name 节点名称, 为空代表这是一个list, 本身不会渲染, 只渲染内容
        Xml(std::string name, Attrs attr, Contents content);

        /// @brief 全指定构造
        /// @param name 节点名称, 为空代表这是一个list, 本身不会渲染, 只渲染内容
        //// @param content 节点内容，可以是string、Xml节点、shared_ptr<Xml>
        template<typename... Subs, typename std::enable_if_t<std::conjunction_v<std::disjunction<
                                                                     std::is_convertible<std::decay_t<Subs>, std::string>,
                                                                     std::is_convertible<std::decay_t<Subs>, std::shared_ptr<Xml>>,
                                                                     std::is_convertible<std::decay_t<Subs>, Xml>>...>,
                                                             int> = 0>
        Xml(std::string name, Attrs attr, Subs... content) : name_(std::move(name)), attr_(std::move(attr)) {
            content_.reserve(sizeof...(content));
            ((content_.emplace_back(make_sub(std::forward<Subs>(content)))), ...);
        }

        /// @brief 无属性构造
        /// @param name 节点名称, 为空代表这是一个list, 本身不会渲染, 只渲染内容
        //// @param content 节点内容，可以是string、Xml节点、shared_ptr<Xml>
        template<typename... Subs, typename std::enable_if_t<std::conjunction_v<std::disjunction<
                                                                     std::is_convertible<std::decay_t<Subs>, std::string>,
                                                                     std::is_convertible<std::decay_t<Subs>, std::shared_ptr<Xml>>,
                                                                     std::is_convertible<std::decay_t<Subs>, Xml>>...>,
                                                             int> = 0>
        Xml(std::string name, Subs... content) : name_(std::move(name)) {
            content_.reserve(sizeof...(content));
            ((content_.emplace_back(make_sub(std::forward<Subs>(content)))), ...);
        }


        /// @brief 设置节点名称, 为空代表这是一个list, 本身不会渲染, 只渲染内容
        Xml &setName(std::string name);


        /// @brief 添加一条属性
        Xml &addAttr(std::string key, std::string value);


        /// @brief 添加一个节点内容
        template<typename Subs, typename std::enable_if_t<std::disjunction_v<
                                                                  std::is_convertible<std::decay_t<Subs>, std::string>,
                                                                  std::is_convertible<std::decay_t<Subs>, std::shared_ptr<Xml>>,
                                                                  std::is_convertible<std::decay_t<Subs>, Xml>>,
                                                          int> = 0>
        Xml &addContent(Subs x) {
            content_.emplace_back(make_sub(std::forward<Subs>(x)));
            return *this;
        }


        /// @brief 将XML节点渲染到输出流
        void render(std::ostream &os) const;

        /// @brief 将XML节点渲染为字符串
        /// @see render(std::ostream &os) const
        std::string render() const;

    private:
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(Xml &&sub) { return std::make_shared<Xml>(std::move(sub)); }
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(std::shared_ptr<Xml> &&sub) { return std::move(sub); }
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(std::string &&sub) { return std::move(sub); }
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(const Xml &sub) { return std::make_shared<Xml>(sub); }
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(const std::shared_ptr<Xml> &sub) { return sub; }
        static std::variant<std::shared_ptr<Xml>, std::string> make_sub(const std::string &sub) { return sub; }
        static void escape_xml(std::ostream &os, const std::string &str);
    };
}// namespace badge
#endif// BADGECPP_XML__HPP_GUARD
