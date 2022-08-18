import React, { useContext } from "react";
import { Card, Form, Input, Row, Col, Button, Select } from "antd";
import { QosOption } from "./index";
import { MdOutlineSubdirectoryArrowRight } from "react-icons/md";

const Subscriber = ({ sub, unSub, showUnsub }) => {
  const [form] = Form.useForm();
  const qosOptions = useContext(QosOption);

  const record = {
    topic: "testtopic/react",
    qos: 0,
  };

  const onFinish = (values) => {
    sub(values);
  };

  const handleUnsub = () => {
    const values = form.getFieldsValue();
    unSub(values);
  };

  const SubForm = (
    <Form
      layout="vertical"
      name="basic"
      form={form}
      initialValues={record}
      onFinish={onFinish}
    >
      <Row gutter={20}>
        <Col span={12}>
          <Form.Item label="Topic" name="topic">
            <Input />
          </Form.Item>
        </Col>
        <Col span={12}>
          <Form.Item label="QoS" name="qos">
            <Select options={qosOptions} />
          </Form.Item>
        </Col>
        <Col span={8} offset={16} style={{ textAlign: "right" }}>
          <Form.Item>
            <Button
              style={{
                marginLeft: "10px",
                background: "#78a52d",
                border: "none",
                display: "flex",
                alignItems: "self-end",
              }}
              type="primary"
              htmlType="submit"
            >
              <MdOutlineSubdirectoryArrowRight size={25} />
              Subscribe
            </Button>
            {showUnsub ? (
              <Button
                type="danger"
                style={{ marginLeft: "10px" }}
                onClick={handleUnsub}
              >
                Unsubscribe
              </Button>
            ) : null}
          </Form.Item>
        </Col>
      </Row>
    </Form>
  );

  return <Card title="Sub">{SubForm}</Card>;
};

export default Subscriber;
