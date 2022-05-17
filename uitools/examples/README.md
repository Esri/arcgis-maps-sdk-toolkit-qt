# UI Tools Examples

This module contains two example apps, [cpp_quick](cpp_quick/cpp_quick.pro) and 
[qml_quick](qml_quick/qml_quick.pro).

Both modules cover the the same functionality, demonstrating how to integrate toolkit 
tools into a C++/Quick and QML/Quick app respectively.

## API Key setup

To run the application and use the tools, please have an [API Key](https://developers.arcgis.com/documentation/mapping-apis-and-services/security/api-keys/) ready to to access the features. 

You can set the API key in a number of ways:

- Input the API key in the prompt dialog on application startup.
- Set the API Key in the environment variable `ARCGIS_RUNTIME_API_KEY`.
- Edit `main.cpp` and hardcode the API key value there.
