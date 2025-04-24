# Vision Connector SDK

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
        * **[Locking policy](#locking-policy)**

## Overview
The Vision Connector SDK is a separately built library that enables users to create custom camera connectors for use with Vision Connector. The Vision Connector Drivers repository contains a pre-built version of this SDK with necessary header files under the ```sdk``` folder. Every custom camera connector package must include ```libVCA-SDK.so```, and the custom camera connector library must link to this .so file.

By default, CMake configuration and build scripts handle the custom camera connector package creation and linkage. For more information, see the [Creating and using a custom camera connector](creating_and_using_a_custom_camera_connector.md) section.

## Classes
### PluginLogger class
The SDK provides logging capabilities through the ```PluginLogger``` class. The ```PluginLogger``` can store registered callbacks with the actual logging implementation. Vision Connector uses this concept to register its own logging logic, but camera connectors can also register their own logging callbacks if needed.
If no callback is registered, a fallback logger writes to the standard output.

While the ```PluginLogger``` provides instance-based macros for different log levels, it's recommended to use the specific log macros provided by each camera connector's ```PluginLogger``` instance.

### Object and ObjectContainer classes
The SDK includes utility classes such as ```Object``` and ```ObjectContainer```. These classes provide a generalized way to handle data between the custom camera connector and the Vision Connector application and are not used individually.

*Essentially, the Object represents a JSON object.*

### CameraStatus class
Extends the ```ObjectContainer``` class to handle specified data for camera status. It is used in the ```Camera``` abstract class.

### CameraParameter class
Extends the ```ObjectContainer``` class to handle specified data for camera parameters.

Camera parameters can have the following fields:

 - **```Name```** - Parameter name
 - **```Description```** - Parameter description
 - **```Value```** - Parameter value
 - **```Min```** - Minimum value (if applicable)
 - **```Max```** - Maximum value (if applicable)
 - **```Increment```** - Expected value increment (if applicable)
 - **```Symbolics```** - Predefined value options (if applicable)
 - **```Type```** - Parameter type
 - **```ReadOnly```** - Parameter accessibility

The camera parameter handles each field as a string, independent of the original value data type. The original data type is defined by the ```type``` field:
 - ```bool```
 - ```int```
 - ```float```
 - ```string```
 - ```enum```

### CameraParameterStatus class
Extends the ObjectContainer class to handle specified data for camera parameter changes.

The status can be either **```OK```** or **```ERROR```** depending on the parameter change success.
For **```OK```** status, the value is the changed parameter value; for **```ERROR```**, it's the error message.

### Image class
The ```Image``` class manages image data retrieved from cameras. Vision Connector supports multipart images. An ```Image``` class instance can contain zero, one, or more image parts. By default, the image instance contains no image parts and doesn't take ownership of added image parts.

Image parts can include metadata, and custom fields can be added to the image instance itself.

### CameraException
Custom exception for camera-related errors.

## Abstract Classes
The custom camera connector's behavior is defined by reimplementing the ```CameraConnector``` and ```Camera``` abstract classes.

### CameraConnector class
Responsible for discovering corresponding cameras and creating unique ID-specific cameras. The unique ID is a special camera identifier used for connection.

#### Virtual methods
 - **```discover```**<br>
Returns the list of available cameras as unique IDs.

</br>

 - **```createCamera```**<br>
Creates an instance of the specified unique ID camera and returns it as a shared pointer.

### Camera class
Manages the underlying camera (handles connection, modifies settings, retrieves images). A ```Camera``` class instance can represent either a physical camera on the network or a simulated/software device.

Camera instances must be created through the ```CameraConnector``` specified by the unique ID.

#### Virtual methods
 - **```startImageAcquisition```**<br>
Handles connection to the underlying camera and prepares it for image acquisition.

</br>

 - **```stopImageAcquisition```**<br>
Handles underlying camera disconnection and frees resource access.

</br>

 - **```getStatus```**<br>
Returns ```ONLINE``` or ```OFFLINE``` based on the actual connection to the underlying camera and image retrieval state.

</br>

 - **```acquireImage```**<br>
Implements main logic for retrieving images from the actual camera. Called by higher-level components on a separate thread. Internal resource access management prevents race conditions.
Returns an ```Image``` instance as shared pointer.

</br>

 - **```getConfig```**<br>
Retrieves the underlying camera's actual configuration. Called by higher-level components on a separate thread. Internal resource access management prevents race conditions.
Returns the ```CameraParameters``` list.

</br>

 - **```setConfig```**<br>
Changes camera configuration according to given CameraParameters. Associates a CameraParameterStatus with each changed parameter. Internal resource access management prevents race conditions.
Returns the ```CameraParameterStatuses``` list.

</br>

 - **```setVersion```**<br>
Sets the camera connector's internal version, which becomes part of the camera configuration. Default value: <b>Not Defined</b>

#### Locking policy
The Camera class ensures thread-safe access to the underlying camera device by synchronizing ```acquireImage```, ```getConfig```, ```setConfig```, ```startImageAcquisition```, and ```stopImageAcquisition``` methods through the ```m_cameraAccessMutex``` mutex. The ```m_cameraAccessMutex``` MUST NOT be used in custom camera class implementation.
<br>

To ensure camera responsiveness, implementations of ```acquireImage```, ```getConfig```, ```setConfig```, ```startImageAcquisition```, and ```stopImageAcquisition``` methods MUST NOT contain active blocking logic.