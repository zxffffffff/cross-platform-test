import React, { useEffect, useRef, useState } from 'react';
import axios from 'axios';
import { Tabs, Button } from 'antd';

const row = 28;
const col = 18;

var global_flag = 0;

const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState('Page1');
  const handleTabChange = (key: string) => {
    setActiveTab(key);
  };

  const [gridData, setGridData] = useState<number[][]>([]);
  for (let i = 0; i < row; i++) {
    const row: number[] = [];
    for (let j = 0; j < col; j++) {
      const index = i * col + j;
      row.push(index);
    }
    gridData.push(row);
  }

  const genArray = (row: number, col: number) => {
    const jsonArray = [];
    for (let i = 0; i < row; ++i) {
      let item = '';
      for (let j = 0; j < col; ++j) {
        if (j > 0) item += ' ';
        item += (global_flag + (i * j * 1000000)).toString();
      }
      jsonArray.push(item);
    }
    ++global_flag;
    return jsonArray;
  };

  const on_net_recv = (jsonObject: any) => {
    const t_start = jsonObject.time;
    const data:string[] = jsonObject.data;
    const newGridData: number[][] = [];
    for (let i = 0; i < row; i++) {
      const newRow: number[] = [];
      const list:string[] = data[i].split(' ');
      for (let j = 0; j < col; j++) {
        newRow.push(Number.parseInt(list[j]));
      }
      newGridData.push(newRow);
    }

    setGridData(newGridData);

    setTimeout(() => {
      const t_stop = new Date().getTime();
      console.log(`${t_stop - t_start} ms`);
    }, 0);
  };

  const handleReq = () => {
    const jsonObject = {
      time: new Date().getTime(),
      data: genArray(row, col),
    };
    const jsonString = JSON.stringify(jsonObject);

    const url = `http://127.0.0.1:8889/pingpong?data=${encodeURIComponent(jsonString)}`;
    axios.get(url)
      .then(response => {
        if (response.status === 200) {
          on_net_recv(response.data);
        }
      })
      .catch(error => {
        console.error(error);
      });
  };

  const [timer, setTimer] = useState(Object);
  const handleTimer = () => {
    if (timer == null) {
      const newTimer = setInterval(() => {
        handleReq();
      }, 10);
      setTimer(newTimer);
    } else {
      clearInterval(timer);
      setTimer(null);
    }
  };

  const [pos, setPos] = useState({ x: 0, y: 0 });
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (canvas) {
      const ctx = canvas.getContext('2d');
      if (ctx) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.strokeStyle = 'red';
        ctx.beginPath();
        ctx.moveTo(pos.x, 0);
        ctx.lineTo(pos.x, window.innerHeight);
        ctx.moveTo(0, pos.y);
        ctx.lineTo(window.innerWidth, pos.y);
        ctx.stroke();
      }
    }
  }, [pos]);

  const handlePanUpdate = (event: React.MouseEvent<HTMLDivElement>) => {
    const { clientX, clientY } = event;
    setPos({ x: clientX, y: clientY });
  };

  const handleNewWindow = () => {
    window.electron.ipcRenderer.sendMessage('ipc-example', ['open-new-window']);
  };

  return (
    <div>
      <Tabs activeKey={activeTab} onChange={handleTabChange}>
        <Tabs.TabPane tab="Page1" key="Page1">
          <div>
            <Button type="primary" onClick={handleReq}>req</Button>
            <Button type="primary" onClick={handleTimer}>timer</Button>
          </div>
          <div style={{ display: 'flex', flexWrap: 'wrap' }}>
            {gridData.map((row, rowIndex) => (
              <React.Fragment key={rowIndex}>
                {row.map((index, colIndex) => (
                  <div
                    key={colIndex}
                    style={{
                      border: '1px solid #ccc',
                      height: '30px',
                      width: '100px',
                      padding: '8px',
                      marginLeft: colIndex === 0 ? 0 : '1px',
                      marginTop: rowIndex === 0 ? 0 : '1px',
                      boxSizing: 'border-box',
                      display: 'flex',
                      justifyContent: 'center',
                      alignItems: 'center',
                    }}
                  >
                    {index}
                  </div>
                ))}
              </React.Fragment>
            ))}
          </div>
        </Tabs.TabPane>
        <Tabs.TabPane tab="Page2" key="Page2">
          <div
            style={{ width: '100vw', height: '100vh' }}
            onMouseMove={handlePanUpdate}
          >
            <canvas ref={canvasRef} width={window.innerWidth} height={window.innerHeight} />
          </div>
        </Tabs.TabPane>
        <Tabs.TabPane tab="Page3" key="Page3">
          <Button type="primary" onClick={handleNewWindow}>NewWindow</Button>
        </Tabs.TabPane>
      </Tabs>
    </div>
  );
};

export default App;
