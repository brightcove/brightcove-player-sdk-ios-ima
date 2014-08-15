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
