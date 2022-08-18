import React from "react";
import { Card, Button, Form, Input, Row, Col } from "antd";
import {
  BsAlignCenter,
  BsPlayFill,
  BsFillDoorClosedFill,
} from "react-icons/bs";
import { MdCancel } from "react-icons/md";
import { Center } from "@mantine/core";
import { TbSubtask } from "react-icons/tb";

const Connection = ({ connect, disconnect, connectBtn }) => {
  const [form] = Form.useForm();
  const record = {
    host: "192.168.0.101",
    clientId: `TCCPLANTA`,
    port: 9001,
  };
  const onFinish = (values) => {
    const { host, clientId, port, username, password } = values;
    const url = `ws://${host}:${port}/mqtt`;
    const options = {
      keepalive: 30,
      protocolId: "MQTT",
      protocolVersion: 4,
      clean: true,
      reconnectPeriod: 1000,
      connectTimeout: 30 * 1000,
      will: {
        topic: "WillMsg",
        payload: "Conexão Encerrada inesperadamente",
        qos: 0,
        retain: false,
      },
      rejectUnauthorized: false,
    };
    options.clientId = clientId;
    options.username = username;
    options.password = password;
    connect(url, options);
  };

  const handleConnect = () => {
    form.submit();
  };

  const handleDisconnect = () => {
    disconnect();
  };

  const ConnectionForm = (
    <Form
      layout="vertical"
      name="basic"
      form={form}
      initialValues={record}
      onFinish={onFinish}
    >
      <Row gutter={20}>
        <Col span={8}>
          <Form.Item label="Host" name="host">
            <Input />
          </Form.Item>
        </Col>
        <Col span={8}>
          <Form.Item
            label={
              <>
                Porta
                <BsFillDoorClosedFill color="#78a52d" size={20} />
              </>
            }
            name="port"
          >
            <Input />
          </Form.Item>
        </Col>
        <Col span={8}>
          <Form.Item label="Cliente" name="clientId">
            <Input />
          </Form.Item>
        </Col>
        <Col span={8}>
          <Form.Item label="Nome de Usuario" name="username">
            <Input />
          </Form.Item>
        </Col>
        <Col span={8}>
          <Form.Item label="Senha" name="password">
            <Input />
          </Form.Item>
        </Col>
      </Row>
    </Form>
  );
  return (
    <Card
      title="Conexão MQTT 🔗"
      actions={[
        <Button
          type="primary"
          style={{
            backgroundColor: "#78a52d",
            border: "none",
            display: "inline-flex",
            alignItems: "center",
            gap: "2px",
          }}
          onClick={handleConnect}
        >
          <BsPlayFill size={21} />
          {connectBtn}
        </Button>,
        <Button
          style={{
            display: "inline-flex",
            alignItems: "center",
            gap: "2px",
          }}
          danger
          onClick={handleDisconnect}
        >
          Disconectar
          <MdCancel />
        </Button>,
      ]}
    >
      {ConnectionForm}
    </Card>
  );
};

export default Connection;
