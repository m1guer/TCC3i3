import React from "react";
import HookMqtt from "./components/Hook";
import { showNotification } from "@mantine/notifications";
import { TbSubtask } from "react-icons/tb";
// Hook or Class
// import ClassMqtt from './components/Class/'
import "./App.css";

function App() {
  return (
    <div className="App">
      <HookMqtt />
      {/* Hook or Class */}
      {/* <ClassMqtt /> */}
    </div>
  );
}

export default App;
