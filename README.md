# IMA Plugin for Brightcove Player SDK for iOS, version 2.2.1.654

Supported Platforms
===================
iOS 8.0 and above.

Installation
============
The IMA Plugin for Brightcove Player SDK provides a static library framework for installation. A dynamic framework will be added in the future when Google releases a dylib version of IMA.

The IMA plugin current supports only version 3.2.1 of Google's IMA library.

CocoaPods
--------------

You can use [CocoaPods][cocoapods] to add the IMA Plugin for Brightcove Player SDK to your project.  You can find the latest `Brightcove-Player-SDK-IMA` podspec [here][podspecs]. The pod will incorporate the correct version of IMA automatically. A subspec is also provided for those that need the AdMob version of IMA. CocoaPods 0.39 or newer is required.

Static Framework example:

    pod 'Brightcove-Player-SDK-IMA'
    
For AdMob example:

    pod 'Brightcove-Player-SDK-IMA/ForAdMob'  

Manual
--------------

To add the IMA Plugin for Brightcove Player SDK to your project manually:

1. Install the latest version of the [Brightcove Player SDK][bcovsdk].
2. Download the latest zipped release of the plugin from our [release page][release].
3. Add the 'BrightcoveIMA.framework' to your project.
4. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the path to the framework. This should have been done automatically unless the framework is stored under a different root directory than your project.
5. On the "Build Phases" tab of your application target, add the following to the "Link
    Binary With Libraries" phase:
    * `BrightcoveIMA.framework`
6. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.
7. Install Google's IMA library 3.2.1, following their [directions][googleima].

Imports
--------------
The IMA Plugin for Brightcove Player SDK can be imported into code a few different ways; `@import BrightcoveIMA;`, `#import <BrightcoveIMA/BrightcoveIMA.h>` or `#import <BrightcoveIMA/[specific class].h>`.

[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/CocoaPods/Specs/tree/master/Specs/Brightcove-Player-SDK-IMA
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases

Quick Start
===========
The BrightcoveIMA plugin is a bridge between [Google's IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. This snippet shows its basic usage with Server Side Ad Rules.

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
                        viewStrategy:nil];
        controller.delegate = self;

        [videoContainerView addSubview:controller.view];  

        NSString *accountID;  // (Brightcove Video Cloud account ID)
        NSString *policyKey;  // (Policy key associated with your account)
        NSString *playlistID; // (ID of the playlist you wish to use)
        BCOVPlaybackService *playbackService = [[BCOVPlaybackService alloc] initWithAccountId:accountID policyKey:policyKey];
        [playbackService findPlaylistWithPlaylistID:playlistID
                                 parameters:nil
                                 completion:^(BCOVPlaylist *playlist,
                                              NSDictionary *jsonResponse,
                                              NSError      *error) {

            [controller setVideos:playlist];
            [controller play];

        }];

Let's break this code down into steps to make it a bit simpler to digest:

1. Create the same IMA settings, ads rendering settings that you would create if you were using Google's IMA iOS SDK directly. These are required.
1. BCOVIMAAdsRequestPolicy provides methods to specify VAST or VMAP/Server Side Ad Rules. Select the appropriate method to select your ads policy.
1. BrightcoveIMA adds some category methods to BCOVPlaybackManager. The first of these is `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:adContainer:companionSlots:viewStrategy:`. Use this method to create your playback controller.

If you have questions or need help, we have a support forum for Brightcove's native Player SDKs at [https://groups.google.com/forum/#!forum/brightcove-native-player-sdks][forum] .

[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[forum]: https://groups.google.com/forum/#!forum/brightcove-native-player-sdks

Play and Pause
===========================
The Brightcove IMA Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension ima_play]` or `-[BCOVSessionProviderExtension ima_pause]` ([BCOVSessionProviderExtension][BCOVIMAComponent]), and not the AVPlayer.

As an example, calling play for the first time on `BCOVPlaybackController` allows BCOVIMA to process preroll ads without any of the content playing before the preroll. For more information on how BCOVIMA overrides the default `BCOVPlaybackController` methods, please check out [BCOVSessionProviderExtension][BCOVIMAComponent].

[BCOVIMAComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/blob/master/ios/BrightcoveIMA.framework/Headers/BCOVIMAComponent.h

Using the Built-In PlayerUI
===========================
If you are using version 5.1 or later of the Brightcove Player SDK, you can take advantage of the built-in player controls with the Brightcove IMA plugin.

To use the PlayerUI, create a `BCOVPlayerView`, called the player view, to contain the playback controls, the video content view, and a special view where IMA can display its ads.

**Note:** The `BrightcovePlayerUI` module is no longer needed and has been removed. (Prior to version 5.1 of the Brightcove Player SDK, the Brightcove PlayerUI plugin was a separate framework and module.) You can remove any imports that reference the Brightcove PlayerUI module. All PlayerUI headers are now found in the `BrightcovePlayerSDK` module.

Create a property in your `UIViewController` to keep track of the `BCOVPUIPlayerView`. The `BCOVPUIPlayerView` will contain both the playback controller's view, and the controls view.

	// PlayerUI's Player View
	@property (nonatomic) BCOVPUIPlayerView *playerView;

Then create your player view. This view contains both the video content view and the view that displays playback and ad controls. This setup is the same no matter what plugin you are using. Set the player view to match the video container from your layout (`videoView`) when it resizes.

    // Create and configure Control View.
    BCOVPUIBasicControlView *controlView = [BCOVPUIBasicControlView basicControlViewWithVODLayout];
    self.playerView = [[BCOVPUIPlayerView alloc] initWithPlaybackController:self.playbackController options:nil controlsView:controlView];
    self.playerView.frame = self.videoView.bounds;
    self.playerView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

Then, add the `BCOVPUIPlayerView` to your video container, `videoView`.

    // Add BCOVPUIPlayerView to your video view.
    [self.videoView addSubview:self.playerView];

The next steps are specific to IMA. Create your playback controller as you did above, but instead of your video container view, pass in the `contentOverlayView` from the player view as your `adContainer`. The `contentOverlayView` is a special view used for overlaying views on the main video content. You should also use `nil` instead of `[manager defaultControlsViewStrategy]` if you were using that as your `viewStrategy` (this was the older method for using built-in controls).

        id<BCOVPlaybackController> controller =
                [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
                        adContainer:self.playerView.contentOverlayView // special view for IMA ad content
                        companionSlots:nil
                        viewStrategy:];
        controller.delegate = self;

Lastly, implement two `BCOVPlaybackControllerAdsDelegate` methods on the playback controller's delegate. Since IMA implements its own set of ad controls, you should hide the Brightcove PlayerUI controls while IMA ads are playing. This prevents unwanted controls from showing up on the screen when the views are larger and more sparse, like when presenting in full-screen mode.

    - (void)playbackController:(id<BCOVPlaybackController>)controller playbackSession:(id<BCOVPlaybackSession>)session didEnterAdSequence:(BCOVAdSequence *)adSequence
    {
      // Hide all controls for ads (so they're not visible when full-screen)
      self.playerView.controlsContainerView.alpha = 0.0;
    }

    - (void)playbackController:(id<BCOVPlaybackController>)controller playbackSession:(id<BCOVPlaybackSession>)session didExitAdSequence:(BCOVAdSequence *)adSequence
    {
      // Show all controls when ads are finished.
      self.playerView.controlsContainerView.alpha = 1.0;
    }

Now, when playing video with ads, you will see the PlayerUI controls while playing video content, plus ad markers on the timeline scrubber (VMAP ads only).

The PlayerUI is highly customizable. For more information and sample code, please see **Custom Layouts** section in the README file of the [Brightcove Native Player SDK repository][BCOVSDK].

[BCOVSDK]: https://github.com/brightcove/brightcove-player-sdk-ios

Customizing Plugin Behavior
===========================
There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

VAST and VMAP/Server Side Ad Rules
--------------------------

BCOVIMA gives you control over how ads requests are made, via the `BCOVIMAAdsRequestPolicy` class. The class provides factory methods for the supported policies. Once you obtain an instance of the correct policy, you need to provide it to the `BCOVPlayerSDKManager` to create a playback controller or a playback session provider. 

In Quick Start, an example of VMAP is given. Here is a VAST example.

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

1. This example is the same as the one provided in the quick-start, except that we are now calling a different `BCOVIMAAdsRequestPolicy` policy method to specify that we want to use VAST.

BCOVIMAAdsRequestPolicy has four factory methods to generate ads request policy; two for VMAP and two for VAST. Two factory methods for VMAP are described in more detail below:

* `+videoPropertiesVMAPAdTagUrlAdsRequestPolicy`: This method returns the default ads request policy, which looks for the `kBCOVIMAAdTag` in each BCOVVideo's properties to determine the VMAP ad tag that should be used to request ads.

* `+adsRequestPolicyWithVMAPAdTagUrl:`: This method returns an ads request policy that always uses the same ad tag for every video. If you use this policy, you do not have to put the ad tag in the BCOVVideos' properties. This is the easiest way to specify an ad tag if you know your ad tag will never change for the lifetime of the application session.

There are two factory methods for VAST.  All of the VAST methods take a BCOVCuePointProgressPolicy. The cue point policy determines which BCOVCuePoints will get processed by the BCOVIMAAdsRequestPolicy.  The VAST policies are as follows:

* `+adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:` This method returns an ads request policy that checks each BCOVVideo for BCOVCuePoints of type 'kBCOVIMACuePointTypeAd' and looks in each of those cue points properties for the key `kBCOVIMAAdTag` to determine the VAST ad tag that should be used to request ads.

* `+adsRequestPolicyFromCuePointPropertiesWithAdTag:adsCuePointProgressPolicy:`  This method returns an ad request policy that uses the specified VAST ad tag for all BCOVCuePoints of type **kBCOVIMACuePointTypeAd**. Properties of the cue point are appended to the ad tag as query parameters.

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
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategy:]` method.

When composing session providers, the session preloading can be enabled from [`BCOVBasicSessionProvider`][basicprovider]; however, preloading sessions with IMA plugin is **strongly discouraged** due to a bug in the Google IMA library when having multiple AVPlayers in memory.

[basicprovider]: https://github.com/brightcove/brightcove-player-sdk-ios/blob/master/ios/dynamic/BrightcovePlayerSDK.framework/Headers/BCOVBasicSessionProvider.h#L31-L46

Frequently Asked Questions
==========================

** I can hear the ads, but I can't see them playing. **

This usually happens when either ad container view is not in view hierarchy, or ad view (which is subview of ad container view) is covered by other views.
