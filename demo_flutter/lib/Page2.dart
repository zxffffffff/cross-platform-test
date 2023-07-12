import 'package:flutter/material.dart';

class Page2 extends StatefulWidget {
  const Page2({super.key, required this.title});

  final String title;

  @override
  State<Page2> createState() => _Page2State();
}

class _Page2State extends State<Page2> {
  var _pos = Offset.zero;

  @override
  Widget build(BuildContext context) {
    return Container(child: LayoutBuilder(
        builder: (BuildContext context, BoxConstraints constraints) {
      var size = constraints.biggest;

      return GestureDetector(
        onPanUpdate: (details) {
          setState(() {
            _pos = details.localPosition;
          });
        },
        child: CustomPaint(
          painter: MyPainter(_pos),
          size: size,
        ),
      );
    }));
  }
}

class MyPainter extends CustomPainter {
  Offset _pos;
  MyPainter(this._pos);

  @override
  void paint(Canvas canvas, Size size) {
    Paint paint = Paint()..color = Colors.red;
    canvas.drawLine(Offset(_pos.dx, 0), Offset(_pos.dx, size.height), paint);
    canvas.drawLine(Offset(0, _pos.dy), Offset(size.width, _pos.dy), paint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
