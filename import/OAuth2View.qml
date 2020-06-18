import QtQuick 2.11
import QtQuick.Controls 2.11
import QtWebView 1.1
import QtQuick.Window 2.12

Dialog {
    id: oAuthView

    property AuthenticationController controller: AuthenticationController {}

    title: webView.title

    closePolicy: Popup.NoAutoClose

    header: Label {
        text: `<h2>${webView.title}</h2>`
        horizontalAlignment: Text.AlignHCenter
    }
    
    footer: DialogButtonBox {
        standardButtons: +DialogButtonBox.Close
        onRejected: controller.cancel();
    }

    implicitWidth: Screen.width
    implicitHeight: Screen.height

    WebView {
        id: webView
        anchors {
            centerIn: parent
            fill: parent
        }
        
        url: controller.currentChallengeUrl

        onLoadingChanged: {
            if (loadRequest.status === WebView.LoadSucceededStatus) {
                forceActiveFocus();
            }
        }

        onTitleChanged: {
            console.log("TITLE: ", title);
            if (title.indexOf("SUCCESS code=") > -1) {
                var authCode = title.replace("SUCCESS code=", "");
                controller.continueWithOAuthAuthorizationCode(authCode);
                oAuthView.accept();
            } else if (title.indexOf("Denied error=") > -1) {
                oAuthView.reject();
            }
        }
    }
}

