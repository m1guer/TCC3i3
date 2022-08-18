import React from "react";
import { Card, Button, Form, Input, Row, Col } from "antd";
import { Group, Button } from "@mantine/core";
import { ShowNotification } from "@mantine/notifications";
class Connection extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      record: {
        host: "192.168.0.101",
        clientId: `TCCPLANTA`,
        port: 9001,
      },
    };
  }

  onRecordChange = (value) => {
    const { record } = this.state;
    const changedRecord = Object.assign(record, value);
    this.setState({ record: changedRecord });
  };

  handleConnect = () => {
    const { host, clientId, port, username, password } = this.state.record;
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
        payload: "Connection Closed abnormally..!",
        qos: 0,
        retain: false,
      },
      rejectUnauthorized: false,
    };
    options.clientId = clientId;
    options.username = username;
    options.password = password;
    this.props.connect(url, options);
  };

  render() {
    const ConnectionForm = (
      <Form
        layout="vertical"
        name="basic"
        initialValues={this.state.record}
        onValuesChange={this.onRecordChange}
      >
        <Row gutter={20}>
          <Col span={8}>
            <Form.Item label="Host" name="host">
              <Input />
            </Form.Item>
          </Col>
          <Col span={8}>
            <Form.Item label="Porta" name="port">
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
        title="Connection"
        actions={[
          <Button type="primary" onClick={this.handleConnect}>
            {this.props.connectBtn}
          </Button>,
          <Button danger onClick={this.props.disconnect}>
            Disconnect
          </Button>,
        ]}
      >
        {ConnectionForm}
      </Card>
    );
  }
}

export default Connection;
