// SCRIPT TO PREVENT RANDOMLY CLOSING OF RUNNING SESSION

// ask if want to close
window.onbeforeunload = function (e) {
    e = e || window.event;
    // For IE and Firefox prior to version 4
    if (e) {
        e.returnValue = 'Sure?';
    }
    // For Safari
    return 'Sure?';
};
