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
