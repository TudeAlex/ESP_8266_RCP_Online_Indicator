  #ESP_8266_RCP_Online_Indicator 

This project is designed to signal presence status with LED, using request to https://panel.rcponline.pl.

## Author

This project was developed by a student intern during a summer internship, with the aim of gaining practical experience in embedded systems and network communication.

Created as part of a university internship – this is not a commercial product.

## INSTALATION AND SETUP

### Step 1: Prepare Arduino IDE
1.  Install the [Arduino IDE](https://www.arduino.cc/en/software).
2.  Add ESP8266 board support. Go to `File > Preferences` and paste the following URL into the "Additional Board Manager URLs" field:
    `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3.  Go to `Tools > Board > Boards Manager`, search for `esp8266`, and install the package.
4.  Select your board from the `Tools > Board` menu (e.g., "NodeMCU 1.0 (ESP-12E Module)").

### Step 2: Library Installation
All the required libraries are included with the ESP8266 board package and do not require separate installation:
* `ESP8266WiFi.h`
* `WiFiClientSecure.h`
* `ESP8266HTTPClient.h`

### Step 3: Configure Credentials
You have two options for configuring the device.

#### Option A: Terminal Menu
This method avoids hardcoding credentials into your source code.
1.  Upload the project to your ESP8266 without any modifications.
2.  Open the **Serial Monitor** in the Arduino IDE (baud rate **115200**).
3.  Follow the on-screen prompts in the menu to enter your Wi-Fi and RCP account credentials.

To use terminal menu you have to modify your code following instructions marked with triple asterisk like this ``` *** comment *** ```.

#### Option B: `secrets.cpp` File
Use this method if the device will operate on a single, unchanging network.
1.  Open the `secrets.cpp` file.
2.  Enter your credentials in the corresponding fields.
    ```cpp
    const char* WIFI_SSID     = "Your_WiFi_SSID";
    const char* WIFI_PASSWORD = "Your_WiFi_Password";
    const char* EMAIL_ADDRESS = "Your-RCP-Email";
    const char* EMAIL_PASSWORD = "Your-RCP-Password";
    ```
3.  Save the file.

### Step 4: Upload the Code
1.  Ensure all project files are in a single folder named `RCP_Project_Public`.
2.  Connect the ESP8266 to your computer and select the correct COM port in `Tools > Port`.
3.  Press the 'Upload' button.

### DEVICE OPERATION

* Upon startup, **all LEDs will briefly light up**. [cite_start]This indicates that the device is attempting to connect to the network and retrieve the status[cite: 2].
* After successfully retrieving the data, only **one LED** will remain lit, corresponding to your current status in RCP Online.
* The status is automatically refreshed every 10 seconds.

##CONNECTION BETWEEN ESP8266 AND LEDs
For user:
- **On site** D6 , Green LED
- **On break** D7 , Blue LED
- **On business exit** D5 , Yellow LED
- **Away** D8 , Red LED

##ERRORS

You can use serial monitor to track errors, some of them will be described here:

* **app Status Code: 302** https://panel.rcponline.pl have security features that block the ability to log in to your account for 5 minutes if there are too many failed login attempts, so if you write something wrong in your credentials, device wont work for while, so be carefull while enterning your crendentials. This error will result with another error **getMyStatus Status Code: 500**.

## IMPORTANT

Code was used and tested only with Polish version of website, to make it work for English version it is necessary to modify the methods `extractPresenceStatus()` and `ledDriver()`


## ADDING_FEATERS

Functions related to retrieving additional data from the RCP panel can be implemented by modifying the `extractPresenceStatus()` method. This method handles the response from the server when requesting the endpoint `GetMyStatus0`.

The response message contains at least the following additional information:
- **Total work time**
- **Time of the last activity**
- **Last Event**
To perform different actions during the login or status-check process, you can also create a modified version of the `getMyStatus0()` method. Its parameters should match those required for the desired request.




