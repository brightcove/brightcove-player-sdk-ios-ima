//
// BCOVIMAAdsRequestPolicy.h
// BrightcoveIMA
//
// Copyright (c) 2023 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>

@class BCOVCuePointProgressPolicy;
@protocol BCOVPlaybackSession;


/**
 * Policy object to generate IMAAdsRequests for use by a
 * given input playback session.
 */
@interface BCOVIMAAdsRequestPolicy : NSObject

#pragma mark VMAP

/**
 * Returns an ads request policy that checks the BCOVVideo object in each
 * playback session for the VMAP ad tag URL to request. If the video object's
 * properties contains an entry whose key is `kBCOVIMAAdTag`, the value
 * of that entry is assumed to be a NSString containing the VMAP ad tag URL.
 * That URL will be used to construct a VMAP IMAAdsRequest for that playback 
 * session.
 *
 * Use this policy when you want the ability to specify a different VMAP ad tag URL
 * for different videos.
 *
 * @return An ads request policy that generates VMAP IMAAdsRequests from
 * information in each playback session's `BCOVVideo.properties`.
 */
+ (instancetype)videoPropertiesVMAPAdTagUrlAdsRequestPolicy;

/**
 * Returns an ads request policy that generates a VMAP IMAAdsRequest with the
 * specified ad tag URL and companion slots for every playback session.
 *
 * Use this policy when you want  to use the same VMAP ad tag URL for all videos.
 *
 * @param VMAPAdTagUrl The ad tag URL to include in the IMAAdsRequest this
 * policy generates. Currently, only VMAP ad tag URLs are supported.
 * @return An ads request policy that generates VMAP IMAAdsRequests from
 * the specified parameters.
 */
+ (instancetype)adsRequestPolicyWithVMAPAdTagUrl:(NSString *)VMAPAdTagUrl;

#pragma mark VAST

/**
 * Returns an ads request policy that checks each BCOVVideo for BCOVCuePoints
 * of type 'kBCOVIMACuePointTypeAd'. If the cuepoint object's
 * properties contains an entry whose key is `kBCOVIMAAdTag`, the value
 * of that entry is assumed to be a NSString containing the VAST ad tag URL.
 * That URL will be used to construct a VAST IMAAdsRequest for that playback
 * session.
 *
 * Use this policy when you want the ability to specify a different VAST ad tag URL
 * for different cuepoints.
 *
 * @param adsCuePointProgressPolicy The cue point progress policy specified for
 * VAST ads requests cue points. If this parameter is nil, `+[BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessAllCuePoints resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:NO]`
 * will be used as default.
 * @return An ads request policy that generates VAST IMAAdsRequests from
 * information in each playback video's `BCOVCuePoint.properties`.
 */
+ (instancetype)adsRequestPolicyWithVASTAdTagsInCuePointsAndAdsCuePointProgressPolicy:(BCOVCuePointProgressPolicy *)adsCuePointProgressPolicy;

/**
 * Returns an ads request policy that checks each BCOVVideo for BCOVCuePoints
 * of type 'kBCOVIMACuePointTypeAd'. The cuepoint properies will be appended on
 * ad tag as query parameters. That URL will be used to construct a VAST IMAAdsRequest
 * for that playback session.
 *
 * Use this policy when you want to use the same VAST ad tag URL for all cuepoints.
 *
 * @param adTag The ad tag URL to include in the IMAAdsRequest this
 * policy generates.
 * @param adsCuePointProgressPolicy The cue point progress policy specified for
 * VAST ads requests cue points. If this parameter is nil, `+[BCOVCuePointProgressPolicy progressPolicyProcessingCuePoints:BCOVProgressPolicyProcessAllCuePoints resumingPlaybackFrom:BCOVProgressPolicyResumeFromContentPlayhead ignoringPreviouslyProcessedCuePoints:NO]`
 * will be used as default.
 * @return An ads request policy that generates VAST IMAAdsRequests from
 * `adTag` and information in each playback video's `BCOVCuePoint.properties`.
 */
+ (instancetype)adsRequestPolicyFromCuePointPropertiesWithAdTag:(NSString *)adTag adsCuePointProgressPolicy:(BCOVCuePointProgressPolicy *)adsCuePointProgressPolicy;
@end
