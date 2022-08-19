import React, { useEffect, useState } from "react";
import { Card, List } from "antd";
import { MdWaterDrop } from "react-icons/md";
const Receiver = ({ payload }) => {
  const [messages, setMessages] = useState([]);

  useEffect(() => {
    if (payload.topic) {
      setMessages((messages) => [...messages, payload]);
    }
  }, [payload]);

  const renderListItem = (item) => (
    <List.Item>
      <List.Item.Meta title={item.topic} description={item.message} />
    </List.Item>
  );

  return (
    <Card
      title={
        <>
          Humidade
          <MdWaterDrop color="#78a52d" />
        </>
      }
    >
      <List
        size="small"
        bordered
        dataSource={messages}
        renderItem={renderListItem}
      />
    </Card>
  );
};

export default Receiver;
