#ifndef D72D6936_566E_4CB0_ABB9_43B5BBE23B2F
#define D72D6936_566E_4CB0_ABB9_43B5BBE23B2F
#ifndef D1C90928_0581_4A89_A0E8_81B41C4580D8
#define D1C90928_0581_4A89_A0E8_81B41C4580D8

    #include <ESPAsyncWebServer.h>
    #include "AsyncJson.h"

    namespace WebServer {

        void addHandler(AsyncWebServer *server, const char *uri, WebRequestMethod method, void (*handler)(AsyncWebServerRequest *request)) {
            server->on(uri, method, handler);
        }

        void addHandler(AsyncWebServer *server, const char *uri, WebRequestMethod method, void (*handler)(AsyncWebServerRequest *request, JsonVariant &json)) {
            AsyncCallbackJsonWebHandler* asyncHandler = new AsyncCallbackJsonWebHandler(uri, handler);
            asyncHandler->setMethod(method);
            server->addHandler(asyncHandler);
        }

        void addHandler(AsyncWebServer *server, const char *uri, WebRequestMethod method, void (*handler)(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)) {
            AsyncCallbackWebHandler* shandler = new AsyncCallbackWebHandler();
            
            shandler->setUri(uri);
            shandler->setMethod(method);
            shandler->onBody(handler);
            shandler->onRequest([](AsyncWebServerRequest *request) {});

            server->addHandler(shandler);
        }

        template<class T>
        void jsonToSetting(T &var, const char *name, const JsonObject &jsonObj) {
            if (jsonObj.containsKey(name)) {
                var = jsonObj[name].as<T>();
            }
        }

        void jsonToSettingStr(char *const &var, const char *name, const JsonObject &jsonObj) {
            if (jsonObj.containsKey(name)) {
                String expName = jsonObj[name].as<String>();
                strcpy(var, expName.c_str());
            }
        }
    }

#endif /* D1C90928_0581_4A89_A0E8_81B41C4580D8 */


#endif /* D72D6936_566E_4CB0_ABB9_43B5BBE23B2F */
