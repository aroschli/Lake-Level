# Lake-Level
Lake level monitoring for East TN

This repository is used for collecting lake level data from public websites, then storing the info in a dedicated location that can be read by other programs. This is implemented with GitHub actions using a yml script to scrape the web data every half hour and store results in a data.json file.

Program currently monitors and stores data for Watts Bar Lake and Fort Loudoun Lake. The stored data is the height, or elevation, of the surface of the water within the lake using units of feet.

Upcoming work will create scripts for ESP32 to read the data.json file and output the information to a 7-segment display.
