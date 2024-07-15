#import <LiveKitWebRTC/LiveKitWebRTC.h>

#if TARGET_OS_IPHONE
#import <Flutter/Flutter.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterMacOS.h>
#endif

@interface FlutterRTCFrameCapturer : NSObject <LKRTCVideoRenderer>

- (instancetype)initWithTrack:(LKRTCVideoTrack*)track
                       toPath:(NSString*)path
                       result:(FlutterResult)result;

@end
