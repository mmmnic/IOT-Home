$(document).ready(function() {
	var socket = io();
	$("#control_board").hide();
	$("#please_wait").show();
	socket.on("server-send-web-update-status", function(Nodes) {
		$("#control_board").show();
		$("#please_wait").hide();
		Nodes.forEach(function(Node) {
			if(Node.TYPE=="O/F") {
				if(Node.STATUS==0) {//off
					$("#"+Node.ID+"_ON").show();
					$("#"+Node.ID+"_OFF").hide();
				}
				if(Node.STATUS==1) {//on
					$("#"+Node.ID+"_ON").hide();
					$("#"+Node.ID+"_OFF").show();
				}
			}
			if(Node.TYPE=="LOG") {
				$("#"+Node.ID).html(Node.STATUS);
			}
		});
	});
	$("#LED2_ON").click(function() {
		socket.emit("web-send-server-control-device",{ID:"LED2", COMMAND:1});
	});

	$("#LED2_OFF").click(function() {
		socket.emit("web-send-server-control-device",{ID:"LED2", COMMAND:0});
	});
});
