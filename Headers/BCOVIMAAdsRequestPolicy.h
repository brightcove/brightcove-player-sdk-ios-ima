//
// BCOVIMAAdsRequestPolicy.h
// BCOVIMA
//
// Copyright (c) 2014 Brightcove, Inc. All rights reserved.
// License: https://accounts.brightcove.com/en/terms-and-conditions
//

#import <Foundation/Foundation.h>


@protocol BCOVPlaybackSession;

@class RACSignal;


/**
 * Policy object to generate a signal of IMAAdsRequest objects for use by a
 * given input playback session.
 *
 * Subclass this class and override the `-signalWithValue:` method to return
 * a signal that sends values of type `IMAAdsRequest` when those requests should
 * be made. The signal should complete when no more ads should be requested.
 *
 * Note: Currently, this policy should be implemented to generate a single VMAP
 * ad request.
 */
@interface BCOVIMAAdsRequestPolicy : NSObject

/**
 * Returns a signal which sends IMAAdsRequests for the VMAP request
 * that correspond to the specified input session. When the signal sends a
 * value, the BCOVIMASessionProvider will issue the corresponding request.
 * The signal returned by this method should send a single IMAAdsRequest object
 * containing a VMAP ad tag URL, and then complete. Or, if no ads should be
 * requested for the given playback session, this method should be implemented
 * to return an empty signal.
 *
 * @param inputSession The playback session for which a signal of IMAAdsRequests
 * is to be returned.
 * @return The signal of IMAAdsRequests that correspond to the input playback
 * session. Returns an empty signal if no ads should be requested for
 * the specified input session.
 */
- (RACSignal *)signalWithValue:(id<BCOVPlaybackSession>)inputSession;

/**
 * Returns an ads request policy that checks the BCOVVideo object in each
 * playback session for the VMAP ad tag URL to request. If the video object's
 * properties contains an entry whose key is `kBCOVIMAAdTag`, the value
 * of that entry is assumed to be a NSString containing the VMAP ad tag URL.
 * That URL will be used to construct a VMAP IMAAdsRequest for that playback 
 * session.
 *
 * @return An ads request policy that generates VMAP IMAAdsRequests from
 * information in each playback session's `BCOVVideo.properties`.
 */
+ (instancetype)videoPropertiesVMAPAdTagUrlAdsRequestPolicy;

/**
 * Returns an ads request policy that checks the BCOVVideo object in each
 * playback session for the VMAP ad tag URL to request. If the video object's
 * properties contains an entry whose key is `kBCOVIMAAdTag`, the value
 * of that entry is assumed to be a NSString containing the VMAP ad tag URL.
 * That URL will be used to construct a VMAP IMAAdsRequest for that playback 
 * session.
 *
 * @param companionAdSlots An array of IMACompanionAdSlot objects to include
 * in the IMAAdsRequest.
 * @return An ads request policy that generates VMAP IMAAdsRequests from
 * information in each playback session's `BCOVVideo.properties`.
 */
+ (instancetype)videoPropertiesVMAPAdTagUrlAdsRequestPolicyWithCompanionAdSlots:(NSArray *)companionAdSlots;

/**
 * Returns an ads request policy that generates a VMAP IMAAdsRequest with the
 * specified ad tag URL and companion slots for every playback session.
 *
 * @param VMAPAdTagUrl The ad tag URL to include in the IMAAdsRequest this
 * policy generates. Currently, only VMAP ad tag URLs are supported.
 * @param companionAdSlots An array of IMACompanionAdSlot objects to include
 * in the IMAAdsRequest.
 * @return An ads request policy that generates VMAP IMAAdsRequests from
 * the specified parameters.
 */
+ (instancetype)adsRequestPolicyWithVMAPAdTagUrl:(NSString *)VMAPAdTagUrl companionAdSlots:(NSArray *)companionAdSlots;

@end
