//
// BCOVIMAComponent.h
// BCOVIMA
//
// Copyright (c) 2014 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>
#import <GoogleAds-IMA-iOS-SDK/IMAAdsLoader.h>
#import <GoogleAds-IMA-iOS-SDK/IMAAdsRenderingSettings.h>

#import "BCOVPlayerSDK.h"


@class BCOVIMASessionProviderOptions;


extern NSString * const kBCOVIMALifecycleEventAdsLoaderLoaded;
extern NSString * const kBCOVIMALifecycleEventAdsLoaderFailed;
extern NSString * const kBCOVIMALifecycleEventAdsManagerDidReceiveAdEvent;
extern NSString * const kBCOVIMALifecycleEventAdsManagerDidReceiveAdError;


/**
 * The default ad tag policy will look for an entry in each session.video's
 * properties with this key and use it as the ad tag for VMAP requests. In
 * other words, each video for which ad rules have been configured should have
 * this key in its properties, and the value should be the ad tag for the VMAP
 * response to use for the rest of the video's ad breaks.
 */
extern NSString * const kBCOVIMAAdTag;


/**
 * Category methods added to BCOVPlayerSDKManager to support IMA.
 */
@interface BCOVPlayerSDKManager (BCOVIMAAdditions)

/**
 * Creates and returns a new playback controller with the specified IMASettings
 * object, IMAAdsRenderingSettings object, and view strategy. The returned
 * playback controller will be configured with an IMA session provider.
 *
 * @param settings An IMASettings that will be used to configure any
 * IMAAdsLoader object used by the returned playback controller.
 * @param adsRenderingSettings An IMAAdsRenderingSettings that will be used to
 * configure any IMAAdsManager object used by the returned playback controller.
 * @param strategy A view strategy that determines the view for the returned
 * playback controller.
 * @return A new playback controller with the specified parameters.
 */
- (id<BCOVPlaybackController>)createIMAPlaybackControllerWithSettings:(IMASettings *)settings adsRenderingSettings:(IMAAdsRenderingSettings *)adsRenderingSettings viewStrategy:(BCOVPlaybackControllerViewStrategy)strategy;

/**
 * Creates and returns a new BCOVIMASessionProvider with the specified
 * parameters.
 *
 * @param settings An IMASettings that will be used to configure any
 * IMAAdsLoader object used by the returned session provider.
 * @param adsRenderingSettings An IMAAdsRenderingSettings that will be used to
 * configure any IMAAdsManager object used by the returned session provider.
 * @param provider A session provider to attach upstream and deliver playback
 * sessions to the returned session provider.
 * @param options The options with which to configure the returned session
 * provider.
 * @return A new BCOVIMASessionProvider with the specified parameters.
 */
- (id<BCOVPlaybackSessionProvider>)createIMASessionProviderWithSettings:(IMASettings *)settings adsRenderingSettings:(IMAAdsRenderingSettings *)adsRenderingSettings upstreamSessionProvider:(id<BCOVPlaybackSessionProvider>)provider options:(BCOVIMASessionProviderOptions *)options;

/**
 * Returns a view strategy that wraps the view it is given with a view that
 * also contains an ad view, for display of IMA advertising.
 *
 * @return A view strategy block that wraps its parameter with an ad view.
 */
- (BCOVPlaybackControllerViewStrategy)BCOVIMAAdViewStrategy;

@end
