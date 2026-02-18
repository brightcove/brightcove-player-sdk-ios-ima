# IMA Plugin for Brightcove Player SDK for iOS, version 7.2.4.13

## Installation

IMA Plugin for Brightcove Player SDK provides a dynamic library framework for installation.

The IMA plugin supports version 3.26.1 of the Google IMA SDK for iOS and version 4.15.1 of the Google IMA SDK for tvOS.

### CocoaPods

You can use [CocoaPods][cocoapods] to add the IMA Plugin for Brightcove Player SDK to your project.  You can find the latest `Brightcove-Player-IMA` podspec [here][podspecs]. The pod will incorporate the correct version of IMA automatically.

CocoaPod Podfile example:

```bash
source 'https://github.com/CocoaPods/Specs'
source 'https://github.com/brightcove/BrightcoveSpecs.git'

platform :ios, '14.0'
use_frameworks!

target 'MyIMAPlayer' do
  pod 'Brightcove-Player-IMA'
end
```

As of IMA release 3.4.1, AdMob is published as a standalone dynamic framework and can no longer be installed as part of the IMA Plugin for the Brightcove Native Player SDK. As such, there is no longer a `ForAdMob` subspec for the podspec. To use AdMob, download it and add it to your project manually, or add the `Google-Mobile-Ads-SDK` CocoaPod to your Podfile:
    
    pod 'Google-Mobile-Ads-SDK'  

### Manual

To add the IMA Plugin for Brightcove Player SDK to your project manually:

1. Download the [Brightcove Player SDK][bcovsdkrelease] framework.
1. Download the [IMA Plugin for Brightcove Player SDK][bcoveimarelease] framework.
1. Download the [Google IMA][googleima] framework.
1. On the "General" tab of your application target, add the **dynamic** framework, `BrightcovePlayerSDK.framework` or `BrightcovePlayerSDK.xcframework`, from the Brightcove Player SDK download to the list of **Frameworks, Libraries, and Embedded Content**. The universal Framework and XCFramework are found in the ios directory of the download. The _Embed_ setting must be "_Embed & Sign_".
1. On the "General" tab of your application target, add `BrightcoveIMA.framework` or `BrightcoveIMA.xcframework` from the IMA Plugin for Brightcove Player SDK download to the list of **Frameworks, Libraries, and Embedded Content**. The _Embed_ setting must be "_Embed & Sign_".
1. On the "General" tab of your application target, add `GoogleInteractiveMediaAds.xcframework` from the Google IMA download to the list of **Frameworks, Libraries, and Embedded Content**. The _Embed_ setting for the XCFrameworks must be "_Embed & Sign_".
1. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the paths to the frameworks. This should have been done automatically unless the framework is stored under a different root directory than your project.
1. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
1. (**Universal Framework** only) On the "Build Phases" tab, add a "Run Script" phase with the command `bash ${BUILT_PRODUCTS_DIR}/${FRAMEWORKS_FOLDER_PATH}/BrightcoveIMA.framework/strip-frameworks.sh`. Check "Run script only when installing". This will remove unneeded architectures from the build, which is important for App Store submission.
1. (**Apple Silicon** only) On the "Build Settings" tab of your application target:
    * Ensure that `arm64` has been added to your "Excluded Architectures" build setting for `Any iOS Simulator SDK`.

### Swift Package Manager

To add the IMA Plugin for Brightcove Player SDK to your project with Swift Package Manager:

1. First [follow the steps][corespm] to add the Core XCFramework with Swift Package Mananger.
1. Add the IMA package to Swift Package Manager using `https://github.com/brightcove/brightcove-player-sdk-ios-ima.git`.
1. Download the [Google IMA][googleima] framework.
1. On the "General" tab of your application target, add `GoogleInteractiveMediaAds.xcframework` to the list of **Frameworks, Libraries, and Embedded Content**. The _Embed_ setting for the XCFrameworks must be "_Embed & Sign_".
1. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the paths to the frameworks. This should have been done automatically unless the framework is stored under a different root directory than your project.

[corespm]: https://github.com/brightcove/brightcove-player-sdk-ios#swift-package-manager

### Imports

The IMA Plugin for Brightcove Player SDK can be imported using:
```swift
 import BrightcoveIMA
```

[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BrightcoveSpecs/tree/master/Brightcove-Player-IMA
[bcovsdkrelease]: https://github.com/brightcove/brightcove-player-sdk-ios/releases
[bcoveimarelease]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases

## Quick Start

The BrightcoveIMA plugin is a bridge between [Google IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. This snippet shows its basic usage with VMAP ad rules.

```swift
[1] let imaSettings = IMASettings()
    if let languageCode = NSLocale.current.languageCode {
        imaSettings.language = languageCode
    }

    let adsRenderingSettings = IMAAdsRenderingSettings()
    adsRenderingSettings.linkOpenerDelegate = self
    adsRenderingSettings.linkOpenerPresentingController = self

    let adTag = "<ad tag>"

[2] let adsRequestPolicy = BCOVIMAAdsRequestPolicy.init(vmapAdTagUrl: adTag)

    let sdkManager = BCOVPlayerSDKManager.sharedManager()
[3] let playbackController = sdkManager.createIMAPlaybackController(with: imaSettings,
                                                                adsRenderingSettings: adsRenderingSettings,
                                                                adsRequestPolicy: adsRequestPolicy,
                                                                adContainer: playerView.contentOverlayView,
                                                                viewController: self,
                                                                companionSlots: nil,
                                                                viewStrategy: nil,
                                                                options: nil)

    playbackController?.delegate = self

    videoView.addSubview(playerView)

    let policyKey = "<your-policy-key>"
    let accountID = "<your-account-id>"
    let videoID = "<your-video-id>"
    let playbackService = BCOVPlaybackService(withAccountId: accountID,
                                              policyKey: policyKey)
    let configuration = [
        BCOVPlaybackService.ConfigurationKeyAssetID: videoID
    ]
    playbackService.findVideo(withConfiguration: configuration,
                              queryParameters: nil) { (video: BCOVVideo?,
                                                       jsonResponse: Any?,
                                                       error: Error?) in
        if let video {
            playbackController?.setVideos([video])
            playbackController?.play()
        }
    }
```

Breaking the code down into steps:

1. Create the same IMA settings, ads rendering settings that you would create if you were using Google's IMA iOS SDK directly. These are required.
1. BCOVIMAAdsRequestPolicy provides methods to specify VAST or VMAP ad rules. Select the appropriate method to select your ads policy.
1. BrightcoveIMA adds some category methods to BCOVPlaybackManager. The first of these is `createIMAPlaybackController(with:adsRenderingSettings:adsRequestPolicy:adContainer:viewController:companionSlots:viewStrategy:)`. Use this method to create your playback controller.

[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/download
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios

## Play and Pause

The Brightcove IMA Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension ima_play]` or `-[BCOVSessionProviderExtension ima_pause]` ([BCOVSessionProviderExtension][BCOVIMAComponent]), and not the AVPlayer.

As an example, calling play for the first time on `BCOVPlaybackController` allows BCOVIMA to process preroll ads without any of the content playing before the preroll. For more information on how BCOVIMA overrides the default `BCOVPlaybackController` methods, please check out [BCOVSessionProviderExtension][BCOVIMAComponent].

[BCOVIMAComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/blob/master/ios/BrightcoveIMA.framework/Headers/BCOVIMAComponent.h

## Seek Without Ads

Use `playbackController.seekWithoutAds(_,completionHandler:)` to resume playback at a specific time without forcing the user to watch ads scheduled before `seekToTime`.

In preparation for `seekWithoutAds(_,completionHandler:)`, disable `autoPlay` when setting up the `BCOVPlaybackController`.

Apple recommends waiting for the status of an AVPlayerItem to change to ready-to-play before using the AVPlayerItem. Therefore, call `seekWithoutAds(_,completionHandler:)` in the `kBCOVPlaybackSessionLifecycleEventReady` handler of the `playbackController:playbackSession:didReceiveLifecycleEvent` method of your `BCOVPlaybackControllerDelegate`.

```swift
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didReceive lifecycleEvent: BCOVPlaybackSessionLifecycleEvent) {

    if kBCOVPlaybackSessionLifecycleEventReady == lifecycleEvent.eventType {

        // self.resumePlayback is a hypothetical instance variable used here for illustration.
        if resumePlayback {
            // seek without playing ads which are scheduled before the seek time, i.e. resume playback.
            controller.seekWithoutAds(seekWithoutAdsValue) { [weak playbackController, weak self] finished in
                if !finished {
                    print("seekWithoutAds failed to finish")
                }

                // fade out the shutter to reveal the player view.
                playbackController?.shutterFadeTime = 0.25
                playbackController?.shutter = false

                // turn off seek without ads - especially important if this player is being used with a playlist
                self?.resumePlayback = false
            }
        }

    }
}
```

The `shutter` and `shutterFadeTime` properties of the `BCOVPlaybackController` can be used along with `seekWithoutAds:completionHandler:` to hide frame-flicker which can occur as the AVPlayer loads assets. In your BCOVPlaybackController set-up code, enable the shutter to hide the player view:

```swift
let sdkManager = BCOVPlayerSDKManager.sharedManager()
let playbackController = sdkManager.createIMAPlaybackController(with: imaSettings,
                                                                adsRenderingSettings: adsRenderingSettings,
                                                                adsRequestPolicy: adsRequestPolicy,
                                                                adContainer: playerView.contentOverlayView,
                                                                viewController: self,
                                                                companionSlots: nil,
                                                                viewStrategy: nil)



playbackController.delegate = self

if resumePlayback {
    // set the shutter fade time to zero to hide the player view immediately.
    playbackController.shutterFadeTime = 0
    playbackController.shutter = true
    
    // disable autoPlay when resuming playback.
    playbackController.isAutoPlay = false
}
```

Note that when Seek Without Ads is used in your app, you might observe network traffic which normally occurs as part of setting up the IMA plugin. This traffic is necessary for proper plugin setup, and does not affect the Seek Without Ads functionality.

## Customizing Plugin Behavior

There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

### VAST and VMAP ad rules

BCOVIMA gives you control over how ads requests are made, via the `BCOVIMAAdsRequestPolicy` class. The class provides factory methods for the supported policies. Once you obtain an instance of the correct policy, you need to provide it to the `BCOVPlayerSDKManager` to create a playback controller or a playback session provider. 

In Quick Start, an example of VMAP is given. Here is a VAST example.

```swift
    let imaSettings = IMASettings()
    if let languageCode = NSLocale.current.languageCode {
        imaSettings.language = languageCode
    }

    let adsRenderingSettings = IMAAdsRenderingSettings()
    adsRenderingSettings.linkOpenerDelegate = self
    adsRenderingSettings.linkOpenerPresentingController = self

[1] let adsRequestPolicy = BCOVIMAAdsRequestPolicy(vastAdTagsInCuePointsAndAdsCuePointProgressPolicy: nil)

    let sdkManager = BCOVPlayerSDKManager.sharedManager()
    let playbackController = sdkManager.createIMAPlaybackController(with: imaSettings,
                                                                    adsRenderingSettings: adsRenderingSettings,
                                                                    adsRequestPolicy: adsRequestPolicy,
                                                                    adContainer: playerView.contentOverlayView,
                                                                    viewController: self,
                                                                    companionSlots: nil,
                                                                    viewStrategy: nil,
                                                                    options: nil)
```

Let's break this code down into steps, to make it a bit simpler to digest:

1. This example is the same as the one provided in the quick-start, except that we are now calling a different `BCOVIMAAdsRequestPolicy` policy method to specify that we want to use VAST.

BCOVIMAAdsRequestPolicy has four factory methods to generate ads request policy; two for VMAP and two for VAST. Two factory methods for VMAP are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads.

* `+adsRequestPolicyWithVMAPAdTagUrl:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

There are two factory methods for VAST.  All of the VAST methods take a BCOVCuePointProgressPolicy. The cue point policy determines which BCOVCuePoints will get processed by the BCOVIMAAdsRequestPolicy.  The VAST policies are as follows:

* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:` This method returns an ads request policy that checks each BCOVVideo for BCOVCuePoints of type 'kBCOVIMACuePointTypeAd' and looks in each of those cue points properties for the key `kBCOVIMAAdTag` to determine the VAST ad tag that should be used to request ads.

* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:`  This method returns an ad request policy that uses the specified VAST ad tag for all BCOVCuePoints of type **kBCOVIMACuePointTypeAd**. Properties of the cue point are appended to the ad tag as query parameters.

You can add VAST ad tag cuepoints to a video by using the `update:` method on the `BCOVVideo` object. The following example adds pre-roll, mid-roll and post-roll cuepoints:

```swift
extension BCOVVideo {

    func updateVideo(withVASTTag vastTag: String) -> BCOVVideo? {

        guard let durationNum = self.properties["duration"] as? NSNumber else {
            return nil
        }

        let durationMiliSeconds = durationNum.doubleValue
        let midpointSeconds = (durationMiliSeconds / 2) / 1000
        let midpointTime = CMTimeMakeWithSeconds(midpointSeconds, preferredTimescale: 1)

        let cuePointPositionTypeAfter = CMTime.positiveInfinity

        return update { (mutableVideo: BCOVMutableVideo?) in
            guard let mutableVideo = mutableVideo else {
                return
            }

            guard let preroll = BCOVCuePoint(type: kBCOVIMACuePointTypeAd, position: CMTime.zero, properties: [kBCOVIMAAdTag:vastTag]),
                  let midroll = BCOVCuePoint(type: kBCOVIMACuePointTypeAd, position: midpointTime, properties: [kBCOVIMAAdTag:vastTag]),
                  let postroll = BCOVCuePoint(type: kBCOVIMACuePointTypeAd, position: cuePointPositionTypeAfter, properties: [kBCOVIMAAdTag:vastTag]) else {
                return
            }

            mutableVideo.cuePoints = BCOVCuePointCollection(array: [
                preroll,
                midroll,
                postroll
            ])
        }
        
    }
}
```

You can also convert cuepoints created in VideoCloud to VAST cuepoints like this:

```swift
func update(video: BCOVVideo) -> BCOVVideo {
    guard let cuePoints = video.cuePoints?.array() as? [BCOVCuePoint] else {
        return video
    }
    var updatedCuepoints = [BCOVCuePoint]()
    for cuePoint in cuePoints {
        // The "metadata" property is the value of the
        // "Key/Value Pairs" field in VideoCloud when
        // creating/editing a Cue Point. In this example
        // the only value in this field is the VAST ad tag URL.
        // If you have additional values you'll need to parse
        // out just the ad tag URL.
        guard let metadata = cuePoint.properties["metadata"] as? String else {
            continue
        }
        let properties = [
            kBCOVIMAAdTag: metadata
        ]
        if let updatedCuePoint = BCOVCuePoint(type: cuePoint.type,
                                              position: cuePoint.position,
                                              properties: properties) {
            updatedCuepoints.append(updatedCuePoint)
        }
    }
    return video.update { (mutableVideo: BCOVMutableVideo?) in
        guard let mutableVideo else {
            return
        }
        mutableVideo.cuePoints = BCOVCuePointCollection(array: updatedCuepoints)
    }
}
```

### Modifying the IMAAdsRequest


The IMA Plugin passes an `IMAAdsRequest` object to a `BCOVIMAPlaybackSessionDelegate` immediately before calling `IMAAdsLoader -requestAdsWithAdsRequest`, allowing the user to first modify the ads request. To receive the ads request callback, create an object that implements the `BCOVIMAPlaybackSessionDelegate` protocol.

```swift
import BrightcovePlayerSDK
import BrightcoveIMA
import GoogleInteractiveMediaAds

class ViewController: UIViewController, BCOVIMAPlaybackSessionDelegate
```

Create a `BCOVIMASessionProvider` using either `createIMAPlaybackControllerWithSettings` or `createIMASessionProviderWithSettings`, and provide an NSDictionary of options with an entry having a key of `kBCOVIMAOptionIMAPlaybackSessionDelegateKey` and a value which is your delegate.

```swift
let imaSessionProviderOptions = [
    kBCOVIMAOptionIMAPlaybackSessionDelegateKey: self
]

let imaSessionProvider = sdkManager.createIMASessionProvider(with: imaSettings,
                                                             adsRenderingSettings: adsRenderingSettings,
                                                             adsRequestPolicy: adsRequestPolicy,
                                                             adContainer: playerView.contentOverlayView,
                                                             viewController: self,
                                                             companionSlots: nil,
                                                             upstreamSessionProvider: nil,
                                                             options: imaSessionProviderOptions)
```

Implement `willCallIMAAdsLoaderRequestAdsWithRequest:forPosition:` in your `BCOVIMAPlaybackSessionDelegate`.

```swift
func willCallIMAAdsLoaderRequestAds(with adsRequest: IMAAdsRequest,
                                    forPosition position: TimeInterval) {
    adsRequest.vastLoadTimeout = 3000.0
}
```
    
### Composing Session Providers

If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `sdkManager.createPlaybackController(withSessionProvider:,viewStrategy:)` method.

When composing session providers, the session preloading can be enabled from [`BCOVBasicSessionProvider`][basicprovider]; however, preloading sessions with IMA plugin is **strongly discouraged** due to a bug in the Google IMA SDK when having multiple AVPlayers in memory.

[basicprovider]: https://github.com/brightcove/brightcove-player-sdk-ios/blob/master/ios/BrightcovePlayerSDK.framework/Headers/BrightcovePlayerSDK-Swift.h#L407-L420

## Registering Ad Overlays

If you are placing any views over ads while they are playing, it is necceessary to register those views with the IMA SDK. Read the **Friendly obstructions** section of the [Open Measurement in the IMA SDK](https://developers.google.com/interactive-media-ads/docs/sdks/ios/omsdk) page for more information.

You can get the current IMAAdDisplayContainer object neccessary to register your overlays from the `playbackController:playbackSession:didEnterAdSequence:` delegate method of your BCOVPlaybackController instance. For example:

```swift
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didEnter adSequence: BCOVAdSequence) {

    let props = session.video.properties
    if let adDisplayContainer = props[kBCOVDAIVideoPropertiesKeyAdDisplayContainer] as? IMAAdDisplayContainer{
        adDisplayContainer.register(adOverlayView)
    }

}
```

To unregister the obstructions when the ad sequence is finished, the `playbackController:playbackSession:didExitAdSequence:` delegate method of your BCOVPlaybackController instance can be used. For example:

```swift
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didExitAdSequence adSequence: BCOVAdSequence) {
    let props = session.video.properties
    if let adDisplayContainer = props[kBCOVDAIVideoPropertiesKeyAdDisplayContainer] as? IMAAdDisplayContainer{
        adDisplayContainer.unregisterAllFriendlyObstructions()
    }
}
```

## AirPlay

**Brightcove does not offer support for using AirPlay with the IMA plugin.** Google’s IMA SDK [does not currently support AirPlay](https://developers.google.com/interactive-media-ads/docs/sdks/ios/client-side/compatibility#apple-airplay-support), and the usage of AirPlay with the IMA plugin is not recommended at this time. There are likely to be unexpected behaviors and bugs for which no solution exists.

However, if you wish to use this functionality in your apps set `enableBackgroundPlayback` to `YES` on `IMASettings` along with enabling AirPlay on your `BCOVPlaybackController`. See the "_AirPlay_" section of the [Core SDK README](https://github.com/brightcove/brightcove-player-sdk-ios#airplay) for more information.

## Picture-in-Picture

The Brightcove IMA plugin can be used with Brightcove's Picture-in-Picture support.

Ads can not be started if Picture-in-Picture is active, however if an ad is playing when Picture-in-Picture begins it will continue playing in the Picture-in-Picture window. Any VAST cue-points encountered after Picture-in-Picture has become active will be skipped. This is a limitation with the Google IMA SDK, see [IMA SDK for iOS > Picture-in-picture](https://developers.google.com/interactive-media-ads/docs/sdks/ios/client-side/picture_in_picture#starting_ads) for more information.

Supported configurations are:
* VAST with pre, mid and/or post-rolls.
* VMAP with a pre-roll.

Set `enableBackgroundPlayback` to `YES` on `IMASettings` and see the "_Picture in Picture_" section of the [Core SDK README](https://github.com/brightcove/brightcove-player-sdk-ios#picture-in-picture) for more information.

If you are using a VAST configuration you will need to use this `BCOVCuePointProgressPolicy`:

```swift
let policy = BCOVCuePointProgressPolicy(processingCuePoints: .processFinalCuePoint,
                                        resumingPlaybackFrom: .fromLastProcessedCuePoint,
                                        ignoringPreviouslyProcessedCuePoints: true)
```

## Audience Segment Targeting

If you'd like to use Audience Segment Targeting with your IMA VAST ad requests you can do so by using the `updateAudienceSegmentTargetingValues` on `BCOVPlaybackController`. For example:

```swift
playbackController.updateAudienceSegmentTargetingValues([
    "account_id": "11223344",
    "account_type": "premium"
])
```

These values will be appended to the `cust_params` query paramater of the IMA ad request URL. For example:

The URL `http://pubads.g.doubleclick.net/gampad/ads` would become `http://pubads.g.doubleclick.net/gampad/ads?cust_params=account_id%3D11223344%26account_type%3Dpremium`.

## AVPlayerViewController Support

### Displaying Ad UI

If you'd like to display your own Ad UI during ad playback you can use the `playbackController:playbackSession:didReceiveLifecycleEvent:` delegate method. Here is an example:

```swift
// MARK: BCOVPlaybackControllerDelegate
func playbackController(_ controller: BCOVPlaybackController,
                        playbackSession session: BCOVPlaybackSession,
                        didReceive lifecycleEvent: BCOVPlaybackSessionLifecycleEvent) {

    if kBCOVDAILifecycleEventAdsManagerDidReceiveAdEvent == lifecycleEvent.eventType {
        if let adEvent = lifecycleEvent.properties["adEvent"] as? IMAAdEvent {
            switch adEvent.type {
                case .STARTED:
                    displayAdUI()
                case .COMPLETE:
                    hideAdUI()
                default:
                    break
            }
        }
    }
    
}
```

##  Frequently Asked Questions

### I can hear the ads, but I can't see them playing.

This usually happens when the ad container view is not in the view hierarchy, or when the ad view (which is a subview of the ad container view) is covered by other views.

## Support

If you have questions, need help or want to provide feedback, please use the [Support Portal](https://supportportal.brightcove.com/s/login/) or contact your Account Manager.  To receive notification of new SDK software releases, subscribe to the Brightcove Native Player SDKs [Google Group](https://groups.google.com/g/brightcove-native-player-sdks).

