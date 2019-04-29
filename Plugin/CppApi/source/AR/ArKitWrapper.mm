/*******************************************************************************
 *  Copyright 2012-2018 Esri
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

#include "ArKitWrapper.h"
#import <ARKit/ARKit.h>

/*!
  \class Esri::ArcGISRuntime::Toolkit::ArcGISARView
  \ingroup AR
  \inmodule ArcGISQtToolkit
  \since Esri::ArcGISRuntime 100.6
  \brief ...
  \sa {AR}
 */

namespace Esri
{
namespace ArcGISRuntime
{
namespace Toolkit
{

@interface MyDelegate : NSObject<ARSessionDelegate>

-(void)session:(ARSession*)session didUpdateFrame:(ARFrame*)frame;

@end

@implementation MyDelegate

-(void)session:(ARSession*)session didUpdateFrame:(ARFrame*)frame
{
    simd_float4x4 projectionMatrix = frame.camera.projectionMatrix;
    // simd_float4x4 transform = frame.camera.transform;
    // simd_float3 eulerAngles = frame.camera.eulerAngles;

    NSLog(@"didUpdateFrame: %f", projectionMatrix.columns[0][0]);
}

@end

void initArSession()
{
    // Create an AR session
    ARSession* arSession = [[ARSession alloc] init];

    // Create an AR session configuration
    ARWorldTrackingConfiguration* configuration = [ARWorldTrackingConfiguration new];

    // Run the view's session
    [arSession runWithConfiguration:configuration];

    // Checks if configuration is supported
    if (ARWorldTrackingConfiguration.isSupported)
        NSLog(@"ARWorldTrackingConfiguration is supported");
    else
        NSLog(@"ARWorldTrackingConfiguration is not supported");

    // delegate to get the frames
    MyDelegate* myDelegate = [[MyDelegate alloc]init];
    arSession.delegate = myDelegate;
}

} // Toolkit
} // ArcGISRuntime
} // Esri
