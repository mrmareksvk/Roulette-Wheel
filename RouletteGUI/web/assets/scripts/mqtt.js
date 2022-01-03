// SCRIPT FOR HANDLING MQTT
// API: https://www.eclipse.org/paho/files/jsdoc/index.html

var connected_flag = false;
var reconnectTimeout = 1000;
var mqtt, server, port;
var clientID = "RouletteControl"; // randomise or unique
var subTopic = "kea/guldbar"; // change if needed
var sendTopic = "kea/guld"; // change if needed

var dbg = false;

function MQTTconnect() {
    server = "broker.hivemq.com";
    port = 8000; // 8000 websocket 1883 TCP

    //console.log("Connecting to " + server + " " + port); // DEBUG

    mqtt = new Paho.MQTT.Client(server, Number(port), clientID);
    //console.log(">connected"); // DEBUG

    var options = {
        timeout: 3,
        //userName: "", //credentials for Brooker authentification
        //password: "", //credentials
        onSuccess: onConnect,
        onFailure: onFailure,
    };

    mqtt.onConnectionLost = onConnectionLost;
    mqtt.onMessageArrived = onMessageArrived;

    mqtt.connect(options);
};

function onConnect() {
    document.getElementById("messages").innerHTML = "Connected to " + server + " on port " + port;
    connected_flag = true;
    document.getElementById("status").innerHTML = "Connected";
    onSubscribe();
};

function onConnectionLost(responseObject) {
    console.log("Connection lost", responseObject.errorCode);
    document.getElementById("status").innerHTML = "Connection Lost";
    document.getElementById("messages").innerHTML = "Connection Lost";
    connected_flag = false;

    if (responseObject.errorCode !== 0) {
        console.log(responseObject.errorMessage);
    }
};

function onFailure(message) {
    console.log("Failed");
    document.getElementById("messages").innerHTML = "Connection Failed-Retrying";
    setTimeout(MQTTconnect, reconnectTimeout);
};

function onMessageArrived(message) {
    out_msg = "Message arrived:<br>" + message.payloadString.replace(/"|{|}/g, '');
    // console.log("Message: " + message.payloadString); // DEBUG
    document.getElementById("messages").innerHTML = out_msg;
    try { //JSON test
        const payload = JSON.parse(message.payloadString);
        // console.log(Object.keys(payload).length);  // DEBUG
        // console.log(message.payloadString); // DEBUG
    } catch (err) {
        console.log(err);
    }
};

function onSubscribe() {
    document.getElementById("messages").innerHTML = "";

    if (connected_flag == false) {
        out_msg = "Not Connected, can't subscribe";
        // console.log(out_msg);
        document.getElementById("messages").innerHTML = out_msg;
        return false;
    }

    // console.log("Subscribing to topic: " + subTopic); // DEBUG
    mqtt.subscribe(subTopic);
    document.getElementById("messages").innerHTML = "Subscribed to:<br><i>" + subTopic + "</i><br>Waiting for messages.";

    if (dbg == true) mqtt.send(sendTopic, '{"status":"on"}', 0, false)

    return false;
};

function sendData(JSONstream) {
    mqtt.send(sendTopic, JSONstream, 0, false);
    // console.log(JSONstream); // DEBUG
}
