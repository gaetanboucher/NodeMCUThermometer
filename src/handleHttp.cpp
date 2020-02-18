#include <Arduino.h>
#include <String.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
#include <credentials.h>
#include <tools.h>
#include <css/layout.h>
#include <css/menu.h>
ADC_MODE(ADC_VCC);

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

std::string getMenu();
std::string getStyleSheet();
std::string getPage();

/** Handle root or redirect to captive portal */
void handleRoot() {
  if (captivePortal()) { // If caprive portal redirect instead of displaying the page.
    return;
  }
  std::string page = getPage();
  std::string probes;
  probes += "      <div class='Center-Top'>";
  probes += "        <p>";
  probes +=            String(getProbeTemp(0)).c_str();
  probes += "          &deg;C";
  probes += "        </p>";
  probes += "      </div>";
  probes += "      <div class='Center-Bottom'>";
  probes += "        <p>";
  probes +=            String(getProbeTemp(1)).c_str();
  probes += "          &deg;C";
  probes += "        </p>";
  probes += "      </div>";
  
  page.replace(page.find("*Center*"),strlen("*Center*"), probes);

  server.send(200, "text/html", page.c_str());
}

std::string getPage() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  std::string Page;
  Page += "<html>";
  Page += "<head>";
  Page += getStyleSheet();
  Page += "</head>";
  Page += "<div class='grid-container'>";
  Page += "  <div class='Head'>";
  Page += "    <div class='Menu' id='h_nav_bar'>";
  Page += getMenu();
  Page += "    </div>";
  Page += "    <div class='Title'>";
  Page += "    <h1><b>Wifi Thermometer</b></h1>";
  Page += "    </div>";
  Page += "  </div>";
  Page += "  <div class='Body'>";
  Page += "    <div class='Center'>";
  Page += "*Center*";
  Page += "    </div>";
  Page += "    <div class='Left'></div>";
  Page += "    <div class='Right'></div>";
  Page += "  </div>";
  Page += "  <div class='Foot'>";
  Page += "    <div class='Battery'>";
  Page +=        "<p>";
  float batteryLevel = ESP.getVcc()*100.0/65353.0;
  Page +=           String(batteryLevel).c_str();
  Page +=        "</p>";
  Page += "    </div>";
  Page += "    <div class='Network'>";
  Page += "      <p>";
  Page += WiFi.SSID().c_str();
  Page += "      </p>";
  Page += "    </div>";
  Page += "    <div class='Status'></div>";
  Page += "  </div>";
  Page += "</div>";
  Page += "</html>";

  return Page;
}

std::string getStyleSheet()
{
  // https://grid.layoutit.com/?id=2KS4RJk
  // https://grid.layoutit.com/?id=Qz70gEg
  // https://grid.layoutit.com/?id=7KaPlza
  std::string header="<style>";
  header  += layout;
  header  += menu;
  header  +=  "</style>";
  return header;
}

std::string getMenu()
{
  std::string header=
 "<ul style='border-collapse: collapse'>\
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
    ";
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

  std::string Page;
  Page += "<html>";
  Page += "<head>";
  Page += getStyleSheet();
  Page += "</head>";
  Page += "<div class='grid-container'>";
  Page += "  <div class='Head'>";
  Page += "    <div class='Menu' id='h_nav_bar'>";
  Page += getMenu();
  Page += "    </div>";
  Page += "    <div class='Title'>";
  Page += "    <h1><b>Wifi Thermometer</b></h1>";
  Page += "    </div>";
  Page += "  </div>";
  Page += "  <div class='Body'>";
  Page += "    <div class='Center'>";
  if (server.client().localIP() == apIP) {
    Page += "<p>You are connected through the soft AP: ";
    Page += softAP_ssid;
    Page += "</p>";
  } else {
    Page += "<p>You are connected through the wifi network: ";
    Page += WiFi.SSID().c_str();
    Page += "</p>";
  }
  Page += "<table><tr><th align='left'>SoftAP config</th></tr>";
  Page +=     "<tr><td>SSID ";
  Page += WiFi.softAPSSID().c_str();
  Page +=   "</td></tr>";
  Page +=   "<tr><td>IP ";
  Page += toStringIp(WiFi.softAPIP()).c_str();
  Page +=  "</td></tr>";
  Page +=  "</table>";
  Page +=  "<br />";
  Page +=  "<table><tr><th align='left'>WLAN config</th></tr>";
  Page +=  "<tr><td>SSID ";
  Page +=  WiFi.SSID().c_str();
  Page +=  "</td></tr>";
  Page +=  "<tr><td>IP ";
  Page +=  toStringIp(WiFi.localIP()).c_str();
  Page +=  "</td></tr>";
  Page +=  "</table>";
  Page +=  "<br />";
  Page +=  "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>";
  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n > 0) {
    for (int i = 0; i < n; i++) {
      Page += "<tr><td>SSID ";
      Page += WiFi.SSID(i).c_str();
      Page += ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : " *"); 
      Page += " ("; 
      Page += String(WiFi.RSSI(i)).c_str();
      Page += ")</td></tr>";
      Page += "<tr><td>SSID ";
      Page += WiFi.SSID(i).c_str();
      Page += ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : " *");
      Page += " (";
      Page += String(WiFi.RSSI(i)).c_str();
      Page += ")</td></tr>";
    }
  } else {
    Page += "<tr><td'>No WLAN found</td></tr>";
  }
  Page += "</table>";
  Page +=   "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>";
  Page +=   "<input type='text' placeholder='network' name='n'/>";
  Page +=   "<br /><input type='password' placeholder='password' name='p'/>";
  Page +=   "<br /><input type='submit' value='Connect/Disconnect'/></form>";
  Page += "<div>";
  Page += "    <div class='Left'></div>";
  Page += "    <div class='Right'></div>";
  Page += "  </div>";
  Page += "  <div class='Foot'>";
  Page += "    <div class='Battery'><p>";
  float batteryLevel = ESP.getVcc()*100.0/65353.0;
  Page += "      <p>";
  Page += batteryLevel;
  Page += "</p>";
  Page += "    </div>";
  Page += "    <div class='Network'><p>NetworkID</p></div>";
  Page += "    <div class='Status'></div>";
  Page += "  </div>";
  Page += "</div>";
  server.send(200, "text/html", Page.c_str());
  server.client().stop(); // Stop is needed because we sent no content length
}

void handleSetup() {
  std::string Page = getPage();
  Page.replace(Page.find("*Center*"),strlen("*Center*"), "<h1><b>Setup page coming soon...</b></h1>");
  server.send(200, "text/html", Page.c_str());
}

void handleHelp() {
  std::string Page = getPage();
  Page.replace(Page.find("*Center*"),strlen("*Center*"), "<h1><b>Help page coming soon...</b></h1>");
  server.send(200, "text/html", Page.c_str());
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
  std::string message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri().c_str();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " ";
    message += server.argName(i).c_str();
    message += ": ";
    message += server.arg(i).c_str();
    message += "\n";
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message.c_str());
}

