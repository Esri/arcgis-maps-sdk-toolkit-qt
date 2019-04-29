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

#include "ArcGISARView.h"

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

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISARView::ArcGISARView(QObject* parent):
  SceneQuickView(parent)
{
}

/*!
  \brief A constructor that accepts an optional \a parent.
 */
ArcGISARView::ArcGISARView(bool renderVideoFeed, QObject* parent):
  SceneQuickView(parent)
{
}

/*!
   \brief The destructor.
 */
ArcGISARView::~ArcGISARView()
{
}

/*!
  \brief ...
 */
Camera ArcGISARView::originCamera()
{
}

/*!
  \brief ...
 */
void ArcGISARView::setOriginCamera(const Camera& originCamera)
{
}

/*!
  \brief ...
 */
double ArcGISARView::translationTransformationFactor()
{
}

/*!
  \brief ...
 */
double ArcGISARView::translationTransformationFactor() const
{

}

/*!
  \brief ...
 */
void ArcGISARView::setTranslationTransformationFactor(double translationTransformationFactor)
{

}

/*!
  \brief ...
 */
bool ArcGISARView::renderVideoFeed() const
{

}

/*!
  \brief ...
 */
void ArcGISARView::setRenderVideoFeed(bool renderVideoFeed)
{

}

/*!
  \brief ...
 */
Point ArcGISARView::arScreenToLocation(const Point& screenPoint) const
{

}

/*!
  \brief ...
 */
void ArcGISARView::resetTracking()
{

}

/*!
  \brief ...
 */
bool ArcGISARView::resetUsingLocationService()
{

}

/*!
  \brief ...
 */
void ArcGISARView::resetUsingSpacialAnchor()
{

}

/*!
  \brief ...
 */
void ArcGISARView::startTracking()
{

}

/*!
  \brief ...
 */
void ArcGISARView::stopTracking()
{

}

// signals

/*!
  \fn void ArcGISARView::originCameraChanged();
  \brief Signal emitted when the \l originCamera property changes.
 */

/*!
  \fn void ArcGISARView::translationTransformationFactorChanged();
  \brief Signal emitted when the \l translationTransformationFactor property changes.
 */

/*!
  \fn void ArcGISARView::renderVideoFeedChanged();
  \brief Signal emitted when the \l renderVideoFeed property changes.
 */

} // Toolkit
} // ArcGISRuntime
} // Esri
