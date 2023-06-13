#! /bin/bash
echo -e "\033[33m Cleaning... \033[0m"
echo -e "\033[33m Deleting test/__pycache__... \033[0m"
rm -rf test/__pycache__

# cmake
# rm -rf build

# txt

echo PWD:$PWD

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

function IsSuffix() {
    local filename="$1"
    local suffix="$2"
    if [ "$(FileSuffix ${filename})" = "$suffix" ]; then
        return 0
    else
        return 1
    fi
}

for file in $PWD/*; do

    
    filename=${file##*/}
    # echo ${filename}

    IsSuffix ${filename} "txt"
    ret=$?
    if [ $ret -eq 0 ]; then
        # echo "the suffix of the ${file} is txt"
        if [ $filename != "CMakeLists.txt" ]; then
            echo -e "\033[33m Deleting ${filename}... \033[0m"
            rm $filename
        fi
    fi

    IsSuffix ${filename} "png"
    ret=$?
    if [ $ret -eq 0 ]; then
        # echo "the suffix of the ${file} is txt"
        if [ $filename != "donotdelete.png" ]; then # TODO
            echo -e "\033[33m Deleting ${filename}... \033[0m"
            rm $filename
        fi
    fi

done