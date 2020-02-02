#include <Arduino.h>
#include <String.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
#include <credentials.h>
#include <tools.h>

boolean captivePortal();

// Web server
extern ESP8266WebServer server;

/* Soft AP network parameters */
extern IPAddress apIP;
extern IPAddress netMsk;
extern const char *softAP_ssid;
extern const char *softAP_password;
extern char ssid[32];
extern char password[32];
extern const char *myHostname;
extern boolean connect;
extern float getProbeTemp(int prodeId);

String getPageHeader();

/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += getPageHeader();
  Page += F("<div>");
  Page += String(F("<br/><br/><br/>"));
  Page += String(F("<p>Probe 1: "))+getProbeTemp(1)+F("</p>");
  Page += String(F("<p>Probe 2: "))+getProbeTemp(2)+F("</p>");

  Page += F("</div></body></html>");

  server.send(200, "text/html", Page);
}

String getPageHeader()
{
  String header=
F(\
  "<html>\
    <head>\
      <style>\
        body {\
          background-color: #141414;\
        }\
        h1, h2, h3 , h4 {\
          color: lemonchiffon;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:50px;\
          font-family:Arial;\
        }\
        p {\
          color: lemonchiffon;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:30px;\
          font-family:Arial;\
        }\
        th {\
          color: #ff6347;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:30px;\
          font-family:Arial;\
        }\
        td,input {\
          color: #lemonchiffon;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:30px;\
          font-family:Arial;\
        }\
        a {\
          font-size:30px;\
          font-family:Arial;\
        }\
        #h_nav_bar div { height: 200px; width: 100%; }\
        #h_nav_bar a { padding:15px; font-weight:bold; float:left; }\
        #h_nav_bar a:link { color:#d0d0d0; background-color:#0000a0; }\
        #h_nav_bar a:visited { color:#c0c0c0; background-color:#0000a0; }\
        #h_nav_bar a:hover { color:#ffffff; background-color:#000060; }\
        #h_nav_bar a:active { color:#f0f0f0; background-color:#00ff00; }\
      </style>\
      <h1><b>Wifi Thermometer</b></h1><br/>\
      <div id='h_nav_bar'>\
          <a href='/home'>Home</a>\
          <a href='/setup'>Setup</a>\
          <a href='/wifi'>Wifi</a>\
          <a href='/help'>Help!</a>\
      </div><br/>\
    </head>\
    <body>\
    <br/><br/><br/>\
    ");
  return header;
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

/** Wifi config page handler */
void handleWifi() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += getPageHeader();
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + ssid + F("</p>");
  }
  Page +=
    String(F(
             "\r\n<br />"
             "<table><tr><th align='left'>SoftAP config</th></tr>"
             "<tr><td>SSID ")) +
    String(softAP_ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.softAPIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN config</th></tr>"
      "<tr><td>SSID ") +
    String(ssid) +
    F("</td></tr>"
      "<tr><td>IP ") +
    toStringIp(WiFi.localIP()) +
    F("</td></tr>"
      "</table>"
      "\r\n<br />"
      "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += String(F("\r\n<tr><td>SSID ")) + WiFi.SSID(i) + ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? F(" ") : F(" *")) + F(" (") + WiFi.RSSI(i) + F(")</td></tr>");
    }
  } else {
    Page += F("<tr><td'>No WLAN found</td></tr>");
  }
  Page += F(
            "</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n'/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>"
            "</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

void handleSetup() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += getPageHeader();
  Page += String(F("<h1><b>Setup page coming soon...</b></h1>"));
  server.send(200, "text/html", Page);
}

void handleHelp() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += getPageHeader();
  Page += String(F("<h1><b>Help page coming soon...</b></h1>"));
  server.send(200, "text/html", Page);
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave() {
  Serial.println("wifi save");
  server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
  server.arg("p").toCharArray(password, sizeof(password) - 1);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
  saveCredentials();
  connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}

