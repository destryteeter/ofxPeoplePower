#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Set framerate
    ofSetFrameRate(60);
    
    ofBackground(255, 255, 255);
    
    // Choose server
    ofxPeoplePower.prefix = ofxPeoplePower.esp_old;
    
    // Set signed in status to false
    signedIn = false;
    signingIn = false;
    isLoggingOut = false;
    
    receiveUserInput = false;
    setDefaultApi();
    
    canScrollDevices = false;
    
    // To use example data sign in using the username 'test' and push enter three times. (There's an issue with the shift key so the first enter adds an '@' sign.)
    // If you would like to user your own data check out Presence by People Power and look into installing a compatible smart plug.
    isExampleData = false;
    
	//the string is printed at the top of the app
	//to give the user some feedback
    message = "Loading mySettings.xml";
    
    //we load our settings file
	//if it doesn't exist we create one
    if (XML.loadFile("myProfile.xml")) {
        XML.loadFile("myProfile.xml");
        
        // Determine if user is singed in
        if (XML.getValue("profile:key","null") == "null") {
            XML.clear();
            setupXML();
            setUsername = false;
            setPassword = false;
            username = "";
            password = "";
            message = "Please sign in.";
        } else {
            // Determine if user has been signed out from another device.
            if (XML.getAttribute("profile:key", "expires", "") == "2030-01-01T00:00:00-05:00") {
                
                // Refresh device data to ensure it is up to date.
                ofxPeoplePower.deviceInfo(XML.getValue("profile:key","null"), XML.getValue("profile:location_id", "null"));
                
                XML.pushTag("profile");
                
                // Update Devices previous devices
                XML.removeTag("devices");
                XML.addTag("devices");
                XML.pushTag("devices");
                ofxPeoplePower.XML.pushTag("response");
                ofxPeoplePower.XML.pushTag("devices");
                XMLTranslate("device");
                
                // Setup device information for GUI
                numberOfDevices = XML.getNumTags("device");
                
                XML.popTag();
                XML.popTag();
                ofxPeoplePower.XML.popTag();
                ofxPeoplePower.XML.popTag();
                
                message = "Currently signed in";
                
                signedIn = true;
                setUsername = true;
                setPassword = true;
                username = "";
                password = "";
                
            } else {
                message = "You are no longer signed in.";
            }
        }
    } else {
        setupXML();
        message = "Unable to load myProfile.xml. New file created.";
    }
    
    XML.saveFile("myProfile.xml");
    
#ifdef DEBUG
    XML.copyXmlToString(temp);
    cout << temp.data() << endl;
#endif
    
    //load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 12);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    graphWidth = ofGetWidth() / 1.1;
    graphPosition = (ofGetWidth() - graphWidth) / 2;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	// Add the message bars at the top and bottom of the app
	// ofSetHexColor(0xDDDDDD);
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);
    output.fill();
    
    // Draw Key rectangle
	ofRect(textSpacing, textSpacing, 312, textSpacing * 9);
    ofRect(20, ofGetHeight() - 40, ofGetWidth()-40, 24);
    
    // Add our key.
    TTF.drawString("********************************", 15, textSpacing * 1.5);
    TTF.drawString("********************************", 15, textSpacing * 10.5);
	ofSetColor(240, 240, 240);
	TTF.drawString("   KEY | ACTION", 30, textSpacing * 2);
    TTF.drawString("-----------------------------", 30, textSpacing * 3);
    
    // Display option to sign in
    if (!signedIn) {
        int row = 4;
        TTF.drawString("   l   | Sign In", 30, textSpacing * row);
        
        // Display option to cancel when signing in
        if (signingIn) {
            TTF.drawString("   tab | Cancel", 30, textSpacing * (row + 1));
        }
        
        // Display test account information
        TTF.drawString("Enter the username 'test' to ", 30, textSpacing * 8);
        TTF.drawString("display sample data.         ", 30, textSpacing * 9);
    }
    
    // Display this when Signed in
    else {
        int row = 4;
        TTF.drawString("   q   | Sign Out", 30, textSpacing * row);
        TTF.drawString("   o   | Toggle Graph Overlay", 30, textSpacing * (row + 1));
        TTF.drawString("   tab | Change data", 30, textSpacing * (row + 2));
        
        // For test information, both data sets are the same
        if (displayGraph == 1 || displayGraph == 2) {
            TTF.drawString("   0   | Show combined graphs", 30, textSpacing * (row + 3));
            TTF.drawString("   1   | Aggregated Cost", 30, textSpacing * (row + 4));
            TTF.drawString("   2   | Usage in kWh", 30, textSpacing * (row + 5));
        }
        
        // Example to display a unique data set - not used
        else if (displayGraph == 3) {
            // Display nothing
        }
        
        // Display receivable flags key
        ofSetColor(0, 0, 0, 200);
        ofRect(textSpacing, ofGetHeight() - ofGetHeight()/4 + textSpacing, ofGetWidth() - textSpacing * 2, textSpacing * 6);
        
        ofSetColor(240, 240, 240);
        TTF.drawString("Set API attributes:", textSpacing * 2, ofGetHeight() - ofGetHeight()/4 + textSpacing * 2); // TODO: add and place key dynamically
        if (receivesAggeregateFlag) {
            TTF.drawString("Use '+' and '-' to set Aggregate flag.", textSpacing * 2, ofGetHeight() - ofGetHeight()/4 + textSpacing * 3);
        }
        if (receivesStartDate) {
            TTF.drawString("Press 's' to change the start date.", textSpacing * 2, ofGetHeight() - ofGetHeight()/4 + textSpacing * 4);
        }
        if (receivesEndDate) {
            TTF.drawString("Press 'e' to change the end date.", textSpacing * 2, ofGetHeight() - ofGetHeight()/4 + textSpacing * 5);
        }
        if (receivesDevice) {
            TTF.drawString("Press 'd' to change the device.", textSpacing * 2, ofGetHeight() - ofGetHeight()/4 + textSpacing * 6);
        }
    }
    
    // Add status message
    TTF.drawString("Status: " + message, 30, ofGetHeight() - 22);
    
    if (setUsername && setPassword) {
        // Display deviceInformation
        ofSetColor(0, 0, 0, 200);
        ofRect(312 + textSpacing * 2, textSpacing, ofGetWidth() - 312 - textSpacing * 3, textSpacing * 9);
        
        ofSetColor(240, 240, 240);
        TTF.drawString("Connected", 312 + textSpacing * 3, textSpacing * 2);
        TTF.drawString("| Description", 312 + textSpacing * 3 + 100, textSpacing * 2);
        TTF.drawString("| ID", 312 + textSpacing * 3 + 350, textSpacing * 2);
        TTF.drawString("------------------------------------------------------", 312 + textSpacing * 3, textSpacing * 3);
        
        // Iterate through devices
        if (numberOfDevices > 5) {
            for (int i = 0 + device + deviceOffset; i < 6 + device + deviceOffset; i ++) {
                canScrollDevices = true;
                
                XML.pushTag("profile");
                XML.pushTag("devices");
                
                string deviceConnected;
                string deviceDescription;
                string deviceId;
                
                XMLSetTagAttributeToString("device", "connected", i);
                deviceConnected = targetString;
                
                XMLSetTagAttributeToString("device", "desc", i);
                deviceDescription = targetString;
                
                XMLSetTagAttributeToString("device", "id", i);
                deviceId = targetString;
                
                XML.popTag();
                XML.popTag();

                TTF.drawString(deviceConnected , 312 + textSpacing * 3, textSpacing * ((i - device) + 4 - deviceOffset));
                TTF.drawString("| " + deviceDescription, 312 + textSpacing * 3 + 100, textSpacing * ((i - device) + 4 - deviceOffset));
                TTF.drawString("| " + deviceId, 312 + textSpacing * 3 + 350, textSpacing * ((i - device) + 4 - deviceOffset));
                
            }
        } else {
            for (int i = 0; i < numberOfDevices; i ++) {
                canScrollDevices = false;
                
                XML.pushTag("profile");
                XML.pushTag("devices");
                
                string deviceConnected;
                string deviceDescription;
                string deviceId;
                
                XMLSetTagAttributeToString("device", "connected", i);
                deviceConnected = targetString;
                
                XMLSetTagAttributeToString("device", "desc", i);
                deviceDescription = targetString;
                
                XMLSetTagAttributeToString("device", "id", i);
                deviceId = targetString;
                
                XML.popTag();
                XML.popTag();
                
                TTF.drawString(deviceConnected , 312 + textSpacing * 3, textSpacing * (i + 4));
                TTF.drawString("| " + deviceDescription, 312 + textSpacing * 3 + 100, textSpacing * (i + 4));
                TTF.drawString("| " + deviceId, 312 + textSpacing * 3 + 350, textSpacing * (i + 4));
                
            }
        }
        if (loadGraph) {
#ifdef DEBUG
        if (printOnce) {
            cout << __PRETTY_FUNCTION__ << "displayGraph: " << displayGraph << endl;
        }
#endif
            if (displayGraph == 1) { // previously displayGraphEnergy
                
                // Go to 'response:usages' tag
                graphXML.pushTag("response");
                graphXML.pushTag("usages");
                
                // Set number of points
                int pts = graphXML.getNumTags("usage");
                
                // Render Grid first
                if (toggleGraphOverlay) {
                    renderGraph(pts,k);
                }
                
                // Then draw points and catmull points on top
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount");
                }
                else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh");
                }
                else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh");
                    graphCatMullPointsForXML("usage", "amount");
                    
                }
                
                graphXML.popTag();
                graphXML.popTag();
            }
            else if (displayGraph == 2) {
                
                // Go to 'response:usages:device' tag
                // Note that each response may vary between api call
                graphXML.pushTag("response");
                graphXML.pushTag("usages");
                graphXML.pushTag("device");
                
                int pts = graphXML.getNumTags("usage");
                
                if (toggleGraphOverlay) {
                    renderGraph(pts,k);
                }
                
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount");
                } else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh");
                } else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh");
                    graphCatMullPointsForXML("usage", "amount");
                }
                
                graphXML.popTag();
                graphXML.popTag();
                graphXML.popTag();
                
            }
#ifdef DEBUG
    printOnce = false;
#endif
        }
        
        if (isLoggingOut) {
            // Draw modal
            output.fill();
            ofSetColor(0, 0, 0, 200);
            ofRect(ofGetWidth() / 4, ofGetHeight() / 2 - textSpacing * 2.25, ofGetWidth() * 1/2, textSpacing * 4);
            ofSetColor(240, 240, 240);
            TTF.drawString("Press '1' to log out of this device", ofGetWidth() / 4 + textSpacing, ofGetHeight() / 2 - textSpacing);
            TTF.drawString("Press '2' to log out of all devices", ofGetWidth() / 4 + textSpacing, ofGetHeight() / 2 + textSpacing);
        }
    }
    else if (signingIn) {
        // Draw modal
        output.fill();
        ofSetColor(0, 0, 0, 200);
        ofRect(ofGetWidth() / 4, ofGetHeight() / 2 - textSpacing * 2.25, ofGetWidth() * 1/2, textSpacing * 4);
        ofSetColor(240, 240, 240);
        TTF.drawString("username: " + username, ofGetWidth() / 2 - textSpacing * 11, ofGetHeight() / 2 - textSpacing);
        TTF.drawString("password: " + password, ofGetWidth() / 2 - textSpacing * 11, ofGetHeight() / 2 + textSpacing);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (signedIn) {

        if (!receiveUserInput) {

            // Log out and quit
            if(key == 'q') {
                
                if (!isExampleData) {
                    isLoggingOut = true;
                }
                else {
                    
                    message = "Signed of test account!";
                    resetUser();
                    setDefaultApi();
                }
            }
            if (isLoggingOut) {
                if (key == '1') {
                    message = "Signed out of this device";
                    resetUser();
                    setDefaultApi();
                }
                if (key == '2') {
                    message = "Signed out of all devices";
                    resetUser();
                    setDefaultApi();
                }
            }
            
            // Load LocationEnergyUsage
            if(key == 'o') {
                if (toggleGraphOverlay == false) {
                    toggleGraphOverlay = true;
                }
                else {
                    toggleGraphOverlay = false;
                }
            }
            
            // Change data set
            if (key == OF_KEY_TAB) {
                
                // Stop loading graph
                loadGraph = false;
                resetReceivableFlags();
                
                // Reset yMax, yMin and k
                k = 0;
                yMax = 0;
                yMin = 0;
                
                // Reset deviceOffset
                deviceOffset = 0;
                
                // Initially display all data points overlayed
                drawGraph = 99;
                
                // Only 2 for example, for a third replace 1st if clause
                if (displayGraph == 2) {
                    displayGraph = 1;
                } else {
                    ++displayGraph;
                }
                
                // Gather graph data points
                loadGraphData();
                
                // begin loading graph again
    #ifdef DEBUG
        printOnce = true;
    #endif
                loadGraph = true;
            }
            
            // Change data set
            if (key == '1' && loadGraph) {
                drawGraph = 1;
            }
            if (key == '2' && loadGraph) {
                drawGraph = 2;
            }
            if (key == '0' && loadGraph) {
                drawGraph = 99;
            }
            
            // Scroll Devices
            if (canScrollDevices) {
                if (key == '>' && deviceOffset <= (numberOfDevices - 7 - device)) {
                    ++deviceOffset;
                }
                if (key == '<' && deviceOffset >= 1 - device) {
                    deviceOffset = deviceOffset - 1;
                }
#ifdef DEBUG
                cout << __PRETTY_FUNCTION__ << "deviceOffset: " << deviceOffset << " || numberOfDevices: " << numberOfDevices << " || device: " << device << endl;
#endif
            }
            
            // Change Api Values
            if (receivesAggeregateFlag) {
                if (key == '+' && AggregateFlag < 5) {
                    ++AggregateFlag;
                    k = 0;
                    yMax = 0;
                    yMin = 0;
                    loadGraphData();
                }
                if (key == '_' && AggregateFlag > 1) { // Needed to use '_' rather than '-' ??
                    AggregateFlag = AggregateFlag - 1;
                    k = 0;
                    yMax = 0;
                    yMin = 0;
                    loadGraphData();
                }
            }
            
            if (receivesStartDate) {
                if (key == 's') {
                    receiveUserInput = true;
                    userInputFlag = "startDate";
                    message = "Enter start date. (Example '2014-08-01T00:00:00')";
                }
            }
            
            if (receivesEndDate) {
                if (key == 'e') {
                    receiveUserInput = true;
                    userInputFlag = "endDate";
                    message = "Enter end date. (Example '2014-08-01T00:00:00' or for today enter 'null')";
                }
            }
            
            if (receivesDevice) {
                if (key == 'd') {
                    ++device;
                    
                    // Reset deviceOffset
                    deviceOffset = 0;
                    
                    XML.pushTag("profile");
                    XML.pushTag("devices");
                    
                    for (int i = 0; i < numberOfDevices; ++i) {
                        
                        string deviceType = XML.getAttribute("device", "type", "null", device);
                        
                        if (deviceType == "21") {
                            ++device;
                        }
                        else if (device >= numberOfDevices){
                            device = 0;
                        }
                        else {
                            break;
                        }
                    }

#ifdef DEBUG
                    cout << __PRETTY_FUNCTION__ << "deviceType: " << XML.getAttribute("device", "type", "null", device) << " || deviceID: " << XML.getAttribute("device", "id", "null", device) << endl;
#endif
                    
                    message = "Graphing data for device with ID: " + XML.getAttribute("device", "id", "null", device);
                    
                    XML.popTag();
                    XML.popTag();
                    
                    k = 0;
                    yMax = 0;
                    yMin = 0;
                    loadGraphData();
                    
                    }
//                    receiveUserInput = true;
//                    userInputFlag = "device";
//                    message = "Enter your device ID here."; // TODO: add toggle to choose between listed devices
//                }
            }
        } else {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && userInput.size() > 0) {
                userInput = userInput.substr(0,userInput.size()-1);
            }
            else if (key == OF_KEY_RETURN) {
                if (userInputFlag == "startDate") {
                    startDate = userInput;
                    message = "start date is now: " + userInput;
                }
                
                if (userInputFlag == "endDate") {
                    endDate = userInput;
                    message = "end date is now: " + endDate;
                }
                
//                if (userInputFlag == "device") {
//                    device = ofToUpper(userInput);
//                    message = "your device is now: " + device;
//                }
                
                k = 0;
                yMax = 0;
                yMin = 0;
                
                loadGraphData();
                
                receiveUserInput = false;
                userInput = "";
                userInputFlag = "";
            }
            else {
                if (!((unsigned)key == 2304 || (unsigned)key == 2305 || (unsigned)key == 2306)) {
                    userInput.append(1,(char)key);
                    message = userInput;
                }
#ifdef DEBUG
                cout << __PRETTY_FUNCTION__ << "userInput: " << userInput << endl;
#endif
            }

        }
    }
    
    // Only option when no signed in
    else if (!signingIn) {
        if (key == 'l') {
            signingIn = true;
            username = "";
            password = "";
            
        }
    }
    
    // While singing in
    else {
        
        // Press TAB key to cancel
        if (key == OF_KEY_TAB) {
            signingIn = false;
            username = "";
            password = "";
            setPassword = false;
            setUsername = false;
            signedIn = false;
        }
        
        // Set username part 1
        if (!setUsername) {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && username.size() > 0) {
                username = username.substr(0,username.size()-1);
            }
            else if (key == OF_KEY_BACKSPACE && username.size() == 0) {
                signingIn = false;
            }
            else if (key == OF_KEY_RETURN) {
                setUsername = true;
            }
            else {
                
                // Don't append shift key
                if (!((unsigned)key == 2304 || (unsigned)key == 2305 || (unsigned)key == 2306)) {
                    username.append(1,(char)key);
                }
            }
            
#ifdef DEBUG
//                cout << __PRETTY_FUNCTION__ << "Unsigned key: " << (unsigned)key << endl;
//                cout << __PRETTY_FUNCTION__ << "Char key: " << (char)key << endl;
                cout << __PRETTY_FUNCTION__ << "Username: " << username << endl;
#endif
            
        }
        
        // Set Password
        else if (setUsername && !setPassword) {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && password.size() > 0) {
                password = password.substr(0,password.size()-1);
            }
            else if (key == OF_KEY_BACKSPACE && password.size() == 0) {
                setUsername = false;
            }
            else if (!(key == OF_KEY_RETURN)) {
                
                // Don't append shift keys
                if (!((unsigned)key == 2304 || (unsigned)key == 2305 || (unsigned)key == 2306)) {
                    password.append(1,(char)key);
                }
#ifdef DEBUG
//                cout << __PRETTY_FUNCTION__ << "Unsigned key: " << (unsigned)key << endl;
//                cout << __PRETTY_FUNCTION__ << "Char key: " << (char)key << endl;
                cout << __PRETTY_FUNCTION__ << "Password: " << password << endl;
#endif
            }
            if (key == OF_KEY_RETURN) {
                message = "Signing in as " + username;
                setPassword = true;
                
                // Did use example account
                if  (username == "test") {
                    
                    // Load example data
                    isExampleData = true;
                    exampleDevices.load("sampleData/deviceInfo.xml");
                    
                    XML.pushTag("profile");
                    
                    // Remove previous devices and load example devices
                    XML.removeTag("devices");
                    XML.addTag("devices");
                    XML.pushTag("devices");
                    exampleDevices.pushTag("response");
                    exampleDevices.pushTag("devices");
                    
                    XMLTranslate("device");
                    
                    // Setup device information for GUI
                    numberOfDevices = XML.getNumTags("device");
                    
                    XML.popTag();
                    XML.popTag();
                    exampleDevices.popTag();
                    exampleDevices.popTag();
                    
                    // Singed in and information reset
                    message = "Login successful";
                    XML.saveFile();
                    signedIn = true;
                    username = "";
                    password = "";
                    setUsername = true;
                    setPassword = true;
                }
                
                // Use real account
                else {
                    isExampleData = false;
                    
                    // get key and asign it to XML
                    // TODO: User temp key... or rather get temp key and delete each time
                    ofxPeoplePower.login(username, password);
                    XML.pushTag("profile",0);
                    XML.setValue("key", ofxPeoplePower.XML.getValue("response:key","null"));
                    XML.setAttribute("key", "expires", ofxPeoplePower.XML.getValue("response:keyExpire","null"), 0);
                    
                    // Get user_id and location_id and asign it to XML
                    ofxPeoplePower.user(XML.getValue("key","null"));
                    XML.setValue("user_id", ofxPeoplePower.XML.getAttribute("response:user", "id", 0));
                    XML.setValue("location_id", ofxPeoplePower.XML.getAttribute("response:locations:location", "id", 0));
                    XML.popTag();
                    
                    // Get user device information
                    ofxPeoplePower.deviceInfo(XML.getValue("profile:key","null"), XML.getValue("profile:location_id", "null"));
                    
                    // Remove previous devices
                    XML.pushTag("profile");
                    XML.removeTag("devices");
                    XML.addTag("devices");
                    XML.pushTag("devices");
                    ofxPeoplePower.XML.pushTag("response");
                    ofxPeoplePower.XML.pushTag("devices");
                    XMLTranslate("device");
                    
                    // Setup device information for GUI
                    numberOfDevices = XML.getNumTags("device");
                    
                    XML.popTag();
                    XML.popTag();
                    ofxPeoplePower.XML.popTag();
                    ofxPeoplePower.XML.popTag();
                    
                    // Return status message fail and reset login values
                    if (XML.getValue("profile:key","null") == "null") {
                        message = "Login failed, please try again!";
                        XML.clear();
                        setupXML();
                        username = "";
                        password = "";
                        setUsername = false;
                        setPassword = false;
                        
                    }
                    
                    // Return status message success and reset login values
                    else {
                        message = "Login successful.  Press TAB to graph your data";
                        XML.saveFile();
                        signedIn = true;
                        username = "";
                        password = "";
                        setUsername = true;
                        setPassword = true;
                    }
                }
#ifdef DEBUG
                XML.copyXmlToString(temp);
                cout << temp.data() << endl;
#endif
                // stop signing in
                signingIn = false;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
