# Internet-of-Things
Internet-of-Things, also known as IoT, is about small devices connected to the Internet executing all kinds of functionality to make our life easier. Within this repository I would like to explore the boundaries of these small devices. Cyber security and large user interface applications for these small devices are always very challenging and not always fully understood.  Within the directories, I would like to solve some challenges and explain exactly what I have done for you to use in your own projects.

## 1. Great user interface for the Wemos! (see wemos-web-ui)
One year ago we started with a project Smart Meter. This board uses a Wemos D1 mini (ESP8266) and is hooked to the energy meter at your house. Every couple of seconds, the measurements are sent using MQTT to the centralized server. A nice project. One idea was, when the device was installed, that the user could see on a web interface (by connecting to the Wemos) a first glance of the measurements and was able to perform all kinds of aspect to configure the device and also how the data is to be shared with our research organisation. Immediatly it was said that this idea is bad due to the fact that the Wemos has limited memory and flash on board. Which is off course true. But ... what if we do not need this memory and flash to create great user interfaces with the Wemos? Anyway, having a great user interface is already a big win to accept these kind of devices.

## 2. Great leaner user interface for the Wemos! (see wemos-web-ui-leaner)
After finishing the first steps, I found a much leaner implementation for the great interface. With this implementation everything is possible to do from the hosted web server. Or in this case, we take the raw files from github, so we do not need any web server. We start with the use of jQuery and built our web interface further when the client is loading this into the browser.

## 3. My own user interface for my Wemos implementation (see wemos-controller)
This project is about my own project based on the previous implementations I have created my own great web interfacen to control the garden light and water points.
