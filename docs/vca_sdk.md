# VCA SDK

## Table of Contents
* **[Overview](#overview)**
* **[Classes](#classes)**
    * **[PluginLogger class](#pluginLogger-class)**
    * **[Object and ObjectContainer classes](#object-and-objectcontainer-classes)**
    * **[CameraStatus class](#camerastatus-class)**
    * **[CameraParameter class](#cameraparameter-class)**
    * **[CameraParameterStatus class](#cameraparameterstatus-class)**
    * **[Image class](#image-class)**
    * **[CameraException](#cameraexception)**
* **[Abstract Classes](#abstract-classes)**
    * **[CameraConnector class](#cameraconnector-class)**
        * **[Virtual methods](#virtual-methods)**
    * **[Camera class](#camera-class)**
        * **[Virtual methods](#virtual-methods-1)**

## Overview
The VCA SDK is a separately built library which allows users to create their own camera connectors and use them with VCA. The VCA Drivers repository contains a pre-built version of this SDK with the necessary header files under the ```sdk``` folder. Every custom camera connector package has to contain the ```libVCA-SDK.so``` and the custom camera conenctor library has to link to this .so file.

By default the custom camera connector package creation and linkage is handled by CMake configuration and build scripts. More information about this in the [Creating and using a custom camera connector](creating_and_using_a_custom_camera_connector.md) section.

## Classes
### PluginLogger class
The SDK provides logging capabilities through the ```PluginLogger``` class. The ```PluginLogger``` can store registered callbacks with the actual logging implementation. VCA uses this concept to register its own logging logic but the camera connector can also register its own logging callback if needed.
If there is no registered callback then a fallback logger is used which writes to the standard output.

While the ```PluginLogger``` provides instance based macros for the different log levels it is not recommended to use them on their own. Each camera connector has its own ```PluginLogger``` instance and its specific log macros. Use these specific macros.


### Object and ObjectContainer classes
The SDK contains some utility classes such as the ```Object``` and the ```ObjectContainer```. These classes provide a generalized way to handle data between the custom camera connector and the VCA application and are not used individually.

*Essentially the Object represents a JSON object.*

### CameraStatus class
Extends the ```ObjectContainer``` class to handle specificated data for camera status. It is used in the ```Camera``` abstract class.

### CameraParameter class
Extends the ```ObjectContainer``` class to handle specificated data for camera parameters.

The camera parameter can have the following fields:

 - **```Name```** - The name of the parameter
 - **```Description```** - Description for the parameter
 - **```Value```** - Value of the parameter
 - **```Min```** - Minimum value of the parameter (if applicable)
 - **```Max```** - Maximum value of the parameter (if applicable)
 - **```Increment```** - Expected value increment (if applicable)
 - **```Symbolics```** - Predefined value options (if applicable)
 - **```Type```** - Type of the parameter
 - **```ReadOnly```** - Accessibility of the parameter


The camera parameter handles each field as string independent from the original value data type. The original data type is defined by the ```type``` field:
 - ```bool```
 - ```int```
 - ```float```
 - ```string```
 - ```enum```

### CameraParameterStatus class
Extends the ObjectContainer class to handle specificated data for camera parameter changes.

Depending on the success of the parameter change the status can be either **```OK```** or **```ERROR```**.
If the status is **```OK```** then the value is the changed parameter value otherwise the error message.

### Image class
The ```Image``` class is responsible for managing the image data retrieved from the cameras. VCA supports multipart images. An instance of the ```Image``` class consist of no, one or more image parts. By default the image instance does not contain any image part. The ```Image``` instance does not take over of the ownership of added image parts.

It is also possible to add metadata to each image part and custom fields to the image instance itself.

### CameraException
Custom exception for camera related errors

## Abstract Classes
The actual behaviour of the custom camera connector is defined by the re-implementation of the ```CameraConnector``` and ```Camera``` abstract classes.

### CameraConnector class
This class is responsible for the discovery of corresponding cameras and the unique ID specific camera creation.

#### Virtual methods
 - **```discover```**<br>
The ```discover``` method has to return the list of available cameras as unique IDs.

</br>

 - **```createCamera```**<br>
The ```createCamera``` method has to create an instance of the unique ID specified camera and returns it as shared pointer.

### Camera class
This class is responsible for managing the underlying  camera (handling connection, modifying settings, retriving images). An instance of the ```Camera``` class can represent either a real/physical camera on the network or a simulated/software device.

A camera instance has to be created through the ```CameraConnector``` specified by the unique ID.

#### Virtual methods
 - **```startImageAcquisition```**<br>
Has to handle connection to the underlying camera and prepare it for image acqusition.

</br>

 - **```stopImageAcquisition```**<br>
Has to handle the disconnection of the underlying camera and free resource access.

</br>

 - **```getStatus```**<br>
Has to return ```ONLINE``` or ```OFFLINE``` depending on the actual connection to the underlying camera and the state of the image retrieval.

</br>

 - **```acquireImage```**<br>
Main logic for retrieving images from the actual camera. This method is called by higher level components on a separate thread. To avoid race condition resource access management is handled internally.
Has to return an instance of ```Image``` as shared pointer.

</br>

 - **```getConfig```**<br>
Retrieves the actual configuration of the underlying camera. This method is called by higher level components on a separate thread. To avoid race condition resource access management is handled internally.
Has to return the list ```CameraParameters```.

</br>

 - **```setConfig```**<br>
Changes the configuration of the camera according to the parameter given CameraParameters. A CameraParameterStatus belongs to each changed camera parameter. To avoid race condition resource access management is handled internally.
Has to return the list ```CameraParameterStatuses```.

</br>

 - **```setVersion```**<br>
Sets the camera connector's internal version which then becomes part of the camera configuration. Default value: <b>Not Defined</b>