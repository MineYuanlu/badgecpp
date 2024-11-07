#define PYBIND11_DETAILED_ERROR_MESSAGES
#include "badgecpp/badge.hpp"
#include "badgecpp/color.hpp"
#include "badgecpp/icons.hpp"
#include "badgecpp/version.hpp"
#include "badgecpp/xml.hpp"
#include <memory>
#include <pybind11/cast.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


namespace py = pybind11;
using namespace badge;

namespace {

    // Trampoline 类用于处理 Icon 的虚函数
    class PyIcon : public Icon {
    public:
        /* Inherit the constructors */
        using Icon::Icon;

        /* Override virtual functions */
        std::string get_uri(const std::optional<Color> &color = std::nullopt, std::optional<BBox> pos = std::nullopt) const override {
            PYBIND11_OVERRIDE_PURE(
                    std::string, /* Return type */
                    Icon,        /* Parent class */
                    get_uri,     /* Name of function */
                    color, pos   /* Arguments */
            );
        }

        std::string get_svg(const std::optional<Color> &color = std::nullopt, std::optional<BBox> pos = std::nullopt) const override {
            PYBIND11_OVERRIDE_PURE(
                    std::string,
                    Icon,
                    get_svg,
                    color, pos);
        }

        Color get_color() const override {
            PYBIND11_OVERRIDE_PURE(
                    Color,
                    Icon,
                    get_color);
        }
    };
}// namespace

PYBIND11_MODULE(pybadge, m) {

    py::class_<Xml>(m, "Xml")
            .def(py::init<>())
            .def(py::init<std::string>(), py::arg("name"))
            .def("render", py::overload_cast<>(&Xml::render, py::const_))
            .def("is_empty", &Xml::isEmpty)
            .def("is_sub_empty", &Xml::isSubEmpty)
            .def(
                    "add_content", [](Xml &self, const std::string &content) -> Xml & {
                        return self.addContent(content);
                    },
                    py::return_value_policy::reference_internal)
            .def(
                    "add_content", [](Xml &self, const Xml &xml) -> Xml & {
                        return self.addContent(xml);
                    },
                    py::return_value_policy::reference_internal)
            .def(
                    "add_content", [](Xml &self, const std::shared_ptr<Xml> &xml_ptr) -> Xml & {
                        return self.addContent(xml_ptr);
                    },
                    py::return_value_policy::reference_internal)
            .def("add_attr", &Xml::addAttr, py::return_value_policy::reference_internal)
            .def("set_name", &Xml::setName, py::return_value_policy::reference_internal);


    py::class_<Color>(m, "Color")
            .def(py::init<>())
            .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), py::arg("r"), py::arg("g"), py::arg("b"), py::arg("a") = 255)
            .def(py::init<std::string, bool>(), py::arg("color_str"), py::arg("fail_silent") = false)
            .def("to_str", &Color::to_str)
            .def("name", &Color::name)
            .def("brightness", &Color::brightness)
            .def("get_color_hex_pair_for_foreground", &Color::getColorHexPairForBackground)
            .def_property(
                    "r",
                    [](Color &self) -> uint8_t { return self.r(); },
                    [](Color &self, uint8_t value) { self.r() = value; })
            .def_property(
                    "g",
                    [](Color &self) -> uint8_t { return self.g(); },
                    [](Color &self, uint8_t value) { self.g() = value; })
            .def_property(
                    "b",
                    [](Color &self) -> uint8_t { return self.b(); },
                    [](Color &self, uint8_t value) { self.b() = value; })
            .def_property(
                    "a",
                    [](Color &self) -> uint8_t { return self.a(); },
                    [](Color &self, uint8_t value) { self.a() = value; })
            .def_property(
                    "rgba",
                    [](Color &self) -> uint32_t { return self.rgba(); },
                    [](Color &self, uint32_t value) { self.rgba() = value; })
            .def("__eq__", &Color::operator==)
            .def("__ne__", &Color::operator!=)
            .def_static("parse_string", [](Color &self, const std::string_view &input) {
                return self.parseString(input);
            })
            .def_static("parse_hex", [](Color &self, const std::string_view &input) {
                return self.parseHex(input);
            })
            .def_static("parse_rgb", [](Color &self, const std::string_view &input) {
                return self.parseRGB(input);
            })
            .def_static("parse_rgba", [](Color &self, const std::string_view &input) {
                return self.parseRGBA(input);
            })
            .def_static("parse_hsl", [](Color &self, const std::string_view &input) {
                return self.parseHSL(input);
            })
            .def_static("parse_hsla", [](Color &self, const std::string_view &input) {
                return self.parseHSLA(input);
            })
            .def_static("parse_name", [](Color &self, const std::string_view &input) {
                return self.parseName(input);
            });


    py::class_<Icon, PyIcon, std::shared_ptr<Icon>>(m, "Icon")
            .def(py::init<>())// 允许在 Python 中继承
            .def("get_uri", &Icon::get_uri,
                 py::arg("color") = std::nullopt,
                 py::arg("pos") = std::nullopt,
                 "获取图标uri格式的数据")
            .def("get_svg", &Icon::get_svg,
                 py::arg("color") = std::nullopt,
                 py::arg("pos") = std::nullopt,
                 "获取图标svg格式的数据")
            .def("get_color", &Icon::get_color, "获取图标基准颜色");


    py::class_<BuiltinIcon, Icon, std::shared_ptr<BuiltinIcon>>(m, "BuiltinIcon")
            .def("get_uri", &BuiltinIcon::get_uri,
                 py::arg("color") = std::nullopt,
                 py::arg("pos") = std::nullopt,
                 "获取图标uri格式的数据")
            .def("get_svg", &BuiltinIcon::get_svg,
                 py::arg("color") = std::nullopt,
                 py::arg("pos") = std::nullopt,
                 "获取图标svg格式的数据")
            .def("get_color", &BuiltinIcon::get_color, "获取图标基准颜色")
            .def_readonly("title", &BuiltinIcon::title, "图标标题")
            .def_readonly("color", &BuiltinIcon::color, "图标颜色")
            .def_readonly("icon", &BuiltinIcon::icon, "图标字符串");


    py::class_<BuiltinIcons>(m, "BuiltinIcons")
            .def_static("icons", &BuiltinIcons::icons, "获取所有的内置图标")
            .def_static("by_title", &BuiltinIcons::by_title, py::arg("title"), "根据标题查找内置图标");


    py::enum_<Style>(m, "Style")
            .value("FLAT", Style::FLAT)
            .value("FLAT_SQUARE", Style::FLAT_SQUARE)
            .value("PLASTIC", Style::PLASTIC)
            .value("FOR_THE_BADGE", Style::FOR_THE_BADGE)
            .value("SOCIAL", Style::SOCIAL)
            .export_values();


    m.def("style_str", &style_str);

    py::class_<Badge>(m, "Badge")
            .def(py::init<
                         std::optional<std::string>,
                         std::optional<Color>,
                         std::optional<std::string>,
                         std::optional<Color>,
                         Style,
                         std::shared_ptr<const Icon>,
                         std::optional<Color>,
                         std::optional<unsigned int>,
                         std::optional<std::string>>(),
                 py::arg_v("label", std::optional<std::string>{}, "标签文字"),
                 py::arg_v("label_color", std::optional<Color>{}, "标签背景颜色"),
                 py::arg_v("message", std::optional<std::string>{}, "信息文字"),
                 py::arg_v("message_color", std::optional<Color>{}, "信息背景颜色"),
                 py::arg_v("style", Style::FLAT, "徽章样式"),
                 py::arg_v("logo", std::shared_ptr<const Icon>{}, "徽章logo"),
                 py::arg_v("logo_color", std::optional<Color>{}, "徽章logo颜色, 默认为logo自带的颜色"),
                 py::arg_v("logo_width", std::optional<unsigned int>{}, "徽章logo宽度, 默认为14"),
                 py::arg_v("id_suffix", std::optional<std::string>{}, "徽章id后缀, 用于区分不同badge"))
            .def_readwrite("label", &Badge::label_, "标签文字")
            .def_readwrite("label_color", &Badge::label_color_, "标签背景颜色")
            .def_readwrite("message", &Badge::message_, "信息文字")
            .def_readwrite("message_color", &Badge::message_color_, "信息背景颜色")
            .def_readwrite("style", &Badge::style_, "徽章样式")
            .def_readwrite("logo", &Badge::logo_, "徽章logo")
            .def_readwrite("logo_color", &Badge::logo_color_, "徽章logo颜色")
            .def_readwrite("logo_width", &Badge::logo_width_, "徽章logo宽度")
            .def_readwrite("id_suffix", &Badge::id_suffix_, "徽章id后缀")
            .def("make_badge", &Badge::makeBadge, "生成徽章字符串")
            .def("make_badge_xml", &Badge::makeBadgeXml, "生成徽章的xml对象");

    py::class_<version>(m, "version")
            .def_static("major", &version::major, "获取主版本号")
            .def_static("minor", &version::minor, "获取次版本号")
            .def_static("patch", &version::patch, "获取修订号")
            .def_static("special", &version::special, "获取特殊版本号")
            .def_static("string", &version::string, "获取版本号字符串")
            .def_static("build_type", &version::build_type, "获取构建类型")
            .def_static("is_debug", &version::is_debug, "是否为调试版本");
}