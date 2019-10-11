var express = require('express');
var session = require('express-session');
var app = express();
var bodyParser = require('body-parser');
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies
app.use(express.static(__dirname + '/public'));
app.use(session({secret: 'iotuser', cookie: { maxAge: 60000 }}));
app.set("view engine", 'ejs');
app.set('views', './views');

var http = require('http').createServer(app);
var fs = require("fs");
var mysql = require('mysql');

var io = require('socket.io')(http);

var sess, usernameUsed;

http.listen(8000, function(){
  console.log('Listening on port 8000');
});

io.on('connection', function(socket){
  console.log('An user connected' + socket.id + " " 
    + socket.rooms);

  socket.on('disconnect', function(){
    console.log('user disconnected!');
  });

  socket.on('chat-message', function(iotObject){
    //console.log(msg);
    var iotObject;// = JSON.parse(msg);

    //////// Get Full DB //////////
    con.query("SELECT IDNODE, TEMP, HUMI, GAS, RAIN, STATUS FROM IOT_HOME", function (err, result, fields){
      // console.log(result);
      //////// Check Command and Control /////////
      // Cửa
      if ((iotObject.COMMAND == 1)){
        switch(iotObject.IDNODE){
          case 'CC':
            if (result[0].STATUS.indexOf("Door Closed") > -1){
              update_Status = result[0].STATUS.replace("Door Closed", "Door Open");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'CN':
            if (result[1].STATUS.indexOf("Door Closed") > -1){
              update_Status = result[1].STATUS.replace("Door Closed", "Door Open");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NB':
            if (result[2].STATUS.indexOf("Door Closed") > -1){
              update_Status = result[2].STATUS.replace("Door Closed", "Door Open");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PD':
            if (result[6].STATUS.indexOf("Door Closed") > -1){
              update_Status = result[6].STATUS.replace("Door Closed", "Door Open");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'GR':
            if (result[7].STATUS.indexOf("Door Closed") > -1){
              update_Status = result[7].STATUS.replace("Door Closed", "Door Open");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }
      else if((iotObject.COMMAND == 2)){
        switch(iotObject.IDNODE){
          case 'CC':
            if (result[0].STATUS.indexOf("Door Open") > -1){
              update_Status = result[0].STATUS.replace("Door Open", "Door Closed");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'CN':
            if (result[1].STATUS.indexOf("Door Open") > -1){
              update_Status = result[1].STATUS.replace("Door Open", "Door Closed");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NB':
            if (result[2].STATUS.indexOf("Door Open") > -1){
              update_Status = result[2].STATUS.replace("Door Open", "Door Closed");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PD':
            if (result[6].STATUS.indexOf("Door Open") > -1){
              update_Status = result[6].STATUS.replace("Door Open", "Door Closed");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'GR':
            if (result[7].STATUS.indexOf("Door Open") > -1){
              update_Status = result[7].STATUS.replace("Door Open", "Door Closed");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }

      // Đèn
      if ((iotObject.COMMAND == 3)){
        switch(iotObject.IDNODE){
          case 'CC':
            if (result[0].STATUS.indexOf("Light Off") > -1){
              update_Status = result[0].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'CN':
            if (result[1].STATUS.indexOf("Light Off") > -1){
              update_Status = result[1].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NB':
            if (result[2].STATUS.indexOf("Light Off") > -1){
              update_Status = result[2].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NK':
            if (result[3].STATUS.indexOf("Light Off") > -1){
              update_Status = result[3].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PN':
            if (result[4].STATUS.indexOf("Light Off") > -1){
              update_Status = result[4].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PT':
            if (result[5].STATUS.indexOf("Light Off") > -1){
              update_Status = result[5].STATUS.replace("Light Off", "Light On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }
      else if ((iotObject.COMMAND == 4)){
        switch(iotObject.IDNODE){
          case 'CC':
            if (result[0].STATUS.indexOf("Light On") > -1){
              update_Status = result[0].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'CN':
            if (result[1].STATUS.indexOf("Light On") > -1){
              update_Status = result[1].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NB':
            if (result[2].STATUS.indexOf("Light On") > -1){
              update_Status = result[2].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NK':
            if (result[3].STATUS.indexOf("Light On") > -1){
              update_Status = result[3].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PN':
            if (result[4].STATUS.indexOf("Light On") > -1){
              update_Status = result[4].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PT':
            if (result[5].STATUS.indexOf("Light On") > -1){
              update_Status = result[5].STATUS.replace("Light On", "Light Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }

      // Quạt
      if ((iotObject.COMMAND == 5)){
        switch(iotObject.IDNODE){
          case 'NB':
            if (result[2].STATUS.indexOf("Fan Off") > -1){
              update_Status = result[2].STATUS.replace("Fan Off", "Fan On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NK':
            if (result[3].STATUS.indexOf("Fan Off") > -1){
              update_Status = result[3].STATUS.replace("Fan Off", "Fan On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PN':
            if (result[4].STATUS.indexOf("Fan Off") > -1){
              update_Status = result[4].STATUS.replace("Fan Off", "Fan On");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }
      else if ((iotObject.COMMAND == 6)){
        switch(iotObject.IDNODE){
          case 'NB':
            if (result[2].STATUS.indexOf("Fan On") > -1){
              update_Status = result[2].STATUS.replace("Fan On", "Fan Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'NK':
            if (result[3].STATUS.indexOf("Fan On") > -1){
              update_Status = result[3].STATUS.replace("Fan On", "Fan Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
          case 'PN':
            if (result[4].STATUS.indexOf("Fan On") > -1){
              update_Status = result[4].STATUS.replace("Fan On", "Fan Off");
              console.log(update_Status);
              con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, iotObject.IDNODE], function (err, result, fields){
                if (err) throw err;
              });
            }
            break;
        }
      }
    });

    

    con.query('UPDATE IOT_HOME SET TEMP = ? ,HUMI =?, GAS =?, RAIN= ?, COMMAND= ? WHERE IDNODE = ?', 
        [iotObject.TEMP, iotObject.HUMI, iotObject.GAS, iotObject.RAIN, iotObject.COMMAND, iotObject.IDNODE], function (err, result, fields){
      if (err) throw err;
    });
  });

  socket.on('status-all', function(msg){
    con.query("SELECT IDNODE, TEMP, HUMI, GAS, RAIN, STATUS FROM IOT_HOME", function (err, result, fields){
      // console.log(result);
      socket.emit('status-all', result);
    });
  });

  socket.on('user-chat', function(msg){
    console.log('user message: ' + msg);

    ///// Make Json
    var iotObject = {
      IDNODE: "NB",
      TEMP: 0,
      HUMI: 0,
      GAS: 0,
      RAIN: 0,
      COMMAND: 0,
      STATUS: "NONE"
    };

    con.query("SELECT IDNODE, TEMP, HUMI, GAS, RAIN, STATUS FROM IOT_HOME", function (err, result, fields){
      if (err) throw err;

      if ("CC_Door".localeCompare(msg) == 0){
        CC_Status = result[0].STATUS;
        update_Status = result[0].STATUS;
        if (CC_Status.indexOf("Door Closed") > -1){
          update_Status = update_Status.replace("Door Closed", "Door Open");
          iotObject.IDNODE = 'CC';
          iotObject.COMMAND = 1;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Door Open", "Door Closed");
          iotObject.IDNODE = 'CC';
          iotObject.COMMAND = 2;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[0].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Cửa Cổng Change State!');
      }

      else if ("CC_Light".localeCompare(msg) == 0){
        CC_Status = result[0].STATUS;
        update_Status = result[0].STATUS;
        if (CC_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'CC';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'CC';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[0].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn Cửa Cổng Change State!');
      }

      else if ("CN_Door".localeCompare(msg) == 0){
        CN_Status = result[1].STATUS;
        update_Status = result[1].STATUS;
        if (CN_Status.indexOf("Door Closed") > -1){
          update_Status = update_Status.replace("Door Closed", "Door Open");
          iotObject.IDNODE = 'CN';
          iotObject.COMMAND = 1;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Door Open", "Door Closed");
          iotObject.IDNODE = 'CN';
          iotObject.COMMAND = 2;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[1].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Cửa Nhà Change State!');
      }

      else if ("CN_Light".localeCompare(msg) == 0){
        CN_Status = result[1].STATUS;
        update_Status = result[1].STATUS;
        if (CN_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'CN';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'CN';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[1].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn Cửa Nhà Change State!');
      }

      else if ("NB_Door".localeCompare(msg) == 0){
        NB_Status = result[2].STATUS;
        update_Status = result[2].STATUS;
        if (NB_Status.indexOf("Door Closed") > -1){
          update_Status = update_Status.replace("Door Closed", "Door Open");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 1;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Door Open", "Door Closed");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 2;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[2].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Cửa nhà bếp Change State!');
      }

      else if ("NB_Light".localeCompare(msg) == 0){
        NB_Status = result[2].STATUS;
        update_Status = result[2].STATUS;
        if (NB_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[2].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn nhà bếp Change State!');
      }

      else if ("NB_Fan".localeCompare(msg) == 0){
        NB_Status = result[2].STATUS;
        update_Status = result[2].STATUS;
        if (NB_Status.indexOf("Fan Off") > -1){
          update_Status = update_Status.replace("Fan Off", "Fan On");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 5;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Fan On", "Fan Off");
          iotObject.IDNODE = 'NB';
          iotObject.COMMAND = 6;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[2].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Quạt nhà bếp Change State!');
      }

      else if ("NK_Light".localeCompare(msg) == 0){
        NK_Status = result[3].STATUS;
        update_Status = result[3].STATUS;
        if (NK_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'NK';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'NK';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[3].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn nhà khách Change State!');
      }

      else if ("NK_Fan".localeCompare(msg) == 0){
        NK_Status = result[3].STATUS;
        update_Status = result[3].STATUS;
        if (NK_Status.indexOf("Fan Off") > -1){
          update_Status = update_Status.replace("Fan Off", "Fan On");
          iotObject.IDNODE = 'NK';
          iotObject.COMMAND = 5;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Fan On", "Fan Off");
          iotObject.IDNODE = 'NK';
          iotObject.COMMAND = 6;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[3].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Quạt nhà khách Change State!');
      }

      else if ("PN_Light".localeCompare(msg) == 0){
        PN_Status = result[4].STATUS;
        update_Status = result[4].STATUS;
        if (PN_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'PN';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'PN';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[4].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn nhà khách Change State!');
      }

      else if ("PN_Fan".localeCompare(msg) == 0){
        PN_Status = result[4].STATUS;
        update_Status = result[4].STATUS;
        if (PN_Status.indexOf("Fan Off") > -1){
          update_Status = update_Status.replace("Fan Off", "Fan On");
          iotObject.IDNODE = 'PN';
          iotObject.COMMAND = 5;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Fan On", "Fan Off");
          iotObject.IDNODE = 'PN';
          iotObject.COMMAND = 6;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[4].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Quạt phòng ngủ Change State!');
      }

      else if ("PT_Light".localeCompare(msg) == 0){
        PT_Status = result[5].STATUS;
        update_Status = result[5].STATUS;
        if (PT_Status.indexOf("Light Off") > -1){
          update_Status = update_Status.replace("Light Off", "Light On");
          iotObject.IDNODE = 'PT';
          iotObject.COMMAND = 3;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Light On", "Light Off");
          iotObject.IDNODE = 'PT';
          iotObject.COMMAND = 4;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[5].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Đèn phòng tắm Change State!');
      }

      else if ("PD_Door".localeCompare(msg) == 0){
        PD_Status = result[6].STATUS;
        update_Status = result[6].STATUS;
        if (PD_Status.indexOf("Door Closed") > -1){
          update_Status = update_Status.replace("Door Closed", "Door Open");
          iotObject.IDNODE = 'PD';
          iotObject.COMMAND = 1;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Door Open", "Door Closed");
          iotObject.IDNODE = 'PD';
          iotObject.COMMAND = 2;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[6].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Cửa phơi đồ Change State!');
      }

      else if ("GR_Door".localeCompare(msg) == 0){
        GR_Status = result[7].STATUS;
        update_Status = result[7].STATUS;
        if (GR_Status.indexOf("Door Closed") > -1){
          update_Status = update_Status.replace("Door Closed", "Door Open");
          iotObject.IDNODE = 'GR';
          iotObject.COMMAND = 1;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        else{
          update_Status = update_Status.replace("Door Open", "Door Closed");
          iotObject.IDNODE = 'GR';
          iotObject.COMMAND = 2;
          var strJson = JSON.stringify(iotObject);
          io.emit('chat-message', iotObject);
        }
        con.query('UPDATE IOT_HOME SET STATUS = ? WHERE IDNODE = ?', [update_Status, result[7].IDNODE], function (err, result, fields){
          if (err) throw err;
        });
        socket.emit('user-chat','Cửa Gara Change State!');
      }

    });
      
  });
});


////////////////// Web Express ///////////////
var con = mysql.createConnection({
  host:"localhost",
  user:"IT_PROJECT",
  password:"12345678",
  database:"IT_SOLUTION"
});

con.connect(function(err){
  if (err) throw err;
  console.log("Connected Database");
});

app.get("/", function(req, res){
  var node = [];
  node.push("aaa");
  node.push("bbb");
  
  sess = req.session;
  if (sess.username == null){
    // res.setHeader('Content-Type', 'text/html');
    res.render('login',{
      text : '',
    });
    return;
  }
  // For Select Database
  con.query("SELECT IDNODE, TEMP, HUMI, GAS, RAIN, STATUS FROM IOT_HOME", function (err, result, fields){
    if (err) throw err;
    res.render("index", {
      NAME: 'IOTHOME',
      node: node,
      data: result,
    });
  });
});

app.get("/control", function(req, res){
  // sess = req.session;
  // console.log(sess);
  // if (!sess.username){
  //   res.render('login');
  //   return;
  // }
  con.query("SELECT IDNODE, TEMP, HUMI, GAS, RAIN, STATUS FROM IOT_HOME", function (err, result, fields){
    if (err) throw err;
    res.render("control", {
      data: result,
    });
  });
});

app.get("/signup", function(req, res){

  var text = '';
  console.log('asdawsd', usernameUsed);
  if (usernameUsed == 1){
    text = 'Account has been used!';
  }
  res.render('signup',{
    text : text,
  });
});

app.post('/signup/process', function(req, res, next){
  var username = req.body.username;
  var pass = req.body.pwd;
  usernameUsed = 0;

  console.log(username + pass);
  var flag = 0;
  try{
    con.query("SELECT USERNAME, PASSWORD FROM USER WHERE USERNAME='" + username + "'", function (err, result, fields){
      if (err) throw err;
      // console.log(result.length);
      if (result.length > 0){
        res.redirect('/signup');
        usernameUsed = 1;
        console.log('asdasd', usernameUsed);
        return;
      }      
      else{
        sql = "INSERT INTO USER (USERNAME, PASSWORD) VALUES (?, ?)";
        con.query(sql, [username, pass], function (err, result){
          if (err){
            var text = "Sign up unsuccessful!";
            res.render('signup', {
              text : text,
            });
            return;
          }
          var text = "Sign up successful!";
          res.redirect('/login');
          usernameUsed = 2;
        });
      }
    });
  } 
  catch (e){

  }
});

app.get("/login", function(req, res){
  
  var text = '';
  if (usernameUsed == 2){
    text = 'Sign up successful!';
  }
  res.render('login',{
    text : text,
  });
});

app.post('/login/process', function(req, res, next){
  var username = req.body.username;
  var pass = req.body.pwd;
  sess = req.session;

  console.log(username + pass);
  con.query("SELECT USERNAME, PASSWORD FROM USER WHERE USERNAME='" + username + "'", function (err, result, fields){
    if (err) throw err;
    if (pass == result[0].PASSWORD){
      sess.username = username;
      console.log(sess.username);
      res.redirect('/control');
    }
  });
});

// http.createServer(function (req, res) {
//   res.writeHead(200, {'Content-Type': 'text/html'});
//   var data = fs.readFileSync(__dirname + "/index.html", "utf-8");
//   data = data.replace("{NAME}", "IT_PROJECT_SOLUTION");
//   res.end(data);
// }).listen(8000);