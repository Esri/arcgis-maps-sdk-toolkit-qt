/*******************************************************************************
 *  Copyright 2012-2019 Esri
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ******************************************************************************/

#import <ARKit/ARKit.h>
#include <QString>

namespace Esri {
namespace ArcGISRuntime {
namespace Toolkit {
namespace Internal {

QString worldMappingStatusToString(ARWorldMappingStatus status)
{
  switch (status)
  {
    case ARWorldMappingStatusNotAvailable:
      return "ARWorldMappingStatusNotAvailable";
    case ARWorldMappingStatusLimited:
      return "ARWorldMappingStatusLimited";
    case ARWorldMappingStatusExtending:
      return "ARWorldMappingStatusExtending";
    case ARWorldMappingStatusMapped:
      return "ARWorldMappingStatusMapped";
    default:
      return "Unknown status";
  }
}

QString worldMappingStatusToDescription(ARWorldMappingStatus status)
{
  switch (status)
  {
    case ARWorldMappingStatusNotAvailable:
      return "No world map is available.";
    case ARWorldMappingStatusLimited:
      return "World tracking has not yet sufficiently mapped the area around the current device position.";
    case ARWorldMappingStatusExtending:
      return "World tracking has mapped recently visited areas, but is still mapping around the current device position.";
    case ARWorldMappingStatusMapped:
      return "World tracking has adequately mapped the visible area.";
    default:
      return "Unknown status";
  }
}

QString trackingStateToString(ARTrackingState trackingState)
{
  switch (trackingState)
  {
    case ARTrackingStateNotAvailable:
      return "ARTrackingStateNotAvailable";
    case ARTrackingStateLimited:
      return "ARTrackingStateLimited";
    case ARTrackingStateNormal:
      return "ARTrackingStateNormal";
    default:
      return "Unknown state";
  }
}

QString trackingStateToDescription(ARTrackingState trackingState)
{
  switch (trackingState)
  {
    case ARTrackingStateNotAvailable:
      return "Camera position tracking is not available.";
    case ARTrackingStateLimited:
      return "Tracking is available, but the quality of results is questionable.";
    case ARTrackingStateNormal:
      return "Camera position tracking is providing optimal results.";
    default:
      return "Unknown state";
  }
}

QString trackingStateReasonToString(ARTrackingStateReason trackingStateReason)
{
  switch (trackingStateReason)
  {
    case ARTrackingStateReasonNone:
      return "ARTrackingStateReasonNone";
    case ARTrackingStateReasonInitializing:
      return "ARTrackingStateReasonInitializing";
    case ARTrackingStateReasonRelocalizing:
      return "ARTrackingStateReasonRelocalizing";
    case ARTrackingStateReasonExcessiveMotion:
      return "ARTrackingStateReasonExcessiveMotion";
    case ARTrackingStateReasonInsufficientFeatures:
      return "ARTrackingStateReasonInsufficientFeatures";
    default:
      return "Unknown reason";
  }
}

QString trackingStateReasonToDescription(ARTrackingStateReason trackingStateReason)
{
  switch (trackingStateReason)
  {
    case ARTrackingStateReasonNone:
      return "The current tracking state is not limited.";
    case ARTrackingStateReasonInitializing:
      return "The AR session has not yet gathered enough camera or motion data to provide "
             "tracking information.";
    case ARTrackingStateReasonRelocalizing:
      return "The AR session is attempting to resume after an interruption.";
    case ARTrackingStateReasonExcessiveMotion:
      return "The device is moving too fast for accurate image-based position tracking.";
    case ARTrackingStateReasonInsufficientFeatures:
      return "The scene visible to the camera does not contain enough distinguishable "
             "features for image-based position tracking.";
    default:
      return "Unknown reason";
  }
}

QString arErrorCodeToString(ARErrorCode errorCode)
{
  switch (errorCode)
  {
    case ARErrorCodeCameraUnauthorized:
      return "ARErrorCodeCameraUnauthorized";
    case ARErrorCodeFileIOFailed:
      return "ARErrorCodeFileIOFailed";
    case ARErrorCodeInsufficientFeatures:
      return "ARErrorCodeInsufficientFeatures";
    case ARErrorCodeInvalidConfiguration:
      return "ARErrorCodeInvalidConfiguration";
    case ARErrorCodeInvalidReferenceImage:
      return "ARErrorCodeInvalidReferenceImage";
    case ARErrorCodeInvalidReferenceObject:
      return "ARErrorCodeInvalidReferenceObject";
    case ARErrorCodeInvalidWorldMap:
      return "ARErrorCodeInvalidWorldMap";
    case ARErrorCodeMicrophoneUnauthorized:
      return "ARErrorCodeMicrophoneUnauthorized";
    case ARErrorCodeObjectMergeFailed:
      return "ARErrorCodeObjectMergeFailed";
    case ARErrorCodeSensorFailed:
      return "ARErrorCodeSensorFailed";
    case ARErrorCodeSensorUnavailable:
      return "ARErrorCodeSensorUnavailable";
    case ARErrorCodeUnsupportedConfiguration:
      return "ARErrorCodeUnsupportedConfiguration";
    case ARErrorCodeWorldTrackingFailed:
      return "ARErrorCodeWorldTrackingFailed";
    default:
      return "Unknown error code";
  }
}

QString arErrorCodeToDescription(ARErrorCode errorCode)
{
  switch (errorCode)
  {
    case ARErrorCodeCameraUnauthorized:
      return "The user has denied your app permission to use the device camera.";
    case ARErrorCodeFileIOFailed:
      return "An error code that indicates ARKit was unable to read or write a required file.";
    case ARErrorCodeInsufficientFeatures:
      return "An error code that indicates ARKit is without the required features to complete a task.";
    case ARErrorCodeInvalidConfiguration:
      return "An error code that indicates the configuration contains ambiguous or erroneous data.";
    case ARErrorCodeInvalidReferenceImage:
      return "An error code that indicates a reference image is unusable.";
    case ARErrorCodeInvalidReferenceObject:
      return "An error code that indicates a reference object is unusable.";
    case ARErrorCodeInvalidWorldMap:
      return "An error code that indicates a world map is unusable.";
    case ARErrorCodeMicrophoneUnauthorized:
      return "An error code that indicates the app does not have user permission to use the microphone.";
    case ARErrorCodeObjectMergeFailed:
      return "An error code that indicates ARKit was unable to merge a detected object.";
    case ARErrorCodeSensorFailed:
      return "An error code that indicates a sensor failed to provide the required input.";
    case ARErrorCodeSensorUnavailable:
      return "An error code that indicates a sensor required to run the session is not available.";
    case ARErrorCodeUnsupportedConfiguration:
      return "An error code that indicates the configuration you ran is not supported on the iOS device.";
    case ARErrorCodeWorldTrackingFailed:
      return "An error code that indicates world tracking has encountered a fatal error.";
    default:
      return "Unknown error code";
  }
}

} // Internal namespace
} // Toolkit namespace
} // ArcGISRuntime namespace
} // Esri namespace
