//
//  ofxPeoplePower.cpp
//
//  Created by Destry Teeter on 7/28/14.
//
//

#include "ofxPeoplePower.h"

//    Notes: use "null" value in unused optional feilds."

// ------------------------------------------------------------------------------------------------------------------------
// *** Account Management ***
// ------------------------------------------------------------------------------------------------------------------------

void ofxPeoplePower::login(string username, string password) {
    URL = prefix + "/login?username=" + username + "&password=" + password + "&expiry=14";
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::logout(string key) {
    URL = prefix + "/logout/" + key;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::user(string key) {
    URL = prefix + "/user/" + key;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

// ------------------------------------------------------------------------------------------------------------------------
// *** Monitoring ***
// ------------------------------------------------------------------------------------------------------------------------

void ofxPeoplePower::deviceStatus(string key, string device_id) {
    URL = prefix + "/deviceStatus/" + key + "/" + device_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::deviceInfo(string key, string location_id) {
    URL = prefix + "/deviceInfo/" + key + "?locationId=" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::currentEnergyUsage(string key, string location_id) {
    URL = prefix + "/currentEnergyUsage/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::currentPower(string key, string location_id) {
    URL = prefix + "/currentPower/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::projection(string key, string location_id) {
    
    URL = prefix + "/projection/" + key + "/" + location_id;
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
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
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
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
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::deviceEnergyUsage(string key, string location_id, string aggregate, string start_date, string end_date, string device_id) {
    if (end_date == "null" && device_id == "null") {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date;
    } else if (!(end_date == "null") && device_id == "null") {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?endDate=" + end_date;
    } else if (end_date == "null" && !(device_id == "null")) {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?deviceId=" + device_id;
    } else {
        URL = prefix + "/deviceEnergyUsage/" + key + "/" + location_id + "/" + aggregate + "/" + start_date + "?endDate=" + end_date + "&deviceId=" + device_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::deviceReadings(string key, string location_id, string device_id, string start_date, string end_date, string param_name) {
    if (end_date == "null" && param_name == "null") {
        URL = prefix + "/deviceReadings/" + key + "/" + device_id + "/" + start_date + "?locationId=" + location_id;
    } else if (!(end_date == "null") && param_name == "null") {
        URL = prefix + "/deviceReadings/" + key + "/" + device_id + "/" + start_date + "?endDate=" + end_date + "&locationId=" + location_id;
    } else if (end_date == "null" && !(param_name == "null")) {
        URL = prefix + "/deviceReadings/" + key + "/" + device_id + "/" + start_date + "?paramName=" + param_name + "&locationId=" + location_id;
    } else {
        URL = prefix + "/deviceReadings/" + key + "/" + device_id + "/" + start_date + "?endDate=" + end_date + "&paramName=" + param_name + "&locationId=" + location_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::lastDeviceReadings(string key, string device_id, string row_count, string start_date, string end_date, string param_name, string location_id) {
    if (end_date == "null" && param_name == "null") {
        URL = prefix + "/lastDeviceReadings/" + key + "/" + device_id + "/" + row_count + "?startDate=" + start_date + "&locationId=" + location_id;
    } else if (!(end_date == "null") && param_name == "null") {
        URL = prefix + "/lastDeviceReadings/" + key + "/" + device_id + "/" + row_count + "?startDate=" + start_date + "&endDate=" + end_date + "&locationId=" + location_id;
    } else if (end_date == "null" && !(param_name == "null")) {
        URL = prefix + "/lastDeviceReadings/" + key + "/" + device_id + "/" + row_count + "?startDate=" + start_date + "&paramName=" + param_name + "&locationId=" + location_id;
    } else {
        URL = prefix + "/lastDeviceReadings/" + key + "/" + device_id + "/" + row_count + "?startDate=" + start_date + "&endDate=" + end_date + "&paramName=" + param_name + "&locationId=" + location_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::GetCurrentParameters(string key, string location_id, string device_id, string param_name) {
    if (device_id == "null" && param_name == "null") {
        URL = prefix + "/deviceParameters/" + key + "/" + location_id + "/true/true";
    } else if (!(device_id == "null") && param_name == "null") {
        URL = prefix + "/deviceParameters/" + key + "/" + location_id + "/true/true" + "?deviceId=" + device_id;
    } else if (device_id == "null" && !(param_name == "null")) {
        URL = prefix + "/deviceParameters/" + key + "/" + location_id + "/true/true" + "?paramName=" + param_name;
    } else {
        URL = prefix + "/deviceParameters/" + key + "/" + location_id + "/true/true" + "?deviceId=" + device_id + "&paramName=" + param_name;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}

void ofxPeoplePower::realTimeDeviceReadings(string key, string location_id, string continuous, string device_id, string client_id) {
    if (device_id == "null" && client_id == "null") {
        URL = prefix + "/realTimeDeviceReadings/" + key + "/" + location_id + "/" + continuous;
    } else if (!(device_id == "null") && client_id == "null") {
        URL = prefix + "/realTimeDeviceReadings/" + key + "/" + location_id + "/" + continuous + "?deviceId=" + device_id;
    } else if (device_id == "null" && !(client_id == "null")) {
        URL = prefix + "/realTimeDeviceReadings/" + key + "/" + location_id + "/" + continuous + "?clientId=" + client_id;
    } else {
        URL = prefix + "/realTimeDeviceReadings/" + key + "/" + location_id + "/" + continuous + "?deviceId=" + device_id + "&clientId=" + client_id;
    }
    ofHttpResponse resp = ofLoadURL(URL);
    ofBuffer buffer = resp.data.getText();
    
    XML.loadFromBuffer(buffer.getText());
    
#ifdef DEBUG
    cout << __PRETTY_FUNCTION__ << URL << endl << buffer << endl;
#endif
}
