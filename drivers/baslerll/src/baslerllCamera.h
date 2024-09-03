#pragma once
#include "Camera.h"
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>

using namespace Pylon;
using namespace Basler_GigECameraParams;
using namespace std;

typedef CBaslerGigECamera Camera_t;


namespace VCA::baslerll
{
    struct MyContext
    {
        // Define some application specific context information here
    };

    class baslerllCamera : public VCA::SDK::v1::Camera
    {
    public:
        explicit baslerllCamera() = delete;
        explicit baslerllCamera(const std::string& cameraType);
        ~baslerllCamera() override = default;
        SDK::v1::CameraStatus getStatus() const override;

    protected:
        std::shared_ptr<VCA::SDK::v1::Image> acquireImage() override;
        SDK::v1::CameraParameters getConfig() override;
        SDK::v1::CameraParameterStatuses setConfig(const SDK::v1::CameraParameters& parametersToChange) override;
        void startImageAcquisition() override;
        void stopImageAcquisition() override;

    private:
        void Open();
        void Close();        
        //private Variables
        IStreamGrabber* pGrabber;
        std::unique_ptr<Camera_t> camera;
        std::unique_ptr<CBaslerGigECamera::StreamGrabber_t> StreamGrabber;
        ITransportLayer *pTl;
        int numBuffers;
        int bufferSize;
        std::vector<MyContext> context;
        std::vector<StreamBufferHandle> handles;
        std::vector<unsigned char*> ppBuffers;
    };
} // namespace VCA::baslerllcd 