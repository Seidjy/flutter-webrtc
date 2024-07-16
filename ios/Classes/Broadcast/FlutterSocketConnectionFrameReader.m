//
//  FlutterSocketConnectionFrameReader.m
//  RCTWebRTC
//
//  Created by Alex-Dan Bumbu on 06/01/2021.
//

#include <mach/mach_time.h>

#import <ReplayKit/ReplayKit.h>
#import <LiveKitWebRTC/RTCCVPixelBuffer.h>
#import <LiveKitWebRTC/RTCVideoFrameBuffer.h>

#import "FlutterSocketConnection.h"
#import "FlutterSocketConnectionFrameReader.h"

const NSUInteger lkMaxReadLength = 10 * 1024;

@interface LMessage : NSObject

@property(nonatomic, assign, readonly) CVImageBufferRef limageBuffer;
@property(nonatomic, copy, nullable) void (^didComplete)(BOOL succes, LMessage* message);

- (NSInteger)appendBytes:(UInt8*)buffer length:(NSUInteger)length;

@end

@interface LMessage ()

@property(nonatomic, assign) CVImageBufferRef limageBuffer;
@property(nonatomic, assign) int limageOrientation;
@property(nonatomic, assign) CFHTTPMessageRef lframedMessage;

@end

@implementation LMessage

- (instancetype)init {
  self = [super init];
  if (self) {
    self.limageBuffer = NULL;
  }

  return self;
}

- (void)dealloc {
  CVPixelBufferRelease(_limageBuffer);
}

/** Returns the amount of missing bytes to complete the message, or -1 when not enough bytes were
 * provided to compute the message length */
- (NSInteger)appendBytes:(UInt8*)buffer length:(NSUInteger)length {
  if (!_lframedMessage) {
    _lframedMessage = CFHTTPMessageCreateEmpty(kCFAllocatorDefault, false);
  }

  CFHTTPMessageAppendBytes(_lframedMessage, buffer, length);
  if (!CFHTTPMessageIsHeaderComplete(_lframedMessage)) {
    return -1;
  }

  NSInteger contentLength = [CFBridgingRelease(CFHTTPMessageCopyHeaderFieldValue(
      _lframedMessage, (__bridge CFStringRef) @"Content-Length")) integerValue];
  NSInteger bodyLength =
      (NSInteger)[CFBridgingRelease(CFHTTPMessageCopyBody(_lframedMessage)) length];

  NSInteger missingBytesCount = contentLength - bodyLength;
  if (missingBytesCount == 0) {
    BOOL success = [self unwrapMessage:self.lframedMessage];
    self.didComplete(success, self);

    CFRelease(self.lframedMessage);
    self.lframedMessage = NULL;
  }

  return missingBytesCount;
}

// MARK: Private Methods

- (CIContext*)imageContext {
  // Initializing a CIContext object is costly, so we use a singleton instead
  static CIContext* imageContext = nil;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    imageContext = [[CIContext alloc] initWithOptions:nil];
  });

  return imageContext;
}

- (BOOL)unwrapMessage:(CFHTTPMessageRef)lframedMessage {
  size_t width = [CFBridgingRelease(CFHTTPMessageCopyHeaderFieldValue(
      _lframedMessage, (__bridge CFStringRef) @"Buffer-Width")) integerValue];
  size_t height = [CFBridgingRelease(CFHTTPMessageCopyHeaderFieldValue(
      _lframedMessage, (__bridge CFStringRef) @"Buffer-Height")) integerValue];
  _limageOrientation = [CFBridgingRelease(CFHTTPMessageCopyHeaderFieldValue(
      _lframedMessage, (__bridge CFStringRef) @"Buffer-Orientation")) intValue];

  NSData* messageData = CFBridgingRelease(CFHTTPMessageCopyBody(_lframedMessage));

  // Copy the pixel buffer
  CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, width, height,
                                        kCVPixelFormatType_32BGRA, NULL, &_limageBuffer);
  if (status != kCVReturnSuccess) {
    NSLog(@"CVPixelBufferCreate failed");
    return false;
  }

  [self copyImageData:messageData toPixelBuffer:&_limageBuffer];

  return true;
}

- (void)copyImageData:(NSData*)data toPixelBuffer:(CVPixelBufferRef*)pixelBuffer {
  CVPixelBufferLockBaseAddress(*pixelBuffer, 0);

  CIImage* image = [CIImage imageWithData:data];
  [self.imageContext render:image toCVPixelBuffer:*pixelBuffer];

  CVPixelBufferUnlockBaseAddress(*pixelBuffer, 0);
}

@end

// MARK: -

@interface FlutterSocketConnectionFrameReader () <NSStreamDelegate>

@property(nonatomic, strong) FlutterSocketConnection* connection;
@property(nonatomic, strong) LMessage* message;

@end

@implementation FlutterSocketConnectionFrameReader {
  mach_timebase_info_data_t _timebaseInfo;
  NSInteger _readLength;
  int64_t _startTimeStampNs;
}

- (instancetype)initWithDelegate:(__weak id<LKRTCVideoCapturerDelegate>)delegate {
  self = [super initWithDelegate:delegate];
  if (self) {
    mach_timebase_info(&_timebaseInfo);
  }

  return self;
}

- (void)startCaptureWithConnection:(FlutterSocketConnection*)connection {
  _startTimeStampNs = -1;

  self.connection = connection;
  self.message = nil;

  [self.connection openWithStreamDelegate:self];
}

- (void)stopCapture {
  [self.connection close];
}

// MARK: Private Methods

- (void)readBytesFromStream:(NSInputStream*)stream {
  if (!stream.hasBytesAvailable) {
    return;
  }

  if (!self.message) {
    self.message = [[LMessage alloc] init];
    _readLength = lkMaxReadLength;

    __weak __typeof__(self) weakSelf = self;
    self.message.didComplete = ^(BOOL success, LMessage* message) {
      if (success) {
        [weakSelf didCaptureVideoFrame:message.limageBuffer
                       withOrientation:message.limageOrientation];
      }

      weakSelf.message = nil;
    };
  }

  uint8_t buffer[_readLength];
  NSInteger numberOfBytesRead = [stream read:buffer maxLength:_readLength];
  if (numberOfBytesRead < 0) {
    NSLog(@"error reading bytes from stream");
    return;
  }

  _readLength = [self.message appendBytes:buffer length:numberOfBytesRead];
  if (_readLength == -1 || _readLength > lkMaxReadLength) {
    _readLength = lkMaxReadLength;
  }
}

- (void)didCaptureVideoFrame:(CVPixelBufferRef)pixelBuffer
             withOrientation:(CGImagePropertyOrientation)orientation {
  int64_t currentTime = mach_absolute_time();
  int64_t currentTimeStampNs = currentTime * _timebaseInfo.numer / _timebaseInfo.denom;

  if (_startTimeStampNs < 0) {
    _startTimeStampNs = currentTimeStampNs;
  }

  LKRTCCVPixelBuffer* rtcPixelBuffer = [[LKRTCCVPixelBuffer alloc] initWithPixelBuffer:pixelBuffer];
  int64_t frameTimeStampNs = currentTimeStampNs - _startTimeStampNs;

  RTCVideoRotation rotation;
  switch (orientation) {
    case kCGImagePropertyOrientationLeft:
      rotation = RTCVideoRotation_90;
      break;
    case kCGImagePropertyOrientationDown:
      rotation = RTCVideoRotation_180;
      break;
    case kCGImagePropertyOrientationRight:
      rotation = RTCVideoRotation_270;
      break;
    default:
      rotation = RTCVideoRotation_0;
      break;
  }

  LKRTCVideoFrame* videoFrame = [[LKRTCVideoFrame alloc] initWithBuffer:rtcPixelBuffer
                                                           rotation:rotation
                                                        timeStampNs:frameTimeStampNs];

  [self.delegate capturer:self didCaptureVideoFrame:videoFrame];
}

@end

@implementation FlutterSocketConnectionFrameReader (NSStreamDelegate)

- (void)stream:(NSStream*)aStream handleEvent:(NSStreamEvent)eventCode {
  switch (eventCode) {
    case NSStreamEventOpenCompleted:
      NSLog(@"server stream open completed");
      break;
    case NSStreamEventHasBytesAvailable:
      [self readBytesFromStream:(NSInputStream*)aStream];
      break;
    case NSStreamEventEndEncountered:
      NSLog(@"server stream end encountered");
      [self stopCapture];
      break;
    case NSStreamEventErrorOccurred:
      NSLog(@"server stream error encountered: %@", aStream.streamError.localizedDescription);
      break;

    default:
      break;
  }
}

@end
