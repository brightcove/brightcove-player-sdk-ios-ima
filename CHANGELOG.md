# 1.5.0
### Additions and Improvements
* Qualified with Google IMA 3.0.0b16. As of beta13, IMA is now a static framework. You will need to remove `#import` references to IMA headers and instead use a module `@import GoogleInteractiveMediaAds;`. Make sure this import occurs **before** importing `BCOVIMA.h`.

### Additions and Improvements
* Fix a bug where VAST postrolls were loading but not playing.

# 1.4.0
### Breaking Changes
* Undocumented lifecycle events such as `kBCOVPlaybackSessionLifecycleEventIMAAdEnter` and `kBCOVPlaybackSessionLifecycleEventIMAAdEnter` have been removed.
* IMA imports in SDK public headers have been removed in anticipation of moving to new IMA library versions that use modules. That means that you will need to import `IMAAdsLoader.h` in your header files.

### Additions and Improvements
* Updated to support Brightcove Analytics V2.
* You must use the Brightcove Player SDK for iOS version 4.4.0+.

# 1.3.0
### Breaking Changes
* `BCOVIMAAdsRequestPolicy` methods have been modified and no longer take a `IMAAdDisplayContainer` object.

| Removed Method (BCOVIMAAdsRequestPolicy)                                                        | Replaced By (BCOVIMAAdsRequestPolicy)                                         |
| ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------- |
| `+videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithAdDisplayContainer:`                           | `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy:`                               |
| `+adsRequestPolicyWithVMAPAdTagUrl:adDisplayContainer:`                                         | `+adsRequestPolicyWithVMAPAdTagUrl:`                                          |
| `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:adDisplayContainer:`    | `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:`     |
| `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:adDisplayContainer:`| `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:` |

For more information on these replacement methods, please see the readme.

* `BCOVPlayerSDKManager` have been modified to take to accommodate an ad view and companion slots.

| Removed Method (BCOVPlayerSDKManager)                                                                  | Replaced By (BCOVPlayerSDKManager)                                                                                                |
| ------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------- |
| `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:viewStrategy:`         | `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:adContainer:companionSlots:viewStrategy:`         |
| `-createIMASessionProviderWithSettings:adsRenderingSettings:adsRequestPolicy:upstreamSessionProvider:` | `-createIMASessionProviderWithSettings:adsRenderingSettings:adsRequestPolicy:adContainer:companionSlots:upstreamSessionProvider:` |

### Additions and Improvements
* Qualified with Google IMA 3.0.0b12.
* Fixed a bug where ads were not visible after the first video in a playlist.
* Fixed a bug that could break frequency capping.

# 1.2.0
### Breaking Changes
* Qualified with Google IMA 3.0.0b10. In this release, Google switched from using a UIWebView to a WKWebView in iOS 8 for its ad processing. This means that you **must** build your app with an arm64 slice. It also means that if you aren't using Cocoapods, you must **optionaly** link against WebKit.framework.
* `BCOVIMASessionProviderOptions` has been removed. This class exposed no functionality externally.
* `+[BCOVPlayerSDKManager BCOVIMAAdViewStrategy]` and `-[BCOVPlayerSDKManager IMAAdViewStrategyWrapperWithViewStrategey:]` have been removed. Views in which ads are to be rendered are now part of the initialization methods. Please consult the readme for more information.
* `BCOVPlayerSDKManager` manager methods have been modified. Previously, the plugin defaulted to supporting VMAP/Server Side Ad Rules and you had to optionaly configure it to support VAST/Cue Points. With this change, you **must** now specificy your desired ad request policy. If you had been using VMAP/SSA by default, you can use `[BCOVIMAAdsRequestPolicy videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithAdDisplayContainer:]` or `[BCOVIMAAdsRequestPolicy adsRequestPolicyWithVMAPAdTagUrl:adDisplayContainer:]`. Please consult the readme for more information.

### Additions and Improvements
* You can now specify the view in which ads will be rendered. You can now pass the IMAAdDisplayContainer into the session provider.

# 1.1.1
### Breaking Changes
* iOS 6 is still deprecated in this release. We have not removed support yet.  iOS 6.x currently accounts for ~2% of global SDK traffic.
* This release has been built with Xcode 6. In Xcode 6, Apple removed armv7s from the list of standard architectures. This release no longer includes an armv7s architecture slice.

### Additions and Improvements
* Qualified with Google IMA 3.0.0b9.
* Internal improvements.


# 1.1.0

### Breaking Changes
* Use of ReactiveCocoa in public APIs within the BCOVIMA Plugin is removed in this release.

### Additions and Improvements
* VAST is now supported. Please see the README.md for more information.
* Upgraded to Google IMA SDK 3.0b7. This release resolves a problem that causes slow timer countdowns on iOS 8. We are working with Google on resolving some known issues in IMA SDK 3.0b7.  If you run into any issues in beta7, please let us know. The known issues include:
    * Learn more button doesn't show up on ads that are shorter than 6 seconds.
    * Sometimes ads stay in memory even after ads are finished.
    * Lock screen and quick look controls may be taken over by ad player.
    * The countdown timer isn't in the correct location.
    * Ads sometime loop if headphones are removed from device during an ad.

# 1.0.8

### Breaking Changes
* Use of ReactiveCocoa in public APIs within the BCOVIMA Plugin is deprecated in this release. Version 1.1.0 will not require clients to install any version of ReactiveCocoa, and properties or methods that return or expect RACSignal objects will be removed. See the header files for guidance on how to update deprecated functionality for compatibility with 1.1.0. The only deprecated method in the BCOVIMA Plugin is:
  * `-[BCOVIMAAdsRequestPolicy signalWithValue:]`

### Additions and Improvements
* Send `kBCOVIMALifecycleEventAdsManagerDidRequestContentPause` and `kBCOVIMALifecycleEventAdsManagerDidRequestContentResume` when the IMAAdsManager requests that the content be paused or resumed.
* Fixed a bug in which the `kBCOVPlaybackSessionLifecycleEventEnd` event could be sent prematurely when using server-side ad rules and no postroll was scheduled to be shown.

# 1.0.7

### Additions and Improvements
* Added an arm-64 architecture slice to the static library, for applications that wish to target the 64-bit architecture.
* Upgraded to Google IMA SDK 3.0b5
* Added a subspec for users of Admob.  Use pod `Brightcove-Player-SDK-IMA/ForAdMob`.


# 1.0.6

### Additions and Improvements
* Fixed a bug that would prevent `kBCOVIMALifecycleEventAdsLoaderLoaded` or `kBCOVIMALifecycleEventAdsLoaderFailed` lifecycle events from being delivered.
* Fixed a retain cycle that could prevent playback sessions from being released.
* Addressed an issue where a small amount of content could sometimes begin playing prior to the first preroll ad for any given video.

# 1.0.5

### Additions and Improvements
* Fixed a bug that caused IMAAdsRenderingSettings properties to be nil'ed out when advancing to a new session.
