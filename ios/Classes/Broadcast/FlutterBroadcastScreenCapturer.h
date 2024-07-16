//
//  FlutterBroadcastScreenCapturer.h
//  RCTWebRTC
//
//  Created by Alex-Dan Bumbu on 06/01/2021.
//

#import <Foundation/Foundation.h>
#import <LiveKitWebRTC/LiveKitWebRTC.h>
NS_ASSUME_NONNULL_BEGIN

extern NSString* const lkRTCScreensharingSocketFD;
extern NSString* const lkRTCAppGroupIdentifier;
extern NSString* const lkRTCScreenSharingExtension;

@class FlutterSocketConnectionFrameReader;

@interface FlutterBroadcastScreenCapturer : LKRTCVideoCapturer
- (void)startCapture;
- (void)stopCapture;
- (void)stopCaptureWithCompletionHandler:(nullable void (^)(void))completionHandler;

@end

NS_ASSUME_NONNULL_END
