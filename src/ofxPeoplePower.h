//
//  ofxPeoplePower.h
//
//  Created by Destry Teeter on 7/28/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxPeoplePower {
    
public:
    
    string URL;

    string esp_old = "https://esp.peoplepowerco.com/espapi/rest";
    
    string prefix;
    string temp;
    
    ofxXmlSettings XML;

    
    // ------------------------------------------------------------------------------------------------------------------------
    // *************************************-----------------------------------------------------------------------------------
    // *** Set up and Account Management ***
    // *************************************-----------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    
    // Login
    // Allow user to login using user credentials (username and password) and application ID.
    // It returns an API key, which is linked to the user record and will be used in all other API calls
    void login(string username, string password);

    
    // Logout
    // Logout the user and remove the API key from the database
    void logout(string key);
    
    // Get user and locations info
    // Shows user info and assigned location(s)
    void user(string key);
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Monitoring ***
    // ------------------------------------------------------------------------------------------------------------------------

    // Get device status
    // Return a device registration status on the server and device location information.
    void deviceStatus(string key, string device_id);

    // GetDevicesInfo
    // Returns a list of user devices linked to energy consumers
    void deviceInfo(string key, string location_id, string user_id, string consumer, string checkPersistent);
    
    // Get location current total energy usage
    // Returns total energy usage and its cost for the specified location
    void currentEnergyUsage(string key, string location_id);

    // Get location current total power consumption
    // Returns instantaneous values of power, rate and its associated cost
    void currentPower(string key, string location_id);

    // Get energy use projection
    // Returns current billing cycle total projection
    void projection(string key, string location_id);

    // Get aggregated location energy usage
    // Returns aggregated amount and energy usage for a specified date range aggregated by different time periods
    // --- AggregateFlag: values=(0 - no, 1 - hour, 2 - day, 3 - month, 4 - week (7 days), 5 - week (5 days))
    // --- StartDate: start date for data collection period in "YYYY-MM-DDThh:mm:ss" format, example 2010-04-03T15:03:42
    // --- EndDate: (optional)
    void locationEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date);

    // Get devices current total energy usage
    // Returns total energy usage and its cost for device on the specified location
    void deviceCurrentEnergyUsage(string key, string location_id, string device_id);

    // Get aggregated device level energy usage
    // Returns energy usage at a device level for a specified period of time and aggregated by different periods. If no device ID is specified then it returns data for all devices in that location
    void deviceEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date, string device_id);

    // Get instantaneous readings history of a device
    // Returns historical information about device instantaneous readings granulated by small period of time
    void realTimeDeviceReadings(string key, string location_id, string continuous, string device_id, string client_id);

    // Get Last N history records
    // Return last N historical information about device instantaneous readings.
    void lastDeviceReadings(string key, string device_id, string row_count, string start_date, string end_date, string location_id, string param_name, string index);

    // Get real time device readings
    // Returns real time parameter readings at a device level. The method will return current data stored on the server and request update from the device
    void deviceReadings(string key, string location_id, string device_id, string start_date, string end_date, string param_name);
    
    // Get Current Device Parameters
    // Returns real time parameter readings at a device level. The method will return current data stored on the server and request update from the device
    // More details...
    void deviceParameters(string key, string location_id, string device_id, string param_name1, string param_name2, string param_name3);
};

