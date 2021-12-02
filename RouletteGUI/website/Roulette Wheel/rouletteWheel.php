<table id="canvasnoborder" width="450" height="450">
    <tr>
        <td align="center">
            <svg id="dynamiccan" height="450" width="450">
            </svg>

        </td>
    </tr>
</table>

<script>
var width1 = document.getElementById('canvasnoborder').offsetWidth;
var height1 = document.getElementById('canvasnoborder').offsetHeight;
//alert("width" + width1 + ",height" + height1);
var arcsegments = 1 / 12;
var LEDs = [
    [1, '#1900ff'],
    [2, 'green'],
    [3, '#1900ff'],
    [4, 'green'],
    [5, '#1900ff'],
    [6, 'green'],
    [7, '#1900ff'],
    [8, 'green'],
    [9, '#1900ff'],
    [10, 'green'],
    [11, '#1900ff'],
    [12, 'green'],
    [13, '#1900ff'],
    [14, 'green'],
    [15, '#1900ff'],
    [16, 'green'],
    [17, '#1900ff'],
    [18, 'green'],
    [19, '#1900ff'],
    [20, 'green'],
    [21, '#1900ff'],
    [22, 'green'],
    [23, '#1900ff'],
    [24, 'green'],
];

for (i = 0; i < 24; i++) {
    //   alert(LEDs[i][1]);
    document.getElementById('dynamiccan').innerHTML += '<path id="arc' + (i + 1) +
        '" fill="none" stroke="' + LEDs[i][1] + '" stroke-width="3" />';
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

//   for (i = 0; i < 12; i++) {
//'<path id="arc1" fill="none" stroke="white" stroke-width="1" />'
//}
//'</svg>';



function polarToCartesian(centerX, centerY, radius, angleInDegrees) {
    var angleInRadians = (angleInDegrees - 90) * Math.PI / 180.0;

    return {
        x: centerX + (radius * Math.cos(angleInRadians)),
        y: centerY + (radius * Math.sin(angleInRadians))
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
</script>