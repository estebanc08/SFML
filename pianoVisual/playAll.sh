#!/bin/bash
function cleanup {
    echo -e "\nExiting..."
    exit 1
}

trap cleanup SIGINT


for file in ./outputMidi/*.mid
do
    filename=$(basename -- "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"
    echo "Now Playing: $file"
    ./piano.out ${filename}
done
