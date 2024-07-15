#import "FlutterWebRTCPlugin.h"

#import <LiveKitWebRTC/RTCMediaStream.h>
#import <LiveKitWebRTC/RTCVideoFrame.h>
#import <LiveKitWebRTC/RTCVideoRenderer.h>
#import <LiveKitWebRTC/RTCVideoTrack.h>

@interface FlutterRTCVideoRenderer
    : NSObject <FlutterTexture, LKRTCVideoRenderer, FlutterStreamHandler>

/**
 * The {@link RTCVideoTrack}, if any, which this instance renders.
 */
@property(nonatomic, strong) LKRTCVideoTrack* videoTrack;
@property(nonatomic) int64_t textureId;
@property(nonatomic, weak) id<FlutterTextureRegistry> registry;
@property(nonatomic, strong) FlutterEventSink eventSink;

- (instancetype)initWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                              messenger:(NSObject<FlutterBinaryMessenger>*)messenger;

- (void)dispose;

@end

@interface FlutterWebRTCPlugin (FlutterVideoRendererManager)

- (FlutterRTCVideoRenderer*)createWithTextureRegistry:(id<FlutterTextureRegistry>)registry
                                            messenger:(NSObject<FlutterBinaryMessenger>*)messenger;

- (void)rendererSetSrcObject:(FlutterRTCVideoRenderer*)renderer stream:(LKRTCVideoTrack*)videoTrack;

@end
