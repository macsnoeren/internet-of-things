# A great user interface for the Wemos
This project is about creating a great web user interface for a small device like the Wemos.

## Introduction
One year ago we started with a project Smart Meter. This board uses a Wemos D1 mini (ESP8266) and is hooked to the energy meter at your house. Every couple of seconds, the measurements are sent using MQTT to the centralized server. A nice project. One idea was, when the device was installed, that the user could see on a web interface (by connecting to the Wemos) a first glance of the measurements and was able to perform all kinds of aspect to configure the device and also how the data is to be shared with our research organisation. Immediatly it was said that this idea is bad due to the fact that the Wemos has limited memory and flash on board. Which is off course true. But ... what if we do not need this memory and flash to create great user interfaces with the Wemos? Anyway, having a great user interface is already a big win to accept these kind of devices.

## Solution
While the Wemos has limited storage capabilities, the great web user interface should not be stored on this storage at all. The main idea is to spin of a basic web interface that dynamically loads itself from resources on the Internet. This already requires that the Wemos is connected to the Internet. It is possible to load great libraries and css from the Internet, so let the browser do the work and only use the Wemos memory to implement the specific aspects. In order to implement this solution, the following steps will be taken into account:
1. Internet webserver hosting the required great web interface files.
2. Wemos webserver spin-of website to load the great web interface.
3. Execute functions on the Wemos to implement a real IoT device using the web interface.


## Case: Light switch
The case that is used is that the Wemos is connected to real lights of the garden. Using the great user interface, it is possible to configure the Wemos and switch the lights on and off.

## Step 1: Webserver hosting great user interface
I have a server on the Internet that uses nginx as webserver. Creating a folder on this domain /wemos to host all the required files. In order to let this work, we need to enable Cross-Origin Resource Sharing (CORS) on HTTP level. This requires the proper configuration of the nginx web server. When you use a different web server, you need to find how to appropiately configure this for your domain. Concerning the security of the solution, I would only enable CORS for a specific part of the domain, namely for hosting the wemos files. See below the configuration file of the nginx website. It is a working website and I have created a directory named "wemos". Within the configuration I have added the header ```Access-Control-Allow-Origin``` only for the location wemos. All files that are hosted will contain this header flag. When the Wemos requests a file from this domain, it will get the appropiate contents and load the required HTML/Javascript/CSS to create great web userinterfaces for this small Wemos device.

```nginx
server {
    listen 443 ssl;
    listen [::]:443 ssl;

    server_name xxxxxxxx.xx;

    ssl_certificate xxxxxxxx.pem;
    ssl_certificate_key xxxxxxxx.pem;
    ssl_session_cache shared:SSL:10m;
    ssl_session_timeout 5m;

    root xxxxxxxx;
    index index.html;

    location /wemos {
        add_header 'Access-Control-Allow-Origin' '*';
    }
}
```

Just to check that it works, we will create a ```demo.html``` file within the /wemos directory of this domain. It does not require to be great already! I have used the following code:

```html
<h1>CONTROL YOUR LIGHTS</h1>
<div class="custom-control custom-switch">
  <input name="light-1" type="checkbox" class="custom-control-input" id="light-1-switch">
  <label class="light-1-label" for="light-1-switch">Toggle the light</label>
</div>
```

## Step 2: Wemos spin-off website
On the Wemos we need to start the great web user interface with a minimal footprint. We will use external libraries that are available on the Internet. Having a webserver already, you could also provide these libraries from this point. Maybe your own libraries. My advice is when using third-party libraries not to provide them yourself in this case. 

While the Wemos required an Internet connection, we will also use WifiManager to create a portal functionality to enable the user to setup the Wifi credentials in an easy way. When the Wemos is connected, it provides a webserver that can be connected to from the network of the user. Everything is developed using Ardiuno IDE and the extention of the Wemos. See the code of the Wemos below:

```c
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h" //https://github.com/tzapu/WiFiManager

// Web server instance that processes the web requests
std::unique_ptr<ESP8266WebServer> server;

// Callback method for the ESP8266 web server to handle when the resource is not found
void handleNotFound() {
  server->send(404, "text/plain", "File not found");
}

// Setup of the WifiManager
void setupWifiManager () {
  WiFiManager wifiManager;
  //wifiManager.resetSettings(); // When you need to reset the Wifi settings.
  
  wifiManager.autoConnect();  // Auto generated name ESP + ChipID
}

// Setup of the web server ESP8266WebServer
void setupWebServer () {
  server->reset(new ESP8266WebServer(WiFi.localIP(), 80));

  // Handle the root of the web server => return spin-off website here!
  // Load scripts and css to be used in the header, like bootstrap and jquery.
  // For cyber security purposes, use the integrity checker of the browser.
  // Load the external documents from the external web server by using a ajax call (using demo.html).
  // when the document is loaded. See the script tag.
  server->on("/", []() {
    server->send(200, "text/html",
    "<!DOCTYPE html lang=\"en\">"
    "<html>"
    "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">"
    "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\" integrity=\"sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk\" crossorigin=\"anonymous\">"
    "<script src=\"https://code.jquery.com/jquery-3.5.1.slim.min.js\" integrity=\"sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj\" crossorigin=\"anonymous\"></script>"
    "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>"
    "<script src=\"https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js\" integrity=\"sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo\" crossorigin=\"anonymous\"></script>"
    "<script src=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js\" integrity=\"sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI\" crossorigin=\"anonymous\"></script>"
    "<script>$(document).ready(function(){  $(\"button\").click(function(){ $(\"#page\").load(\"https://vmacman.jmnl.nl/wemos/demo.html\"); }); });</script>"
    "<title>Wemos great web interface demo</title>"
    "</head>"
    "<body>"
    "<div id=\"page\">Loading your great web user interface, please wait ...</div>"
    "</body>"
    "</html>");
  });

  // Handle resources that are not found.
  server->onNotFound(handleNotFound);
  server->begin();
  
  Serial.println("HTTP server started.");
  Serial.println(WiFi.localIP());
}

// Setup method that is called only once
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial debugging
  Serial.begin(115200);

  setupWifiManager();
  Serial.println("connected...yeey :)");

  setupWebServer();
}

// Method that is continously called!
void loop() {
  server->handleClient();
}
```

# Ideas that popped up during the development process
Always write down you idea!
1. The client browser can do a lot more things! It is possible to use the clients cpu to calculate keys for encryption. When ready, the result is sent to the Wemos: end-to-end encryption.
