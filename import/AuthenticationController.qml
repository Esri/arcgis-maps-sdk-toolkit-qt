import QtQml 2.12

import Esri.ArcGISRuntime 100.8

QtObject {
  id: authenticationController

  property url currentChallengeUrl: internal.currentChallenge ? internal.currentChallenge.authorizationUrl : "";
  property string currentAuthenticatingHost: internal.currentChallenge ? internal.currentChallenge.authenticatingHost.toString() : "";
  property int currentChallengeType: internal.currentChallenge ? internal.currentChallenge.authenticationChallengeType : Enums.AuthenticationChallengeTypeUnknown;
  property int currentChallengeFailureCount: internal.currentChallenge ? internal.currentChallenge.failureCount : 0;
  property var clientCertificateInfos: AuthenticationManager.clientCertificateInfos

  function addClientCertificate(...args) {
      return AuthenticationManager.addClientCertificate(...args);
  }

  function continueWithUsernamePassword(...args) {
      internal.currentChallenge.continueWithUsernamePassword(...args);
      internal.currentChallenge = null;
  }

  function continueWithOAuthAuthorizationCode(...args) {
      internal.currentChallenge.continueWithOAuthAuthorizationCode(...args);
      internal.currentChallenge = null;
  }

  function continueWithClientCertificate(...args) {
      internal.currentChallenge.continueWithClientCertificate(...args);
      internal.currentChallenge = null;
  }

  function continueWithSslHandshake(...args) {
      internal.currentChallenge.continueWithSslHandshake(...args);
      internal.currentChallenge = null;
  }

  function cancel(...args) {
      internal.currentChallenge.cancel(...args);
      internal.currentChallenge = null;
  }
  
  property QtObject internal: QtObject {
      property AuthenticationChallenge currentChallenge: null;

      property Connections managerConnection: Connections {
          target: AuthenticationManager
          ignoreUnknownSignals: false
          onAuthenticationChallenge: {
              console.log("CHALLENGE");
              internal.currentChallenge = challenge;
          }
      }
  }
}
