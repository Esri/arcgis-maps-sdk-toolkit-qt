# UI Tools Examples

This module contains the example app, [UitoolsExample](UitoolsExamples.pro).

This module demonstrates how to integrate the Qt toolkit tools into a C++ app using Qt Quick UI controls.

## Access token setup

To run the application and use the tools, please have an [access token](http://links.esri.com/create-an-api-key) ready to access the features. 

You can set the access token in a number of ways:

- Input the access token in the prompt dialog on application startup.
- Set the access token in the environment variable `ARCGIS_RUNTIME_API_KEY`.
- Edit `main.cpp` and hardcode the access token value there.