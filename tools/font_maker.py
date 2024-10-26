import json
import os

snippet = """#include "badgecpp/font.hpp"
namespace {
    [[maybe_unused]] bool loaded = ([]() {
        badge::Fonts::createFont("%s", badge::Font{
                                                                // clang-format off
            {
%s
                    },
                                                                // clang-format on
                                                                %d});
        return true;
    })();
}
"""
FONT_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "src", "fonts")
assert os.path.isdir(FONT_DIR), "Font directory not found: " + FONT_DIR


def make_font_cpp(font_name: str, font_file: str, font_size: int):
    font_cpp_file = os.path.join(FONT_DIR, font_name + ".cpp")
    assert not os.path.isfile(font_cpp_file), "Font cpp file already exists: " + font_cpp_file
    with open(font_file, 'r') as f:
        font_data: "list[tuple[int,int,float]]" = json.load(f)
    font_data_str = ",".join(f"{{{x[0]},{x[1]},{x[2]}}}" for x in font_data)
    code = snippet % (font_name, font_data_str, font_size)

    with open(font_cpp_file, 'w') as f:
        f.write(code)
    print("Generated font cpp file: " + font_cpp_file)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Generate font cpp files from json files.')
    parser.add_argument('font_name', type=str, help='Name of the font.')
    parser.add_argument('font_file', type=str, help='Path to the json file containing the font data.')
    parser.add_argument('font_size', type=int, help='Size of the font.')
    args = parser.parse_args()

    make_font_cpp(args.font_name, args.font_file, args.font_size)
