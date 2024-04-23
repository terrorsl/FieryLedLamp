#include"web.h"
#include"FieryLedLamp.h"

AsyncWebServer server(80);

const char *template_list[]=
{
    "EFFECT_LIST",
    "POWER",
    "EFFECT",
    "BRIGHTNESS",
    "SPEED",
    "SCALE"
};

String openTemplate(const char *page_name)
{
    String content;
    fs::File file = LittleFS.open(page_name,"r");
    for(size_t index=0;index<file.size();index++)
    {
        content += file.read();
    }
    file.close();
    return content;
}

String findTemplate(String &content)
{
    int first=0;
    while(true)
    {
        first=content.indexOf("%",first);
        if(first==-1)
            break;
        int second=content.indexOf("%",first+1);
        String temp=content.substring(first+1, second-1);
        int pos;
        pos=temp.indexOf("include");
        if(pos!=-1)
        {
            String temp_content=openTemplate(temp.substring(pos+7).c_str());
            content.replace(content.substring(first, second), temp_content);
            first +=temp_content.length();
            continue;
        }
        pos=temp.indexOf("begin");
        if(pos!=-1)
        {
            String end_content=content.substring(first, second);
            String content_name = temp.substring(pos+6, temp.length()-1);
            end_content.replace("begin", "end");
            int end=content.indexOf(end_content);

            String tmp=content.substring(second+1, end);

            content.remove(first, end-first+end_content.length());

            content_name = "%{"+ content_name + "}%";
            content.replace(content_name, tmp);
            //String temp_content = temp
        }
    }
    return content;
};

String getContentType(AsyncWebServerRequest *request, String filename)
{
    if (request->hasArg("download")) return F("application/octet-stream");
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

String template_processor(const String& var)
{
    String ret;

    Languages lang = lamp.get_language();
    for(unsigned int index=0;index<sizeof(template_list)/sizeof(template_list[0]);index++)
    {
        if(var!=template_list[index])
            continue;
        switch(index)
        {
        case 0:
            {
                for(int index=0;index<FieryLedLampEffectTypes::MaxEffect;index++)
                {
                    ret += (String("<option value=\"")+index+String("\">")+lang.GetEffect(index)+String("</option>"));
                }
            }
            break;
        default:
            {
                ret = lang.GetTemplateName(index-1);
            }
            break;
        }
        break;
    }
    return ret;
}

void notFound(AsyncWebServerRequest *request) {
    String contentType = getContentType(request, request->url());
    if(LittleFS.exists(request->url()))
    {
        request->send(LittleFS, request->url(),contentType);
    }
    else
        request->send(404, "text/plain", "Not found");
}
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    //Handle body
    DBG_PRINT("%s",request->url().c_str());
}
void mainPage(AsyncWebServerRequest *request) {
    request->send(LittleFS, "/web/main.html",String(), false, template_processor);
}
void effectPage(AsyncWebServerRequest *request) {
    String content = openTemplate("/web/effect.html");
    content = findTemplate(content);
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", content.c_str(), template_processor);
    request->send(response);
    //request->send(LittleFS, "/web/effect.html",String(), false, template_processor);
}
void effectGroupPage(AsyncWebServerRequest *request) {

}
/*void effectPostPage(AsyncWebServerRequest *request) {
    request->params();
}*/
void effectPostPage(AsyncWebServerRequest *request, JsonVariant &json)
{
    if(json.containsKey("power"))
    {
        lamp.power_button(json["power"].as<bool>());
        request->send(200);
        return;
    }
    if(json.containsKey("effect"))
    {
        
    }
    request->send(403);
};
void saveMqttPage(AsyncWebServerRequest *request) {
    request->params();
}

void FieryLedLamp::setup_web_server()
{
    DBG_PRINT("setup web\n");
    server.onNotFound(notFound);
    server.onRequestBody(onBody);

    server.on("/", HTTP_GET, mainPage);
    server.on("/effect", HTTP_GET, effectPage);
    server.on("/effect/fire", HTTP_GET, effectGroupPage);
    //server.on("/effect", HTTP_POST, effectPostPage);
    server.on("/wifi", HTTP_GET, effectPage);
    server.on("/mqtt", HTTP_GET, effectPage);
    server.on("/mqtt", HTTP_POST, saveMqttPage);

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/effect", effectPostPage);
    server.addHandler(handler);
    server.begin();
}
void FieryLedLamp::connect_web()
{
};

/*#include"FieryLedLamp.h"

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
}*/