print("from test.js");

function setup() {

	var mesh = Scene_addMeshNode("robot.FBX");
    var matrix = [1.0,0,0,0,
                  0,1.0,0,0,
                  0,0,1,0,
                  0,0,0,1];
    Node_setModelMatrix(mesh, matrix);
}

function draw() {

}

setup();
