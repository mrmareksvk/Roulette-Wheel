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

//alert("width" + width1 + ",height" + height1);
var arcsegments = 1 / 12;

// Draw Basic Arc Frames
popLines();
popLEDs();

function popLines(){
    for (i = 0; i < 24; i++) {
        //alert(LEDs[i][1]);

        document.getElementById('dynamiccan').innerHTML += '<path id="arc' + (i + 1) +
            '" fill="none" stroke="white" stroke-width="3" filter="url(#sofGlow' + (i + 1) + ')"/>';
    }
}

function popLEDs(){
    for (i = 0; i < 24; i++) {
        //alert(LEDs[i][1]);

        document.getElementById('dynamiccan').innerHTML += '<defs><filter id="sofGlow' + (i + 1) + '" height="400%" width="400%" x="-150%" y="-150%"><!-- Thicken out the original shape --><feMorphology operator="dilate" radius="4" in="SourceAlpha" result="thicken" /><!-- Use a gaussian blur to create the soft blurriness of the glow --><feGaussianBlur in="thicken" stdDeviation="10" result="blurred" /><!-- Change the colour --><feFlood flood-color="' + LEDs[i][1] + '" result="glowColor" /><!-- Color in the glows --><feComposite in="glowColor" in2="blurred" operator="in" result="softGlow_colored" /><!--	Layer the effects together --><feMerge><feMergeNode in="softGlow_colored"/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>';

    }
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


// Get Arc Points
for (i = 0; i < 24; i++) {
    var info = document.querySelector('#arc' + (i + 1)).attributes[5].nodeValue;
    var points1 = [];
    //var points2 = [];
    //alert(info);
    points1 = info.split(" ");
    //alert(points1.join("\n"));
    points1.splice(0, 1);
    points1.splice(2, 9);
    //alert(points1.join("\n"));
    //alert(points1[0]);
    //alert(points1[1]);
    points[i][0] = points1[0];
    points[i][1] = points1[1];
}
//alert(points.join("\n"));

// Draw Lines
for (i = 0; i < 12; i++) {
    //   alert(LEDs[i][1]);
    document.getElementById('dynamiccan').innerHTML += '<line x1="' + points[i][0] + '" y1="' + points[i][1] +
        '" x2="' + points[i + 12][0] + '" y2="' + points[i + 12][1] + '" stroke="white" />';
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

// Roulette Spin Button Pressed
function RouletteSpin() {

    //var anicount = 1;
    var LEDcount = 1;
    var speed = 100;
    var LEDcount = rand(1,12);
    let start = Date.now(); // remember start time

    let timer = setInterval(function(){
        let timePassed = Date.now() - start;

        for (i = 0; i < 24; i++) {
            //   alert(LEDs[i][1]);
            document.querySelector('#sofGlow' + (i+1)).childNodes[5].attributes[0].value = LEDs[i][1];
        }

        document.querySelector('#sofGlow' + LEDcount).childNodes[5].attributes[0].value = "white";
        document.querySelector('#sofGlow' + (LEDcount+12)).childNodes[5].attributes[0].value = "white";

        //anicount ++;
        LEDcount ++;

        if (LEDcount == 13) {
            LEDcount = 1;
            //alert(LEDs[i][1]);
          }

        if (timePassed >= 3000) {
            clearInterval(timer); // finish the animation after 2 seconds
            return;
          }

    }, rand(15,50))
}

// Random Function
function rand (min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

// Sleep Function
function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
    //alert("next");
  }
