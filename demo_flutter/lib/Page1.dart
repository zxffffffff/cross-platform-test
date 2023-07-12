import 'dart:async';
import 'dart:convert';
import 'dart:ffi';

import 'package:demo_flutter/tcp_client.dart';
import 'package:flutter/material.dart';

final row = 28;
final col = 18;

class Page1 extends StatefulWidget {
  const Page1({super.key});

  @override
  State<Page1> createState() => _Page1State();
}

var flag = 0;

class _Page1State extends State<Page1> {
  List<String> itemStr = List.filled(row * col, "");
  int selectedIndex = -1;
  final tcpClient = TcpClient();
  Timer? timer = null;

  void onPressedRun() {
    if (tcpClient.isRun()) {
      tcpClient.close();
    } else {
      tcpClient.run();
    }

    tcpClient.onRecv((buf) {
      var jsonObject = jsonDecode(buf);
      var t_start = jsonObject["time"];
      List<String> data = List<String>.from(jsonObject["data"]);
      var row = data.length;
      var col = 0;
      for (var i = 0; i < row; ++i) {
        String item = data[i];
        var list = item.split(' ');
        col = list.length;
        for (var j = 0; j < col; ++j) {
          setState(() {
            itemStr[i * col + j] = list[j];
          });
        }
      }

      WidgetsBinding.instance.addPostFrameCallback((timeStamp) {
        var t_stop = new DateTime.now().millisecondsSinceEpoch;
        print("${t_stop - t_start} ms");
      });
    });
  }

  List<String> genArray(int row, int col) {
    List<String> jsonArray = [];
    for (int i = 0; i < row; ++i) {
      var item = '';
      for (int j = 0; j < col; ++j) {
        if (j > 0) item += ' ';
        item += (flag + (i * j * 1000000)).toString();
      }
      jsonArray.add(item);
    }
    ++flag;
    return jsonArray;
  }

  void onPressedReq() {
    var jsonObject = {
      "time": new DateTime.now().millisecondsSinceEpoch,
      "data": genArray(row, col),
    };
    tcpClient.send(jsonEncode(jsonObject));
  }

  void onPressedTimer() {
    if (timer == null) {
      timer = Timer.periodic(Duration(milliseconds: 10), (timer) {
        onPressedReq();
      });
    } else {
      timer!.cancel();
      timer = null;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Row(
          children: [
            TextButton(onPressed: onPressedRun, child: Text("run")),
            TextButton(onPressed: onPressedReq, child: Text("ping-pong")),
            TextButton(onPressed: onPressedTimer, child: Text("timer-10ms")),
          ],
        ),
        Expanded(
            child: GridView.builder(
          gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: col,
            mainAxisSpacing: 1.0,
            crossAxisSpacing: 1.0,
            childAspectRatio: 3.33,
          ),
          itemBuilder: (BuildContext context, int index) {
            return GestureDetector(
              onTap: () {
                setState(() {
                  selectedIndex = index;
                });
              },
              child: Container(
                width: 100,
                height: 30,
                color: selectedIndex == index ? Colors.red : Colors.blue,
                child: Center(
                  child: Text(
                    itemStr[index],
                    style: TextStyle(
                      fontSize: 16,
                      color: Colors.white,
                    ),
                  ),
                ),
              ),
            );
          },
          itemCount: col * row,
        ))
      ],
    );
  }
}
