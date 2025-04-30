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

The Industrial Edge Vision Connector combines industrial cameras with the Industrial Edge ecosystem through an Ethernet interface. It publishes camera data using an integrated ZeroMQ server, enabling high-throughput communication with other applications. Starting with version 1.2.0, the Vision Connector provides feature for importing customer camera connectors into the application for connecting and acquiring images from additional Camera vendors (using camera vendors C++ API) or remote endpoints (such as FTP or cloud storage) which are not supported natively by the Vision Connector. This documentation provides guidance, instructions, and exmaple for creating such custom camera connectors. 

### General Task
This project provides guidance for implementing the Vision Connector drivers feature and assists in the creation of custom camera connectors. 

## Requirements

### Prerequisites
To create a custom camera connector, ensure the following prerequisites are met:
- Experience with Siemens Vision Connector application and its features
- Proficiency in C++ programming and CMake build system
- Understanding of the Vision Connector SDK (v1.0.0)
- Development environment meeting project specifications
- Access to and understanding of camera vendor's API documentation
- Camera vendor's C++ headers and libraries for integration

### Used components
- Vision Connector V1.2.0
- Vision Connector SDK V1.0.0

## Installation
To build and prepare your camera connector for use in Vision Connector, we provide utility scripts. Users should use the scripts in `src/requirements` to install prerequisite packages. For detailed information, please refer to the [Installing the connector](docs/creating_and_using_a_custom_camera_connector.md#installing-the-connector) and [Packaging the connector](docs/creating_and_using_a_custom_camera_connector.md#packaging-the-connector) sections.

## Step-by-Step Guide to building a connector
To use a custom camera connector in Vision Connector, you first need to prepare the corresponding package file. Once prepared, you can upload it to Vision Connector, adding your camera connector to the application in a few simple steps.

1. **Prepare the Development Environment**  
    Ensure all prerequisites are installed and the development environment is set up as described in the [Development Environment Setup](docs/development_environment.md) section.

2. **Understand the Vision Connector SDK**  
    Familiarize yourself with the Vision Connector SDK by reviewing the [Vision Connector SDK Guide](docs/vca_sdk.md).

3. **Create the Custom Camera Connector**  
    - Use the provided SDK and follow the [Custom Camera Connector Guide](docs/creating_and_using_a_custom_camera_connector.md) to implement your connector.
    - Integrate the camera vendor's API and ensure compatibility with the Vision Connector.

4. **Build the Connector**  
    - Use the utility scripts in `src/requirements` to install necessary dependencies.
    - Follow the instructions in the [Installing the connector](docs/creating_and_using_a_custom_camera_connector.md#installing-the-connector) section to build the connector.

5. **Package the Connector**  
    - Package the connector as described in the [Packaging the connector](docs/creating_and_using_a_custom_camera_connector.md#packaging-the-connector) section.

6. **Upload the Connector**  
    - Access the Vision Connector application and upload the packaged connector file.
    - Follow the steps in the [Using the connector in Vision Connector](docs/creating_and_using_a_custom_camera_connector.md#using-the-connector-in-vca) section to complete the process.

7. **Test the Connector**  
    - Verify the functionality of the custom camera connector by testing it with the Vision Connector application.
    - Debug and refine as necessary to ensure optimal performance.


## Documentation

### Core Documentation
* [Repository Overview](docs/repository_overview.md)
* [Development Environment Setup](docs/development_environment.md)
* [Vision Connector SDK Guide](docs/vca_sdk.md)
* [Custom Camera Connector Guide](docs/creating_and_using_a_custom_camera_connector.md)

### Additional Resources
* [Industrial Edge Hub](https://iehub.eu1.edge.siemens.cloud/#/documentation)
* [Industrial Edge Forum](https://www.siemens.com/industrial-edge-forum)
* [Industrial Edge Landing Page](https://new.siemens.com/global/en/products/automation/topic-areas/industrial-edge/simatic-edge.html)
* [Industrial Edge GitHub](https://github.com/industrial-edge)
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