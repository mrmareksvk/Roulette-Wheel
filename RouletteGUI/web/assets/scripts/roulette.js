var width1 = document.querySelector('#roulettecontainer').clientWidth;
var height1 = document.querySelector('#roulettecontainer').clientHeight;
var ratio = 0;

if (height1 < width1) {
    width1 = height1;
    height1 = height1;

} else {
    width1 = width1;
    height1 = width1;
}

// SVG tags
document.getElementById('roulettecontainer').innerHTML =
    '<svg id="dynamiccan" height="' + height1 +
    '" width="' + width1 +
    '" viewBox="0 0 100% 100%" preserveAspectRatio="none"></svg>';
var arcsegments = 1 / 12;

// LED Variables
var LEDs = [
    [1, 'green'],
    [2, 'red'],
    [3, 'green'],
    [4, 'red'],
    [5, 'green'],
    [6, 'red'],
    [7, 'green'],
    [8, 'red'],
    [9, 'green'],
    [10, 'red'],
    [11, 'green'],
    [12, 'red'],
    [13, 'green'],
    [14, 'red'],
    [15, 'green'],
    [16, 'red'],
    [17, 'green'],
    [18, 'red'],
    [19, 'green'],
    [20, 'red'],
    [21, 'green'],
    [22, 'red'],
    [23, 'green'],
    [24, 'red'],
];

// All Points of All Arcs
var points = [
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0],
    [0, 0]
];

// Draw Basic Arc Frames
popLines();
popLEDs();

function popLines() {
    for (i = 0; i < 24; i++) {
        document.getElementById('dynamiccan').innerHTML += '<path id="arc' + (i + 1) +
            '" fill="none" stroke="white" stroke-width="3" filter="url(#sofGlow' + (i + 1) + ')"/>';
    }
}

function popLEDs() {
    for (i = 0; i < 24; i++) {
        document.getElementById('dynamiccan').innerHTML += '<defs><filter id="sofGlow' + (i + 1) + '" height="400%" width="400%" x="-150%" y="-150%"><!-- Thicken out the original shape --><feMorphology operator="dilate" radius="4" in="SourceAlpha" result="thicken" /><!-- Use a gaussian blur to create the soft blurriness of the glow --><feGaussianBlur in="thicken" stdDeviation="10" result="blurred" /><!-- Change the colour --><feFlood flood-color="' + LEDs[i][1] + '" result="glowColor" /><!-- Color in the glows --><feComposite in="glowColor" in2="blurred" operator="in" result="softGlow_colored" /><!--	Layer the effects together --><feMerge><feMergeNode in="softGlow_colored"/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>';

    }
}

// Arc Formulas 1
function polarToCartesian(centerX, centerY, radius, angleInDegrees) {
    var addedDegrees = 5;
    var angleInRadians = (angleInDegrees - 90) * Math.PI / 180.0;

    return {
        x: centerX + (radius * Math.cos(angleInRadians + addedDegrees)),
        y: centerY + (radius * Math.sin(angleInRadians + addedDegrees))
    };
}

// Arc Formulas 2
function describeArc(x, y, radius, startAngle, endAngle) {
    var start = polarToCartesian(x, y, radius, endAngle);
    var end = polarToCartesian(x, y, radius, startAngle);

    var largeArcFlag = endAngle - startAngle <= 180 ? "0" : "1";

    var d = [
        "M", start.x, start.y,
        "A", radius, radius, 0, largeArcFlag, 0, end.x, end.y
    ].join(" ");

    return d;
}

// Arc paths
for (i = 0; i < 12; i++) {
    document.getElementById("arc" + (i + 1)).setAttribute("d", describeArc(width1 / 2, height1 / 2, (width1 - 50) / 2, (
        arcsegments *
        360) * i, (arcsegments * 360) * (i + 1)));
}

for (i = 12; i < 24; i++) {
    document.getElementById("arc" + (i + 1)).setAttribute("d", describeArc(width1 / 2, height1 / 2, (width1 - 50) / 3, (
        arcsegments *
        360) * i, (arcsegments * 360) * (i + 1)));
}

// Get Arc Points
for (i = 0; i < 24; i++) {
    var info = document.querySelector('#arc' + (i + 1)).attributes[5].nodeValue;
    var points1 = [];
    points1 = info.split(" ");
    points1.splice(0, 1);
    points1.splice(2, 9);

    points[i][0] = points1[0];
    points[i][1] = points1[1];
}

// Draw Lines
for (i = 0; i < 12; i++) {
    //   alert(LEDs[i][1]);
    document.getElementById('dynamiccan').innerHTML += '<line x1="' + points[i][0] + '" y1="' + points[i][1] +
        '" x2="' + points[i + 12][0] + '" y2="' + points[i + 12][1] + '" stroke="white" />';
}

// ---------- ONCLICK EVENT
var gameDuration = 3000; // ms
var segmentDuration = 0;
var activateRoullete = false;
var data = {};
data["att"] = {}

// TODO: universal JSON-cmd builder

function RouletteSpin() {
    let LEDcount = rand(1, 12, false);
    data["cmd"] = "roulette"
    data["att"]["start"] = LEDcount;

    let start = Date.now(); // start time
    activateRoullete = true;

    let n = 0;

    let game = setInterval(function() {
        let timePassed = Date.now() - start;

        for (i = 0; i < 24; i++) {
            //console.log(LEDs[i][1]);
            document.querySelector('#sofGlow' + (i + 1)).childNodes[5].attributes[0].value = LEDs[i][1];
        }

        document.querySelector('#sofGlow' + LEDcount).childNodes[5].attributes[0].value = "white";
        document.querySelector('#sofGlow' + (LEDcount + 12)).childNodes[5].attributes[0].value = "white";

        ++n;

        if (timePassed >= gameDuration) {
            clearInterval(game);
            activateRoullete = false;
            // console.log(LEDcount); // DEBUG
            // console.log(n); // DEBUG - check / show bug
            return;
        }

        LEDcount++;

        if (LEDcount > 12) {
            LEDcount = 1;
        }

    }, rand(15, 50, false))
}

// Random Function
function rand(min, max, dbg = false) {

    let randNum = Math.floor(Math.random() * (max - min)) + min;
    if (activateRoullete == true) {
        // console.log(~~(gameDuration / randNum)); // debug - SENT
        data["att"]["t"] = randNum;
        data["att"]["stop"] = ((~~(gameDuration / randNum) - (12 - data["att"]["start"])) % 12);
        if (data["att"]["stop"] == 0) data["att"]["stop"] = 12;

        sendData(JSON.stringify(data));
    }
    if (dbg == true) console.log("Rn:", randNum);

    return randNum;
}
