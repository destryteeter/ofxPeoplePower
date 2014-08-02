//
//  ofxPeoplePower.cpp
//
//  Created by Destry Teeter on 7/28/14.
//
//


#include "ofxPeoplePower.h"

// Notes: use "null" value in unused optional feilds.

// ------------------------------------------------------------------------------------------------------------------------
// *** Account Management ***
// ------------------------------------------------------------------------------------------------------------------------

void ofxPeoplePower::login(string username, string password) {
    URL = prefix + "/login?username=" + username + "&password=" + password + "&expiry=14";
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::logout(string key) {
    URL = prefix + "/logout/" + key;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::user(string key) {
    URL = prefix + "/user/" + key;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

// ------------------------------------------------------------------------------------------------------------------------
// *** Monitoring ***
// ------------------------------------------------------------------------------------------------------------------------

void ofxPeoplePower::deviceStatus(string key, string device_id) {
    URL = prefix + "/deviceStatus/" + key + "/" + device_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::deviceInfo(string key, string location_id, string user_id, string consumer, string checkPersistent) {
    URL = prefix + "/deviceInfo/" + key + "?locationId=" + location_id + "&userId=" + user_id + "&consumers=" + consumer + "&checkPersisetnt=" + checkPersistent;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::currentEnergyUsage(string key, string location_id) {
    URL = prefix + "/currentEnergyUsage/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::currentPower(string key, string location_id) {
    URL = prefix + "/currentPower/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::projection(string key, string location_id) {

    URL = prefix + "/projection/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::locationEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date) {
    if (end_date == "null") {
        URL = prefix + "/locationEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date;
    } else {
        URL = prefix + "/locationEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?endDate=" + end_date;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::deviceCurrentEnergyUsage(string key, string location_id, string device_id) {
    if (device_id == "null") {
        URL = prefix + "/deviceCurrentEnergyUsage/" + key + "/" + location_id;
    } else {
        URL = prefix + "/deviceCurrentEnergyUsage/" + key + "/" + location_id + "?deviceID=" + device_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::deviceEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date, string device_id) {
    if (end_date == "null" && device_id == "null") {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date;
    } else if (!(end_date == "null") && device_id == "null") {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?endDate=" + end_date;
    } else if (end_date == "null" && !(device_id == "null")) {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "&deviceId=" + device_id;
    } else {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?endDate=" + end_date + "&deviceId=" + device_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::deviceReadings(string key, string device_id, string start_date, string end_date, string location_id, string param_name, string index, string interval) {
    URL = prefix + "/deviceReadings/" + key + "/" + device_id + "/" + start_date + "?endDate=" + end_date + "&locationId=" + location_id + "&paramName=" + param_name + "&index=" + index + "&interval=" + interval;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::lastDeviceReadings(string key, string device_id, string row_count, string start_date, string end_date, string location_id, string param_name, string index) {
    URL = prefix + "/lastDeviceReadings/" + key + "/" + device_id + "/" + row_count + "?startDate=" + start_date + "&endDate=" + end_date + "&locationId=" + location_id + "&paramName=" + param_name + "&index=" + index;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::deviceParameters(string key, string location_id, string device_id, string param_name1, string param_name2, string param_name3) {
    URL = prefix + "/deviceParameters/" + key + "?locationId=" + location_id + "&deviceId=" + device_id + "&paramName=" + param_name1 + "&paramName=" + param_name2 + "&paramName=" + param_name3;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

void ofxPeoplePower::realTimeDeviceReadings(string key, string location_id, string continuous, string device_id, string client_id) {
    URL = prefix + "/realTimeDeviceReadings/" + key + "/" + location_id + "/" + continuous + "&deviceId=" + device_id + "&clientId=" + client_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
}

// ------------------------------------------------------------------------------------------------------------------------
// *** Control ***
// ------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------
// *** Rules ***
// ------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------
// *** Compare ***
// ------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------
// *** Quiz ***
// ------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------
// *** Market ***
// ------------------------------------------------------------------------------------------------------------------------
