#if TARGET_OS_IPHONE
#import <Flutter/Flutter.h>
#elif TARGET_OS_OSX
#import <FlutterMacOS/FlutterMacOS.h>
#endif

#import <Foundation/Foundation.h>
#import <LiveKitWebRTC/LiveKitWebRTC.h>

@class FlutterRTCVideoRenderer;
@class FlutterRTCFrameCapturer;

void postEvent(FlutterEventSink sink, id _Nullable event);

typedef void (^CompletionHandler)(void);

typedef void (^CapturerStopHandler)(CompletionHandler handler);

@interface FlutterWebRTCPlugin : NSObject <FlutterPlugin,
                                           LKRTCPeerConnectionDelegate,
                                           FlutterStreamHandler
#if TARGET_OS_OSX
                                           ,
                                           RTCDesktopMediaListDelegate,
                                           RTCDesktopCapturerDelegate
#endif
                                           >

@property(nonatomic, strong) LKRTCPeerConnectionFactory* peerConnectionFactory;
@property(nonatomic, strong) NSMutableDictionary<NSString*, LKRTCPeerConnection*>* peerConnections;
@property(nonatomic, strong) NSMutableDictionary<NSString*, LKRTCMediaStream*>* localStreams;
@property(nonatomic, strong) NSMutableDictionary<NSString*, LKRTCMediaStreamTrack*>* localTracks;
@property(nonatomic, strong) NSMutableDictionary<NSNumber*, FlutterRTCVideoRenderer*>* renders;
@property(nonatomic, strong)
    NSMutableDictionary<NSString*, CapturerStopHandler>* videoCapturerStopHandlers;

@property(nonatomic, strong) NSMutableDictionary<NSString*, LKRTCFrameCryptor*>* frameCryptors;
@property(nonatomic, strong) NSMutableDictionary<NSString*, LKRTCFrameCryptorKeyProvider*>* keyProviders;

#if TARGET_OS_IPHONE
@property(nonatomic, retain) UIViewController* viewController; /*for broadcast or ReplayKit */
#endif

@property(nonatomic, strong) FlutterEventSink eventSink;
@property(nonatomic, strong) NSObject<FlutterBinaryMessenger>* messenger;
@property(nonatomic, strong) LKRTCCameraVideoCapturer* videoCapturer;
@property(nonatomic, strong) FlutterRTCFrameCapturer* frameCapturer;
@property(nonatomic, strong) AVAudioSessionPort preferredInput;
@property(nonatomic) BOOL _usingFrontCamera;
@property(nonatomic) NSInteger _lastTargetWidth;
@property(nonatomic) NSInteger _lastTargetHeight;
@property(nonatomic) NSInteger _lastTargetFps;

- (LKRTCMediaStream*)streamForId:(NSString*)streamId peerConnectionId:(NSString*)peerConnectionId;
- (LKRTCRtpTransceiver*)getRtpTransceiverById:(LKRTCPeerConnection*)peerConnection Id:(NSString*)Id;
- (NSDictionary*)mediaStreamToMap:(LKRTCMediaStream*)stream ownerTag:(NSString*)ownerTag;
- (NSDictionary*)mediaTrackToMap:(LKRTCMediaStreamTrack*)track;
- (NSDictionary*)receiverToMap:(LKRTCRtpReceiver*)receiver;
- (NSDictionary*)transceiverToMap:(LKRTCRtpTransceiver*)transceiver;

- (BOOL)hasLocalAudioTrack;
- (void)ensureAudioSession;
- (void)deactiveRtcAudioSession;

- (LKRTCRtpReceiver*)getRtpReceiverById:(LKRTCPeerConnection*)peerConnection Id:(NSString*)Id;
- (LKRTCRtpSender*)getRtpSenderById:(LKRTCPeerConnection*)peerConnection Id:(NSString*)Id;

@end
