# BCOVIMA Plugin for Brightcove Player SDK for iOS, version 1.0.7.211

Installation
============
The easiest way to get started with BCOVIMA is to install using [CocoaPods][pods]. Simply add the `Brightcove-Player-SDK-IMA` pod to your Podfile:

```
platform :ios, '6.1'

pod 'Brightcove-Player-SDK-IMA'
```

If you don't use CocoaPods, you can download BCOVIMA from the [release page][releases] and manually install the archive and header files into your Xcode project manually, as you would with any other static library. You must also manually install [Google's IMA iOS SDK v3][googleima], following their installation instructions.

Quick Start
===========
BCOVIMA is a bridge between [Google's IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. Currently (as of 2014-05-16) it works only with server-side ad rules. Its use is relatively straightforward:

```objc

    [1] IMASettings *imaSettings = [[IMASettings alloc] init];
        imaSettings.ppid = kViewControllerIMAPublisherID;
        imaSettings.language = kViewControllerIMALanguage;
    
        IMAAdsRenderingSettings *renderSettings = [[IMAAdsRenderingSettings alloc] init];
        renderSettings.webOpenerPresentingController = self;
    
        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
        id<BCOVPlaybackController> controller =
    [2]         [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
    [3]                 viewStrategy:[manager BCOVIMAAdViewStrategy]];
        controller.delegate = self;

        [self.view addSubview:controller.view];  
    
        NSString *token;      // (Brightcove Media API token with URL access)
        NSString *playlistID; // (ID of the playlist you wish to use)
        BCOVCatalogService *catalog = [[BCOVCatalogService alloc] initWithToken:token];
        [catalog findPlaylistWithPlaylistID:playlistID
                                 parameters:nil
                                 completion:^(BCOVPlaylist *playlist,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {
    
    [4]     BCOVPlaylist *playlistWithAdTags = myUpdatePlaylistWithAdTags(playlist);
            [controller setVideos:playlist];
            [controller play];
    
        }];

```

Let's break this code down into steps, to make it a bit simpler to digest:

1. You create the same IMA settings and ads rendering settings that you would create if you were using Google's IMA iOS SDK directly, and these are required.
1. BCOVIMA adds some category methods to BCOVPlaybackManager. The first of these is `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:viewStrategy:`. Use this method to create your playback controller. 
1. The second category method added to BCOVPlaybackManager is `-BCOVIMAAdViewStrategy`. You must either pass this view strategy into the method you call to create a playback controller, or compose it into your own view strategy. If you don't include this view strategy, you won't see any ads.
1. Each BCOVVideo passed to the `-[BCOVPlaybackController setVideos:]` method (for which you wish to show ads) must have a property whose name is `kBCOVIMAAdTag`, and whose value is your ad tag. For brevity, this example does this in a helper function called `myUpdatePlaylistWithAdTags()`, but the [sample code shows in detail how this is done][SampleBCOVIMA].

If you have questions or need help, we have a support forum for Brightcove's native Player SDKs at [https://groups.google.com/forum/#!forum/brightcove-native-player-sdks][forum] .

[releases]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases
[pods]: http://cocoapods.org
[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[SampleBCOVIMA]: https://github.com/BrightcoveOS/SampleBCOVIMA/blob/master/SampleBCOVIMA/ViewController.m#L175-L202
[forum]: https://groups.google.com/forum/#!forum/brightcove-native-player-sdks

Customizing Plugin Behavior
===========================
There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

View Strategy
-------------
You can provide a custom view strategy to the BCOVPlaybackManager when you are constructing your playback controller or session provider, rather than specify the BCOVIMAAdViewStrategy directly. However, your custom view strategy must compose the BCOVIMAAdViewStrategy. In other words, at some point your view strategy must pass the `view` that it is given to the BCOVIMAAdViewStrategy, and the view that the strategy gives you must be composed into the view hierarchy that your strategy returns. You can see an example of what this looks like in the [SampleBCOVIMA example code][SampleBCOVIMA2].

[SampleBCOVIMA2]: https://github.com/BrightcoveOS/SampleBCOVIMA/blob/master/SampleBCOVIMA/ViewController.m#L246-L268
Composing Session Providers
---------------------------
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategyWithFrame:]` method. You can see an example of what this looks like in the [SampleBCOVIMAWidevine example code][SampleBCOVIMAWidevine].

[SampleBCOVIMAWidevine]: https://github.com/BrightcoveOS/SampleBCOVIMAWidevine/blob/master/SampleBCOVIMAWidevine/ViewController.m#L101-L108

Ads Request Policy
------------------
BCOVIMA gives you a lot of control over how ads requests are made, via the BCOVIMAAdsRequestPolicy class. The primary use case for using a specific policy is to use companion ads. Once you obtain an instance of the policy (there are three class factory methods on BCOVIMAAdsRequestPolicy for this purpose), you specify it in the BCOVIMASessionProviderOptions that you pass to the BCOVPlayerSDKManager when creating a BCOVIMASessionProvider. The three factory methods are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads. 
* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithCompanionAdSlots:`: This method returns an ads request policy which behaves like `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`, but also populates each ads request with the specified IMACompanionAdSlots. For more information on using IMACompanionAdSlots, see [Google's IMA iOS SDK documentation][companiondocs].
* `+adsRequestPolicyWithVMAPAdTagUrl:companionAdSlots:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

[companiondocs]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/ads#companions

Frequently Asked Questions
==========================
** I'm trying to use a VAST ad tag, but I don't see any ads. What gives? **

BCOVIMA currently (as of 2014-05-16) works only with ad tags that return server-side ad rules. For example, VMAP or server-side ad rules for DFP.

** I can hear the ads, but I can't see them playing. **

This usually happens when you have not properly composed `-[BCOVPlayerSDKManager BCOVIMAAdViewStrategy]` into the view strategy you passed when creating the playback controller.
