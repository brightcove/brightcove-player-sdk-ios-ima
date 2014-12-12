# BCOVIMA Plugin for Brightcove Player SDK for iOS, version 1.2.0.477

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
        
        IMAAdDisplayContainer *adDisplayContainer = [[IMAAdDisplayContainer alloc] initWithAdContainer:<UIView of AdContainer> companionSlots:nil];

    
    [2] BCOVIMAAdsRequestPolicy *adsRequestPolicy = [BCOVIMAAdsRequestPolicy videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithAdDisplayContainer:adDisplayContainer];

        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
        id<BCOVPlaybackController> controller =
    [3]         [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
    [4]                 viewStrategy:[manager defaultControlsViewStrategy]];
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

    [5]     BCOVPlaylist *playlistWithAdTags = myUpdatePlaylistWithAdTags(playlist);
            [controller setVideos:playlist];
            [controller play];

        }];

```

Let's break this code down into steps, to make it a bit simpler to digest:

1. You create the same IMA settings, ads rendering settings, and **Ad display Container** that you would create if you were using Google's IMA iOS SDK directly, and these are required.
1. BCOVIMAAdsRequestPolicy provides methods to specify VAST or VMAP/Server Side Ad Rules. Select the appropriate method to select your ads policy.
1. BCOVIMA adds some category methods to BCOVPlaybackManager. The first of these is `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:viewStrategy:`. Use this method to create your playback controller.
1. You must either pass this view strategy into the method you call to create a playback controller, or compose it into your own view strategy. If you don't include this view strategy, you won't see any ads.
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

BCOVIMA gives you control over how ads requests are made, via the `BCOVIMAAdsRequestPolicy` class. The class provides factory methods for the supported policies. Once you obtain an instance of the correct policy, you need to provide it to the `BCOVPlayerSDKManager` to create a playback controller or a playback session provider. 

In Quick Start, an example of VMAP is gaven. Here is a VAST example.

```objc

        IMASettings *imaSettings = [[IMASettings alloc] init];
        imaSettings.ppid = kViewControllerIMAPublisherID;
        imaSettings.language = kViewControllerIMALanguage;

        IMAAdsRenderingSettings *renderSettings = [[IMAAdsRenderingSettings alloc] init];
        renderSettings.webOpenerPresentingController = self;
        
        IMAAdDisplayContainer *adDisplayContainer = [[IMAAdDisplayContainer alloc] initWithAdContainer:<UIView of AdContainer> companionSlots:nil];

    
    [1] BCOVIMAAdsRequestPolicy *adsRequestPolicy = [BCOVIMAAdsRequestPolicy adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:nil adDisplayContainer:adDisplayContainer];

        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
        id<BCOVPlaybackController> controller =
                [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
                        viewStrategy:[manager defaultControlsViewStrategy]];
    
```
Let's break this code down into steps, to make it a bit simpler to digest:

1. This example is the same as the one provided in the quickstart, except that we are now calling a different `BCOVIMAAdsRequestPolicy` policy method to specify that we want to use VAST.

BCOVIMAAdsRequestPolicy has four factory methods to generate ads request policy; two for VMAP and two for VAST. Two factory methods for VMAP are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithAdDisplayContainer:`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads with the specified IMAAdDisplayContainer. For more information on using IMAAdDisplayContainer, see [Google's IMA iOS SDK documentation][companiondocs].

* `+adsRequestPolicyWithVMAPAdTagUrl:adDisplayContainer:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

There are two factory methods for VAST.  All of the VAST methods take a BCOVCuePointProgressPolicy. The cue point policy determines which BCOVCuePoints will get processed by the BCOVIMAAdsRequestPolicy.  The VAST policies are as follows:

* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:adDisplayContainer:` This method returns an ads request policy that checks each BCOVVideo for BCOVCuePoints of type 'kBCOVIMACuePointTypeAd' and looks in each of those cuepoints properties for the key `kBCOVIMAAdTag` to determine the VAST ad tag that should be used to request ads with the specified IMAAdDisplayContainer. For more information on using IMAAdDisplayContainer, see [Google's IMA iOS SDK documentation][companiondocs].

* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:adDisplayContainer:`  This method returns an ad request policy that uses the specified VAST ad tag for all BCOVCuePoints of type 'kBCOVIMACuePointTypeAd'. Properties of the cue point are appended to the ad tag as query parameters with the specified IMAAdDisplayContainer. For more information on using IMAAdDisplayContainer, see [Google's IMA iOS SDK documentation][companiondocs].

[companiondocs]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/quickstart

View Strategy
-------------
You can provide a custom view strategy to the BCOVPlaybackManager when you are constructing your playback controller or session provider, rather than specify the defaultControlsViewStrategy directly. With a custom view strategy, the ad container view and ad companion slots can be tied with the video content view. This is an example of custom view strategy.

```objc

    BCOVPlaybackControllerViewStrategy customViewStrategy = ^UIView* (UIView *view, id<BCOVPlaybackController> playbackController){
        
        BCOVPlaybackControllerViewStrategy defaultControlsViewStrategy = [playbackManager defaultControlsViewStrategy];
        UIView *contentAndDefaultControlsView = defaultControlsViewStrategy(view, playbackController);
        
   [1]  [<UIView of Ad container view> insertSubview:contentAndDefaultControlsView atIndex:0];
        
        return <UIView of Ad container view>;
    };
    
```
1. When construct view hierarchy, make sure the content view won't cover the any subviews (ad view) in ad container view. 

Composing Session Providers
---------------------------
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategyWithFrame:]` method. You can see an example of what this looks like in the [SampleBCOVIMAWidevine example code][SampleBCOVIMAWidevine].

[SampleBCOVIMAWidevine]: https://github.com/BrightcoveOS/SampleBCOVIMAWidevine/blob/master/SampleBCOVIMAWidevine/ViewController.m#L88-L113


Frequently Asked Questions
==========================

** I can hear the ads, but I can't see them playing. **

This usually happens when either ad container view is not in view hierarchy, or ad view (which is subview of ad container view) is covered by other views.
