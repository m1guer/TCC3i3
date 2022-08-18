[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=30&pause=1000&width=435&lines=TCC+3i3)](https://git.io/typing-svg)
<br>
[![Typing SVG](https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=30&pause=1000&color=0FA400&width=435&lines=Setup+Mosquitto)](https://git.io/typing-svg)
1. Setting up Mosquitto
   - Download from: [Mosquitto](https://mosquitto.org/download/)
     - Install
2. Configuring the ports will be used and websocks
* For Linux Users ![image]({https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black})
  - sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
  - sudo apt-get update
  - sudo apt-get install mosquitto
  - sudo apt-get install mosquitto-clients
  - sudo apt clean
* Windows users
  - Just Install the .exe
3. Creating a configuration file .conf
* For Linux Users
  - go to mosquitto default installation(YOUUSER/etc/mosquitto)
  - on mosquitto default run terminal commands
  ```
  nano *.conf
  ```
  - put this on *.conf
  ```
  listener 1883
  listener 9001
  alow_anonymous = true
  ```
  - CTRL + O to overwrite
    - now run 
    ```
    mosquitto -v -c *.conf
    ```


<br>
<h2>Linguagens Usadas<h2>
<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=cp,react,nodejs,css,js,git" />
  </a>
</p>
