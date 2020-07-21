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
 <label class="custom-control-label" for="light-1-switch">Toggle light 1</label>
</div>
<div class="custom-control custom-switch">
 <input name="light-2" type="checkbox" class="custom-control-input" id="light-2-switch">
 <label class="custom-control-label" for="light-2-switch">Toggle light 2</label>
</div>
<div class="custom-control custom-switch">
 <input name="light-3" type="checkbox" class="custom-control-input" id="light-3-switch">
 <label class="custom-control-label" for="light-3-switch">Toggle light 3</label>
</div>
<div class="custom-control custom-switch">
 <input name="light-4" type="checkbox" class="custom-control-input" id="light-4-switch">
 <label class="custom-control-label" for="light-4-switch">Toggle light 4</label>
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

  Serial.println("Connected with Wi-Fi!");
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
		 "<script>$(document).ready(function(){  $(\"#page\").load(\"https://vmacman.jmnl.nl/wemos/demo.html\"); });</script>"
		 "<title>Wemos great web interface demo</title>"
		 "</head>"
		 "<body>"
		 "<div id=\"page\">Loading your great web user interface, please wait ...</div>"
		 "</body>"
		 "</html>");
    });

    // When the website want to have some Wemos action, it can do this using the /action page
    // Get parameters of the request to perform the required action of the user.
    // This method is an example to do this
    server->on("/action", []() {
        String message = "Create some action here...\n\n";
        message += "URI: ";
        message += server->uri();
        message += "\nMethod: ";
        message += (server->method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server->args();
        message += "\n";
        for (uint8_t i = 0; i < server->args(); i++) {
            message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
        }
        server->send(200, "text/plain", message);
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
  setupWebServer();
}

// Method that is continously called!
void loop() {
  server->handleClient(); // Handling the web requests
}
```

When you program the Wemos at this moment, it searched for a Wi-Fi connection. If no Wi-Fi is found, you connect to the AP and setup the Wi-Fi connections. Finally, it will start-up and connect to your Wi-Fi AP and spin-up the web server. Loading the internet page in your favorite browsers shows already an web interface that contains more information than initially programmed into the memory of the Wemos!

## Step 3: Interfacing with Wemos
While having a great web interface, we still need to interface with the Wemos. Internet-of-things is about connecting the physical world to the cyber world. In order to do so, we need to extent the current ```demo.html``` and ```wemos-web-ui.ino``` files to create some action. Both applications (HTML and Wemos) require to know the IP address of the device in order to work.

### Step 3.1: Extend the Wemos application
We need to extent the spin-off website, so the website has knowledge about the Ip address the Wemos is connected to. Within the method setupWebServer I have added some code to the server handling of the ```/action``` directory. A light, using the id ```lightId``` can be used to indicate which light you want to toggle. The status of the light is send using the variable ```value``` that can be ```on``` or ```off```. The following code will switch on and off the builtin led when ```lightId``` is equal to one.
```c
server->on("/action", []() {
    int lightId = 0;
    boolean value = 0;
    for (uint8_t i = 0; i < server->args(); i++) {
        if ( server->argName(i) == "light" ) {
            lightId = server->arg(i).toInt();
        }

        if ( server->argName(i) == "value" ) {
            value = (server->arg(i) == "on" ? true : false);
        }
    }

    if ( lightId == 1 ) {
        if ( value ) {
        digitalWrite(LED_BUILTIN, LOW);
        
        } else {
        digitalWrite(LED_BUILTIN, HIGH);
        }
    }
    server->send(200, "text/plain", "ack");            
});
```

Within the same method, we add information to the root handling. The IP address of the device is added in this case (```var ip = \"" + WiFi.localIP().toString() + "\"; ```). Within the other scripts the variable ```ip``` can be used to redirect to the local IP address of the Wemos.

```c
"<script>var ip = \"" + WiFi.localIP().toString() + "\"; $(document).ready(function(){  $(\"#page\").load(\"https://vmacman.jmnl.nl/wemos/demo.html\"); });</script>"
```

You can find the example of this file in the repo: ```wemos-web-ui.ino```.

### Step 3.2: Creating a new demo file
Creating a new demo file ```demo-action.html``` that is hosted by our web server. We add some scripting and a ```div``` element that displays the information that comes from the Wemos. The following code is used to create the demo-action.html:
```html
<script>
$('#ip').html(ip);
function toggleLight(id, element) {
    $('#action').load("http://" + ip + "/action?light=" + id + "&value=" + (element.checked ? "on" : "off"));
}
</script>

<h1>CONTROL YOUR LIGHTS</h1>
<div class="custom-control custom-switch">
  <input name="light-1" type="checkbox" class="custom-control-input" id="light-1-switch" onchange="toggleLight(1, this);">
  <label class="custom-control-label" for="light-1-switch">Toggle light 1</label>
</div>
<div class="custom-control custom-switch">
  <input name="light-2" type="checkbox" class="custom-control-input" id="light-2-switch" onchange="toggleLight(2, this);">
  <label class="custom-control-label" for="light-2-switch">Toggle light 2</label>
</div>
<div class="custom-control custom-switch">
  <input name="light-3" type="checkbox" class="custom-control-input" id="light-3-switch" onchange="toggleLight(3, this);">
  <label class="custom-control-label" for="light-3-switch">Toggle light 3</label>
</div>
<div class="custom-control custom-switch">
  <input name="light-4" type="checkbox" class="custom-control-input" id="light-4-switch" onchange="toggleLight(4, this);">
  <label class="custom-control-label" for="light-4-switch">Toggle light 4</label>
</div>
<br/>
<h1>Reaction from Wemos <span id="ip"></span></h1>
<div id="action">Action...</div>
```

When you browse to your Wemos page, you can toggle the lights. If you toggle light 1 you should see the blue led on the Wemos is going on and off. You have successfully implemented some action!

# Without a web server?
Yes, that is possible. Of course you need a web server, but you do not need your own web server. You can use github.com as well. Maybe it is a better option, while the Wemos will alway load the most recent version. If you would like to use the github version, you can load the raw file. You can find the link to this file on github. In my case, I have used the development version link: ```https://raw.githubusercontent.com/macsnoeren/internet-of-things/development/wemos-web-ui/demo-action.html```. If you change the spin-off code of the Wemos, you do not require any own web server.

```c
"<script>var ip = \"" + WiFi.localIP().toString() + "\"; $(document).ready(function(){  $(\"#page\").load(\"https://raw.githubusercontent.com/macsnoeren/internet-of-things/development/wemos-web-ui/demo-action.html\"); });</script>"
```

And does it work?

# A great web user interface
What is a great web user interface? Anyway, I show you what I mean by a great web user interface for the Wemos. It is nice that you only need to browse to your Wemos web server and that the Wemos eventually uses the Internet to create a nice big web interface that suits all the needs of the users. This demo can be found in the repo: ```demo-great.html```. Off course you can create much better and greater web user interface. However, the basis to do so is given by this example!

# Ideas that popped up during the development process
Always write down you idea!
1. The client browser can do a lot more things! It is possible to use the clients cpu to calculate keys for encryption. When ready, the result is sent to the Wemos: end-to-end encryption.
