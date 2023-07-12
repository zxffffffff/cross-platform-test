import 'dart:io';

typedef HandleRecv = void Function(String buf);

class TcpClient {
  Socket? m_socket = null;
  HandleRecv? m_handleRecv = null;

  run() {
    Socket.connect('127.0.0.1', 8888).then((socket) {
      print(
          'Connected to: ${socket.remoteAddress.address}:${socket.remotePort}');

      m_socket = socket;

      // 接收数据
      socket.listen((data) {
        if (m_handleRecv != null) {
          m_handleRecv!(String.fromCharCodes(data));
        }
        print('recv: ${data.length}');
      });
    }).catchError((e) {
      print('Error: $e');
    });
  }

  close() {
    m_socket?.destroy(); // 关闭连接
    m_socket = null;
  }

  bool isRun() {
    return m_socket != null;
  }

  send(buf) {
    // 发送数据
    m_socket?.write(buf);
    print('send: ${buf.length}');
  }

  onRecv(HandleRecv f) {
    m_handleRecv = f;
  }
}
