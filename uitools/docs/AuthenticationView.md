# Authentication View

A view for handling authentication challenges and automatically launching the appropriate UI for each type of authentication. Declare an AuthenticationView in your QML file. The AuthenticationView will then be connected to all authentication challenges, and will automatically launch the appropriate view for the type of challenge. Supported security formats include:

- ArcGIS Token (UserCredentialsView)
- HTTP Basic (UserCredentialsView)
- HTTP Digest (UserCredentialsView)
- IWA (UserCredentialsView)
- OAuth 2.0 (OAuth2View)
- SAML (OAuth2View)
- PKI (ClientCertificateView)
- SSL Handshake Warnings (SslHandshakeView)

NOTE: OAuth 2.0 uses a WebView. To use an OAuthView you must call `QtWebView::initialize()` before the QGuiApplication instance is created. See [Qt WebView](https://doc.qt.io/qt-6/qtwebview-index.html).

![Authentication View image](https://developers.arcgis.com/qt/toolkit/api-reference/images/authenticationview_qml.png)

Details on how to use the Basemap Galley can be found here:
- [QML Type/UI Controls/QtQuick](https://developers.arcgis.com/qt/toolkit/api-reference/qml-authenticationview.html)
- [C++ Classes/Qt Widgets](https://developers.arcgis.com/qt/toolkit/api-reference/esri-arcgisruntime-toolkit-authenticationview.html)
