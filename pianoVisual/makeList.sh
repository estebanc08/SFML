#!/bin/bash
function cleanup {
    echo -e "\nExiting..."
    exit 1
}

trap cleanup SIGINT

echo -n "" > songList.txt

for file in ./outputMidi/*.mid
do
    filename=$(basename -- "$file")
    extension="${filename##*.}"
    filename="${filename%.*}"
    if [ ! -f "outputWav/$filename.wav" ]; then
        fluidsynth -F "./outputWav/${filename}.wav" soundfont.sf2 "./outputMidi/${filename}.mid"
    fi
    echo "$filename" >> songList.txt
done

sed -i '1{/^$/d}' songList.txt
