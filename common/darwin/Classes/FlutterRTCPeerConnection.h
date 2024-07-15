#import "FlutterWebRTCPlugin.h"

@interface LKRTCPeerConnection (Flutter) <FlutterStreamHandler>
@property(nonatomic, strong, nonnull) NSMutableDictionary<NSString*, LKRTCDataChannel*>* dataChannels;
@property(nonatomic, strong, nonnull)
    NSMutableDictionary<NSString*, LKRTCMediaStream*>* remoteStreams;
@property(nonatomic, strong, nonnull)
    NSMutableDictionary<NSString*, LKRTCMediaStreamTrack*>* remoteTracks;
@property(nonatomic, strong, nonnull) NSString* flutterId;
@property(nonatomic, strong, nullable) FlutterEventSink eventSink;
@property(nonatomic, strong, nullable) FlutterEventChannel* eventChannel;
@end

@interface FlutterWebRTCPlugin (RTCPeerConnection)

- (void)peerConnectionCreateOffer:(nonnull NSDictionary*)constraints
                   peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                           result:(nonnull FlutterResult)result;

- (void)peerConnectionCreateAnswer:(nonnull NSDictionary*)constraints
                    peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                            result:(nonnull FlutterResult)result;

- (void)peerConnectionSetLocalDescription:(nonnull LKRTCSessionDescription*)sdp
                           peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                                   result:(nonnull FlutterResult)result;

- (void)peerConnectionSetRemoteDescription:(nonnull LKRTCSessionDescription*)sdp
                            peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                                    result:(nonnull FlutterResult)result;

- (void)peerConnectionAddICECandidate:(nonnull LKRTCIceCandidate*)candidate
                       peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                               result:(nonnull FlutterResult)result;

- (void)peerConnectionGetStats:(nonnull LKRTCPeerConnection*)peerConnection
                        result:(nonnull FlutterResult)result;

- (void)peerConnectionGetStatsForTrackId:(nonnull NSString*)trackID
                          peerConnection:(nonnull LKRTCPeerConnection*)peerConnection
                                  result:(nonnull FlutterResult)result;

- (nonnull LKRTCMediaConstraints*)parseMediaConstraints:(nonnull NSDictionary*)constraints;

- (void)peerConnectionSetConfiguration:(nonnull LKRTCConfiguration*)configuration
                        peerConnection:(nonnull LKRTCPeerConnection*)peerConnection;

- (void)peerConnectionGetRtpReceiverCapabilities:(nonnull NSDictionary*)argsMap
                                          result:(nonnull FlutterResult)result;

- (void)peerConnectionGetRtpSenderCapabilities:(nonnull NSDictionary*)argsMap
                                        result:(nonnull FlutterResult)result;

- (void)transceiverSetCodecPreferences:(nonnull NSDictionary*)argsMap
                                result:(nonnull FlutterResult)result;

- (nullable NSString*)stringForSignalingState:(RTCSignalingState)state;

- (nullable NSString*)stringForICEGatheringState:(RTCIceGatheringState)state;

- (nullable NSString*)stringForICEConnectionState:(RTCIceConnectionState)state;

- (nullable NSString*)stringForPeerConnectionState:(RTCPeerConnectionState)state;

@end
