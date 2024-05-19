#!/usr/bin/bash


declare -a arr=(95 96 97 98 99 100 101 102 103 104 105)

for i in "${arr[@]}"
do
  echo "Starting V = $i V ..."
  for filename in data/trc/CD188/${i}V/*trc; do
    ./makeRawTree "$filename"
  done
  echo "hadding..."
  hadd data/root/CD188/treeraw_${i}V.root data/root/CD188/${i}V/*root
done



