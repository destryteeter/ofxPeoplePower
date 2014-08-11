#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255, 255, 255);
    
    // Choose server
    ofxPeoplePower.prefix = ofxPeoplePower.esp_old;
    
    // Set signed in status to false
    signedIn = false;
    signingIn = false;
    
    receiveUserInput = false;
    setDefaultApi();
    
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
            setUsername = 0;
            setPassword = false;
            username = "";
            password = "";
            message = "Please sign in. Enter the first part of your username (before @ sign) and press return:";
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
                setUsername = 2;
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
    
    if (setUsername == 2 && setPassword) {
        // Display deviceInformation
        ofSetColor(0, 0, 0, 200);
        ofRect(312 + textSpacing * 2, textSpacing, ofGetWidth() - 312 - textSpacing * 3, textSpacing * (numberOfDevices + 2.5));
        
        ofSetColor(240, 240, 240);
        TTF.drawString("Connected", 312 + textSpacing * 3, textSpacing * 2);
        TTF.drawString("| Description", 312 + textSpacing * 3 + 100, textSpacing * 2);
        TTF.drawString("| ID", 312 + textSpacing * 3 + 350, textSpacing * 2);
        TTF.drawString("------------------------------------------------------", 312 + textSpacing * 3, textSpacing * 3);
        
        // Iterate through devices
        for (int i = 0; i < numberOfDevices; i ++) {
            
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
                
                // Set constant multiplier // TODO: Set multiplier dynamically
                float multiplier = 20;
                
                // Set number of points
                int pts = graphXML.getNumTags("usage");
                
                // Render Grid first
                if (toggleGraphOverlay) {
                    renderGraph(pts,multiplier);
                }
                
                // Then draw points and catmull points on top
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                }
                else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                }
                else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                    
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
                
                float multiplier = 180;
                
                int pts = graphXML.getNumTags("usage");
                
                if (toggleGraphOverlay) {
                    renderGraph(pts,multiplier);
                }
                
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                } else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                } else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                }
                
                graphXML.popTag();
                graphXML.popTag();
                graphXML.popTag();
                
            }
#ifdef DEBUG
    printOnce = false;
#endif
        }
    }
    else if (signingIn) {
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
                    // Log out of account (Logs out of all devices)
                    ofxPeoplePower.logout(XML.getValue("profile:key", "null"));
                }
            
                // Delete myProfile.xml
                ofFile::removeFile("myProfile.xml");
                XML.clear();
                XML.clear(); // Make sure XML is blank.
                
                ofxPeoplePower.XML.clear();
                ofxPeoplePower.XML.clear(); // Make sure XML is blank.
                
                // Create and save a black myProfile.xml
                setupXML();
                XML.saveFile("myProfile.xml");
                
                message = "Signed out!";
                
                signedIn = false;
                setUsername = 0;
                setPassword = false;
                username = "";
                password = "";
                
                displayGraph = 0;
                loadGraph = false;
                
                setDefaultApi();
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
            
            // Change Api Values
            if (receivesAggeregateFlag) {
                if (key == '+' && AggregateFlag < 5) {
                    ++AggregateFlag;
                    loadGraphData();
                }
                if (key == '_' && AggregateFlag > 1) { // Needed to use '_' rather than '-' ??
                    AggregateFlag = AggregateFlag - 1;
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
                    
                    receiveUserInput = true;
                    userInputFlag = "device";
                    message = "Enter your device ID here."; // TODO: add toggle to choose between listed devices
                }
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
                
                if (userInputFlag == "device") {
                    device = ofToUpper(userInput);
                    message = "your device is now: " + device;
                }
                
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
                cout << __PRETTY_FUNCTION__ << "userInput: " << userInput << endl;
            }

        }
    }
    
    // Only option when no signed in
    else if (!signingIn) {
        if (key == 'l') {
            signingIn = true;
            
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
            setUsername = 0;
            signedIn = false;
        }
        
        // Set username part 1
        if (setUsername == 0) {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && username.size() > 0) {
                username = username.substr(0,username.size()-1);
            }
            else if (key == OF_KEY_RETURN) {
                message = "Enter the second part of your username (after @ sign) and press return:"; // IMPORTANT
                
                username = username.append("@");
                setUsername = 1;
            }
            else {
                username.append(1,(char)key);
            }
        }
        
        // Set username part 2
        else if (setUsername == 1) {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && username.size() > 0) {
                username = username.substr(0,username.size()-1);
            }
            else if (key == OF_KEY_RETURN) {
                
                message = "Enter your password: ";
                setUsername = 2;
            }
            else {
                username.append(1,(char)key);
            }
        }
        
        // Set Password
        else if (setUsername == 2 && !setPassword) {
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && password.size() > 0) {
                password = password.substr(0,password.size()-1);
            }
            else if (!(key == OF_KEY_RETURN)) {
                password.append(1,(char)key);
            }
            if (key == OF_KEY_RETURN) {
                message = "Signing in as " + username;
                setPassword = true;
                
                // Did use example account
                if  (username == "test@") {
                    
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
                    setUsername = 2;
                    setPassword = true;
                }
                
                // Use real account
                else {
                    isExampleData = false;
                    
//                    HardCoded usernamepassword... remove before commit
                    username = "";
                    password = "";
                    
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
                        setUsername = 0;
                        setPassword = false;
                        
                    }
                    
                    // Return status message success and reset login values
                    else {
                        message = "Login successful.  Press TAB to graph your data";
                        XML.saveFile();
                        signedIn = true;
                        username = "";
                        password = "";
                        setUsername = 2;
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
