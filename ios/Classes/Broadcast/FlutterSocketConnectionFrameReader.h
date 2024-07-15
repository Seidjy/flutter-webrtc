//
//  FlutterSocketConnectionFrameReader.h
//  RCTWebRTC
//
//  Created by Alex-Dan Bumbu on 06/01/2021.
//

#import <AVFoundation/AVFoundation.h>
#import <LiveKitWebRTC/RTCVideoCapturer.h>

NS_ASSUME_NONNULL_BEGIN

@class FlutterSocketConnection;

@interface FlutterSocketConnectionFrameReader : LKRTCVideoCapturer

- (instancetype)initWithDelegate:(__weak id<LKRTCVideoCapturerDelegate>)delegate;
- (void)startCaptureWithConnection:(nonnull FlutterSocketConnection*)connection;
- (void)stopCapture;

@end

NS_ASSUME_NONNULL_END
