// setup our ros object
var ros = new ROSLIB.Ros({
    url: 'ws://localhost:9090'
});


// add listeners for connection, error, and close events to our ros object
ros.on('connection', function () {
    document.getElementById("status").innerHTML = "Connected";
});

ros.on('error', function (error) {
    document.getElementById("status").innerHTML = "Error";
});

ros.on('close', function () {
    document.getElementById("status").innerHTML = "Closed";
});


// for publishing a Topic
cmd_vel_listener = new ROSLIB.Topic({
    ros: ros,
    name: "/cmd_vel",
    messageType: 'geometry_msgs/Twist'
});

move = function (linear, angular) {
    var twist = new ROSLIB.Message({
        linear: {
            x: linear,
            y: 0, // bottomSpeed
            z: 0
        },
        angular: {
            x: 0,
            y: 0, // topSpeed
            z: angular
        }
    });

    cmd_vel_listener.publish(twist);
}


// for subscribing to a Topic
var txt_listener = new ROSLIB.Topic({
    ros: ros,
    name: '/txt_msg',
    messageType: 'std_msgs/String'
});

txt_listener.subscribe(function (m) {
    document.getElementById("msg").innerHTML = m.data;
    move(1, 0);
});



// Listener for a key being pressed
window.addEventListener("keydown", checkKeyPress, false);
window.addEventListener("keyup", checkKeyUp, false);

window.onload=function() {
    var mapSaver = document.getElementById("map-saver");
    var myForm = document.getElementById("my-form");
    var submit = document.getElementById("sub-button");

    if (mapSaver) {
        mapSaver.addEventListener("click", function() {
            move(7, 0);
        }, false);
    }
};

// this function checks to see if the a control has been inputted
function checkKeyPress(key) {
    // i - Toggle
    if (key.keyCode == "73") {
        if (document.getElementById("both").checked) {
            document.getElementById("both").checked = false;
            document.getElementById("top").checked = true;
            document.getElementById("both-label").style = "background : #6c757d;";
            document.getElementById("top-label").style = "background : gold;";
        }

        else if (document.getElementById("top").checked) {
            document.getElementById("top").checked = false;
            document.getElementById("bottom").checked = true;
            document.getElementById("top-label").style = "background : #6c757d;";
            document.getElementById("bottom-label").style = "background : gold;";
        }

        else if (document.getElementById("bottom").checked) {
            document.getElementById("bottom").checked = false;
            document.getElementById("top-slow").checked = true;
            document.getElementById("bottom-label").style = "background : #6c757d;";
            document.getElementById("top-s-label").style = "background : gold;";
        }

        else if (document.getElementById("top-slow").checked) {
            document.getElementById("top-slow").checked = false;
            document.getElementById("bottom-slow").checked = true;
            document.getElementById("top-s-label").style = "background : #6c757d;";
            document.getElementById("bottom-s-label").style = "background : gold;";
        }

        else if (document.getElementById("bottom-slow").checked) {
            document.getElementById("bottom-slow").checked = false;
            document.getElementById("both").checked = true;
            document.getElementById("bottom-s-label").style = "background : #6c757d;";
            document.getElementById("both-label").style = "background : gold;";
        }
    }

    // k - toggle lift settings
    else if (key.keyCode == "75") {
        if (document.getElementById("both-lifts").checked) {
            document.getElementById("both-lifts").checked = false;
            document.getElementById("left").checked = true;
            document.getElementById("both-lift-label").style = "background : #6c757d;";
            document.getElementById("left-label").style = "background : gold;";
        }

        else if (document.getElementById("left").checked) {
            document.getElementById("left").checked = false;
            document.getElementById("right").checked = true;
            document.getElementById("left-label").style = "background : #6c757d;";
            document.getElementById("right-label").style = "background : gold;";
        }

        else if (document.getElementById("right").checked) {
            document.getElementById("right").checked = false;
            document.getElementById("both-lifts").checked = true;
            document.getElementById("right-label").style = "background : #6c757d;";
            document.getElementById("both-lift-label").style = "background : gold;";
        }
    }



    // a - Left
    if (key.keyCode == "65") {
        move(0, 1);
        document.getElementById("aKey").style.color = "Gold";
    }

    // w _ Forward
    else if (key.keyCode == "87") {
        // Checks for different states
        if (document.getElementById("both").checked) {
            console.log("Working");
            move(1, 0);
        }
        else if (document.getElementById("top").checked)
            move(2, 0);
        else if (document.getElementById("bottom").checked)
            move(3, 0);
        else if (document.getElementById("top-slow").checked)
            move(4, 0);
        else if (document.getElementById("bottom-slow").checked)
            move(5, 0);

        document.getElementById("wKey").style.color = "Gold";
    }

    // d - Right
    else if (key.keyCode == "68") {
        move(0, -1);
        document.getElementById("dKey").style.color = "Gold";
    }

    // s - Backward
    else if (key.keyCode == "83") {
        // Checks for different states
        if (document.getElementById("both").checked)
            move(-1, 0);
        else if (document.getElementById("top").checked)
            move(-2, 0);
        else if (document.getElementById("bottom").checked)
            move(-3, 0);
        else if (document.getElementById("top-slow").checked)
            move(-4, 0);
        else if (document.getElementById("bottom-slow").checked)
            move(-5, 0);

        document.getElementById("sKey").style.color = "Gold";
    }

    // o - Lift up
    else if (key.keyCode == "79") {
        //move(1, 1);

        // Checks for different states
        if (document.getElementById("both-lifts").checked)
            move(1, 1);
        else if (document.getElementById("left").checked)
            move(2, 1);
        else if (document.getElementById("right").checked)
            move(3, 1);

        document.getElementById("oKey").style.color = "Gold";
    }

    // p - Lift down
    else if (key.keyCode == "80") {
        //move(1, -1);

        // Checks for different states
        if(document.getElementById("both").checked)
            move(1, -1);
        else if(document.getElementById("top").checked)
            move(2, -1);
        else if(document.getElementById("bottom").checked)
            move(3, -1);

        document.getElementById("pKey").style.color = "Gold";
    }

    // q - Deenergize
    else if (key.keyCode == "81") {
        move(-1, -1);
        document.getElementById("qKey").style.color = "Gold";
    }

    // e - Energize
    else if (key.keyCode == "69") {
        move(-1, 1);
        document.getElementById("eKey").style.color = "Gold";
    }

    // b - Kill
    else if (key.keyCode == "66") {
        move(0, 0);
    }
}

function checkKeyUp(key) {
    // If you pick up w, a, s, d, o, or p; cancel any movement,
    // Change color of control
    // a - Left
    if (key.keyCode == "65") {
        move(0, 0);
        document.getElementById("aKey").style.color = "Black";
    }

    // w - Forward
    else if (key.keyCode == "87") {
        move(0, 0);
        document.getElementById("wKey").style.color = "Black";
    }

    // d - Right
    else if (key.keyCode == "68") {
        move(0, 0);
        document.getElementById("dKey").style.color = "Black";
    }

    // s - Backward
    else if (key.keyCode == "83") {
        move(0, 0);
        document.getElementById("sKey").style.color = "Black";
    }

    // o - Lift down
    else if (key.keyCode == "79") {
        move(0, 0);
        document.getElementById("oKey").style.color = "Black";
    }

    //p - Lift up
    else if (key.keyCode == "80") {
        move(0, 0);
        document.getElementById("pKey").style.color = "Black";
    }

    // q - Deenergize
    else if (key.keyCode == "81") {
        document.getElementById("qKey").style.color = "Black";
    }

    // e - Energize
    else if (key.keyCode == "69") {
        document.getElementById("eKey").style.color = "Black";
    }
}
