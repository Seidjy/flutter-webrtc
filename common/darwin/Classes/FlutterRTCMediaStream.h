#import <Foundation/Foundation.h>
#import "FlutterWebRTCPlugin.h"

@interface LKRTCMediaStreamTrack (Flutter)
@property(nonatomic, strong, nonnull) id settings;
@end

@interface FlutterWebRTCPlugin (RTCMediaStream)

- (void)getUserMedia:(nonnull NSDictionary*)constraints result:(nonnull FlutterResult)result;

- (void)createLocalMediaStream:(nonnull FlutterResult)result;

- (void)getSources:(nonnull FlutterResult)result;

- (void)mediaStreamTrackHasTorch:(nonnull LKRTCMediaStreamTrack*)track result:(nonnull FlutterResult)result;

- (void)mediaStreamTrackSetTorch:(nonnull LKRTCMediaStreamTrack*)track
                           torch:(BOOL)torch
                          result:(nonnull FlutterResult)result;

- (void)mediaStreamTrackSetZoom:(nonnull LKRTCMediaStreamTrack*)track
                           zoomLevel:(double)zoomLevel
                          result:(nonnull FlutterResult)result;

- (void)mediaStreamTrackSwitchCamera:(nonnull LKRTCMediaStreamTrack*)track result:(nonnull FlutterResult)result;

- (void)mediaStreamTrackCaptureFrame:(nonnull LKRTCMediaStreamTrack*)track
                              toPath:(nonnull NSString*)path
                              result:(nonnull FlutterResult)result;

- (void)selectAudioInput:(nonnull NSString*)deviceId result:(nullable FlutterResult)result;

- (void)selectAudioOutput:(nonnull NSString*)deviceId result:(nullable FlutterResult)result;
@end
