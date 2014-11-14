# BCOVIMA Plugin for Brightcove Player SDK for iOS, version 1.1.1.453

Requirements
============
This plugin supports iOS 6.1+.

Installation
============
You can use [Cocoapods][cocoapods] to add the IMA Plugin for Brightcove Player SDK to your project.  You can find the latest `Brightcove-Player-SDK-IMA` podspec [here][podspecs].

To add the IMA Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
1. Download the latest zip'ed release of the plugin from our [release page][release].
1. Add the contents of Library and Headers to the project.
1. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `libBCOVIMA.a`
1. On the "Build Settings" tab of your application target:
    * Ensure that BCOVIMA headers are in your application's "Header Search Path".
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
1. Install Google's IMA library 3.0b9, following their [directions][googleima].

[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/CocoaPods/Specs/tree/master/Specs/Brightcove-Player-SDK-IMA
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases

Quick Start
===========
BCOVIMA is a bridge between [Google's IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. Its use is relatively straightforward. This example uses Server Side Ad Rules.

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

[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[SampleBCOVIMA]: https://github.com/BrightcoveOS/SampleBCOVIMA/blob/master/SampleBCOVIMA/ViewController.m#L172-L221
[forum]: https://groups.google.com/forum/#!forum/brightcove-native-player-sdks

Customizing Plugin Behavior
===========================
There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

VAST and VMAP/Server Side Ad Rules
--------------------------
By default, BCOVIMA is configured to support VMAP or Server Side Ad Rules. As of 1.1, BCOVIMA also supports using VAST ad tags in conjunction with BCOVCuepoints.

In order to use VAST, you must retrieve a `BCOVIMASessionProviderOptions` object from the `+[BCOVIMASessionProviderOptions VASTOptions]` method and use it to create a `BCOVPlaybackSessionProvider` from `-[BCOVPlayerSDKManager createIMASessionProviderWithSettings:adsRenderingSettings:upstreamSessionProvider:]`.

```objc

        IMASettings *imaSettings = [[IMASettings alloc] init];
        imaSettings.ppid = kViewControllerIMAPublisherID;
        imaSettings.language = kViewControllerIMALanguage;

        IMAAdsRenderingSettings *renderSettings = [[IMAAdsRenderingSettings alloc] init];
        renderSettings.webOpenerPresentingController = self;

   [1]  BCOVIMASessionProviderOptions *sessionProviderOption = [BCOVIMASessionProviderOptions VASTOptions];

        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
   [2]  id<BCOVPlaybackSessionProvider> playbackSessionProvider =
                [manager createIMASessionProviderWithSettings:imaSettings
                        adsRenderingSettings:renderSettings upstreamSessionProvider:nil
                        options:sessionProviderOption];

   [3] id<BCOVPlaybackController> playbackController =
                [manager createPlaybackControllerWithSessionProvider:playbackSessionProvider
                        viewStrategy:[manager BCOVIMAAdViewStrategy]];

```

Let's break this code down into steps, to make it a bit simpler to digest:

1. BCOVIMA allows you to customize the plugin behavior by creating a BCOVIMASessionProviderOptions.  In this case, use the `-VASTOptions` method to use VAST.  By default, these options will look for an ad tag in each BCOVCuePoint.
1. BCOVIMA adds a category method to the BCOVPlaybackManager.  One of these methods is `-createIMASessionProviderWithSettings:adsRenderingSettings:upstreamSessionProvider:options`. Use this method to create a IMA session provider.
1. Create the playback controller with the IMA session provider.

BCOVIMA gives you a lot of control over how ads requests are made, via the BCOVIMAAdsRequestPolicy class. Once you obtain an instance of the policy (there are class factory methods on BCOVIMAAdsRequestPolicy for this purpose), you specify it in the BCOVIMASessionProviderOptions that you pass to the BCOVPlayerSDKManager when creating a BCOVIMASessionProvider.

There are three factory methods for VMAP are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads.
* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithCompanionAdSlots:`: This method returns an ads request policy which behaves like `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`, but also populates each ads request with the specified IMACompanionAdSlots. For more information on using IMACompanionAdSlots, see [Google's IMA iOS SDK documentation][companiondocs].
* `+adsRequestPolicyWithVMAPAdTagUrl:companionAdSlots:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

There are four factory methods for VAST.  All of the VAST methods take a BCOVCuePointProgressPolicy. The cue point policy determines which BCOVCuePoints will get processed by the BCOVIMAAdsRequestPolicy.  The VAST policies are as follows:

* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:` This method returns an ads request policy that checks each BCOVVideo for BCOVCuePoints of type 'kBCOVIMACuePointTypeAd' and looks in each of those cuepoints properties for the key `kBCOVIMAAdTag` to determine the VAST ad tag that should be used to request ads.
* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:companionAdSlots` This method returns an ads request policy which behaves like `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy`, but also populates each ads request with the specified IMACompanionAdSlots. For more information on using IMACompanionAdSlots, see [Google's IMA iOS SDK documentation][companiondocs].
* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:`  This method returns an ad request policy that uses the specified VAST ad tag for all BCOVCuePoints of type 'kBCOVIMACuePointTypeAd'. Properties of the cue point are appended to the ad tag as query parameters.
* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:companionAdSlots` This method returns an ads request policy which behaves like `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:`, but also populates each ads request with the specified IMACompanionAdSlots.

[companiondocs]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/ads#companions

View Strategy
-------------
You can provide a custom view strategy to the BCOVPlaybackManager when you are constructing your playback controller or session provider, rather than specify the BCOVIMAAdViewStrategy directly. However, your custom view strategy must compose the BCOVIMAAdViewStrategy. In other words, at some point your view strategy must pass the `view` that it is given to the BCOVIMAAdViewStrategy, and the view that the strategy gives you must be composed into the view hierarchy that your strategy returns. You can see an example of what this looks like in the [SampleBCOVIMA example code][SampleBCOVIMA2].

[SampleBCOVIMA2]: https://github.com/BrightcoveOS/SampleBCOVIMA/blob/master/SampleBCOVIMA/ViewController.m#L248-L280

Composing Session Providers
---------------------------
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategyWithFrame:]` method. You can see an example of what this looks like in the [SampleBCOVIMAWidevine example code][SampleBCOVIMAWidevine].

[SampleBCOVIMAWidevine]: https://github.com/BrightcoveOS/SampleBCOVIMAWidevine/blob/master/SampleBCOVIMAWidevine/ViewController.m#L88-L113


Frequently Asked Questions
==========================

** I can hear the ads, but I can't see them playing. **

This usually happens when you have not properly composed `-[BCOVPlayerSDKManager BCOVIMAAdViewStrategy]` into the view strategy you passed when creating the playback controller.
