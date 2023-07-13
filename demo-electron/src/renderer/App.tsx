import React, { useState } from 'react';
import { Tabs, Row, Col, Button } from 'antd';

const { TabPane } = Tabs;

const App: React.FC = () => {
  const [activeTab, setActiveTab] = useState('Page1');

  const handleTabChange = (key: string) => {
    setActiveTab(key);
  };

  const gridData: number[][] = [];
  for (let i = 0; i < 28; i++) {
    const row: number[] = [];
    for (let j = 0; j < 18; j++) {
      const index = i * 18 + j;
      row.push(index);
    }
    gridData.push(row);
  }

  return (
    <div>
      <Tabs activeKey={activeTab} onChange={handleTabChange}>
        <Tabs.TabPane tab="Page1" key="Page1">
          <div style={{ display: 'flex', flexWrap: 'wrap' }}>
            {gridData.map((row, rowIndex) => (
              <React.Fragment key={rowIndex}>
                {row.map((col, colIndex) => (
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
                    {col}
                  </div>
                ))}
              </React.Fragment>
            ))}
          </div>
        </Tabs.TabPane>
        <Tabs.TabPane tab="Page2" key="Page2">
          {/* Page2 内容 */}
        </Tabs.TabPane>
        <Tabs.TabPane tab="Page3" key="Page3">
          <Button type="primary">Button</Button>
        </Tabs.TabPane>
      </Tabs>
    </div>
  );
};

export default App;
