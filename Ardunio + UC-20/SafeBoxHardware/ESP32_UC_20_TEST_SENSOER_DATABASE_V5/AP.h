#include <Arduino.h>


void notFound(AsyncWebServerRequest *request);
String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);
String processor(const String& var);