#include "CameraException.h"
#include "baslerllCamera.h"


namespace VCA::baslerll
{    
    std::string pixelTypeToString(EPixelType pixelType){
        switch (pixelType)
        {
            case PixelType_Undefined: return "Undefined";
            case PixelType_Mono1packed: return "Mono1packed";
            case PixelType_Mono2packed: return "Mono2packed";
            case PixelType_Mono4packed: return "Mono4packed";
            case PixelType_Mono8: return "Mono8";
            case PixelType_Mono8signed: return "Mono8signed";
            case PixelType_Mono10: return "Mono10";
            case PixelType_Mono10packed: return "Mono10packed";
            case PixelType_Mono10p: return "Mono10p";
            case PixelType_Mono12: return "Mono12";
            case PixelType_Mono12packed: return "Mono12packed";
            case PixelType_Mono12p: return "Mono12p";
            case PixelType_Mono16: return "Mono16";
            case PixelType_BayerGR8: return "BayerGR8";
            case PixelType_BayerRG8: return "BayerRG8";
            case PixelType_BayerGB8: return "BayerGB8";
            case PixelType_BayerBG8: return "BayerBG8";
            case PixelType_BayerGR10: return "BayerGR10";
            case PixelType_BayerRG10: return "BayerRG10";
            case PixelType_BayerGB10: return "BayerGB10";
            case PixelType_BayerBG10: return "BayerBG10";
            case PixelType_BayerGR12: return "BayerGR12";
            case PixelType_BayerRG12: return "BayerRG12";
            case PixelType_BayerGB12: return "BayerGB12";
            case PixelType_BayerBG12: return "BayerBG12";
            case PixelType_RGB8packed: return "RGB8packed";
            case PixelType_BGR8packed: return "BGR8packed";
            case PixelType_RGBA8packed: return "RGBA8packed";
            case PixelType_BGRA8packed: return "BGRA8packed";
            case PixelType_RGB10packed: return "RGB10packed";
            case PixelType_BGR10packed: return "BGR10packed";
            case PixelType_RGB12packed: return "RGB12packed";
            case PixelType_BGR12packed: return "BGR12packed";
            case PixelType_RGB16packed: return "RGB16packed";
            case PixelType_BGR10V1packed: return "BGR10V1packed";
            case PixelType_BGR10V2packed: return "BGR10V2packed";
            case PixelType_YUV411packed: return "YUV411packed";
            case PixelType_YUV422packed: return "YUV422packed";
            case PixelType_YUV444packed: return "YUV444packed";
            default: break;
        }
    return "Unknown";
}

    baslerllCamera::baslerllCamera(const std::string& cameraType) : Camera(cameraType)
    {
        m_version = "0.0.2";
    }

    std::shared_ptr<VCA::SDK::v1::Image> baslerllCamera::acquireImage()
    {
        auto image = std::make_shared<SDK::v1::Image>();
        if(StreamGrabber){
            GrabResult ptrGrabResult;
            if ( StreamGrabber->GetWaitObject().Wait( s_imageAcquisitionTimeout )) {
                StreamGrabber->RetrieveResult( ptrGrabResult );
                if ( ptrGrabResult.Succeeded() ) {
                    const VCA::SDK::v1::ImageDetail imageDetail(
                    VCA::SDK::v1::CameraInformation(cameraUniqueId(), "0"),
                    VCA::SDK::v1::ImageInformation(increaseImageSequenceCounter(), ptrGrabResult.GetSizeX(), ptrGrabResult.GetSizeY(), pixelTypeToString(ptrGrabResult.GetPixelType())),
                    VCA::SDK::v1::PTPInformation("Disabled", "0"));
                    auto imageData = std::make_unique<SDK::v1::ImageData>((unsigned char *)ptrGrabResult.Buffer(), ptrGrabResult.GetBufferSize());
                    image->addImagePart(std::move(imageData), imageDetail);
                } 
                // Requeue the buffer
                StreamGrabber->QueueBuffer( ptrGrabResult.Handle(), ptrGrabResult.Context() );
            }
        }
        return image;
    }

    void baslerllCamera::Open(){
        PylonInitialize();
        CTlFactory& TlFactory = CTlFactory::GetInstance();
        pTl = TlFactory.CreateTl( Camera_t::DeviceClass() );
        camera = std::make_unique<Camera_t>( pTl->CreateDevice( m_uniqueId.c_str() ) );
        camera->Open();
    }

    void baslerllCamera::startImageAcquisition() {
        Open();
        pGrabber = camera->GetStreamGrabber(0);
        StreamGrabber = std::make_unique<CBaslerGigECamera::StreamGrabber_t>(pGrabber);
        StreamGrabber->Open();
        bufferSize = (int) camera->PayloadSize();
        numBuffers = 10 ;
        StreamGrabber->MaxBufferSize = bufferSize;
        StreamGrabber->MaxNumBuffer = numBuffers;
        StreamGrabber->PrepareGrab();

        ppBuffers.resize(numBuffers);
        context.resize(numBuffers);
        handles.resize(numBuffers);
        for ( int i = 0; i < numBuffers; ++i )
        {
            ppBuffers[i] = new unsigned char[bufferSize];
            handles[i] = StreamGrabber->RegisterBuffer( ppBuffers[i], bufferSize);
        }
        for ( int i = 0; i < numBuffers; ++i ) {
            // Enqueue image buffers and use the buffer's index as context
            // information
            StreamGrabber->QueueBuffer( handles[i], & context[i] );
        }

        // Start image acquisition
        camera->AcquisitionStart.Execute();
    }

    void baslerllCamera::stopImageAcquisition() {
        if(camera){
            // The camera is in continuous mode, stop the image acquisition
            camera->AcquisitionStop.Execute();
            // Flush the input queue
            StreamGrabber->CancelGrab();
            StreamGrabber->StopStreamingIfMandatory();
            // Consume all items from the output queue
            GrabResult ptrGrabResult;
            while ( StreamGrabber->GetWaitObject().Wait(0) ) {
                StreamGrabber->RetrieveResult( ptrGrabResult );
                if ( ptrGrabResult.Status() == Canceled )
                cout << "Got canceled buffer" << endl;
            }
            StreamGrabber->FlushBuffersToOutput();        
            // Deregister and free buffers
            for ( int i = 0; i < numBuffers; ++i ) {
                StreamGrabber->DeregisterBuffer(handles[i]);
                delete [] ppBuffers[i];
            }

            // Clean up
            StreamGrabber->FinishGrab();
            StreamGrabber->Close();
            StreamGrabber.reset();
            Close();
        }
    }

    void baslerllCamera::Close(){        
        CTlFactory& TlFactory = CTlFactory::GetInstance();
        camera->Close();
        TlFactory.ReleaseTl( pTl );
        camera.reset();
        PylonTerminate();
    }

    SDK::v1::CameraParameters baslerllCamera::getConfig()
    {
        SDK::v1::CameraParameters cameraParameters;
        bool closed = camera == nullptr;
        if(closed){
            Open();
        }
        auto symbPrepare = [](StringList_t sl) {
            SDK::v1::Symbolics s;
            for(uint16_t i=0; i < sl.size();i++){
                s.push_back(sl[i].c_str());
            }
            return s;
        };

        GenApi::INodeMap *nodemap = camera->GetNodeMap();

        auto widthParam = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Int, 
                            "Width", 
                            to_string(camera->Width.GetValue()),
                            "Image width",
                            "1",
                            to_string(camera->WidthMax.GetValue()),
                            false, "1");
        cameraParameters.push_back(widthParam);

        auto heightParam = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Int, 
                            "Height", 
                            to_string(camera->Height.GetValue()),
                            "Image height",
                            "1",
                            to_string(camera->HeightMax.GetValue()),
                            false, "1");
        cameraParameters.push_back(heightParam);

        bool afrrate = CBooleanParameter(nodemap, "AcquisitionFrameRateEnable").GetValue();
        auto acfrEnabled = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Bool, 
                            "Acquition Frame Rate Enabled", 
                            afrrate ? "1":"0",
                            "Acquition Frame Rate Enabled",
                            "",
                            "",
                            false, "");
        cameraParameters.push_back(acfrEnabled);

        CFloatParameter(nodemap, "AcquisitionFrameRate").GetValue();

        auto acfr = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Float, 
                            "Acquisition Frame Rate", 
                            to_string(CFloatParameter(nodemap, "AcquisitionFrameRate").GetValue()),
                            "Acquisition Frame Rate",
                            "0",
                            "100",
                            false, "0.01");
        cameraParameters.push_back(acfr);

        
        auto en = CEnumParameter(nodemap, "TriggerMode");
        StringList_t sl;
        en.GetSymbolics(sl);

        auto triggerMode = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Enum, 
                            "Trigger Mode", 
                            std::string(sl.at(en.GetIntValue())),
                            "Trigger Mode",
                            "",
                            "",
                            false, "");

        triggerMode.setSymbolics(symbPrepare(sl));
        cameraParameters.push_back(triggerMode);

        auto lcm = CEnumParameter(nodemap, "BslLightControlMode");
        StringList_t slSelectedLightControl;
        en.GetSymbolics(slSelectedLightControl);

        std::string currentLightMode = std::string(slSelectedLightControl[lcm.GetIntValue()]);
        auto blsLightMode = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Enum, 
                            "BslLightControlMode", 
                            currentLightMode,
                            "BslLightControlMode",
                            "",
                            "",
                            false, "");

        blsLightMode.setSymbolics(symbPrepare(slSelectedLightControl));
        cameraParameters.push_back(blsLightMode);


        
        if(currentLightMode == "On"){
            CCommandParameter(nodemap, "BslLightControlEnumerateDevices").Execute();
            // Select light device 1

            auto ldsNode = CEnumParameter(nodemap, "BslLightDeviceSelector");
            StringList_t slLightControlSelector;
            ldsNode.GetSymbolics(slLightControlSelector);

            auto lds = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Enum, 
                                "BslLightDeviceSelector", 
                                std::string(slLightControlSelector.at(ldsNode.GetIntValue())),
                                "BslLightDeviceSelector",
                                "",
                                "",
                                false, "");

            lds.setSymbolics(symbPrepare(slLightControlSelector));
            cameraParameters.push_back(lds);

            // Set the nominal current of device 1 to 100 mA
            float current = CFloatParameter(nodemap, "BslLightDeviceCurrent").GetValue();
            auto ldc = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Float, 
                            "BslLightDeviceCurrent", 
                            to_string(current),
                            "BslLightDeviceCurrent",
                            "",
                            "",
                            false, "0.1");
           cameraParameters.push_back(ldc);

            // Set the brightness to 100 %
            float bright = CFloatParameter(nodemap, "BslLightDeviceBrightness").GetValue();
            auto ldb = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Float, 
                            "BslLightDeviceBrightness", 
                            to_string(bright),
                            "BslLightDeviceBrightness",
                            "",
                            "",
                            false, "0.1");
           cameraParameters.push_back(ldb);

            // Set the operation mode to Strobe
            auto ldomNode = CEnumParameter(nodemap, "BslLightDeviceOperationMode");
            StringList_t slLightDeviceOperationMode;
            ldomNode.GetSymbolics(slLightDeviceOperationMode);

            auto ldom = SDK::v1::CameraParameter::CreateParameter(SDK::v1::ParameterType::Enum, 
                                "BslLightDeviceOperationMode", 
                                std::string(slLightDeviceOperationMode.at(ldomNode.GetIntValue())),
                                "BslLightDeviceOperationMode",
                                "",
                                "",
                                false, "");

            ldom.setSymbolics(symbPrepare(slLightDeviceOperationMode));
            cameraParameters.push_back(ldom);
        }

        if(closed){
            Close();
        }
        //Push paramaters
        return cameraParameters;
    }

    SDK::v1::CameraParameterStatuses baslerllCamera::setConfig(const SDK::v1::CameraParameters& parametersToChange)
    {
        SDK::v1::CameraParameterStatuses parameterStatuses;
        bool closed = camera == nullptr;
        if(closed){
            Open();
        }
        GenApi::INodeMap *nodemap = camera->GetNodeMap();

        for (const auto& parameterToChange : parametersToChange)
        {
            const auto parameterName = parameterToChange.name();
            const auto parameterValue = parameterToChange.value();
            bool status = false;

            if(parameterName == "Width"){
                camera->Width.SetValue(std::stoi(parameterValue));
                status = true;
            }
            else if(parameterName == "Height"){
                camera->Height.SetValue(std::stoi(parameterValue));
                status = true;
            }
            else if(parameterName == "Acquition Frame Rate Enabled"){
                CBooleanParameter(nodemap, "AcquisitionFrameRateEnable").SetValue(parameterValue == "1");
                status = true;
            }
            else if(parameterName == "Acquisition Frame Rate"){
                CFloatParameter(nodemap, "AcquisitionFrameRate").SetValue(std::stof(parameterValue));
                status = true;
            }
            else if(parameterName == "Trigger Mode"){
                camera->TriggerMode.SetValue( parameterValue == "On" ? TriggerModeEnums::TriggerMode_On : TriggerModeEnums::TriggerMode_Off);
                status = true;
            }
            else if(parameterName == "BslLightControlMode"){
                status = CEnumParameter(nodemap, "BslLightControlMode").TrySetValue(parameterValue.c_str());
            }
            else if(parameterName == "BslLightDeviceSelector"){
                status = CEnumParameter(nodemap, "BslLightDeviceSelector").TrySetValue(parameterValue.c_str());
            }
            else if(parameterName == "BslLightDeviceCurrent"){
                status = CFloatParameter(nodemap, "BslLightDeviceCurrent").TrySetValue(std::stof(parameterValue));
            }
            else if(parameterName == "BslLightDeviceBrightness"){
                status = CFloatParameter(nodemap, "BslLightDeviceBrightness").TrySetValue(std::stof(parameterValue));
            }
            else if(parameterName == "BslLightDeviceOperationMode"){
                status = CEnumParameter(nodemap, "BslLightDeviceOperationMode").TrySetValue(parameterValue.c_str());
            }

            parameterStatuses.push_back(SDK::v1::CameraParameterStatus(parameterName, parameterValue,
                                        status ? SDK::v1::CameraParameterStatus::Status::OK : SDK::v1::CameraParameterStatus::Status::ERROR));
        }

        if(closed){
            Close();
        }
        return parameterStatuses;
    }

    SDK::v1::CameraStatus baslerllCamera::getStatus() const
    {
        const auto status =
            isConnected() ? SDK::v1::CameraStatus::Status::ONLINE : SDK::v1::CameraStatus::Status::OFFLINE;
        return SDK::v1::CameraStatus(status);
    }

} // namespace VCA::baslerll