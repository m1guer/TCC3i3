import React from "react";
import { Card, Button, Form, Input, Row, Col } from "antd";
import {
  BsAlignCenter,
  BsPlayFill,
  BsFillDoorClosedFill,
  BsFillKeyFill,
} from "react-icons/bs";
import { AiOutlineKey } from "react-icons/ai";
import { MdCancel } from "react-icons/md";
import { GiPlantWatering, GiThreeLeaves } from "react-icons/gi";
import { HiOutlineUserCircle } from "react-icons/hi";
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
          <Form.Item
            label={
              <>
                Nome de usuario
                <HiOutlineUserCircle size="25px " color="#78a52d" />
              </>
            }
            name="username"
          >
            <Input />
          </Form.Item>
        </Col>
        <Col span={8}>
          <Form.Item
            label={
              <>
                <>
                  Senha
                  <br></br>
                  <BsFillKeyFill size="25px" color="#78a52d" gap={"2px"} />
                </>
              </>
            }
            name="password"
          >
            <Input />
          </Form.Item>
        </Col>
      </Row>
    </Form>
  );
  return (
    <Card
      title={
        <div
          style={{
            fontSize: "25px",
            gap: "2px",
            //display: "flex",
            //alignItems: "right",
          }}
        >
          Conexão MQTT
          <GiThreeLeaves color="#78a52d" size={75} justifyContent="left" />
        </div>
      }
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
          Desconectar
          <MdCancel />
        </Button>,
      ]}
    >
      {ConnectionForm}
    </Card>
  );
};

export default Connection;
