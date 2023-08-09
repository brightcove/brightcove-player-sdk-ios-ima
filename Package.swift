// swift-tools-version:5.5
// The swift-tools-version declares the minimum version of Swift required to build this package.
// https://developer.apple.com/documentation/xcode/creating_a_standalone_swift_package_with_xcode?language=objc
import PackageDescription

let BrightcovePlayerSDK = Package(
    name: "IMA Plugin for Brightcove Player SDK",
    platforms: [
        .iOS(.v14), .tvOS(.v12)
    ],
    products: [
        .library(
            name: "BrightcoveIMA",
            targets: ["BrightcoveIMA"])
    ],
    targets: [
        .binaryTarget(
            name: "BrightcoveIMA",
            path: "xcframework/BrightcoveIMA.xcframework"
        )
    ]
)

