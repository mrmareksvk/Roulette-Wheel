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

document.getElementById('roulettecontainer').innerHTML =
'<svg id="dynamiccan" height="' + height1 +
'" width="' + width1 +
'" viewBox="0 0 100% 100%" preserveAspectRatio="none"></svg>';

//alert("width" + width1 + ",height" + height1);
var arcsegments = 1 / 12;

for (i = 0; i < 24; i++) {
    //   alert(LEDs[i][1]);

    document.getElementById('dynamiccan').innerHTML += '<defs><filter id="sofGlow' + i + '" height="400%" width="400%" x="-150%" y="-150%"><!-- Thicken out the original shape --><feMorphology operator="dilate" radius="4" in="SourceAlpha" result="thicken" /><!-- Use a gaussian blur to create the soft blurriness of the glow --><feGaussianBlur in="thicken" stdDeviation="10" result="blurred" /><!-- Change the colour --><feFlood flood-color="' + LEDs[i][1] + '" result="glowColor" /><!-- Color in the glows --><feComposite in="glowColor" in2="blurred" operator="in" result="softGlow_colored" /><!--	Layer the effects together --><feMerge><feMergeNode in="softGlow_colored"/><feMergeNode in="SourceGraphic"/></feMerge></filter></defs>';

    document.getElementById('dynamiccan').innerHTML += '<path id="arc' + (i + 1) +
        '" fill="none" stroke="white" stroke-width="3" filter="url(#sofGlow' + i + ')"/>';
}


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

for (i = 0; i < 12; i++) {
    //   alert(LEDs[i][1]);
    document.getElementById('dynamiccan').innerHTML += '<line x1="' + points[i][0] + '" y1="' + points[i][1] +
        '" x2="' + points[i + 12][0] + '" y2="' + points[i + 12][1] + '" stroke="white" />';
}
/*for (i = 0; i < 12; i++) {
document.getElementById('dynamiccan').innerHTML +=
    '<Path x1 = "0" y1 = "0" x2 = "200" y2 = "200" style = "stroke:rgb(255,255,255);stroke-width:2" />';
}*/



function polarToCartesian(centerX, centerY, radius, angleInDegrees) {
    var addedDegrees = 5;
    var angleInRadians = (angleInDegrees - 90) * Math.PI / 180.0;

    return {
        x: centerX + (radius * Math.cos(angleInRadians + addedDegrees)),
        y: centerY + (radius * Math.sin(angleInRadians + addedDegrees))
    };
}

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

function RouletteSpin() {
    
    var oldColor1;
    var oldColor2;
    for (i = 0; i < 12; i++) {
        oldColor1 = document.querySelector('#sofGlow' + i).childNodes[5].attributes[0].value;
        oldColor2 = document.querySelector('#sofGlow' + (i+12)).childNodes[5].attributes[0].value;
        document.querySelector('#sofGlow' + i).childNodes[5].attributes[0].value = "white";
        document.querySelector('#sofGlow' + (i+12)).childNodes[5].attributes[0].value = "white";
        sleep(500);
        alert("next");
        document.querySelector('#sofGlow' + i).childNodes[5].attributes[0].value = oldColor1;
        document.querySelector('#sofGlow' + (i+12)).childNodes[5].attributes[0].value = oldColor2;
    }
}

function sleep(milliseconds) {
    const date = Date.now();
    let currentDate = null;
    do {
      currentDate = Date.now();
    } while (currentDate - date < milliseconds);
    //alert("next");
  }