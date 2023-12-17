#!/bin/bash
curr_dir=$(pwd)
cd ../../frontend/brahma
npm run build
cd $curr_dir
rm -r data/
cp -r ../../frontend/brahma/build data/
rm -r data/assets
# rm data/report.html || true
for i in `find data | grep -E "\.css$|\.html$|\.js$"`; do gzip -9 -v "$i" ; done
for i in `find data | grep -E "\.map$"`; do rm "$i" ; done