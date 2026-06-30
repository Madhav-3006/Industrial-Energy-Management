# Industrial Energy Management System

The **Industrial Energy Management System** was developed as part of our internship at **PHYTEC Embedded Pvt. Ltd.** The objective of this project was to design and implement an Industrial Internet of Things (IIoT) based solution for monitoring electrical energy consumption and remotely controlling electrical loads. The project combines embedded hardware, cloud communication, and real-time visualization to provide an efficient and user-friendly energy monitoring system.

The system is built around the **RBA5D2X Rugged Board**, which acts as the main processing unit. An **ACS712-20A Hall-effect current sensor** is used to measure the load current, and the acquired analog data is processed using the onboard ADC. The embedded application, developed in **Embedded C** on **Embedded Linux**, calculates the RMS current, electrical power, and cumulative energy consumption. These values are then transmitted to **ThingsBoard Community Edition** through the **MQTT protocol** using the Eclipse Mosquitto MQTT Broker.

A web-based dashboard was developed in ThingsBoard to display current, power, and energy values in real time. The dashboard also provides relay control and energy reset functionalities, allowing users to remotely operate the connected electrical load. During testing, the system was validated using **60 W** and **100 W incandescent bulbs**, and it successfully demonstrated reliable current measurement, power calculation, energy monitoring, MQTT communication, and remote relay operation.

This repository contains the complete source code, hardware documentation, dashboard configuration, project images, and demonstration video developed during the internship. The project provided us with valuable hands-on experience in embedded Linux development, Industrial IoT, MQTT communication, ADC interfacing, ThingsBoard dashboard development, and system integration. It also helped us understand how embedded systems can be applied to real-world industrial energy monitoring applications.

---

### Developed By

**Aditya Raghu**

**Madhav Saiprasad**

Department of Electronics and Communication Engineering  
PES University

**Internship Organization:** PHYTEC Embedded Pvt. Ltd.  
**Duration:** 1 June 2026 – 30 June 2026
