# ITProjectSolution

## Author: Hao, Quoc, Nhu, Thuc, Khoa
## Slave: Hao, Nhu
## Master: Thuc
## Server: Khoa
## Door: Quoc

This is about an IOT Home (smart home) that can control through website.

Functions:
All devices are controled through website, some can be control by perripheral devices such as, keypad, switch, etc...

detect gas leak, air humid, room temperature, rain.

When rain is detected cloth hangers are pulled inside house.

Door open/close when enter right password on keypad or received open command from server

Light auto turn on when detect people.

JSON protocol:
{"TEMP":<float>, "HUMI":<float>, "GAS":<float>, "RAIN":<float>, "COMMAND":<int>, "ID":"String", "Status": "String"}
  
ID: Nhà Bếp: "NB", Nhà Khách: "NK", Phòng Ngủ: "PN", Phòng Tắm: "PT", Phơi Đồ: "PD", Gara: "GR", Cửa Cổng: "CC", Cửa Nhà: "CN"   
COMMAND: 1/2 = Cửa ON/OFF; 3/4 = Đèn ON/OFF; 5/6 = Quạt ON/OFF


Note: I was granted permission by my team to put this repo to my personal git for my other intent.

Thanks to my teammates:

Hao https://github.com/vanhao051212

Nhu https://github.com/ngonhu159

Thuc https://github.com/wathui99

Khoa https://github.com/fxanhkhoa

