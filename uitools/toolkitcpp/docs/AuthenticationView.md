[![ArcGIS Maps SDK for Qt](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt-0b5394)](https://developers.arcgis.com/qt/) [![ArcGIS Maps SDK for Qt toolkit](https://img.shields.io/badge/ArcGIS%20Maps%20SDK%20for%20Qt%20toolkit-ea4d13)](https://github.com/Esri/arcgis-maps-sdk-toolkit-qt) [![Qt Quick UI components](https://img.shields.io/badge/Qt%20Qt%20Quick%20UI%20components-ea4d13)](../../toolkitcpp/)

# AuthenticationView

The [AuthenticationView](https://developers.arcgis.com/qt/toolkit/api-reference/qml-authenticationview.html) handles authentication challenges and automatically launching the appropriate UI for each type of authentication. Declare an AuthenticationView in your QML file. The AuthenticationView will then be connected to all authentication challenges, and will automatically launch the appropriate view for the type of challenge. Supported security formats include:

- ArcGIS Token (UserCredentialsView)
- HTTP Basic (UserCredentialsView)
- HTTP Digest (UserCredentialsView)
- IWA (UserCredentialsView)
- OAuth 2.0 (OAuth2View)
- SAML (OAuth2View)
- PKI (ClientCertificateView)
- SSL Handshake Warnings (SslHandshakeView)

NOTE: OAuth 2.0 uses a WebView. To use an OAuthView you must call `QtWebView::initialize()` before the QGuiApplication instance is created. See [Qt WebView](https://doc.qt.io/qt-6/qtwebview-index.html).

![AuthenticationView image](https://developers.arcgis.com/qt/toolkit/api-reference/images/authenticationview_qml.png)