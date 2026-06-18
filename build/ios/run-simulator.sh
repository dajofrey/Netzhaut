#!/bin/bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

echo "Building Netzhaut iOS static libraries..."
make -f build/automation/lib-ios.mk IOS_SDK=iphonesimulator

echo "Building NetzhautApp for iOS Simulator..."
xcodebuild \
    -project build/ios/Netzhaut.xcodeproj \
    -scheme NetzhautApp \
    -destination 'generic/platform=iOS Simulator' \
    -configuration Debug \
    build

echo "Done. Open build/ios/Netzhaut.xcodeproj in Xcode to run on a simulator."
