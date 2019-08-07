# IMA Plugin for Brightcove Player SDK for iOS, version 6.4.5.699

Supported Platforms
==========
iOS 10.0 and above.

Installation
==========
IMA Plugin for Brightcove Player SDK provides a dynamic library framework for installation.

The IMA plugin supports version 3.9.0 of the Google IMA SDK for iOS. The Podspec for the IMA Plugin for Brightcove Player SDK for iOS references version 3.9.0 of the Google IMA iOS SDK as instructed by the [IMA Release History](https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/history).

CocoaPods
----------

You can use [CocoaPods][cocoapods] to add the IMA Plugin for Brightcove Player SDK to your project.  You can find the latest `Brightcove-Player-IMA` podspec [here][podspecs]. The pod will incorporate the correct version of IMA automatically. CocoaPods 1.0 or newer is required.

CocoaPod Podfile example:

```
source 'https://github.com/brightcove/BrightcoveSpecs.git'

pod 'Brightcove-Player-IMA'
```

As of IMA release 3.4.1, AdMob is published as a standalone dynamic framework and can no longer be installed as part of the IMA Plugin for the Brightcove Native Player SDK. As such, there is no longer a `ForAdMob` subspec for the podspec. To use AdMob, download it and add it to your project manually, or add the `Google-Mobile-Ads-SDK` CocoaPod to your Podfile:
    
    pod 'Google-Mobile-Ads-SDK'  

When updating your installation, it's a good idea to refresh the local copy of your BrightcoveSpecs repository so that you have the latest podspecs locally, just like you would update your CococaPods master repository.

Typically if you use `pod update` in Terminal this will happen automatically, or alternatively you can update explicitly with `pod repo update brightcove`. (Your BrightcoveSpecs repository may have a different name if you explicitly added it to your list of podspecs repos.)

Manual
----------

To add the IMA Plugin for Brightcove Player SDK to your project manually:

1. Download the [Brightcove Player SDK][bcovsdk] framework.
1. Download the [IMA Plugin for Brightcove Player SDK][bcovima] framework.
1. Download the [Google IMA][googleima] framework.
1. On the "General" tab of your application target, add the **dynamic** framework, BrightcovePlayerSDK.framework, from the Brightcove Player SDK download to the list of **Embedded Binaries**. The dynamic framework, BrightcovePlayerSDK.framework, is found in the ios/dynamic directory of the download.
1. On the "General" tab of your application target, add BrightcoveIMA.framework from the IMA Plugin for Brightcove Player SDK download to the list of **Embedded Binaries**.
1. On the "General" tab of your application target, add GoogleInteractiveMediaAds.framework from the Google IMA download to the list of **Embedded Binaries**.
1. On the "Build Settings" tab of your application target, ensure that the "Framework Search Paths" include the paths to the frameworks. This should have been done automatically unless the framework is stored under a different root directory than your project.
1. On the "Build Settings" tab of your application target:
    * Ensure that `-ObjC` has been added to the "Other Linker Flags" build setting.

Imports
----------
The IMA Plugin for Brightcove Player SDK can be imported into code a few different ways; `@import BrightcoveIMA;`, `#import <BrightcoveIMA/BrightcoveIMA.h>` or `#import <BrightcoveIMA/[specific class].h>`. You can import the `GoogleInteractiveMediaAds` and `BrightcovePlayerSDK` modules in similar fashion.

[cocoapods]: http://cocoapods.org
[podspecs]: https://github.com/brightcove/BrightcoveSpecs/tree/master/Brightcove-Player-IMA
[release]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/releases

Quick Start
==========
The BrightcoveIMA plugin is a bridge between [Google IMA iOS SDK v3][googleima] and the [Brightcove Player SDK for iOS][bcovsdk]. This snippet shows its basic usage with Server Side Ad Rules.

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

        NSString *policyKey = <your-policy-key>;
        NSString *accountId = <your-account-id>;
        NSString *videoID = <your-video-id>;
        BCOVPlaybackService *playbackService = [[BCOVPlaybackService alloc] initWithAccountId:accountID
                                                                                    policyKey:policyKey];
        [playbackService findVideoWithVideoID:videoID
                                   parameters:nil
                                   completion:^(BCOVVideo    *video,
                                                NSDictionary *jsonResponse,
                                                NSError      *error) {

            [controller setVideos:@[ video ]];
            [controller play];

        }];

Breaking the code down into steps:

1. Create the same IMA settings, ads rendering settings that you would create if you were using Google's IMA iOS SDK directly. These are required.
1. BCOVIMAAdsRequestPolicy provides methods to specify VAST or VMAP/Server Side Ad Rules. Select the appropriate method to select your ads policy.
1. BrightcoveIMA adds some category methods to BCOVPlaybackManager. The first of these is `-createIMAPlaybackControllerWithSettings:adsRenderingSettings:adsRequestPolicy:adContainer:companionSlots:viewStrategy:`. Use this method to create your playback controller.

If you have questions or need help, we have a support forum for Brightcove's native Player SDKs at [https://groups.google.com/forum/#!forum/brightcove-native-player-sdks][forum] .

[googleima]: https://developers.google.com/interactive-media-ads/docs/sdks/ios/v3/
[bcovsdk]: https://github.com/brightcove/brightcove-player-sdk-ios
[bcovima]: https://github.com/brightcove/brightcove-player-sdk-ios-ima
[forum]: https://groups.google.com/forum/#!forum/brightcove-native-player-sdks

Play and Pause
==========
The Brightcove IMA Plugin implements custom play and pause logic to ensure the smoothest possible ad experience. Therefore, you will need to make sure that you use the play method on the `BCOVPlaybackController` or the `-[BCOVSessionProviderExtension ima_play]` or `-[BCOVSessionProviderExtension ima_pause]` ([BCOVSessionProviderExtension][BCOVIMAComponent]), and not the AVPlayer.

As an example, calling play for the first time on `BCOVPlaybackController` allows BCOVIMA to process preroll ads without any of the content playing before the preroll. For more information on how BCOVIMA overrides the default `BCOVPlaybackController` methods, please check out [BCOVSessionProviderExtension][BCOVIMAComponent].

[BCOVIMAComponent]: https://github.com/brightcove/brightcove-player-sdk-ios-ima/blob/master/ios/BrightcoveIMA.framework/Headers/BCOVIMAComponent.h

Using the Built-In PlayerUI
==========
If you are using version 5.1 or later of the Brightcove Player SDK, you can take advantage of the built-in player controls with the Brightcove IMA plugin.

**Note:** The `BrightcovePlayerUI` module is no longer needed and has been removed. (Prior to version 5.1 of the Brightcove Player SDK, the Brightcove PlayerUI plugin was a separate framework and module.) You can remove any imports that reference the Brightcove PlayerUI module. All PlayerUI headers are now found in the `BrightcovePlayerSDK` module.

In your `UIViewController`, create a `BCOVPUIPlayerView` property called the player view, to contain the playback controls, the video content view, and a special view where IMA can display its ads.

```
// PlayerUI's player view
@property (nonatomic) BCOVPUIPlayerView *playerView;
```

Then create your player view; supply a nil playback controller which will be added later. This player view contains both the video content view and the view that displays playback controls and ad controls. This setup is the same no matter what plugin you are using. Set up the player view to match the video container from your layout (`videoView`) when it resizes.

```
// Create and configure Control View.
BCOVPUIBasicControlView *controlView = [BCOVPUIBasicControlView basicControlViewWithVODLayout];
    
// Create the player view with a nil playback controller.
self.playerView = [[BCOVPUIPlayerView alloc] initWithPlaybackController:nil options:nil controlsView:controlView];
// Add BCOVPUIPlayerView to your video view.
[self.videoView addSubview:self.playerView];
```

You'll need to set up the layout for the player view, you can do this with Auto Layout or the older Springs & Struts method. 

**Springs & Struts**
```
self.playerView.frame = self.videoView.bounds;
self.playerView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
```

**Auto Layout**
```
self.playerView.translatesAutoresizingMaskIntoConstraints = NO;
[NSLayoutConstraint activateConstraints:@[
                                          [self.playerView.topAnchor constraintEqualToAnchor:self.videoView.topAnchor],
                                          [self.playerView.rightAnchor constraintEqualToAnchor:self.videoView.rightAnchor],
                                          [self.playerView.leftAnchor constraintEqualToAnchor:self.videoView.leftAnchor],
                                          [self.playerView.bottomAnchor constraintEqualToAnchor:self.videoView.bottomAnchor],
                                         ]];
```

Creating the playback controller is specific to IMA. Create your playback controller as you did above, but instead of your video container view, pass in the `contentOverlayView` from the player view as your `adContainer`. The `contentOverlayView` is a special view used for overlaying views on the main video content. You should also use `nil` instead of `[manager defaultControlsViewStrategy]` if you were using that as your `viewStrategy` (this was the older method for using built-in controls).

```
// Create the playback controller.
id<BCOVPlaybackController> controller =
                [manager createIMAPlaybackControllerWithSettings:imaSettings
                        adsRenderingSettings:renderSettings
                        adsRequestPolicy:adsRequestPolicy
                        adContainer:self.playerView.contentOverlayView // special view for IMA ad content
                        companionSlots:nil
                        viewStrategy:nil];
controller.delegate = self;

// Assign new playback controller to the player view.
// This associates the playerController's session with the PlayerUI.
// You can keep this player view around and assign new
// playback controllers to it as they are created.
self.playerView.playbackController = self.playbackController;
```

Lastly, implement two `BCOVPlaybackControllerAdsDelegate` methods on the playback controller's delegate. Since IMA implements its own set of ad controls, you should hide the Brightcove PlayerUI controls while IMA ads are playing. This prevents unwanted controls from showing up on the screen when the views are larger and more sparse, like when presenting in full-screen mode.

```
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
```

Now, when playing video with ads, you will see the PlayerUI controls while playing video content, plus ad markers on the timeline scrubber (VMAP ads only).

The PlayerUI is highly customizable. For more information and sample code, please see **Custom Layouts** section in the README.md file of the [Brightcove Native Player SDK repository][BCOVSDK].

[BCOVSDK]: https://github.com/brightcove/brightcove-player-sdk-ios

Seeking Without Ads
==========
Use `-[BCOVPlaybackController seekWithoutAds:(CMTime)time completionHandler:(void (^)(BOOL finished))completion]` to resume playback at a specific time without forcing the user to watch ads scheduled before `time`. When using `seekWithoutAds:completionHandler:`, `autoPlay` should be disabled in the `BCOVPlaybackController`. `seekWithoutAds:completionHandler:` should be called on or after receiving `kBCOVPlaybackSessionLifecycleEventReady` in your `playbackController:playbackSession:didReceiveLifecycleEvent` delegate method.

The `shutter` and `shutterFadeTime` properties of the `BCOVPlaybackController` can be used along with `seekWithoutAds:completionHandler:` to hide frame-flicker which can occur as the AVPlayer loads assets. In your BCOVPlaybackController setup code, close the shutter to hide the player view:

```
  NSObject<BCOVPlaybackController> *playbackController;
        
  playbackController = [sdkManager createFWPlaybackControllerWithAdContextPolicy:nil
                                                                    viewStrategy:nil];
  playbackController.delegate = self;
        
  if (self.willSeekWithoutAds)
  {
    // set the shutter fade time to zero to hide the player view immediately.
    playbackController.shutterFadeTime = 0.0;
    playbackController.shutter = YES;
    
    // disable autoPlay when resuming playback.
    playbackController.autoPlay = NO;
  }
```

Apple recommends waiting for the status of an AVPlayerItem to change to ready-to-play before using the AVPlayerItem; call `seekWithoutAds:completionHandler:` in the `playbackController:playbackSession:didReceiveLifecycleEvent` method of your `BCOVPlaybackControllerDelegate` delegate.


```
- (void)playbackController:(NSObject<BCOVPlaybackController>*)controller
           playbackSession:(NSObject<BCOVPlaybackSession>*)session
  didReceiveLifecycleEvent:(BCOVPlaybackSessionLifecycleEvent *)lifecycleEvent
{
  if ([kBCOVPlaybackSessionLifecycleEventReady isEqualToString:lifecycleEvent.eventType])
  {
    if (self.willSeekWithoutAds)
    {
      __weak typeof(controller) weakController = controller;

      // seek without playing ads which are scheduled before the seek time, i.e. resume playback.
      [controller seekWithoutAds:CMTimeMake(seekWithoutAdsValue, seekWithoutAdsTimescale)
             completionHandler:^(BOOL finished){

        if (!finished)
        {
          NSLog (@"seekWithoutAds failed to finish");
        }

        typeof(controller) strongController = weakController;
        if (strongController)
        {
          // fade out the shutter to reveal the player view.
          strongController.shutterFadeTime = 0.25;
          strongController.shutter = NO;

          // turn off seeking without ads - especially important if this player is being used with a playlist
          self.willSeekWithoutAds = NO;
        }

      }];
    }
  }
}
```

Note that when Seeking Without Ads is enabled in your app, you will still see the network traffic that normally occurs as part of setting up the IMA plugin. This traffic is necessary for proper plugin setup, and does not affect the Seeking Without Ads functionality.

Customizing Plugin Behavior
==========
There are a couple of configuration points in BCOVIMA. You can combine BCOVIMA with another plugin for the Brightcove Player SDK for iOS, you can create a custom view strategy, and you can supply a custom ads request policy.

VAST and VMAP/Server Side Ad Rules
----------

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

Modifying the IMAAdsRequest
----------

The IMA Plugin passes an `IMAAdsRequest` object to a `BCOMIMAPlaybackSessionDelegate` immediately before calling `IMAAdsLoader -requestAdsWithAdsRequest`, allowing the user to first modify the ads request. To receive the ads request callback, create an object that implements the `BCOVIMAPlaybackSessionDelegate` protocol.

    @import BrightcovePlayerSDK;
    @import BrightcoveIMA;

    @interface MyViewController : UIViewController <BCOVIMAPlaybackSessionDelegate>

Create a `BCOVIMASessionProvider` using either `createIMAPlaybackControllerWithSettings` or `createIMASessionProviderWithSettings`, and provide an NSDictionary of options with an entry having a key of `kBCOVIMAOptionIMAPlaybackSessionDelegateKey` and a value which is your delegate.

    NSDictionary *imaSessionProviderOptions = @{ kBCOVIMAOptionIMAPlaybackSessionDelegateKey: self };
    
    id<BCOVPlaybackSessionProvider> imaSessionProvider =
        [sdkManager createIMASessionProviderWithSettings:imaSettings
                                    adsRenderingSettings:renderSettings
                                        adsRequestPolicy:adsRequestPolicy
                                             adContainer:self.playerView.contentOverlayView
                                          companionSlots:ni
                                 upstreamSessionProvider:nil
                                                 options:imaSessionProviderOptions];

Implement `willCallIMAAdsLoaderRequestAdsWithRequest:forPosition:` in your `BCOVIMAPlaybackSessionDelegate`.

    - (void)willCallIMAAdsLoaderRequestAdsWithRequest:(IMAAdsRequest *)adsRequest
                                          forPosition:(NSTimeInterval)position
    {
        adsRequest.vastLoadTimeout = 3000.;
    }


View Strategy
----------
You can provide a custom view strategy to the BCOVPlaybackManager when you are constructing your playback controller or session provider, rather than specify the defaultControlsViewStrategy directly. With a custom view strategy, the ad container view and ad companion slots can be tied with the video content view. This is an example of custom view strategy.

    BCOVPlaybackControllerViewStrategy customViewStrategy = ^UIView* (UIView *view, id<BCOVPlaybackController> playbackController){
        
        BCOVPlaybackControllerViewStrategy defaultControlsViewStrategy = [playbackManager defaultControlsViewStrategy];
        UIView *contentAndDefaultControlsView = defaultControlsViewStrategy(view, playbackController);
        
        [<UIView of video container> addSubview:contentAndDefaultControlsView];
        
        return <UIView of video container>;
    };
    
Composing Session Providers
----------
If you are using more than one plugin to the Brightcove Player SDK for iOS that needs to create a customized playback controller, you must instead compose a chain of session providers and pass the final session provider to the `-[BCOVPlayerSDKManager createPlaybackControllerWithSessionProvider:viewStrategy:]` method.

When composing session providers, the session preloading can be enabled from [`BCOVBasicSessionProvider`][basicprovider]; however, preloading sessions with IMA plugin is **strongly discouraged** due to a bug in the Google IMA SDK when having multiple AVPlayers in memory.

[basicprovider]: https://github.com/brightcove/brightcove-player-sdk-ios/blob/fd5e766693e533854f202f270d3d62e32ceaae04/ios/dynamic/BrightcovePlayerSDK.framework/Headers/BCOVBasicSessionProvider.h#L31-L46

Registering Ad Overlays
==========
If you are placing any views over ads while they are playing, it is necceessary to register those views with the IMA SDK. Read the **Friendly obstructions** section of the [Open Measurement in the IMA SDK ](https://developers.google.com/interactive-media-ads/docs/sdks/ios/omsdk) page for more information.

You can get the current IMAAdDisplayContainer object neccessary to register your overlays from the `playbackController:didAdvanceToPlaybackSession:` delegate method of your BCOVPlaybackController instance. For example:
```
- (void)playbackController:(id<BCOVPlaybackController>)controller didAdvanceToPlaybackSession:(id<BCOVPlaybackSession>)session
{
    NSDictionary *props = session.video.properties;
    IMAAdDisplayContainer *adDisplayContainer = props[kBCOVIMAVideoPropertiesKeyAdDisplayContainer];
    [adDisplayContainer registerVideoControlsOverlay:self.adOverlayView];
}
```

Frequently Asked Questions
==========
_I can hear the ads, but I can't see them playing._

This usually happens when the ad container view is not in the view hierarchy, or when the ad view (which is a subview of the ad container view) is covered by other views.
