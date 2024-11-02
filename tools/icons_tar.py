import re
import os
import unicodedata
from typing import Optional, TypedDict


class Icon(TypedDict):
    title: str
    hex: str
    slug: "Optional[str]"
    svg: str


ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ASSETS_DIR = os.path.join(ROOT_DIR, "assets")
SIMPLE_ICONS_DIR = os.path.join(ASSETS_DIR, 'icons')
INDEX_JSON_PATH = os.path.join(SIMPLE_ICONS_DIR, '_data', 'simple-icons.json')
ICONS_FILE_DIR = os.path.join(SIMPLE_ICONS_DIR, 'icons')

OUTPUT_DATA_PATH = os.path.join(ASSETS_DIR, 'icon.bin')
OUTPUT_INDEX_PATH = os.path.join(ASSETS_DIR, 'icon.idx')


def cvt(icons: "list[Icon]") -> "list[Icon]":

    # 定义标题到 slug 的特殊字符替换表
    TITLE_TO_SLUG_REPLACEMENTS = {
        '+': 'plus',
        '.': 'dot',
        '&': 'and',
        'đ': 'd',
        'ħ': 'h',
        'ı': 'i',
        'ĸ': 'k',
        'ŀ': 'l',
        'ł': 'l',
        'ß': 'ss',
        'ŧ': 't',
    }

    # 构建用于匹配需要替换的特殊字符的正则表达式
    special_chars = ''.join(re.escape(char) for char in TITLE_TO_SLUG_REPLACEMENTS.keys())
    TITLE_TO_SLUG_CHARS_REGEX = re.compile(f'[{special_chars}]')

    # 构建用于移除非小写字母和非数字字符的正则表达式
    TITLE_TO_SLUG_RANGE_REGEX = re.compile('[^a-z0-9]')

    def title_to_slug(title: str) -> str:
        # 转为小写
        slug = title.lower()
        # 替换特殊字符
        slug = TITLE_TO_SLUG_CHARS_REGEX.sub(lambda match: TITLE_TO_SLUG_REPLACEMENTS.get(match.group(0), ''), slug)
        # Unicode 规范化
        slug = unicodedata.normalize('NFD', slug)
        # 移除所有非小写字母和非数字字符
        slug = TITLE_TO_SLUG_RANGE_REGEX.sub('', slug)
        return slug

    # 假设 icons 数据结构中包含 'icons' 键，且其值为图标列表
    for icon in icons:
        title = icon.get('title')
        if not title:
            continue  # 跳过没有标题的图标

        # 获取 slug，如果存在则使用，否则生成
        slug = icon.get('slug') or title_to_slug(title)

        # 构建相对路径，假设 SVG 文件直接位于 folder 目录下
        # 如果 SVG 文件在子目录中，请相应调整
        relative_path = f"{slug}.svg"

        icon['svg'] = relative_path
    return icons


def index_maker(icons: "list[Icon]", icon_folder: str, index_output: str, data_output: str):
    os.makedirs(os.path.dirname(index_output), exist_ok=True)
    os.makedirs(os.path.dirname(data_output), exist_ok=True)

    offset = 0

    with open(index_output, 'w', encoding='utf-8') as index_f:
        with open(data_output, 'wb') as data_f:
            for icon in icons:
                title = icon.get('title')
                svg = icon.get('svg')
                hex = icon.get('hex', "")
                if not title or not svg:
                    continue
                icon_path = os.path.join(icon_folder, svg)

                assert os.path.exists(icon_path), f"Icon file {icon_path} not found: {title}"

                with open(icon_path, 'rb') as svg_f:
                    svg_content = svg_f.read()
                length = len(svg_content)
                data_f.write(svg_content)

                assert '\t' not in hex, f"Invalid hex code: {hex}"
                assert '\n' not in title, f"Invalid title: {title}"

                index_f.write(f"{offset}\t{length}\t{hex}\t{title}\n")
                offset += length


if __name__ == "__main__":
    import json

    with open(INDEX_JSON_PATH, 'r', encoding='utf-8') as f:
        icons: "list[Icon]" = json.load(f)["icons"]

    icons = cvt(icons)

    index_maker(icons, ICONS_FILE_DIR,
                OUTPUT_INDEX_PATH, OUTPUT_DATA_PATH)
