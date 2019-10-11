var express = require('express');
var app = express();
app.use(express.static("./public"));
app.set("view engine","ejs");
app.set("views", "./views");
var port = 3000;
var server = require("http").Server(app);

//socket io and pass port
var io = require("socket.io")(server);

io.on("connection", function(socket) {
	console.log(socket.id + " connected");
    socket.on("disconnect", function() {
        console.log(socket.id + " disconnected");
    });

    //socket send to esp update status
	socket.broadcast.emit('server-send-esp-update-status');

	socket.on("esp-send-server-update-status", function(data) {
		/*data la:
		[{ID: , STATUS: ,TYPE: (LOG, O/F)}, ...]*/
		socket.broadcast.emit('server-send-web-update-status',data);
	});

	socket.on("web-send-server-control-device",function(data) {
		/*data la:
		{ID: , COMMAND: }*/
		socket.broadcast.emit('server-send-esp-control-device',data);
	});
});


app.get('/control', function(req, res, next) {
    res.render("control");
});

server.listen(process.env.PORT || port);