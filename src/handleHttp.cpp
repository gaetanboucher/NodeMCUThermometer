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

String getMenu();
String getStyleSheet();

/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  String Page;
  Page += F("<html>");
  Page += F("<head>");
  Page += getStyleSheet();
  Page += F("</head>");
  Page += F("<body>");
  Page += F("<div class='grid-container'>");
  Page += F("  <div class='Head'>");
  Page += F("    <div class='Menu'>");
  Page += getMenu();
  Page += F("    </div>");
  Page += F("    <div class='Title'>");
  Page += F("    <h1><b>Wifi Thermometer</b></h1>");
  Page += F("    </div>");
  Page += F("  </div>");
  Page += F("  <div class='Body'>");
  Page += F("    <div class='Center'>");
  Page += F("      <div class='Center-Top'>");
  Page += String(F("<p>Probe 1: "))+getProbeTemp(1)+F("&deg;C</p>");
  Page += F("      </div>");
  Page += F("      <div class='Center-Bottom'>");
  Page += String(F("<p>Probe 1: "))+getProbeTemp(1)+F("&deg;C</p>");
  Page += F("      </div>");
  Page += F("    </div>");
  Page += F("    <div class='Left'></div>");
  Page += F("    <div class='Right'></div>");
  Page += F("  </div>");
  Page += F("  <div class='Foot'>");
  Page += F("    <div class='Battery'><p>100%</p></div>");
  Page += F("    <div class='Network'><p>NetworkID</p></div>");
  Page += F("    <div class='Status'></div>");
  Page += F("  </div>");
  Page += F("</div>");


  Page += F("</body>");
  Page += F("</html>");

  server.send(200, "text/html", Page);
}

String getStyleSheet()
{
  // https://grid.layoutit.com/?id=2KS4RJk
  // https://grid.layoutit.com/?id=Qz70gEg
  // https://grid.layoutit.com/?id=7KaPlza
  String header=
F(\
"<style>\
        .grid-container {\
          display: grid;\
          grid-template-columns: 1fr;\
          grid-template-rows: 0.2fr 1.8fr 0.2fr;\
          grid-template-areas: 'Head' 'Body' 'Foot';\
        }\
        \
        .Head {\
          display: grid;\
          grid-template-columns: 0.1fr 1.9fr;\
          grid-template-rows: 1fr;\
          grid-template-areas: 'Menu Title';\
          grid-area: Head;\
        }\
        \
        .Menu { grid-area: Menu; }\
        \
        .Title { grid-area: Title; }\
        \
        .Body {\
          display: grid;\
          grid-template-columns: 1fr 1fr 1fr 1fr 1fr;\
          grid-template-rows: 1fr [whatsthis] 1fr;\
          grid-template-areas: 'Left Center Center Center Right' 'Left Center Center Center Right';\
          grid-area: Body;\
        }\
        \
        .Center {\
          display: grid;\
          grid-template-columns: 1fr 1fr 1fr;\
          grid-template-rows: 1fr 1fr;\
          grid-template-areas: 'Center-Top Center-Top Center-Top' 'Center-Bottom Center-Bottom Center-Bottom';\
          grid-area: Center;\
        }\
        \
        .Center-Top { grid-area: Center-Top; }\
        \
        .Center-Bottom { grid-area: Center-Bottom; }\
        \
        .Left { grid-area: Left; }\
        \
        .Right { grid-area: Right; }\
        \
        .Foot {\
          display: grid;\
          grid-template-columns: 2.1fr 0.7fr 0.2fr;\
          grid-template-rows: 1fr;\
          grid-template-areas: 'Status Network Battery';\
          grid-area: Foot;\
        }\
        \
        .Battery { grid-area: Battery; }\
        \
        .Network { grid-area: Network; }\
        \
        .Status { grid-area: Status; }\
        \
        body {\
          background-color: #141414;\
        }\
\
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
          color: silver;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:30px;\
          font-family:Arial;\
        }\
        td,input {\
          color: lemonchiffon;\
          margin-left: 40px;\
          margin-right: 40px;\
          font-size:30px;\
          font-family:Arial;\
        }\
        a {\
          font-size:30px;\
          font-family:Arial;\
        }\
        ul {\
          list-style-type: none;\
          margin: 0;\
          padding: 0;\
          overflow: hidden;\
          background-color: red;\
        }\
        \
        li {\
          float: left;\
        }\
        \
        li a, .dropbtn {\
          display: inline-block;\
          color: white;\
          text-align: center;\
          padding: 14px 16px;\
          text-decoration: none;\
        }\
        \
        li a:hover, .dropdown:hover .dropbtn {\
          background-color: red;\
        }\
        \
        li.dropdown {\
          display: inline-block;\
        }\
        \
        .dropdown-content {\
          display: none;\
          position: absolute;\
          background-color: #f9f9f9;\
          min-width: 160px;\
          box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);\
          z-index: 1;\
        }\
        \
        .dropdown-content a {\
          color: black;\
          padding: 12px 16px;\
          text-decoration: none;\
          display: block;\
          text-align: left;\
        }\
        \
        .dropdown-content a:hover {background-color: #f1f1f1;}\
        \
        .dropdown:hover .dropdown-content {\
          display: block;\
        }\
      </style>\
    ");
  return header;
}

String getMenu()
{
  String header=
F( "<div class='Head'>\
        <div class='Title'>\
        </div>\
        <div class='Menu' id='h_nav_bar'>\
          <ul style='border-collapse: collapse'>\
            <li class='dropdown'>\
              <a href='javascript:void(0)' class='dropbtn' style='border-collapse: collapse;height: 100px; width: 100px; margin: 0; padding: 0;'>\
                <table  style='border-collapse: collapse;height: 100px; width: 100px; margin: 0; padding: 0;'>\
                  <tr>\
                    <td style='border-collapse: collapse;width: 15px; margin: 0; padding: 0;'>\
                    </td>\
                    <td>\
                      <table style='height: 100px; width: 70px; border-collapse: collapse; margin: 0; padding: 0;'>\
                      <tr>\
                        <td style='border-collapse: collapse;width: 80px; margin: 0; padding: 0;border-bottom: 2px solid white;'>\
                        </td>\
                      </tr>\
                      <tr>\
                        <td style='border-collapse: collapse;width: 80px; margin: 0; padding: 0;border-bottom: 2px solid white;'>\
                        </td>\
                      </tr>\
                      <tr>\
                        <td style='border-collapse: collapse;width: 80px; margin: 0; padding: 0;border-bottom: 2px solid white;'>\
                        </td>\
                      </tr>\
                      <tr>\
                        <td style='border-collapse: collapse;width: 80px; margin: 0; padding: 0;'>\
                        </td>\
                      </tr>\
                      </table>\
                    </td>\
                    <td style='border-collapse: collapse;width: 15px; margin: 0; padding: 0;'>\
                    </td>\
                  </tr>\
                </table>\
              </a>\
              <div class='dropdown-content'>\
                <a href='/home'>Home</a>\
                <a href='/setup'>Setup</a>\
                <a href='/wifi'>Wifi</a>\
                <a href='/help'>Help!</a>\
              </div>\
            </li>\
          </ul>\
        </div>\
      </div>\
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
  Page += F("<html>");
  Page += getStyleSheet();
  Page += F("<div class='grid-container'>");
  Page += getMenu();
  Page += F("<body>");
  Page += F("<div class='Main'>");
  if (server.client().localIP() == apIP) {
    Page += String(F("<p>You are connected through the soft AP: ")) + softAP_ssid + F("</p>");
  } else {
    Page += String(F("<p>You are connected through the wifi network: ")) + String(ssid) + F("</p>");
  }
  Page +=
    String(F("<table><tr><th align='left'>SoftAP config</th></tr>"
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
  Page += F("</table>"
            "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
            "<input type='text' placeholder='network' name='n'/>"
            "<br /><input type='password' placeholder='password' name='p'/>"
            "<br /><input type='submit' value='Connect/Disconnect'/></form>");
  Page += F("<div>");
  Page += F("<div>");
  Page += F("</body></html>");
  server.send(200, "text/html", Page);
  server.client().stop(); // Stop is needed because we sent no content length
}

void handleSetup() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F("<html>");
  Page += getStyleSheet();
  Page += F("<div class='grid-container'>");
  Page += getMenu();
  Page += F("<body>");
  Page += F("<div class='Main'>");
  Page += String(F("<h1><b>Setup page coming soon...</b></h1>"));
  Page += F("<div>");
  Page += F("<div>");
  Page += F("</body></html>");
  server.send(200, "text/html", Page);
}

void handleHelp() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");

  String Page;
  Page += F("<html>");
  Page += getStyleSheet();
  Page += F("<div class='grid-container'>");
  Page += getMenu();
  Page += F("<body>");
  Page += F("<div class='Main'>");
  Page += String(F("<h1><b>Help page coming soon...</b></h1>"));
  Page += F("<div>");
  Page += F("<div>");
  Page += F("</body></html>");
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

