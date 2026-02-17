#!/bin/bash

GITHUB_URL="https://github.com/Seeed-Studio/Grove_Temper_Humidity"
OUTPUT_FILE="seeedTH02.zip"

# Download the zip file
curl -L -o "$OUTPUT_FILE" "$GITHUB_URL"

if [ $? -eq 0 ]; then
    echo "Downloaded to $OUTPUT_FILE"
else
    echo "Download failed"
    exit 1
fi