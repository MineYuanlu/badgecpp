from typing import Optional
from typing import Optional, List, Any
from typing import Optional, Tuple, Union
from enum import Enum


class Xml:
    """
    XML节点类
    """

    def __init__(self, name: Optional[str] = None) -> None:
        """
        初始化XML节点

        :param name: 节点名称, 为空代表这是一个列表节点, 本身不会被渲染, 只渲染其内容
        """
        ...

    def render(self) -> str:
        """
        将XML节点渲染为字符串

        :return: XML的字符串表示
        """
        ...

    def is_empty(self) -> bool:
        """
        判断本节点及所有子节点是否为空

        :return: 如果为空返回True, 否则返回False
        """
        ...

    def is_sub_empty(self) -> bool:
        """
        判断所有子节点是否为空

        :return: 如果所有子节点为空返回True, 否则返回False
        """
        ...

    def add_content(self, content: Union[str, 'Xml']) -> 'Xml':
        """
        添加一个节点内容

        :param content: 内容，可以是字符串或Xml节点
        :return: 自身引用
        """
        return self

    def add_attr(self, key: str, value: str) -> 'Xml':
        """
        添加一条属性

        :param key: 属性键
        :param value: 属性值
        :return: 自身引用
        """
        return self

    def set_name(self, name: str) -> 'Xml':
        """
        设置节点名称, 为空代表这是一个列表节点, 本身不会被渲染, 只渲染其内容

        :param name: 节点名称
        :return: 自身引用
        """
        return self


class Color:
    """
    颜色类
    """

    def __init__(self) -> None:
        """
        默认构造函数
        """
        ...

    def __init__(self, r: int, g: int, b: int, a: int = 255) -> None:
        """
        使用RGBA值初始化颜色

        :param r: 红色分量 (0-255)
        :param g: 绿色分量 (0-255)
        :param b: 蓝色分量 (0-255)
        :param a: Alpha分量 (0-255), 默认为255
        """
        ...

    def __init__(self, color_str: str, fail_silent: bool = False) -> None:
        """
        使用字符串初始化颜色

        :param color_str: 颜色字符串
        :param fail_silent: 解析失败时是否静默, 默认为False
        """
        ...

    def to_str(self) -> str:
        """
        转换为最短CSS颜色字符串

        :return: #RGB 或 #RRGGBB 或 rgba(R,G,B,A) 字符串
        """
        ...

    def name(self) -> Optional[str]:
        """
        获取颜色的名称, 若无则返回None

        :return: 颜色名称或None
        """
        ...

    def brightness(self) -> float:
        """
        获取颜色的亮度值, 范围[0, 1]

        :return: 亮度值
        """
        ...

    def get_color_hex_pair_for_background(self) -> Tuple[str, str]:
        """
        将当前颜色作为背景颜色, 获取文字颜色和阴影颜色的Hex字符串

        :return: (文字颜色, 阴影颜色) 元组
        """
        ...

    @property
    def r(self) -> int:
        """
        获取或设置红色分量 (0-255)
        """
        ...

    @r.setter
    def r(self, value: int) -> None:
        ...

    @property
    def g(self) -> int:
        """
        获取或设置绿色分量 (0-255)
        """
        ...

    @g.setter
    def g(self, value: int) -> None:
        ...

    @property
    def b(self) -> int:
        """
        获取或设置蓝色分量 (0-255)
        """
        ...

    @b.setter
    def b(self, value: int) -> None:
        ...

    @property
    def a(self) -> int:
        """
        获取或设置Alpha分量 (0-255)
        """
        ...

    @a.setter
    def a(self, value: int) -> None:
        ...

    @property
    def rgba(self) -> int:
        """
        获取或设置RGBA整体值
        """
        ...

    @rgba.setter
    def rgba(self, value: int) -> None:
        ...

    def __eq__(self, other: 'Color') -> bool:
        """
        判断两个颜色是否相等

        :param other: 另一个Color对象
        :return: 如果相等返回True, 否则返回False
        """
        ...

    def __ne__(self, other: 'Color') -> bool:
        """
        判断两个颜色是否不等

        :param other: 另一个Color对象
        :return: 如果不等返回True, 否则返回False
        """
        ...

    @staticmethod
    def parse_string(input: str) -> Optional['Color']:
        """
        解析字符串为Color对象

        :param input: 颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_hex(input: str) -> Optional['Color']:
        """
        解析Hex字符串为Color对象

        :param input: Hex颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_rgb(input: str) -> Optional['Color']:
        """
        解析RGB字符串为Color对象

        :param input: RGB颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_rgba(input: str) -> Optional['Color']:
        """
        解析RGBA字符串为Color对象

        :param input: RGBA颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_hsl(input: str) -> Optional['Color']:
        """
        解析HSL字符串为Color对象

        :param input: HSL颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_hsla(input: str) -> Optional['Color']:
        """
        解析HSLA字符串为Color对象

        :param input: HSLA颜色字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...

    @staticmethod
    def parse_name(input: str) -> Optional['Color']:
        """
        解析颜色名称为Color对象

        :param input: 颜色名称字符串
        :return: 解析成功返回Color对象, 否则返回None
        """
        ...


class Icon:
    """
    图标基类
    """

    def get_uri(self, color: Optional[Color] = None, pos: Optional[Any] = None) -> str:
        """
        获取图标uri格式的数据

        :param color: 颜色，可选
        :param pos: 位置，可选
        :return: 图标的URI字符串
        """
        ...

    def get_svg(self, color: Optional[Color] = None, pos: Optional[Any] = None) -> str:
        """
        获取图标svg格式的数据

        :param color: 颜色，可选
        :param pos: 位置，可选
        :return: 图标的SVG字符串
        """
        ...

    def get_color(self) -> Color:
        """
        获取图标基准颜色

        :return: 图标的基准颜色
        """
        ...


class BuiltinIcon(Icon):
    """
    内置图标类
    """

    title: str
    """图标标题"""

    color: Color
    """图标颜色"""

    icon: str
    """图标字符串"""

    def get_uri(self, color: Optional[Color] = None, pos: Optional[Any] = None) -> str:
        """
        获取图标uri格式的数据

        :param color: 颜色，可选
        :param pos: 位置，可选
        :return: 图标的URI字符串
        """
        ...

    def get_svg(self, color: Optional[Color] = None, pos: Optional[Any] = None) -> str:
        """
        获取图标svg格式的数据

        :param color: 颜色，可选
        :param pos: 位置，可选
        :return: 图标的SVG字符串
        """
        ...

    def get_color(self) -> Color:
        """
        获取图标基准颜色

        :return: 图标的基准颜色
        """
        ...


class BuiltinIcons:
    """
    内置图标管理类
    """

    @staticmethod
    def icons() -> List[BuiltinIcon]:
        """
        获取所有的内置图标

        :return: 内置图标列表
        """
        ...

    @staticmethod
    def by_title(title: str) -> Optional[BuiltinIcon]:
        """
        根据标题查找内置图标

        :param title: 图标标题
        :return: 内置图标实例，如果没有找到则返回 None
        """
        ...


class Style(Enum):
    """
    徽章样式枚举
    """
    FLAT = 0  # 平面样式
    FLAT_SQUARE = 1  # 平面方形样式
    PLASTIC = 2  # 塑料样式
    FOR_THE_BADGE = 3  # 徽章风格
    SOCIAL = 4  # 社交风格


FLAT = Style.FLAT
"""平面样式"""

FLAT_SQUARE = Style.FLAT_SQUARE
"""平面方形样式"""

PLASTIC = Style.PLASTIC
"""塑料样式"""

FOR_THE_BADGE = Style.FOR_THE_BADGE
"""徽章风格"""


def style_str(style: Style) -> str:
    """
    将样式枚举转换为字符串

    :param style: 徽章样式
    :return: 样式对应的字符串表示
    """
    ...


class Badge:
    """
    徽章类
    """

    def __init__(
        self,
        label: Optional[str] = None,
        label_color: Optional[Color] = None,
        message: Optional[str] = None,
        message_color: Optional[Color] = None,
        style: Style = Style.FLAT,
        logo: Optional[Icon] = None,
        logo_color: Optional[Color] = None,
        logo_width: Optional[int] = None,
        id_suffix: Optional[str] = None
    ) -> None:
        """
        初始化徽章

        :param label: 标签文字
        :param label_color: 标签背景颜色
        :param message: 信息文字
        :param message_color: 信息背景颜色
        :param style: 徽章样式
        :param logo: 徽章logo
        :param logo_color: 徽章logo颜色, 默认为logo自带的颜色
        :param logo_width: 徽章logo宽度, 默认为14
        :param id_suffix: 徽章id后缀, 用于区分不同badge
        """
        ...

    label: Optional[str]
    """标签文字"""

    label_color: Optional[Color]
    """标签背景颜色"""

    message: Optional[str]
    """信息文字"""

    message_color: Optional[Color]
    """信息背景颜色"""

    style: Style
    """徽章样式"""

    logo: Optional[Icon]
    """徽章logo"""

    logo_color: Optional[Color]
    """徽章logo颜色, 默认为logo自带的颜色"""

    logo_width: Optional[int]
    """徽章logo宽度, 默认为14"""

    id_suffix: Optional[str]
    """徽章id后缀, 用于区分不同badge"""

    def make_badge(self) -> str:
        """
        生成徽章字符串

        :return: 徽章的字符串表示
        """
        ...

    def make_badge_xml(self) -> Xml:
        """
        生成徽章的xml对象

        :return: 徽章的XML表示
        """
        ...


class version:
    """
    版本信息类
    """

    @staticmethod
    def major() -> int:
        """
        获取主版本号

        :return: 主版本号，未知时为0
        """
        ...

    @staticmethod
    def minor() -> int:
        """
        获取次版本号

        :return: 次版本号，未知时为0
        """
        ...

    @staticmethod
    def patch() -> int:
        """
        获取修订号

        :return: 修订号，未知时为0
        """
        ...

    @staticmethod
    def special() -> Optional[str]:
        """
        获取特殊版本号

        :return: 特殊版本号，未知时为None
        """
        ...

    @staticmethod
    def string() -> Optional[str]:
        """
        获取版本号字符串

        :return: 版本号字符串，未知时为None
        """
        ...

    @staticmethod
    def build_type() -> Optional[str]:
        """
        获取构建类型

        :return: 构建类型，"DEBUG"/"RELEASE"，未知时为None
        """
        ...

    @staticmethod
    def is_debug() -> bool:
        """
        是否为调试版本

        :return: 如果是调试版本返回True，否则返回False，未知时为False
        """
        ...
