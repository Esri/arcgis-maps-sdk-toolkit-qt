import QtQuick 2.5
import QtWebView 1.1

WebView {
    id: webView
    anchors.fill: parent
    url: authView.authChallenge.authorizationUrl

    onLoadingChanged: {
        if (loadRequest.status === WebView.LoadSucceededStatus) {
            if (title.indexOf("SUCCESS code=") > -1) {
                var authCode = title.replace("SUCCESS code=", "");
                console.log("authCode: ", authCode);
                authView.authChallenge.continueWithOAuthAuthorizationCode(authCode);
                webView.visible = false;
            } else if (title.indexOf("Denied error=") > -1) {
                console.log("title: ", title);
                authView.authChallenge.cancel();
                webView.visible = false;
            }
        }
    }
}
