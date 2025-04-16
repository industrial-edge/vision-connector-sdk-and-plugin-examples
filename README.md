# Vision Connector Drivers

## Table of Contents
* **[Description](#description)**
    * **[Overview](#overview)**
    * **[General Task](#general-task)**
* **[Requirements](#requirements)**
    * **[Prerequisites](#prerequisites)**
    * **[Used components](#used-components)**
* **[Installation](#installation)**
* **[Usage](#usage)**
* **[Documentation](#documentation)**
* **[Contribution](#contribution)**
* **[License and Legal Information](#license-and-legal-information)**
* **[Disclaimer](#disclaimer)**


## Description

### Overview

The Industrial Edge Vision Connector App, or VCA for short combines industrial cameras with the Industrial Edge ecosystem. The Vision Connector uses an Ethernet interface to establish a connection to the cameras. The VCA publishes the data using the ZeroMQ server integrated in the Vision Connector with high throughput to other applications.

As of version 1.2.0 it is possible to integrate any camera vendor's C++ API with simple implementation steps through custom camera connectors for VCA. 

### General Task
This project was created to introduce you to the Vision Connector drivers feature and to help the creation of custom camera connectors.

## Requirements

### Prerequisites
To develop a custom camera connector, the developer should meet the following requirements:
- Familiarity with the Vision Connector application and its features.
- Understanding of the VCA SDK, including structure and usage.
- Working knowledge of C++ and build tools such as CMake
- A properly set up development environment aligned with the project requirements.
- Access to the camera vendor's API documentation.
- Availability of the camera vendor's C++ headers and libraries required for teh integration.

### Used components
- Vision Connector V1.2.0
- VCA SDK V1.0.0

## Installation
To build and prepeare your camera connector to use in VCA we are providing utility scripts. For more information please refer to the [Installing the connector](docs/creating_and_using_a_custom_camera_connector.md#installing-the-connector) and [Packaging the connector](docs/creating_and_using_a_custom_camera_connector.md#packaging-the-connector) sections.

## Usage
To use a custom camera connector in VCA first you will need to prepare the corresponding package file. When the package file is prepared you can upload it in VCA thus adding your camera connector to the application in a few sinmple steps.

For the usage step-by-step guide please refer to the [Using the connector in VCA](docs/creating_and_using_a_custom_camera_connector.md#using-the-connector-in-vca) section.

## Documentation

To get familiar with the Vision Connector Drivers project please read the provided documentation

* [Repository overview](docs/repository_overview.md)
* [How to setup the development environment](docs/development_environment.md)
* [Introduction to VCA SDK](docs/vca_sdk.md)
* [Creating and using a custom camera connector](docs/creating_and_using_a_custom_camera_connector.md)

You can find further documentation and help in the following links

* [Industrial Edge Hub](https://iehub.eu1.edge.siemens.cloud/#/documentation)
* [Industrial Edge Forum](https://www.siemens.com/industrial-edge-forum)
* [Industrial Edge landing page](https://new.siemens.com/global/en/products/automation/topic-areas/industrial-edge/simatic-edge.html)
* [Industrial Edge GitHub page](https://github.com/industrial-edge)
* [Industrial Edge Learning Path](https://siemens-learning-simaticedge.sabacloud.com)

## Contribution

Thank you for your interest in contributing. Anybody is free to report bugs, unclear documentation, and other problems regarding this repository in the Issues section.
Additionally everybody is free to propose any changes to this repository using Pull Requests.

If you haven't previously signed the [Siemens Contributor License Agreement](https://cla-assistant.io/industrial-edge/) (CLA), the system will automatically prompt you to do so when you submit your Pull Request. This can be conveniently done through the CLA Assistant's online platform. Once the CLA is signed, your Pull Request will automatically be cleared and made ready for merging if all other test stages succeed.

## License and Legal Information

Please read the [Legal information](LICENSE.txt).

## Disclaimer

IMPORTANT - PLEASE READ CAREFULLY:

This documentation describes how you can download and set up containers which consist of or contain third-party software. By following this documentation you agree that using such third-party software is done at your own discretion and risk. No advice or information, whether oral or written, obtained by you from us or from this documentation shall create any warranty for the third-party software. Additionally, by following these descriptions or using the contents of this documentation, you agree that you are responsible for complying with all third party licenses applicable to such third-party software. All product names, logos, and brands are property of their respective owners. All third-party company, product and service names used in this documentation are for identification purposes only. Use of these names, logos, and brands does not imply endorsement.