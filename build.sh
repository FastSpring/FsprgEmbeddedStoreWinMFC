#!/bin/sh

rm -R -f FsprgEmbeddedStoreWinMFCSDK
./build_project.sh "TestApp" "Release" "FsprgEmbeddedStoreWinMFCSDK"
./build_project.sh "FsprgEmbeddedStore" "Source" "FsprgEmbeddedStoreWinMFCSDK/src"
./build_project.sh "Example1" "Source" "FsprgEmbeddedStoreWinMFCSDK/src"

# FsprgEmbeddedStoreStyle.zip
zip -r FsprgEmbeddedStoreStyle FsprgEmbeddedStoreStyle -x *.svn* *.DS_Store*
mv FsprgEmbeddedStoreStyle.zip FsprgEmbeddedStoreWinMFCSDK 

# HOW_TO.html
mkdir -p FsprgEmbeddedStoreWinMFCSDK/HOW_TO
cp -R -f HOW_TO/* FsprgEmbeddedStoreWinMFCSDK/HOW_TO
perl ./Markdown_1.0.1/Markdown.pl --html4tags HOW_TO.mdown >> ./FsprgEmbeddedStoreWinMFCSDK/HOW_TO.html

# RELEASE_NOTES.html
perl ./Markdown_1.0.1/Markdown.pl --html4tags RELEASE_NOTES.mdown >> ./FsprgEmbeddedStoreWinMFCSDK/RELEASE_NOTES.html

# License.txt
cp -R License.txt FsprgEmbeddedStoreWinMFCSDK

# Package and remove temp directory
zip -r FsprgEmbeddedStoreWinMFCSDK FsprgEmbeddedStoreWinMFCSDK
rm -R -f FsprgEmbeddedStoreWinMFCSDK
