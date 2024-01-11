#include"FieryLedLamp.h"

String getContentType(String filename)
{
    if (lamp.Web()->hasArg("download")) return F("application/octet-stream");
    else if (filename.endsWith(".htm")) return F("text/html");
    else if (filename.endsWith(".html")) return F("text/html");
    else if (filename.endsWith(".json")) return F("application/json");
    else if (filename.endsWith(".css")) return F("text/css");
    else if (filename.endsWith(".js")) return F("application/javascript");
    else if (filename.endsWith(".png")) return F("image/png");
    else if (filename.endsWith(".gif")) return F("image/gif");
    else if (filename.endsWith(".jpg")) return F("image/jpeg");
    else if (filename.endsWith(".ico")) return F("image/x-icon");
    else if (filename.endsWith(".xml")) return F("text/xml");
    else if (filename.endsWith(".pdf")) return F("application/x-pdf");
    else if (filename.endsWith(".zip")) return F("application/x-zip");
    else if (filename.endsWith(".gz")) return F("application/x-gzip");
    return F("text/plain");
}

void handleFileRead()
{
    String path=lamp.Web()->uri();
    if (path.endsWith("/"))
         path += F("index.html");
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";
    if (LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
        if (LittleFS.exists(pathWithGz))
            path += ".gz";
        File file = LittleFS.open(path, "r");
        lamp.Web()->streamFile(file, contentType);
        file.close();
    }
    else
        lamp.Web()->send(404, "Not found");
    Serial.println("\tFile Not Found");
}

void FieryLedLamp::setup_web_server()
{
    web->onNotFound(handleFileRead);
}