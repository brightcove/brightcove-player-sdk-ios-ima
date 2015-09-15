# BCOVIMA Plugin for Brightcove Player SDK for iOS, version 1.5.0.537

Requirements
============
This plugin supports iOS 7.0+.

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
1. Install Google's IMA library 3.0b16, following their [directions][googleima].

[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/CocoaPods/Specs/tree/master/Specs/Brightcove-Player-SDK-IMA
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases

Quick Start
===========
BCOVIMA is a bridge between [Google's IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. Its use is relatively straightforward. This example uses Server Side Ad Rules.

    [1] IMASettings *imaSettings = [[IMASettings alloc] init];
        imaSettings.ppid = kViewControllerIMAPublisherID;
        imaSettings.language = kViewControllerIMALanguage;

        IMAAdsRenderingSettings *renderSettings = [[IMAAdsRenderingSettings alloc] init];
        renderSettings.webOpenerPresentingController = self;
    
        UIView *videoContainerView = <UIView of video container>;
        NSString *adTag = <ad tag>;
    
    [2] BCOVIMAAdsRequestPolicy *adsRequestPolicy = [BCOVIMAAdsRequestPolicy adsRequestPolicyWithVMAPAdTagUrl:adTag];

        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
        id<BCOVPlaybackController> controller =
    [3]         [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
                        adContainer:videoContainerView 
                        companionSlots:nil
                        viewStrategy:[manager defaultControlsViewStrategy]];
        controller.delegate = self;

        [videoContainerView addSubview:controller.view];  

        NSString *token;      // (Brightcove Media API token with URL access)
        NSString *playlistID; // (ID of the playlist you wish to use)
        BCOVCatalogService *catalog = [[BCOVCatalogService alloc] initWithToken:token];
        [catalog findPlaylistWithPlaylistID:playlistID
                                 parameters:nil
                                 completion:^(BCOVPlaylist *playlist,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {

            [controller setVideos:playlist];
            [controller play];

        }];

Let's break this code down into steps, to make it a bit simpler to digest:

1. You create the same IMA settings, ads rendering settings that you would create if you were using Google's IMA iOS SDK directly, and these are required.
1. BCOVIMAAdsRequestPolicy provides methods to specify VAST or VMAP/Server Side Ad Rules. Select the appropriate method to select your ads policy.
1. BCOVIMA adds some category methods to BCOVPlaybackManager. The first of these is `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:adContainer:companionSlots:viewStrategy:`. Use this method to create your playback controller.

If you have questions or need help, we have a support forum for Brightcove's native Player SDKs at [https://groups.google.com/forum/#!forum/brightcove-native-player-sdks][forum] .

[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[forum]: https://groups.google.com/forum/#!forum/brightcove-native-player-sdks

Customizing Plugin Behavior
===========================
There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

VAST and VMAP/Server Side Ad Rules
--------------------------

BCOVIMA gives you control over how ads requests are made, via the `BCOVIMAAdsRequestPolicy` class. The class provides factory methods for the supported policies. Once you obtain an instance of the correct policy, you need to provide it to the `BCOVPlayerSDKManager` to create a playback controller or a playback session provider. 

In Quick Start, an example of VMAP is gaven. Here is a VAST example.

        IMASettings *imaSettings = [[IMASettings alloc] init];
        imaSettings.ppid = kViewControllerIMAPublisherID;
        imaSettings.language = kViewControllerIMALanguage;

        IMAAdsRenderingSettings *renderSettings = [[IMAAdsRenderingSettings alloc] init];
        renderSettings.webOpenerPresentingController = self;

        UIView *videoContainerView = <UIView of video container>;
    
    [1] BCOVIMAAdsRequestPolicy *adsRequestPolicy = [BCOVIMAAdsRequestPolicy adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:nil];

        BCOVPlayerSDKManager *manager = [BCOVPlayerSDKManager sharedManager];
        id<BCOVPlaybackController> controller =
                [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
                        adContainer:videoContainerView
                        companionSlots:nil
                        viewStrategy:[manager defaultControlsViewStrategy]];
    
Let's break this code down into steps, to make it a bit simpler to digest:

1. This example is the same as the one provided in the quickstart, except that we are now calling a different `BCOVIMAAdsRequestPolicy` policy method to specify that we want to use VAST.

BCOVIMAAdsRequestPolicy has four factory methods to generate ads request policy; two for VMAP and two for VAST. Two factory methods for VMAP are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads.

* `+adsRequestPolicyWithVMAPAdTagUrl:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

There are two factory methods for VAST.  All of the VAST methods take a BCOVCuePointProgressPolicy. The cue point policy determines which BCOVCuePoints will get processed by the BCOVIMAAdsRequestPolicy.  The VAST policies are as follows:

* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:` This method returns an ads request policy that checks each BCOVVideo for BCOVCuePoints of type 'kBCOVIMACuePointTypeAd' and looks in each of those cuepoints properties for the key `kBCOVIMAAdTag` to determine the VAST ad tag that should be used to request ads.

* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:`  This method returns an ad request policy that uses the specified VAST ad tag for all BCOVCuePoints of type **kBCOVIMACuePointTypeAd**. Properties of the cue point are appended to the ad tag as query parameters.

[companiondocs]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/quickstart

View Strategy
-------------
You can provide a custom view strategy to the BCOVPlaybackManager when you are constructing your playback controller or session provider, rather than specify the defaultControlsViewStrategy directly. With a custom view strategy, the ad container view and ad companion slots can be tied with the video content view. This is an example of custom view strategy.

    BCOVPlaybackControllerViewStrategy customViewStrategy = ^UIView* (UIView *view, id<BCOVPlaybackController> playbackController){
        
        BCOVPlaybackControllerViewStrategy defaultControlsViewStrategy = [playbackManager defaultControlsViewStrategy];
        UIView *contentAndDefaultControlsView = defaultControlsViewStrategy(view, playbackController);
        
        [<UIView of video container> addSubview:contentAndDefaultControlsView];
        
        return <UIView of video container>;
    };
    
Composing Session Providers
---------------------------
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategy:]` method. You can see an example of what this looks like in the [BasicIMAWidevinePlayer example code][wv-ima-sample].

When composing session providers, the session preloading can be enabled from [`BCOVBasicSessionProvider`][basicprovider]; however, preloading sessions with IMA plugin is **strongly discouraged** due to a bug in the Google IMA library when having multiple AVPlayers in memory.

[wv-ima-sample]: https://github.com/BrightcoveOS/ios-player-samples/blob/master/IMA%2BWidevine/BasicIMAWidevinePlayer/objc/BasicIMAWidevinePlayer/ViewController.m#L63-L97
[basicprovider]: https://github.com/brightcove/brightcove-player-sdk-ios/blob/master/Headers/BCOVBasicSessionProvider.h#L31-L46

Frequently Asked Questions
==========================

** I can hear the ads, but I can't see them playing. **

This usually happens when either ad container view is not in view hierarchy, or ad view (which is subview of ad container view) is covered by other views.
