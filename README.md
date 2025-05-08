# Vision Connector Drivers

## Table of Contents

* **[Description](#description)**
    * **[Overview](#overview)**
    * **[General Task](#general-task)**
* **[Requirements](#requirements)**
    * **[Prerequisites](#prerequisites)**
        * **[Development Skills](#development-skills)**
        * **[System Requirements](#system-requirements)**
        * **[Camera Integration Requirements](#camera-integration-requirements)**
    * **[Used Components](#used-components)**
        * **[Core Components](#core-components)**
        * **[Build Environment](#build-environment)**
* **[Installation](#installation)**
* **[Step-by-Step Guide to Building a Connector](#step-by-step-guide-to-building-a-connector)**
    * **[Prepare the Development Environment](#prepare-the-development-environment)**
    * **[Understand the Vision Connector SDK](#understand-the-vision-connector-sdk)**
    * **[Create the Custom Camera Connector](#create-the-custom-camera-connector)**
    * **[Build the Connector](#build-the-connector)**
    * **[Package the Connector](#package-the-connector)**
    * **[Upload the Connector](#upload-the-connector)**
    * **[Test the Connector](#test-the-connector)**
* **[Documentation](#documentation)**
    * **[Core Documentation](#core-documentation)**
    * **[Additional Resources](#additional-resources)**
* **[Contribution](#contribution)**
* **[License and Legal Information](#license-and-legal-information)**
* **[Disclaimer](#disclaimer)**

## Description

### Overview

The Industrial Edge Vision Connector combines industrial cameras with the Industrial Edge ecosystem through an Ethernet interface. It publishes camera data using an integrated ZeroMQ server, enabling high-throughput communication with other applications. Starting with version 1.2.0, the Vision Connector provides feature for importing customer camera connectors into the application for connecting and acquiring images from additional Camera vendors (using camera vendors C++ API) or remote endpoints (such as FTP or cloud storage) which are not supported natively by the Vision Connector. This documentation provides guidance, instructions, and exmaple for creating such custom camera connectors. 

### General Task
This project provides guidance for implementing the Vision Connector drivers feature and assists in the creation of custom camera connectors. 

## Requirements

Here's the revised prerequisites section with improved organization and clarity:

### Prerequisites

#### Development Skills
- Proficiency in C++ programming
- Experience with CMake build system
- Familiarity with Siemens Vision Connector application and its features
- Understanding of the Vision Connector SDK (v1.0.0)

#### System Requirements
Choose one of the following:
- **For Local Build:**
  - Debian 11 (bullseye) operating system
  - Development environment meeting project specifications
- **For Docker Build:**
  - Any operating system supporting Docker
  - Docker Engine installed and configured

#### Camera Integration Requirements
- Access to camera vendor's API documentation
- Camera vendor's C++ headers and libraries
- Understanding of camera vendor's API and protocols

### Used Components
#### Core Components
- Vision Connector V1.2.0
- Vision Connector SDK V1.0.0

#### Build Environment
- **Local Build:**
  - Debian 11 (bullseye)
  - Required development tools (installed via provided scripts)
- **Docker Build:**
  - Docker Engine
  - Provided Docker container configuration

## Installation
To build and prepare your camera connector for use in Vision Connector, we provide utility scripts. The development environment requires Debian 11 (bullseye); if you're using a different operating system, it's strongly recommended to use the Docker container approach instead. For users with Debian 11, the scripts in `src/requirements` can be used to install prerequisite packages. For detailed information, please refer to the [Installing the connector](docs/creating_and_using_a_custom_camera_connector.md#installing-the-connector) and [Packaging the connector](docs/creating_and_using_a_custom_camera_connector.md#packaging-the-connector) sections.

Here's the revised section with a note about skipping step 1 when using Docker:

## Step-by-Step guide to building a connector
To use a custom camera connector in Vision Connector, you first need to prepare the corresponding package file. Once prepared, you can upload it to Vision Connector.

The following steps elaborate on preparing a package using the Vision Connector SDK and OpenCV as an example. However, users can adapt the process to suit their specific camera SDK or desired remote endpoint requirements.

1. **Prepare the Development Environment**  
    > **Note:** Skip this step if you're using the Docker container approach for building.
    
    - For local builds: Ensure all prerequisites are installed and set up the development environment as described in the [Development Environment Setup](docs/development_environment.md).
    - For Docker builds: Use the provided Docker container configuration to set up the environment.

2. **Understand the Vision Connector SDK**  
    - Review the [Vision Connector SDK Guide](docs/vca_sdk.md) to understand its features and usage.

3. **Create the Custom Camera Connector**  
    - Follow the [Custom Camera Connector Guide](docs/creating_and_using_a_custom_camera_connector.md) to implement your connector.
    - Integrate the camera vendor's API and ensure it is compatible with the Vision Connector.

4. **Build the Connector**  
    - **For local builds**: Use the utility scripts in `src/requirements` to install dependencies, then follow the [Preparing Package Structure and Installation](docs/creating_and_using_a_custom_camera_connector.md#preparing-package-structure-and-installation) section to build the connector.
    - **For Docker builds**: Use the Docker container to build the connector as per the instructions in the guide.

5. **Package the Connector**  
    - Package the connector following the steps in the [Packaging the Connector](docs/creating_and_using_a_custom_camera_connector.md#packaging-the-connector) section.

6. **Upload the Connector**  
    - Open the Vision Connector application and upload the packaged connector file.
    - Refer to the [Using the Connector in Vision Connector](docs/creating_and_using_a_custom_camera_connector.md#using-the-connector-in-vca) section for detailed steps.

7. **Test the Connector**  
    - Test the custom camera connector with the Vision Connector application.
    - Debug and refine the connector to ensure optimal performance.

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