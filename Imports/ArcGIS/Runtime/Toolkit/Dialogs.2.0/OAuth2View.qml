import QtQuick 2.5
import QtWebView 1.1

WebView {
    id: webView
    anchors.fill: parent
    property var challenge: authView.authChallenge
    url: challenge ? challenge.authorizationUrl : ""

    onLoadingChanged: {
        if (loadRequest.status === WebView.LoadSucceededStatus) {
            if (title.indexOf("SUCCESS code=") > -1) {
                var authCode = title.replace("SUCCESS code=", "");
                if (challenge)
                    challenge.continueWithOAuthAuthorizationCode(authCode);
                webView.visible = false;
            } else if (title.indexOf("Denied error=") > -1) {
                if (challenge)
                    challenge.cancel();
                webView.visible = false;
            }
        }
    }
}
