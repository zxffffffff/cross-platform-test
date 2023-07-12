import 'package:flutter/material.dart';

class Page3 extends StatefulWidget {
  const Page3({super.key, required this.title});

  final String title;

  @override
  State<Page3> createState() => _Page3State();
}

class _Page3State extends State<Page3> {
  void onPressed() {
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Container(
        child: TextButton(onPressed: onPressed, child: Text("TextButton")));
  }
}
