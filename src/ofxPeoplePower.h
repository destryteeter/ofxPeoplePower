//
//  ofxPeoplePower.h
//
//  Created by Destry Teeter on 7/28/14.
//
//

#pragma once

#include "ofMain.h"
//#include "Poco/Net/HTTPRequest.h"
#include "ofxXmlSettings.h"

//ofHttpResponse resp;
//ofBuffer buffer;

class ofxPeoplePower {
    
public:
    
    string URL;
    
    // Old API
    string developer_old = "https://developer.peoplepowerco.com/espapi/rest";
    string sbox_old = "https://sbox.peoplepowerco.com/espapi/rest";
    string esp_old = "https://esp.peoplepowerco.com/espapi/rest";
    
    // New API ---- TODO: requires adding HTTP request header http://forum.openframeworks.cc/t/https-request-in-of/7457
    string developer_xml = "https://developer.peoplepowerco.com/cloud/xml";
    string sbox_xml = "https://sbox.peoplepowerco.com/cloud/xml";
    string esp_xml = "https://esp.peoplepowerco.com/cloud/xml";

    // TODO: add json support
    // string developer_json = "https://esp.peoplepowerco.com/cloud/json";
    // string sbox_json = "https://esp.peoplepowerco.com/cloud/json";
    // string esp_json = "https://esp.peoplepowerco.com/cloud/json";
    
    string prefix;
    string temp;
    
    ofxXmlSettings XML;

    
    // ------------------------------------------------------------------------------------------------------------------------
    // *************************************-----------------------------------------------------------------------------------
    // *** Set up and Account Management ***
    // *************************************-----------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    
    // Login
    // Allow user to login using user credentials (username and password) and application ID. It returns an API key, which is linked to the user record and will be used in all other API calls
    void login(string username, string password);

    void loginByKey(string key);
    
    // Logout
    // Logout the user and remove the API key from the database
    void logout(string key);
    
    // Renew Password
    // Renew Password
    void newPassword(string username, string forward_id);
    
    // Register User Account
    // Register User Account
//    void registerUser(string email, string password);
    
    // Create Location
    // Create Location
//    void addLocation(string location);
    
    // Get user and locations info
    // Shows user info and assigned location(s)
    void user(string key);
    
    // Edit User Account
    // Edit User Account
//    void updateUser(string key, string input_xml); TODO: How do I post an XML file?
    
    // Email verification
    // Send email verification
//    void emailVerificationMessage(string key, string forward_id);
    
    // Edit Location
    // Edit Location
    //    void updateLocation(string key, sting location_id, xml input_xml);

    // Location Parameters
    // Location Parameters
//    void locationParams();
    
    // Get Location Users
    // Get Location Users
//  void searchLoactionUsers(string key, string location_id, string search_by, string limit);
    
    // Register a user for an event
    // Register a user for an event
//    void userEvent(string event_type, string user_email);
    
    // Add Location Event
    // Add Location Event
//    void createLocationEvent();
    
    // ------------------------------------------------------------------------------------------------------------------------
    // -- *** Billing Info ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    // Get Utilities By Zip Code
    // Retrieve Utilities by Zip Code
    void utilities(string country_id, string state_id, string zip, string name);
    
    // Get Billing Rates By Utility Id and Zip Code
    // Retrieve Rates by Utility Id and Zip Code
    void billingRates(string utility_id, string state_id, string zip);
    
    // Get Average Budget
    // Return average budget for the territory of the location
    void avarageBudget(string country_id, string state_id);
    
    // Get Billing Info by Location
    // Retrieve Billing Info by Location
    void billingInfoGET(string key, string location_id); // GET
    
    // Post location billing info
    // Post location billing information: billing day, rate and budget. Make use of POST.
//    void billingInfoPOST(string key, string location_id, string input_xml); // POST
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** User notifications ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    // Setup User Notification Token Mapping
    // Setup User Notification Token Mapping
//    void notificationTokenPOST(string key, string app_id, string notification_token, string device_id); // POST
    
    // Delete User Notification Token Mapping
    // Delete device notification token for iOS mobile devices (iPhone, iPad)
//    void notificationTokenDELETE(string key, string notification_token);
    
    // Post user notification subscriptions
    // Allows to change subscriptions to system notifications.
//    void ...;
    
    // Get user notification subscriptions
    // Allows a user to see his subscriptions to system notifications like monthly projection change or billing rate change.
//    void ...;
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Device types *** TODO: ?
    // ------------------------------------------------------------------------------------------------------------------------

    // Device Types
    // Get supported device types
//    void deviceTypes(string key, string device_type_id, string status);
    
    // Create/Modify DeviceType
    // Create Modify DeviceType
    
    
    // GreenXHubActivation
    // Hub activation
    
    
    // TEDMonitorActivation
    // TED activation
    
    
    // Send Device Activation Info
    // Send email to user about device activation procedure, on requested deviceType
    
    
    // Register new device
    // Register new device
    
    
    // Remove device
    // Remove device from location
    
    
    // Get device status
    // Return a device registration status on the server and device location information.
    
    
    // Move device to other location
    // Move device to other location
    
    
    // Set device attributes
    // Set device attributes
    
//    void deviceTypeAttrs(string key);
//    void deviceTypesPOST(string key, int device_type_id, string name, string attributes);
//    void deviceTypesPOST(string key, int operation, int device_type_id);
//    void deviceTypeImage(string key, string device_type_id); // TODO: add image?
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Device Activation ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    void deviceStatus(string key, string device_id);
//    void deviceActivationInfo(string key, string location_id, string device_type_id, string activationEmail);
//    void IOTSDKHubActivation(string input_xml);
//    void deviceRegistration(string key, string location_id, string device_id, string device_type_id, string authenticationTokenFlag, string start_date);
//    void deleteDevice(string key, string location_id, string device_id, string clean);
//    void moveDevice(string key, string location_id, string device_id, string newLocation_id, string start_date, string moveAppliances);
//    void activateTed(string input xml);
//    void activateBl(string device_model, string device_id, string response_url);
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Consumer Mapping ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Monitoring ***
    // ------------------------------------------------------------------------------------------------------------------------
    
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
    void deviceEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date, string device_id, string device_type_category);

    // Get instantaneous readings history of a device
    // Returns historical information about device instantaneous readings granulated by small period of time
    void realTimeDeviceReadings(string key, string location_id, string continuous, string device_id, string client_id);

    // Get Last N history records
    // Return last N historical information about device instantaneous readings.
    void lastDeviceReadings(string key, string device_id, string row_count, string start_date, string end_date, string location_id, string param_name, string index);

    // Get real time device readings
    // Returns real time parameter readings at a device level. The method will return current data stored on the server and request update from the device
    void deviceReadings(string key, string device_id, string start_date, string end_date, string location_id, string param_name, string index, string interval);
    
    // Get Current Device Parameters
    // Returns real time parameter readings at a device level. The method will return current data stored on the server and request update from the device
    // More details...
    void deviceParameters(string key, string location_id, string device_id, string param_name1, string param_name2, string param_name3);

    // GetSocketStatuses
    // Return power strip socket statuses
//    void remoteControl(string key, string location_id, string device_id); // Depriciated
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Control ***
    // ------------------------------------------------------------------------------------------------------------------------
    
//    void deviceParametersPOST(string key, string location_id, string input_xml);
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Rules ***
    
    // void createRule
    // void getRules
    // void deleteRules
    // void postEvent
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Compare ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    // void compareToStatistics
    // void compareToOthers
    // void Saving
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Quiz ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Market ***
    // ------------------------------------------------------------------------------------------------------------------------
    
    
    // ------------------------------------------------------------------------------------------------------------------------
    // *************-----------------------------------------------------------------------------------------------------------
    // *** CLOUD ***
    // *************-----------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // Requires adding a http request hearer... TODO
    // For now, use cloud html api's
    // https://developer.peoplepowerco.com/espapi/cloud/test/index.html
    // https://sbox.peoplepowerco.com/espapi/cloud/test/index.html
    // https://esp.peoplepowerco.com/espapi/cloud/test/index.html
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Set up and Account Management ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Account MAnagement ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Organizations ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** User Communications ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Device Products ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Device Management ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Monitoring and Control ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** GreenButton ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Device Files ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Shares ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Rules ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** Paid Services ***
    // ------------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------------------------------
    // *** User Interface ***
    // ------------------------------------------------------------------------------------------------------------------------
    
};

