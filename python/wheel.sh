#!/bin/bash

# Build wheel

DIR=$(dirname $(realpath $0))

pushd $DIR > /dev/null

echo "Building wheel:" $(pwd)

rm -rf build dist pybadge.egg-info

python3 setup.py bdist_wheel

popd > /dev/null