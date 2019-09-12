#include "OpenHMI_WindowSystem.h"

#ifdef OS_WIN

#endif

#ifdef OS_QNX
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <string.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
#endif

#ifdef OS_LINUX
    #include <fcntl.h>
    #include <unistd.h>

    #ifdef IVI_LAYERMANAGER
        #include <ilm/ilm_client.h>
        #include <ilm/ilm_control.h>
    #endif
#endif

#ifdef OS_ANDROID
    #include <fcntl.h>
    #include <unistd.h>

    #include <errno.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/resource.h>

    #ifdef TECHNIQUE_SURFACEFLINGER
        #include <gui/SurfaceComposerClient.h>
        #include <gui/ISurfaceComposer.h>
        #include <gui/Surface.h>
        #include <ui/DisplayInfo.h>
        #include <utils/Log.h>
        #include <GLES/gl.h>
        #include <EGL/egl.h>
        #include <linux/input.h>

        #define TOUCH_LOG_TAG        "NativeInputManager"
        #define TOUCH_MAX_POINT        16
        
        #define test_bit(bit, array)    ((array)[(bit)/8] & (1<<((bit)%8)))
        
        namespace android
        {
            // ===================== TOUCH ========================
            //
            const int32_t NativeInputManager::LocalInputListener::RAW_X_MIN = 0;
            const int32_t NativeInputManager::LocalInputListener::RAW_X_MAX = 1919;
            const int32_t NativeInputManager::LocalInputListener::RAW_Y_MIN = 0;
            const int32_t NativeInputManager::LocalInputListener::RAW_Y_MAX = 719;
            const int32_t NativeInputManager::LocalInputListener::RAW_TOUCH_MIN = 0;
            const int32_t NativeInputManager::LocalInputListener::RAW_TOUCH_MAX = 255;
            const TouchAffineTransformation NativeInputManager::LocalInputListener::AFFINE_TRANSFORM = TouchAffineTransformation(1, 0, 0, 0, 1, 0);
#define DISPLAY_WIDTH 1920
#define DISPLAY_HEIGHT 720
            
            NativeInputManager::LocalInputListener::LocalInputListener(NativeInputListener* listener) : mNativeInputListener(listener) {}

            NativeInputManager::LocalInputListener::~LocalInputListener() {}

            void NativeInputManager::LocalInputListener::notifyConfigurationChanged(const NotifyConfigurationChangedArgs* args)
            {
                ALOGD("notifyConfigurationChanged\n");
                if (mNativeInputListener != NULL) {
                    // mNativeInputListener->notifyConfigurationChanged(args);
                }
            }

            void NativeInputManager::LocalInputListener::notifyDeviceReset(const NotifyDeviceResetArgs* args) 
            {
                ALOGD("notifyDeviceReset\n");
                if (mNativeInputListener != NULL) {
                    // mNativeInputListener->notifyDeviceReset(args);
                }
            }

            void NativeInputManager::LocalInputListener::notifyKey(const NotifyKeyArgs* args) 
            {
                ALOGD("notifyKey\n");
                if (mNativeInputListener != NULL) {
                    // mNativeInputListener->notifyKey(args);
                }
            }

            void NativeInputManager::LocalInputListener::notifyMotion(const NotifyMotionArgs* args)
            {
                ALOGD( "notifyMotion action = %d, id: %d, display_id: %d\n", args->action, args->deviceId, args->displayId );
                
                Coordinate pointer[TOUCH_MAX_POINT];
                for ( uint32_t ui=0; ui < args->pointerCount; ++ui ) {
                    float fx = args->pointerCoords[ui].getX();
                    float fy = args->pointerCoords[ui].getY();
                    
                    pointer[ui].fX = toDisplayX( toRawX( fx ), DISPLAY_WIDTH );
                    pointer[ui].fY = toDisplayY( toRawY( fy ), DISPLAY_HEIGHT );

                    pointer[ui].iId = args->deviceId;
                }

                if ( mNativeInputListener != NULL && args->pointerCount > 0 ) {
                    mNativeInputListener->notifyMotion(args->eventTime, args->pointerCount, pointer, static_cast<NativeInputListener_Motion_Type>(args->action));
                }
            }

            void NativeInputManager::LocalInputListener::notifySwitch(const NotifySwitchArgs* args) 
            {
                ALOGD("notifySwitch\n");
                if (mNativeInputListener != NULL) 
                {
                    //mNativeInputListener->notifySwitch(args);
                }
            }
            
            int32_t NativeInputManager::LocalInputListener::toRawX(float displayX)
            {
                return int32_t(displayX * (RAW_X_MAX - RAW_X_MIN + 1) / DISPLAY_WIDTH + RAW_X_MIN);
            }
            
            int32_t NativeInputManager::LocalInputListener::toRawY(float displayY)
            {
                return int32_t(displayY * (RAW_Y_MAX - RAW_Y_MIN + 1) / DISPLAY_HEIGHT + RAW_Y_MIN);
            }

            float NativeInputManager::LocalInputListener::toCookedX(float rawX, float rawY)
            {
                    AFFINE_TRANSFORM.applyTo(rawX, rawY);
                return rawX;
            }

            float NativeInputManager::LocalInputListener::toCookedY(float rawX, float rawY)
            {
                AFFINE_TRANSFORM.applyTo(rawX, rawY);
                return rawY;
            }

            float NativeInputManager::LocalInputListener::toDisplayX(int32_t rawX, int32_t displayWidth)
            {
                return float(rawX - RAW_X_MIN) * displayWidth / (RAW_X_MAX - RAW_X_MIN + 1);
            }

            float NativeInputManager::LocalInputListener::toDisplayY(int32_t rawY, int32_t displayHeight)
            {
                return float(rawY - RAW_Y_MIN) * displayHeight / (RAW_Y_MAX - RAW_Y_MIN + 1);
            }

            NativeInputManager::NativePointerController::NativePointerController()
                : mHaveBounds(false) 
                , mMinX(0), mMinY(0)
                , mMaxX(0), mMaxY(0)
                , mX(0), mY(0)
                , mButtonState(0) 
            {
                ;
            }

            NativeInputManager::NativePointerController::~NativePointerController() 
            {
                ;
            }

            void NativeInputManager::NativePointerController::setBounds(float minX, float minY, float maxX, float maxY) 
            {
                mHaveBounds = true;
                mMinX = minX;
                mMinY = minY;
                mMaxX = maxX;
                mMaxY = maxY;
            }

            void NativeInputManager::NativePointerController::setPosition(float x, float y) 
            {
                mX = x;
                mY = y;
            }

            void NativeInputManager::NativePointerController::setButtonState(int32_t buttonState) 
            {
                mButtonState = buttonState;
            }

            int32_t NativeInputManager::NativePointerController::getButtonState() const 
            {
                return mButtonState;
            }

            void NativeInputManager::NativePointerController::getPosition(float* outX, float* outY) const 
            {
                *outX = mX;
                *outY = mY;
            }

            bool NativeInputManager::NativePointerController::getBounds(float* outMinX, float* outMinY, float* outMaxX, float* outMaxY) const 
            {
                *outMinX = mMinX;
                *outMinY = mMinY;
                *outMaxX = mMaxX;
                *outMaxY = mMaxY;
                return mHaveBounds;
            }

            void NativeInputManager::NativePointerController::move(float deltaX, float deltaY) 
            {
                mX += deltaX;
                if (mX < mMinX) mX = mMinX;
                if (mX > mMaxX) mX = mMaxX;
                mY += deltaY;
                if (mY < mMinY) mY = mMinY;
                if (mY > mMaxY) mY = mMaxY;
            }

            void NativeInputManager::NativePointerController::fade(Transition) 
            {
                ;
            }

            void NativeInputManager::NativePointerController::unfade(Transition) 
            {
                ;
            }

            void NativeInputManager::NativePointerController::setPresentation(Presentation) 
            {
                ;
            }

            void NativeInputManager::NativePointerController::setSpots(const PointerCoords*, const uint32_t*, BitSet32) 
            {
                ;
            }

            void NativeInputManager::NativePointerController::clearSpots() 
            {
                ;
            }

            NativeInputManager::NativeInputReaderPolicy::NativeInputReaderPolicy() 
            {
                ALOGD("NativeInputReaderPolicy\n");
                sp<IBinder> display(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
                DisplayInfo info;
                SurfaceComposerClient::getDisplayInfo(display, &info);

                externalViewport.displayId         = 0;
                externalViewport.orientation     = 0;
                externalViewport.logicalLeft     = 0;
                externalViewport.logicalTop     = 0;
                externalViewport.logicalRight     = info.w;
                externalViewport.logicalBottom     = info.h;
                externalViewport.physicalLeft     = 0;
                externalViewport.physicalTop    = 0;
                externalViewport.physicalRight     = info.w;
                externalViewport.physicalBottom = info.h;
                externalViewport.deviceWidth     = info.w;
                externalViewport.deviceHeight     = info.h;
                externalViewport.uniqueId         = String8::empty();

                mConfig.excludedDeviceNames.clear();
                mConfig.pointerGesturesEnabled = true;
                mConfig.showTouches = false;
                mConfig.setPhysicalDisplayViewport( ViewportType::VIEWPORT_INTERNAL, externalViewport );
                mConfig.setPhysicalDisplayViewport( ViewportType::VIEWPORT_EXTERNAL, externalViewport );

                mPointerController = new NativePointerController();

                ALOGD( String8::format( "w: %d, h: %d\n", info.w, info.h ) );
            }

            NativeInputManager::NativeInputReaderPolicy::~NativeInputReaderPolicy() 
            {
                ;
            }

            void NativeInputManager::NativeInputReaderPolicy::getReaderConfiguration(InputReaderConfiguration* outConfig) 
            { 
                *outConfig = mConfig;
            }

            sp<PointerControllerInterface> NativeInputManager::NativeInputReaderPolicy::obtainPointerController(int32_t deviceId) 
            {
                return mPointerController;
            }

            void NativeInputManager::NativeInputReaderPolicy::notifyInputDevicesChanged(const Vector<InputDeviceInfo>& inputDevices) 
            {
                ALOGD("notifyInputDevicesChanged\n");

                for(auto& device: inputDevices) {
                    ALOGD(device.getIdentifier().name.string());                
                    ALOGD(device.getIdentifier().descriptor.string());                
                }
                mInputDevices = inputDevices;
            }

            sp<KeyCharacterMap> NativeInputManager::NativeInputReaderPolicy::getKeyboardLayoutOverlay(const InputDeviceIdentifier& identifier) 
            {
                return NULL;
            }

            String8 NativeInputManager::NativeInputReaderPolicy::getDeviceAlias(const InputDeviceIdentifier& identifier) 
            {
                return String8::empty();
            }

            TouchAffineTransformation NativeInputManager::NativeInputReaderPolicy::getTouchAffineTransformation( const String8& inputDeviceDescriptor, int32_t surfaceRotation ) 
            {
                return LocalInputListener::AFFINE_TRANSFORM;
            }

            InputDevice* NativeInputManager::NativeInputReader::createDeviceLocked( int32_t deviceId, int32_t controllerNumber, const InputDeviceIdentifier& identifier, uint32_t classes )
            {
                InputDevice *device = InputReader::createDeviceLocked(deviceId, controllerNumber, identifier, classes);
                
                if ( classes & INPUT_DEVICE_CLASS_CURSOR  ) 
                {
                    ALOGD("skip mouse device\n");
                    device->setEnabled( FALSE, systemTime() );    
                }
                return device;
            }

            NativeInputManager::NativeInputManager(NativeInputListener* listener) 
                : mNativeInputListener(listener) 
            {
                ALOGD("NativeInputManager construct");
                mEventHub = new EventHub();
                mLocalInputListener = new LocalInputListener(mNativeInputListener);
                mNativeInputReaderPolicy = new NativeInputReaderPolicy();
                mReader = new NativeInputReader( mEventHub, mNativeInputReaderPolicy, mLocalInputListener );
                mReaderThread = new InputReaderThread( mReader );
            }

            NativeInputManager::~NativeInputManager()
            {
                ;
            }

            status_t NativeInputManager::start()
            {
                status_t result = mReaderThread->run( "OpenHMI_InputReader", PRIORITY_URGENT_DISPLAY );
                if (result) {
                    ALOGE("Could not start InputReader thread due to error %d.", result);
                    return result;
                }

                return OK;
            }

            status_t NativeInputManager::stop() 
            {
                status_t result = mReaderThread->requestExitAndWait();

                if (result) {
                    ALOGW("Could not stop InputDispatcher thread due to error %d.", result);
                }

                return OK;
            }

            NativeInputManager* TOUCH::s_pReader = NULL;

            void TOUCH::nativeInputReaderSetListener(NativeInputListener* nativeInputListener) 
            {
                s_pReader = new NativeInputManager( nativeInputListener );
            }

            void TOUCH::nativeInputReaderStart()
            {
                if ( s_pReader != NULL ) {
                    s_pReader->start();
                }
            }

            void TOUCH::nativeInputReaderStop() 
            {
                if ( s_pReader != NULL ) {
                    s_pReader->stop();
                }
            }

            void TOUCH::nativeInputReaderClear() 
            {
                if ( s_pReader != NULL ) {
                    //s_pReader->stop();
                    delete s_pReader;
                    s_pReader = NULL;
                }
            }

            // ====================== WSF =========================
            //
            WindowSurface::WindowSurface(float x, float y, uint32_t w, uint32_t h, uint32_t layer, const char* name) 
            {
                status_t err;

                sp<SurfaceComposerClient> surfaceComposerClient = new SurfaceComposerClient;
                err = surfaceComposerClient->initCheck();
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposerClient::initCheck error: %#x\n", err);
                    return;
                }

                // Get main display parameters.
                sp<IBinder> mainDpy = SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain);
                DisplayInfo mainDpyInfo;
                err = SurfaceComposerClient::getDisplayInfo(mainDpy, &mainDpyInfo);
                if (err != NO_ERROR) {
                    fprintf(stderr, "ERROR: unable to get display characteristics\n");
                    return;
                }

                if (mainDpyInfo.orientation != DISPLAY_ORIENTATION_0 &&
                    mainDpyInfo.orientation != DISPLAY_ORIENTATION_180) {
                        // rotated
                        displayWidth = mainDpyInfo.h;
                        displayHeight = mainDpyInfo.w;
                } else {
                    displayWidth = mainDpyInfo.w;
                    displayHeight = mainDpyInfo.h;
                }

                // Default to full-screen size
                if (w != 0) {
                    width = w;
                } else {
                    width = displayWidth;
                }
                if (h != 0) {
                    height = h;
                } else {
                    height = displayHeight;
                }

                sp<SurfaceControl> sc = surfaceComposerClient->createSurface(
                    String8(name), width, height,
                    PIXEL_FORMAT_RGBX_8888, 0);
                if (sc == NULL || !sc->isValid()) {
                    fprintf(stderr, "Failed to create SurfaceControl\n");
                    return;
                }

                SurfaceComposerClient::openGlobalTransaction();
                err = sc->setLayer( layer );     // default on top
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::setLayer error: %#x\n", err);
                    return;
                }

                if ((x != 0) || (y != 0)) {
                    err = sc->setPosition(x, y);
                    if (err != NO_ERROR) {
                        fprintf(stderr, "SurfaceComposer::setPosition error: %#x\n", err);
                        return;
                    }
                }

                err = sc->show();
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::show error: %#x\n", err);
                    return;
                }
                SurfaceComposerClient::closeGlobalTransaction();

                mSurfaceControl = sc;
            }

            EGLNativeWindowType WindowSurface::getSurface() const {
                sp<ANativeWindow> anw = mSurfaceControl->getSurface();
                return (EGLNativeWindowType) anw.get();
            }

            WindowSurface::~WindowSurface() {
                mSurfaceControl = NULL;
            }

            void WindowSurface::setAlpha(float alpha) {
                SurfaceComposerClient::openGlobalTransaction();
                status_t err;

                err = mSurfaceControl->setAlpha(alpha);
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::setAlpha error: %#x\n", err);
                }

                SurfaceComposerClient::closeGlobalTransaction();
            }

            void WindowSurface::setLayer(uint32_t layer) {
                SurfaceComposerClient::openGlobalTransaction();
                status_t err;

                err = mSurfaceControl->setLayer(layer);
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::setLayer error: %#x\n", err);
                }

                SurfaceComposerClient::closeGlobalTransaction();
            }

            void WindowSurface::setVisible(bool visible) {
                SurfaceComposerClient::openGlobalTransaction();
                status_t err;

                if (visible) {
                    err = mSurfaceControl->show();
                } else {
                    err = mSurfaceControl->hide();
                }

                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::show/hide error: %#x\n", err);
                }

                SurfaceComposerClient::closeGlobalTransaction();
            }

            void WindowSurface::resize(uint32_t w, uint32_t h) {
                /// Default to full-screen size
                if (w == 0) {
                    w = displayWidth;
                }

                if (h == 0) {
                    h = displayHeight;
                }

                SurfaceComposerClient::openGlobalTransaction();
                status_t err;

                err = mSurfaceControl->setSize(w, h);
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::setSize error: %#x\n", err);
                } else {
                    width = w;
                    height = h;
                }

                SurfaceComposerClient::closeGlobalTransaction();
            }

            void WindowSurface::setPosition(float x, float y) {
                SurfaceComposerClient::openGlobalTransaction();
                status_t err;

                err = mSurfaceControl->setPosition(x, y);
                if (err != NO_ERROR) {
                    fprintf(stderr, "SurfaceComposer::setPosition error: %#x\n", err);
                }

                SurfaceComposerClient::closeGlobalTransaction();
            }

            unsigned WSF::wsfGetVersion ()
            {
                return (unsigned)WSF_VERSION;
            }

            WSFHandle WSF::wsfCreateDisplaySurface (float x, float y, unsigned w, unsigned h, unsigned layer, const char* name)
            {
                android::WindowSurface* surface = new android::WindowSurface(x, y, (uint32_t)w, (uint32_t)h, (uint32_t)layer, name);
                WSFHandle handle = static_cast<WSFHandle>(surface);
                return handle;
            }

            /* Full-screen android_createDisplaySurface() is gone since 6.0.0 */
            EGLNativeWindowType WSF::android_createDisplaySurface (void)
            {
                static WSFHandle fullscreenHandle = wsfCreateDisplaySurface(0, 0, 0, 0, 0, "fullscreen");
                return wsfGetEGLNativeWindow(fullscreenHandle);
            }

            EGLNativeWindowType WSF::wsfGetEGLNativeWindow (WSFHandle handle)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                return surface->getSurface();
            }

            void WSF::wsfDestroySurface (WSFHandle handle)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                delete surface;
            }

            void WSF::wsfSetAlpha (WSFHandle handle, float alpha)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                surface->setAlpha(alpha);
            }

            void WSF::wsfSetLayer (WSFHandle handle, unsigned layer)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                surface->setLayer(layer);
            }

            void WSF::wsfSetVisible (WSFHandle handle, int visible)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                surface->setVisible(visible ? true : false);
            }

            void WSF::wsfResize (WSFHandle handle, unsigned w, unsigned h)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                surface->resize((uint32_t)w, (uint32_t)h);
            }

            void WSF::wsfSetPosition (WSFHandle handle, float x, float y)
            {
                android::WindowSurface* surface = static_cast<android::WindowSurface*>(handle);
                surface->setPosition(x, y);
            }
        }
    #endif
#endif

namespace OpenHMI
{
#define    WINDOW_CLASS        L"OpenHMIClass"
#define WINDOW_TITLE        L"OpenHMI - "

#ifdef OS_QNX
    #define WINDOW_GROUP(LayID)        #LayID
#endif

#ifdef OS_LINUX
    #ifdef TECHNIQUE_WAYLAND
        static INT     g_iX = 0;
        static INT     g_iY = 0;
        static UINT    g_uiID = 0;
        static BOOLEAN g_bEnterButton = FALSE;

        struct tagWaylandContext
        {
            struct wl_compositor    *wlCompositor;
            struct wl_subcompositor *wlSubCompositor;
            struct wl_surface       *wlSurface;
            struct wl_surface       *wlParentSurface;
            struct wl_subsurface    *wlSubSurface;
            struct wl_registry      *wlRegistry;
            struct wl_shell         *wlShell;
            struct wl_shell_surface *wlShellSurface;
            struct wl_seat          *wlSeat;
            struct wl_pointer       *wlPointer;
            struct wl_keyboard      *wlKeyboard;
            struct wl_touch         *wlTouch;
    #ifdef IVI_LAYERMANAGER
            t_ilm_surface           ilmSurfaceId;
            t_ilm_layer             ilmLayerId;
    #endif

            tagWaylandContext()
            {
                reset();
            }

            void reset()
            {
                wlCompositor    = NULL;
                wlSubCompositor = NULL;
                wlSurface       = NULL;
                wlParentSurface = NULL;
                wlSubSurface    = NULL;
                wlRegistry      = NULL;
                wlShell         = NULL;
                wlShellSurface  = NULL;
                wlSeat          = NULL;
                wlPointer       = NULL;
                wlKeyboard      = NULL;
                wlTouch         = NULL;
    #ifdef IVI_LAYERMANAGER
                ilmSurfaceId    = 0;
                ilmLayerId      = 0;
    #endif
            }
        };
    #endif // TECHNIQUE_WAYLAND
#endif // OS_LINUX

///////////////////////////////////////////////////////////////////////////////////////
    tagScreenContext::tagScreenContext()
    {
        reset();
    }

    void tagScreenContext::reset()
    {
        uiScreenWidth = 0;
        uiScreenHeight = 0;

#ifdef OS_WIN
        hWnd = NULL;
        hDC = NULL;
#endif

#ifdef OS_QNX
        screen_ctx = NULL;
        screen_win = NULL;
        pScreen_dispaly = NULL;
#endif

#ifdef OS_LINUX
    #ifdef TECHNIQUE_X11
        pDisplay = NULL;
        win = 0;
        gc = 0;
    #endif

    #ifdef TECHNIQUE_FB

    #endif

    #ifdef TECHNIQUE_WAYLAND
        wlDisplay = NULL;
        wlEglWindow = NULL;
    #endif
#endif

#ifdef OS_ANDROID
    #ifdef TECHNIQUE_ACTIVITY
        win = NULL;
    #endif

    #ifdef TECHNIQUE_SURFACEFLINGER
        win = NULL;
    #endif
#endif
    }

    BOOLEAN    tagScreenContext::operator == (const tagScreenContext &src) const
    {
        if (uiScreenWidth != src.uiScreenWidth || uiScreenHeight != src.uiScreenHeight)
        {
            return FALSE;
        }

#ifdef OS_WIN
        if (hWnd != src.hWnd || hDC != src.hDC)
        {
            return FALSE;
        }
#endif

#ifdef OS_QNX
        if (screen_ctx != src.screen_ctx || screen_win != src.screen_win || pScreen_dispaly != src.pScreen_dispaly)
        {
            return FALSE;
        }
#endif

#ifdef OS_LINUX
    #ifdef TECHNIQUE_X11
        if (pDisplay != src.pDisplay || win != src.win || gc != src.gc)
        {
            return FALSE;
        }
    #endif

    #ifdef TECHNIQUE_FB

    #endif

    #ifdef TECHNIQUE_WAYLAND
        if (wlDisplay != src.wlDisplay || wlEglWindow != src.wlEglWindow)
        {
            return FALSE;
        }
    #endif
#endif

#ifdef OS_ANDROID
    #ifdef TECHNIQUE_ACTIVITY
        if (win != src.win)
        {
            return FALSE;
        }
    #endif

    #ifdef TECHNIQUE_SURFACEFLINGER
        if (win != src.win)
        {
            return FALSE;
        }
    #endif
#endif

        return TRUE;
    }

    BOOLEAN    tagScreenContext::operator != (const tagScreenContext &src) const
    {
        return ! (*this == src);
    }

///////////////////////////////////////////////////////////////////////////////////////
    CScreenTouchServer::CTuioClient::CTuioClient(CScreenTouchServer *pServer, const String& strHost, UINT uiPort)
        : CSocketClient(strHost, uiPort, TRUE, 256)
        , m_pServer(pServer)
        , m_aryBuffer(0, 256)
    {
        ;
    }

    FLOAT CScreenTouchServer::CTuioClient::convertFloat(FLOAT fValue)
    {
        union
        {
            float    f;
            BYTE    abt[4];
        }u;

        BYTE *pbt = (BYTE*)&fValue;

        u.abt[0] = pbt[3];
        u.abt[1] = pbt[2];
        u.abt[2] = pbt[1];
        u.abt[3] = pbt[0];

        return u.f;
    }

    void CScreenTouchServer::CTuioClient::onDataArrived(BYTE * pbtData, INT iDataSize)
    {
        INT iSize = iDataSize;
        INT iPosition = 0;

        m_aryBuffer.writeBuffer(pbtData, (UINT)iSize);
        m_aryBuffer.setPosition(0);

        INT iFlag = 1;
        BYTE * pbtVar = (BYTE *)&iFlag;

        this->sendData(pbtVar, sizeof(INT));

        while (iSize > 0)
        {
            INT iTag = (INT)m_aryBuffer.readBYTE();
            switch (iTag)
            {
            case 115://'s': compact TUIO message "set"
                {
                    if (iSize < 25)
                    {
                        iSize = 0;
                    }
                    else
                    {
                        iPosition += 25 ;
                        iSize -= 25;

                        UINT uiId = ntohl(m_aryBuffer.readUINT());
                        INT iX = (INT)(convertFloat(m_aryBuffer.readFLOAT()) * m_pServer->m_context.uiScreenWidth);
                        INT iY = (INT)(convertFloat(m_aryBuffer.readFLOAT()) * m_pServer->m_context.uiScreenHeight);
                        m_aryBuffer.readFLOAT(); // XSpeed, not be used.
                        m_aryBuffer.readFLOAT(); // YSpeed, not be used.
                        m_aryBuffer.readFLOAT(); //consume

                        if (m_pServer->m_cursorManager.isExist(uiId))
                        {
                            m_pServer->m_cursorManager.updateCursor(uiId, iX, iY);
                        }
                        else
                        {
                            m_pServer->m_cursorManager.addCursor(uiId, iX, iY);
                        }
                    }
                }
                break;

            case 97://'a': compact TUIO message "alive"
                {
                    if (iSize < 5)
                    {
                        iSize = 0;
                    }
                    else
                    {
                        INT iAlive_Amount = ntohl(m_aryBuffer.readINT());
                        INT iMsg_Size = iAlive_Amount * 4 + 5;
                        if (iSize < iMsg_Size)
                        {
                            iSize = 0;
                        }
                        else
                        {
                            iPosition += iMsg_Size;
                            iSize -= iMsg_Size;

                            Array2<UINT> aryAliveId;
                            while (iAlive_Amount > 0)
                            {
                                --iAlive_Amount;
                                INT iAlive_id = ntohl(m_aryBuffer.readINT()); //INT iAlive_id = *(INT *)acTemp;
                                aryAliveId.addItem((UINT)iAlive_id);
                            }

                            m_pServer->m_cursorManager.removeCursor(aryAliveId);
                        }
                    }
                }
                break;

            case 35://'#': OSC bundle
            case 47://'/': OSC message
                {
                    iPosition = m_aryBuffer.getPosition();
                    iSize = 0;
                }
                break;

            default://something wrong
                {
                    iPosition = iSize ;
                    iSize = 0;
                }
                break;
            }
        }

        m_aryBuffer.setPosition(iPosition);
        if (m_aryBuffer.bytesAvailable() <= 0)
        {
            m_aryBuffer.clear();
        }
        else
        {
            // Abandon the processed data.
            // Cleanup every time is low performance, need improve!
            INT iValidSize = m_aryBuffer.bytesAvailable();
            ByteArray aryTmp(iValidSize);
            m_aryBuffer.readBuffer(aryTmp.buffer(), iValidSize);
            m_aryBuffer.clear();
            m_aryBuffer.writeBuffer(aryTmp.buffer(), iValidSize);
        }
    }

///////////////////////////////////////////////////////////////////////////////////////

    CScreenTouchServer::CSimulationClient::CSimulationClient(CScreenTouchServer *pServer, const String& strHost, UINT uiPort)
        : CSocketClient(strHost, uiPort, TRUE, 256)
        , m_pServer(pServer)
        , m_bActiveSimulator(FALSE)
        , m_bRequestFeedback(FALSE)
        , m_bRequestScreenSnapshot(FALSE)
        , m_aryBuffer(0, 256)
        , m_uiSerialNumber(0)
    {
        ;
    }

    #define REP_DATA_TAG                0x4E455647
    #define REP_TYPE_RENDERTREE            0
    #define REP_TYPE_SNAPSHORT            1
    #define REP_TYPE_DBG_LOGICDATA        2
    #define REP_TYPE_DBG_RENDERTREE        3
    #define REP_TYPE_DBG_NOTIFYEND        4

    #define TRANSPORT_START                0
    #define TRANSPORT_PROCESS            1
    #define TRANSPORT_END                2
    #define TRANSPORT_LIMIT                200

    void CScreenTouchServer::CSimulationClient::sendBackRenderTree(const String &strXMLData)
    {
        stopHeartBeat();

        if (!m_bRequestFeedback)
        {
            return;
        }

        if (strXMLData.length() == 0)
        {
            return;
        }

        UINT uiDataLen = strXMLData.length() * 2;
        BYTE* pbtMem = new BYTE[uiDataLen];
        String::convertToBYTE_UTF16(strXMLData, pbtMem, uiDataLen);

        sendBackData(REP_TYPE_RENDERTREE, pbtMem, uiDataLen);

        delete[] pbtMem;

        startHeartBeat();
    }

    void CScreenTouchServer::CSimulationClient::sendBackScreenSnapshot(UINT uiWidth, UINT uiHeight, const BYTE *pbtBuffer)
    {
        stopHeartBeat();

        if (!m_bRequestScreenSnapshot)
        {
            return;
        }
        m_bRequestScreenSnapshot = FALSE;

        UINT uiDataLen = uiWidth * uiHeight * 4;
        BYTE* pbtData = const_cast<BYTE*>(pbtBuffer);

        UINT uiSegment = uiDataLen / TRANSPORT_LIMIT;

        // Send back screen snapshot data to server
        {
            // send header
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, 4, 4, 4 = 22 Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(REP_TYPE_SNAPSHORT);                // 1 Type
            bary.writeBYTE(TRANSPORT_START);                // 1 Type
            bary.writeUINT(uiDataLen);                        // 4 Size
            bary.writeUINT(uiWidth);                        // 4 Width
            bary.writeUINT(uiHeight);                        // 4 Height
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            sendData(bary.buffer(), 22);
        }

        while ( uiSegment != 0 )
        {
            // send data
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, n, 4= 14 + n Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(REP_TYPE_SNAPSHORT);                // 1 Type
            bary.writeBYTE(TRANSPORT_PROCESS);                // 1 Type
            bary.writeUINT(TRANSPORT_LIMIT);                // 4 Size
            bary.writeBuffer(pbtData, TRANSPORT_LIMIT);        // n Data
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            if (sendData(bary.buffer(), TRANSPORT_LIMIT + 14) != TRANSPORT_LIMIT + 14)
            {
                break;
            }

            uiSegment--;
            pbtData += TRANSPORT_LIMIT;
        }

        UINT uiLeft = uiDataLen % TRANSPORT_LIMIT;
        {
            // send data
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, n, 4 = 14 + n Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(REP_TYPE_SNAPSHORT);                // 1 Type
            bary.writeBYTE(TRANSPORT_END);                    // 1 Type
            bary.writeUINT(uiLeft);                            // 4 Size
            bary.writeBuffer(const_cast<BYTE*>(pbtData), uiLeft);                // n Data
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            sendData(bary.buffer(), uiLeft + 14);
        }

        startHeartBeat();
    }

    void CScreenTouchServer::CSimulationClient::sendBackDebugData(EDebugData eDebugData, const String& strData)
    {
        stopHeartBeat();

        if (strData.length() == 0)
        {
            return;
        }

        UINT uiDataLen = strData.length() * 2;
        BYTE* pbtData = new BYTE[uiDataLen];
        String::convertToBYTE_UTF16(strData, pbtData, uiDataLen);

        if (eDebugData == DD_LOGIC_DATA)
        {
            sendBackData(REP_TYPE_DBG_LOGICDATA, pbtData, uiDataLen);
        }
        else if (eDebugData == DD_RENDER_TREE)
        {
            sendBackData(REP_TYPE_DBG_RENDERTREE, pbtData, uiDataLen);
        }

        delete[] pbtData;

        startHeartBeat();
    }

    void CScreenTouchServer::CSimulationClient::notifyDebugDataSendEnd()
    {
        OpenHMI::ByteArray bary;

        // 4, 1, 4 = 9 Bytes
        bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
        bary.writeBYTE(REP_TYPE_DBG_NOTIFYEND);            // 1 Type
        bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

        sendData(bary.buffer(), 9);
    }

    void CScreenTouchServer::CSimulationClient::onDataArrived(BYTE * pbtData, INT iDataSize)
    {
        // Simulate event macro defines.
        #define EVT_DATA_TAG        0x4E455645
        #define CMD_DATA_TAG        0x4E455646

        // EVENT
        #define EVT_TYPE_TOUCH        0
        #define EVT_TYPE_HARDKEY    1
        #define EVT_TYPE_USB        2

        #define TOUCH_DATA_SIZE        16
        #define HARDKEY_DATA_SIZE    12
        #define USB_DATA_SIZE        11

        // COMMAND
        #define CMD_TYPE_SIMULATOR_START        0
        #define CMD_TYPE_SIMULATOR_STOP            1
        #define CMD_TYPE_FEEDBACK_START            2
        #define CMD_TYPE_FEEDBACK_STOP            3
        #define CMD_TYPE_SNAPSHOT_REQUEST        4
        #define CMD_TYPE_DEBUGDATA_REQUEST        5

        #define COMMAND_DATA_SIZE                9

        m_aryBuffer.writeBuffer(pbtData, (UINT)iDataSize);
        m_aryBuffer.setPosition(0);

        INT iPosition = 0;
        INT iSize = m_aryBuffer.bytesAvailable();

        BOOLEAN bQuit = FALSE;
        while (iSize >= 5 && !bQuit) // Basic size requirement is 5 (Tag + Type).
        {
            UINT iEvtTag = m_aryBuffer.readUINT();

            switch ( iEvtTag )
            {
            case EVT_DATA_TAG:
                {
                    INT iType = INT(m_aryBuffer.readBYTE());
                    switch ( iType )
                    {
                    case EVT_TYPE_TOUCH:
                        {
                            if (iSize < TOUCH_DATA_SIZE)
                            {
                                iPosition = m_aryBuffer.getPosition();
                                iPosition -= 5;
                                bQuit = TRUE;
                                break;
                            }

                            UINT uiID = (UINT)(m_aryBuffer.readBYTE());
                            INT iStatus = (INT)(m_aryBuffer.readSHORT());
                            INT iX = (INT)(m_aryBuffer.readSHORT());
                            INT iY = (INT)(m_aryBuffer.readSHORT());
                            m_aryBuffer.readUINT(); // SerialNumber, not be used.

                            iPosition = m_aryBuffer.getPosition();
                            iSize -= TOUCH_DATA_SIZE;

                            switch (iStatus)
                            {
                            case 0:
                                if (m_pServer->getSimulatorDelegate())
                                {
                                    m_pServer->getSimulatorDelegate()->requestAddTouchPoint(uiID, iX, iY);
                                }
                                else
                                {
                                    m_pServer->m_cursorManager.addCursor(uiID, iX, iY);
                                }
                                break;

                            case 1:
                                if (m_pServer->getSimulatorDelegate())
                                {
                                    m_pServer->getSimulatorDelegate()->requestUpdateTouchPoint(uiID, iX, iY);
                                }
                                else
                                {
                                    m_pServer->m_cursorManager.updateCursor(uiID, iX, iY);
                                }
                                break;

                            case 2:
                                if (m_pServer->getSimulatorDelegate())
                                {
                                    m_pServer->getSimulatorDelegate()->requestRemoveTouchPoint(uiID, iX, iY);
                                }
                                else
                                {
                                    m_pServer->m_cursorManager.removeCursor(uiID, iX, iY);
                                }
                                break;

                            default:
                                CLogSDK::error2(L"CScreenTouchServer::CSimulationSocketListener::onReceiveData: Invalid cursor status of simulator-touch.");
                            }
                        }
                        break;

                    case EVT_TYPE_HARDKEY:
                        {
                            if (iSize < HARDKEY_DATA_SIZE)
                            {
                                iPosition = m_aryBuffer.getPosition();
                                iPosition -= 5;
                                bQuit = TRUE;
                                break;
                            }

                            UINT uiKeyCode = UINT(m_aryBuffer.readBYTE());
                            INT iKeyStatus = INT(m_aryBuffer.readSHORT());
                            UINT uiSerialNumber = m_aryBuffer.readUINT();

                            iPosition = m_aryBuffer.getPosition();
                            iSize -= HARDKEY_DATA_SIZE;

                            if (m_pServer->getSimulatorDelegate())
                            {
                                m_pServer->getSimulatorDelegate()->requestHardkey(uiKeyCode, iKeyStatus);
                            }
                            else
                            {
                                if (m_pServer->getNotifier())
                                {
                                    m_pServer->getNotifier()->dispatchHardKeyMessage(uiKeyCode, iKeyStatus);
                                }

                                if (CONFIG::getOutputTouch())
                                {
                                    CLogSDK::debug2(String::format(L"CScreenTouchServer::CSimulationSocketListener::onReceiveData: [%.4u] Hard Key: Code=%u, Status=%d",
                                                                    uiSerialNumber, uiKeyCode, iKeyStatus));
                                }
                            }
                        }
                        break;

                    case EVT_TYPE_USB:
                        {
                            if (iSize < USB_DATA_SIZE)
                            {
                                iPosition = m_aryBuffer.getPosition();
                                iPosition -= 5;
                                bQuit = TRUE;
                                break;
                            }

                            UINT uiPortNumber = UINT(m_aryBuffer.readBYTE());
                            UINT uiPortStatus = UINT(m_aryBuffer.readBYTE());
                            m_aryBuffer.readUINT();//UINT uiSerialNumber = m_aryBuffer.readUINT();

                            iPosition = m_aryBuffer.getPosition();
                            iSize -= USB_DATA_SIZE;

                            if (m_pServer->getSimulatorDelegate())
                            {
                                if (uiPortStatus == 0)
                                {
                                    m_pServer->getSimulatorDelegate()->requestConnectUSB(uiPortNumber);
                                }
                                else if (uiPortStatus == 1)
                                {
                                    m_pServer->getSimulatorDelegate()->requestDisconnectUSB(uiPortNumber);
                                }
                            }
                        }
                        break;

                    default:
                        CLogSDK::error2(L"CScreenTouchServer::CSimulationSocketListener::onReceiveData: Invalid type value of simulator-input.");
                        break;
                    }

                    break;
                }
            case CMD_DATA_TAG:
                {
                    if (iSize < COMMAND_DATA_SIZE)
                    {
                        iPosition = m_aryBuffer.getPosition();
                        iPosition -= 4;
                        bQuit = TRUE;
                        break;
                    }

                    INT iType = INT(m_aryBuffer.readBYTE());
                    switch ( iType )
                    {
                    case CMD_TYPE_SIMULATOR_START:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_SIMULATOR_START.");
                        if (m_bActiveSimulator == FALSE)
                        {
                            m_bActiveSimulator = TRUE;

                            if (m_pServer->getSimulatorDelegate())
                            {
                                m_pServer->getSimulatorDelegate()->start();
                            }
                        }
                        break;

                    case CMD_TYPE_SIMULATOR_STOP:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_SIMULATOR_STOP.");
                        if (m_bActiveSimulator == TRUE)
                        {
                            m_bActiveSimulator = FALSE;

                            if (m_pServer->getSimulatorDelegate())
                            {
                                m_pServer->getSimulatorDelegate()->stop();
                            }
                        }

                        m_bRequestFeedback = FALSE;
                        m_bRequestScreenSnapshot = FALSE;
                        break;

                    case CMD_TYPE_FEEDBACK_START:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_FEEDBACK_START.");
                        m_bRequestFeedback = TRUE;
                        if (m_pServer->getNotifier())
                        {
                            m_pServer->getNotifier()->requestPaintHMI();
                        }
                        break;

                    case CMD_TYPE_FEEDBACK_STOP:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_FEEDBACK_STOP.");
                        m_bRequestFeedback = FALSE;
                        break;

                    case CMD_TYPE_SNAPSHOT_REQUEST:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_SNAPSHOT_REQUEST.");
                        m_bRequestScreenSnapshot = TRUE;

                        if (m_pServer->getNotifier())
                        {
                            m_pServer->getNotifier()->requestPaintHMI();
                        }
                        break;

                    case CMD_TYPE_DEBUGDATA_REQUEST:
                        CLogSDK::debug2(L"CScreenTouchServer received CMD_TYPE_DEBUGDATA_REQUEST.");
                        if (m_pServer->getNotifier())
                        {
                            m_pServer->getNotifier()->dispatchSystemMessage(CBaseMessage::MS_SEND_DEBUG_DATA_TO_SIMULATOR, NULL);
                        }
                        break;
                    }

                    m_aryBuffer.readUINT();

                    iPosition = m_aryBuffer.getPosition();
                    iSize -= COMMAND_DATA_SIZE;
                }
                break;
            default:
                {
                    CLogSDK::error2(String::format(L"CScreenTouchServer::CSimulationSocketListener::onReceiveData: Invalid data, size is %d", iSize));
                    m_aryBuffer.clear();
                    iPosition = 0;
                    return;
                }
            }
        }

        // Abandon the processed data.
        m_aryBuffer.setPosition(iPosition);
        if (m_aryBuffer.bytesAvailable() <= 0)
        {
            m_aryBuffer.clear();
        }
        else
        {
            INT iValidSize = m_aryBuffer.bytesAvailable();
            ByteArray aryTmp(iValidSize);
            m_aryBuffer.readBuffer(aryTmp.buffer(), iValidSize);
            m_aryBuffer.clear();
            m_aryBuffer.writeBuffer(aryTmp.buffer(), iValidSize);
        }
    }

    void CScreenTouchServer::CSimulationClient::sendBackData(BYTE btType, const BYTE* pbtData, UINT uiDataLen)
    {
        {
            // send header
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, 4 = 14 Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(btType);                        // 1 Type
            bary.writeBYTE(TRANSPORT_START);                // 1 Type
            bary.writeUINT(uiDataLen);                        // 4 Size
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            sendData(bary.buffer(), 14);
        }

        UINT uiSegment = uiDataLen / TRANSPORT_LIMIT;
        while ( uiSegment != 0 )
        {
            // send data
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, n, 4 = 14 + n Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(btType);                            // 1 Type
            bary.writeBYTE(TRANSPORT_PROCESS);                // 1 Type
            bary.writeUINT(TRANSPORT_LIMIT);                // 4 Size
            bary.writeBuffer(const_cast<BYTE*>(pbtData), TRANSPORT_LIMIT);        // n Data
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            if (sendData(bary.buffer(), TRANSPORT_LIMIT + 14) != (TRANSPORT_LIMIT + 14))
            {
                break;
            }

            uiSegment--;
            pbtData += TRANSPORT_LIMIT;
        }

        UINT uiLeft = uiDataLen % TRANSPORT_LIMIT;
        {
            // send data
            OpenHMI::ByteArray bary;
            // 4, 1, 1, 4, n, 4 = 14 + n Bytes
            bary.writeUINT(REP_DATA_TAG);                    // 4 Tag
            bary.writeBYTE(btType);                            // 1 Type
            bary.writeBYTE(TRANSPORT_END);                    // 1 Type
            bary.writeUINT(uiLeft);                            // 4 Size
            bary.writeBuffer(const_cast<BYTE*>(pbtData), uiLeft);                // n Data
            bary.writeUINT(++m_uiSerialNumber);                // 4 SerialNumber

            sendData(bary.buffer(), uiLeft + 14);
        }
    }
///////////////////////////////////////////////////////////////////////////////////////
    Array<CScreenTouchServer>    CScreenTouchServer::s_aryInstance;

#ifdef OS_WIN
    Map< HWND, CScreenTouchServer, tagNumCompare<HWND> >  CScreenTouchServer::s_mapTouchServer(FALSE);
#endif
    CScreenTouchServer*    CScreenTouchServer::getInstance(UINT uiIndex)
    {
        if (s_aryInstance.getItem(uiIndex) != NULL)
        {
            return s_aryInstance.getItem(uiIndex);
        }

        for (UINT i = s_aryInstance.size(); i <= uiIndex; i++)
        {
            s_aryInstance.addItem(NULL);
        }

        CScreenTouchServer* pInstance = new CScreenTouchServer(uiIndex);
        s_aryInstance.setItem(uiIndex, pInstance);

        return pInstance;
    }

    void CScreenTouchServer::deleteInstance(UINT uiIndex)
    {
        if (uiIndex == 10000)
        {
            s_aryInstance.clear();
        }
        else
        {
            CScreenTouchServer* pInstance = s_aryInstance.getItem(uiIndex);
            if (pInstance != NULL)
            {
                s_aryInstance.setItem(uiIndex, NULL);
            }
        }
    }

    CScreenTouchServer::CScreenTouchServer(UINT uiIndex)
        : m_uiIndex(uiIndex)
        , m_pNotifier(NULL)
        , m_pSimulatorDelegate(NULL)
        , m_bTuioUse(FALSE)
        , m_strTuioHost(L"127.0.0.1")
        , m_uiTuioPort(3334)
        , m_bSimulationUse(FALSE)
        , m_strSimulationHost(L"127.0.0.1")
        , m_uiSimulationPort(6666)
        , m_pTuioClient(NULL)
        , m_pSimulationClient(NULL)
    {
        ;
    }

    CScreenTouchServer::~CScreenTouchServer()
    {
        ;
    }

    void CScreenTouchServer::setWindowTransparency(BOOLEAN bValue, EReceiveTouch eRecvTouch)
    {
#ifdef OS_QNX
        if (m_context.screen_win == NULL)
        {
            return;
        }

        INT iTransparency = SCREEN_TRANSPARENCY_SOURCE;

        if (bValue == TRUE)
        {
            iTransparency = SCREEN_TRANSPARENCY_SOURCE_OVER;
        }

        if (screen_set_window_property_iv(m_context.screen_win, SCREEN_PROPERTY_TRANSPARENCY, &iTransparency))
        {
            CLogSDK::error2(L"CScreenTouchServer::setWindowTransparency: Set SCREEN_PROPERTY_TRANSPARENCY error.");
        }

        if (m_context.screen_win == NULL)
        {
            return;
        }

    #define mapToScreenSenstivity(i)    (i)

        INT iType = mapToScreenSenstivity(eRecvTouch);
        if ( screen_set_window_property_iv(m_context.screen_win, SCREEN_PROPERTY_SENSITIVITY, &iType) )
        {
            CLogSDK::error2(L"CScreenTouchServer::setWindowTransparency: Set SCREEN_PROPERTY_SENSITIVITY error.");
        }

        return;
#endif

#ifdef OS_LINUX
    #ifdef TECHNIQUE_X11
        if (bValue == TRUE)
        {
            #ifdef IVI_LAYERMANAGER
                ilm_surfaceSetVisibility(CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iSurfaceID, ILM_FALSE);
            #else
                XUnmapWindow(m_context.pDisplay, m_context.win);
            #endif
        }
        else
        {
            #ifdef IVI_LAYERMANAGER
                ilm_surfaceSetVisibility(CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iSurfaceID, ILM_TRUE);
            #else
                XMapWindow(m_context.pDisplay, m_context.win);
            #endif
        }

        #ifdef IVI_LAYERMANAGER
            ilm_commitChanges();
        #else
            XFlush(m_context.pDisplay);
        #endif

        return;
    #endif

    #ifdef TECHNIQUE_WAYLAND

    #endif
#endif

#ifdef OS_ANDROID
    #ifdef TECHNIQUE_SURFACEFLINGER
        if (bValue == TRUE)
        {
            //wsfSetAlpha(m_wfsHandle, 0.0f);
            ::android::WSF::wsfSetAlpha(m_wfsHandle, 1.0f); // doesn't support to set the background to be transparent, by default, the background is transparent.
        }
        else
        {
            ::android::WSF::wsfSetAlpha(m_wfsHandle, 1.0f);
        }
    #endif
#endif

        CLogSDK::warning2(L"Set Transparency operation is no effect in this environment.");

        return;
    }

#ifdef OS_WIN
    BOOLEAN CScreenTouchServer::initialize()
    {
        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        //set screen touch thread priority;
        setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
        setName(L"ScreenTouch");
        start();

        m_waitStart.wait();

        if (m_context.hWnd == NULL)
        {
            return FALSE;
        }

        if (m_bTuioUse)
        {
            m_pTuioClient = new CTuioClient(this, m_strTuioHost, m_uiTuioPort);
            m_pTuioClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriTuioInput);
            m_pTuioClient->setName(L"TuioInput");
            m_pTuioClient->start();
        }

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();

        if (m_bTuioUse)
         {
             m_pTuioClient->prepareStop();
         }
         if (m_bSimulationUse)
         {
             m_pSimulationClient->prepareStop();
         }

        //this->waitFor();
        if (m_bTuioUse)
        {
            m_pTuioClient->waitFor();
            delete m_pTuioClient;
            m_pTuioClient = NULL;
        }
        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        if (m_context.hDC != NULL)
        {
            ::ReleaseDC(m_context.hWnd, m_context.hDC);
            m_context.hDC = NULL;
        }

        if (m_context.hWnd != NULL)
        {
            ::DestroyWindow(m_context.hWnd);
            ::PostQuitMessage(0);
            m_context.hWnd = NULL;
        }

        m_context.reset();

        m_cursorManager.unInitialize();
    }

    void CScreenTouchServer::run()
    {
        HINSTANCE hInstance= ::GetModuleHandle(NULL);

        WNDCLASSEX wcex;
        wcex.cbSize            = sizeof(WNDCLASSEX);
        wcex.style            = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc    = WndProc;
        wcex.cbClsExtra        = 0;
        wcex.cbWndExtra        = 0;
        wcex.hInstance        = hInstance;
        wcex.hIcon            = 0;
        wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName    = 0;
        wcex.lpszClassName    = WINDOW_CLASS;
        wcex.hIconSm        = 0;

        ::RegisterClassEx(&wcex);

        INT iLeft = 0;
        INT iTop = 0;

        UINT uiDisplayCount = CConfigParser::getInstance()->getDisplayCount();

        if (uiDisplayCount == 1)
        {
            iLeft = (GetSystemMetrics(SM_CXSCREEN) -  m_context.uiScreenWidth) / 2;
            iTop = (GetSystemMetrics(SM_CYSCREEN) -  m_context.uiScreenHeight) / 2;
        }
        else
        {
            iLeft = 100 * (m_uiIndex + 1);
            iTop = 100 * (m_uiIndex + 1);
        }

        m_context.hWnd = ::CreateWindow(WINDOW_CLASS, FORMATVAR_STRING(String(WINDOW_TITLE) + m_strScreenName),
                                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                                        iLeft, iTop,
                                        m_context.uiScreenWidth + GetSystemMetrics(SM_CXEDGE) * 2 + 2,
                                        m_context.uiScreenHeight + GetSystemMetrics(SM_CYEDGE) * 2 + GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION) - 1,
                                        NULL, NULL, hInstance, NULL);
        if (NULL == m_context.hWnd)
        {
            m_waitStart.notify();
            return;
        }

        m_context.hDC = ::GetDC(m_context.hWnd);

        s_mapTouchServer.addItem(m_context.hWnd, this);

        INT iParentScreen = CConfigParser::getInstance()->findParentScreenByDisplayID(m_uiIndex);

        if (iParentScreen >= 0)
        {
            CConfigParser::tagScreenInfo* pCurScreenInfo = CConfigParser::getInstance()->getScreenInfo(m_uiIndex);
            CConfigParser::tagScreenInfo* pParentScreenInfo = CConfigParser::getInstance()->getScreenInfo(iParentScreen);

            HWND hParent = ::FindWindow(WINDOW_CLASS, FORMATVAR_STRING(String(WINDOW_TITLE) + pParentScreenInfo->strName));

            if (hParent != NULL)
            {
                ::SetParent(m_context.hWnd, hParent);

                LONG_PTR Style = ::GetWindowLongPtr(m_context.hWnd, GWL_STYLE);
                ::SetWindowLongPtr(m_context.hWnd, GWL_STYLE, Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX);

                ::MoveWindow(m_context.hWnd,
                              pCurScreenInfo->iOffsetX, pCurScreenInfo->iOffsetY, m_context.uiScreenWidth, m_context.uiScreenHeight,
                             true);

//                 BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
//                 POINT    ptPos;
//                 ptPos.x = pCurScreenInfo->iOffsetX;
//                 ptPos.y = pCurScreenInfo->iOffsetY;
//                 POINT ptSrc;
//                 ptSrc.x = 0;
//                 ptSrc.y = 0;
//                 SIZE szWnd;
//                 szWnd.cx = m_context.uiScreenWidth;
//                 szWnd.cy = m_context.uiScreenHeight;
//
//                 //::SetWindowLongPtr(m_context.hWnd, GWL_STYLE, Style | WS_POPUP);
//                 ::SetWindowLongPtr(m_context.hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
//                 ::UpdateLayeredWindow(m_context.hWnd, m_context.hDC, &ptPos, &szWnd, NULL, &ptSrc, 0, &blend, ULW_ALPHA);
            }
        }

        ::ShowWindow(m_context.hWnd, SW_SHOW);
        ::UpdateWindow(m_context.hWnd);

        m_waitStart.notify();

        MSG msg;
        while (!m_bPrepareStop && GetMessage(&msg, NULL, 0, 0))
        {
            if ((msg.hwnd == m_context.hWnd) && (msg.message == WM_NULL))
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK CScreenTouchServer::WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
    {
        static UINT s_uiCursorID = 1;

        switch (uiMsg)
        {
        case WM_LBUTTONDOWN:
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                s_uiCursorID++;
                SHORT stX = LOWORD(lParam);
                SHORT stY = HIWORD(lParam);
                pServer->m_cursorManager.addCursor(s_uiCursorID, stX, stY);

                ::SetCapture(pServer->m_context.hWnd);
            }
            break;

        case WM_MOUSEMOVE:
            if (wParam & MK_LBUTTON)
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                SHORT stX = LOWORD(lParam);
                SHORT stY = HIWORD(lParam);
                pServer->m_cursorManager.updateCursor(s_uiCursorID, stX, stY);
            }
            break;

        case WM_LBUTTONUP:
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                SHORT stX = LOWORD(lParam);
                SHORT stY = HIWORD(lParam);
                pServer->m_cursorManager.removeCursor(s_uiCursorID, stX, stY);

                ::ReleaseCapture();
            }
            break;

         case WM_PAINT:
             {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                PAINTSTRUCT ps;
                ::BeginPaint(pServer->m_context.hWnd, &ps);

                 if (pServer->getNotifier())
                 {
                     pServer->getNotifier()->requestPaintHMI();
                 }

                ::EndPaint(pServer->m_context.hWnd, &ps);
             }
             break;

        case WM_ERASEBKGND:
            break;

        case WM_ACTIVATE:
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                if (pServer->getNotifier())
                {
                    pServer->getNotifier()->requestPaintHMI();
                }
            }
            break;

        case WM_HARD_KEY:
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                if (pServer->getNotifier())
                {
                    pServer->getNotifier()->dispatchHardKeyMessage(HIWORD(wParam), (SHORT)LOWORD(wParam));
                }
            }
            break;

        /*case WM_USB:
            CMain::getInstance()->pushMessage(new CMsgUsb(HIWORD(wParam), (SHORT)LOWORD(wParam)));
            break;*/

        case WM_CLOSE:
            {
                CScreenTouchServer *pServer = s_mapTouchServer.getValueByKey(hWnd);

                if (pServer->getNotifier())
                {
                    pServer->getNotifier()->requestQuitHMI();
                }
                //PostMessage(hWnd, WM_NULL, 0, 0); // Wake up "GetMessage".
            }
            break;

        case WM_DESTROY:
            break;

        default:
            return ::DefWindowProc(hWnd, uiMsg, wParam, lParam);
        }

        return 0;
    }
#endif

#ifdef OS_QNX
    BOOLEAN CScreenTouchServer::initialize()
    {
        CLogSDK::debug2(L"CScreenTouchServer::initialize: Begin to create one QNX Screen window.");

        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        INT iDevNum       = 0;

        INT iDisplayID = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iDisplayID;
        if (iDisplayID < 0)
        {
            CLogSDK::warning2(L"The Display ID in the Config.ini file is not correct, will use the default value");
            iDisplayID = 0;
        }

        if (screen_create_context(&m_context.screen_ctx, SCREEN_APPLICATION_CONTEXT))
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Screen create context failed.");
            return FALSE;
        }

        screen_get_context_property_iv(m_context.screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &iDevNum);

        CLogSDK::debug2(String::format(L"CScreenTouchServer::initialize: Display count := %d", iDevNum));

        if (iDevNum <= 0)
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: No display.");
            return FALSE;
        }

        if (iDisplayID >= iDevNum)
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: The Display ID in the Config.ini file is over range.");
            return FALSE;
        }

        m_context.pScreen_dispaly =  new screen_display_t[iDevNum];
        if (m_context.pScreen_dispaly == NULL)
        {
            return FALSE;
        }

        if ( screen_get_context_property_pv(m_context.screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **) (m_context.pScreen_dispaly)) )
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Could not get Screen displays");
            return FALSE;
        }

        INT iLayID = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iLayerID;

        if (CConfigParser::getInstance()->getSurfaceCount(iDisplayID, iLayID) > 1)
        {
            if (CConfigParser::getInstance()->isFirstSurfaceInLayer(m_uiIndex))
            {
                if ( screen_create_window(&m_context.screen_win, m_context.screen_ctx))
                {
                    CLogSDK::error2(L"CScreenTouchServer::initialize: Screen create window error.");
                    return FALSE;
                }

                if (screen_create_window_group(m_context.screen_win, WINDOW_GROUP(iLayID)))
                {
                    CLogSDK::error2(L"CScreenTouchServer::initialize: Screen create window group error.");
                    return FALSE;
                }
            }
            else
            {
                if (screen_create_window_type(&m_context.screen_win, m_context.screen_ctx, SCREEN_CHILD_WINDOW))
                {
                    CLogSDK::error2(L"CScreenTouchServer::initialize: Screen create child window error.");
                    return FALSE;
                }
                if (screen_join_window_group(m_context.screen_win, WINDOW_GROUP(iLayID)))
                {
                    CLogSDK::error2(L"CScreenTouchServer::initialize: Screen join window group error.");
                    return FALSE;
                }
            }
        }
        else
        {
            if ( screen_create_window(&m_context.screen_win, m_context.screen_ctx))
            {
                CLogSDK::error2(L"CScreenTouchServer::initialize: Screen create window error.");
                return FALSE;
            }
        }

        if ( screen_set_window_property_pv(m_context.screen_win, SCREEN_PROPERTY_DISPLAY, (void **) (&m_context.pScreen_dispaly[iDisplayID])) )
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Screen set display error.");
            return FALSE;
        }

        INT aiSize[2];
        aiSize[0] = m_context.uiScreenWidth;        //ENV::getScreenWidth();
        aiSize[1] = m_context.uiScreenHeight;        //ENV::getScreenHeight();
        if (screen_set_window_property_iv(m_context.screen_win, SCREEN_PROPERTY_SIZE, aiSize))
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Screen set Size property error.");
            return FALSE;
        }

        INT iOffsetX = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetX;
        INT iOffsetY = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetY;

        INT aiPos[2] = {iOffsetX, iOffsetY};
        if (screen_set_window_property_iv(m_context.screen_win, SCREEN_PROPERTY_POSITION, aiPos))
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Screen set Position property error.");
            return FALSE;
        }

        char acName[32];
        String::convertToMBCS(m_strScreenName, acName, 32);

        if (screen_set_window_property_cv(m_context.screen_win, SCREEN_PROPERTY_ID_STRING, strlen(acName), acName))
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Set Screen Name property is error.");
        }

        CLogSDK::debug2(L"CScreenTouchServer::initialize: Successfully to create one QNX Screen window.");

        if (m_bTuioUse)
        {
            m_pTuioClient = new CTuioClient(this, m_strTuioHost, m_uiTuioPort);
            m_pTuioClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriTuioInput);
            m_pTuioClient->setName(L"TuioInput");
            m_pTuioClient->start();
        }
        else
        {
            //set screen touch thread priority;
            setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
            setName(L"ScreenTouch");
            start();
        }

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();
        if (m_bTuioUse)
        {
            m_pTuioClient->prepareStop();
        }
        if (m_bSimulationUse)
        {
            m_pSimulationClient->prepareStop();
        }

        this->waitFor();
        if (m_bTuioUse)
        {
            m_pTuioClient->waitFor();
            delete m_pTuioClient;
            m_pTuioClient = NULL;
        }
        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        if (m_context.screen_win != NULL)
        {
            screen_destroy_window(m_context.screen_win);
        }

        if (m_context.screen_ctx != NULL)
        {
            screen_destroy_context(m_context.screen_ctx);
        }

        if (m_context.pScreen_dispaly != NULL)
        {
            delete [] m_context.pScreen_dispaly;
            m_context.pScreen_dispaly = NULL;
        }

        m_context.reset();

        m_cursorManager.unInitialize();
    }

    void CScreenTouchServer::run()
    {
        nativeEventLoop();
    }

    void CScreenTouchServer::nativeEventLoop()
    {
        INT                 iType;
        screen_event_t         screen_ev;
        screen_create_event(&screen_ev);

        // For mouse press event.
        static BOOLEAN bMousePressed = FALSE;

        while (!m_bPrepareStop)
        {
            INT iOffsetX = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetX;
            INT iOffsetY = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetY;

            INT aiPos[2] = {0,0};
            INT    iId = -1;

            screen_get_event(m_context.screen_ctx, screen_ev, -1);
            screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TYPE, &iType);

            switch (iType)
            {
            case SCREEN_EVENT_MTOUCH_TOUCH:
                {
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_POSITION, aiPos);
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TOUCH_ID, &iId);

                    m_cursorManager.addCursor(iId, aiPos[0] - iOffsetX, aiPos[1] - iOffsetY);
                }
                break;

            case SCREEN_EVENT_MTOUCH_MOVE:
                {
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_POSITION, aiPos);
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TOUCH_ID, &iId);

                    m_cursorManager.updateCursor(iId, aiPos[0]- iOffsetX, aiPos[1]- iOffsetY);
                }
                break;

            case SCREEN_EVENT_MTOUCH_RELEASE:
                {
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_POSITION, aiPos);
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TOUCH_ID, &iId);

                    m_cursorManager.removeCursor(iId, aiPos[0]- iOffsetX, aiPos[1] - iOffsetY);
                }
                break;

            case SCREEN_EVENT_POINTER:
                {
                    INT iButton = 0;

                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_BUTTONS, &iButton);
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_POSITION, aiPos);
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TOUCH_ID, &iId);

                    if (iButton == SCREEN_LEFT_MOUSE_BUTTON)
                    {
                        // press
                        if (bMousePressed == FALSE)
                        {
                            m_cursorManager.addCursor(iId, aiPos[0]- iOffsetX, aiPos[1] - iOffsetY);
                        }
                        else // update
                        {
                            m_cursorManager.updateCursor(iId, aiPos[0]- iOffsetX, aiPos[1] - iOffsetY);
                        }

                        bMousePressed = TRUE;
                    }
                    else // Left mouse button was released or press other buttons
                    {
                        // Left mouse button was released
                        if (bMousePressed)
                        {
                            m_cursorManager.removeCursor(iId, aiPos[0]- iOffsetX, aiPos[1] - iOffsetY);
                            bMousePressed = FALSE;
                        }
                    }
                }

                break;

            case SCREEN_EVENT_POST:
                if (m_pNotifier)
                {
                    m_pNotifier->requestPaintHMI();
                }
                break;

            case SCREEN_EVENT_CLOSE:
                {
                    prepareStop();

                    //screen_destroy_window(screen_win);
                    if (m_pNotifier)
                    {
                        m_pNotifier->requestQuitHMI();
                    }
                    return;
                }
                break;
            }
        }
    }

#endif

#ifdef OS_LINUX

#ifdef TECHNIQUE_X11
    BOOLEAN CScreenTouchServer::initialize()
    {
         LONG                        lX11Screen    = 0;
         XVisualInfo*                x11Visual    = 0;
         Colormap                    x11Colormap    = 0;
         Window                        sRootWindow;
         XSetWindowAttributes        sWA;
         UINT                        ui32Mask;
         INT                            i32Depth;
          INT                         i32Width;
          INT                            i32Height;

        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        XInitThreads();
         // Initializes the display and screen
         m_context.pDisplay = XOpenDisplay(0);
         if (!m_context.pDisplay)
         {
            CLogSDK::error2(L"CScreenTouchServer::initialize: XOpen display error.");
             return FALSE;
         }

         lX11Screen = XDefaultScreen( m_context.pDisplay );

        INT iRootDisPlay = CConfigParser::getInstance()->findParentScreenByDisplayID(m_uiIndex);

         if(iRootDisPlay < 0)
         {
            sRootWindow = RootWindow(m_context.pDisplay, lX11Screen);
         }
         else
         {
             sRootWindow = CScreenTouchServer::getInstance(iRootDisPlay)->getScreenContext()->win;
         }

         // Gets the window parameters
     //    sRootWindow = RootWindow(m_context.pDisplay, lX11Screen);
         i32Depth  = DefaultDepth(m_context.pDisplay, lX11Screen);
         //i32Depth = 32;

         x11Visual = new XVisualInfo;
         XMatchVisualInfo( m_context.pDisplay, lX11Screen, i32Depth, TrueColor, x11Visual);
         if (!x11Visual)
         {
             CLogSDK::error2(L"CScreenTouchServer::initialize: Unable to acquire visual.");
             return FALSE;
         }

         x11Colormap = XCreateColormap( m_context.pDisplay, sRootWindow, x11Visual->visual, AllocNone );
         sWA.colormap = x11Colormap;

         // Add to these for handling other events
         sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | ButtonMotionMask;
         sWA.backing_store = Always;

         // Set the window mask attributes
         ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap | CWBackingStore;
          i32Width  = m_context.uiScreenWidth  < ((UINT)XDisplayWidth(m_context.pDisplay, lX11Screen)) ? m_context.uiScreenWidth : (UINT)XDisplayWidth(m_context.pDisplay, lX11Screen);
          i32Height = m_context.uiScreenHeight < ((UINT)XDisplayHeight(m_context.pDisplay,lX11Screen)) ? m_context.uiScreenHeight: (UINT)XDisplayHeight(m_context.pDisplay,lX11Screen);

         // Creates the X11 window
         m_context.win = XCreateWindow(    m_context.pDisplay,
                                         sRootWindow,
                                        CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetX,
                                        CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iOffsetY,
                                        i32Width, i32Height, 0,
                                         x11Visual->depth, InputOutput,
                                         x11Visual->visual, ui32Mask, &sWA);

        m_protocols = XInternAtom(m_context.pDisplay, "WM_DELETE_WINDOW", True);
         XSetWMProtocols(m_context.pDisplay, m_context.win, &m_protocols, 1);

        XMapWindow(m_context.pDisplay, m_context.win);
         XFlush(m_context.pDisplay);

        /*m_context.pDisplay = XOpenDisplay(NULL);
        if( !m_context.pDisplay )
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: X11 open display error.");
            return FALSE;
        }

        INT iScreenNum = DefaultScreen(m_context.pDisplay);
        m_context.win = XCreateSimpleWindow(m_context.pDisplay,
                                            RootWindow(m_context.pDisplay, iScreenNum),
                                            0, 0, m_context.uiScreenWidth, m_context.uiScreenHeight, 3,
                                            BlackPixel(m_context.pDisplay, iScreenNum),
                                            WhitePixel(m_context.pDisplay, iScreenNum));
        if (m_context.win == 0)
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: X11 create simple window error.");
            return FALSE;
        }

        m_protocols = XInternAtom(m_context.pDisplay, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(m_context.pDisplay, m_context.win, &m_protocols, 1);

        // select event
        XSelectInput(m_context.pDisplay, m_context.win,
                     ExposureMask | KeyPressMask | KeyReleaseMask |
                     ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | StructureNotifyMask);

        XMapWindow(m_context.pDisplay, m_context.win);
        XFlush(m_context.pDisplay);*/

        m_context.gc = XCreateGC(m_context.pDisplay, m_context.win, 0, NULL);
        if (m_context.gc == 0)
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: X11 create GC error.");
            return FALSE;
        }

#ifdef IVI_LAYERMANAGER
        t_ilm_layer layerid = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iLayerID;
        t_ilm_surface surfaceid = CConfigParser::getInstance()->getScreenInfo(m_uiIndex)->iSurfaceID;

        ilm_init();

        //ilm_surfaceCreate((t_ilm_nativehandle) m_context.win, i32Width, i32Height, ILM_PIXELFORMAT_RGBA_8888, &surfaceid);
        ilm_surfaceSetNativeContent((t_ilm_nativehandle)m_context.win, i32Width, i32Height, ILM_PIXELFORMAT_RGBA_8888,  surfaceid);
        ilm_surfaceSetSourceRectangle(surfaceid, 0, 0, i32Width, i32Height);
        ilm_surfaceSetDestinationRectangle(surfaceid, 0, 0, i32Width, i32Height);
        ilm_layerAddSurface(layerid, surfaceid);
         ilm_surfaceSetVisibility(surfaceid, ILM_TRUE);
        ilm_surfaceSetOpacity(surfaceid, 1);

        e_ilmErrorTypes ilmErr = ilm_commitChanges();

        if (ilmErr != ILM_SUCCESS)
        {
            CLogSDK::error2(L"CScreenTouchServer::initialize: Layer Manager has commit failed.");
        }
#endif
        //set screen touch thread priority;
        setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
        setName(L"ScreenTouch");
        start();

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();
        if (m_bSimulationUse)
        {
            m_pSimulationClient->prepareStop();
        }

        this->waitFor();
        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        XFreeGC(m_context.pDisplay, m_context.gc);
        XDestroyWindow(m_context.pDisplay, m_context.win);
        XCloseDisplay(m_context.pDisplay);

        m_context.reset();

        m_cursorManager.unInitialize();

#ifdef IVI_LAYERMANAGER
        ilm_destroy();
#endif
    }

    void CScreenTouchServer::run()
    {
        nativeEventLoop();
    }

    void CScreenTouchServer::nativeEventLoop()
    {
        while (!m_bPrepareStop)
        {
            XEvent event;
            XNextEvent(m_context.pDisplay, &event);

            switch(event.type)
            {
            case ButtonPress:
                {
                    m_cursorManager.addCursor(1, event.xbutton.x, event.xbutton.y);
                }
                break;

            case MotionNotify:
                {
                    m_cursorManager.updateCursor(1, event.xmotion.x, event.xmotion.y);
                }
                break;

            case ButtonRelease:
                {
                    m_cursorManager.removeCursor(1, event.xbutton.x, event.xbutton.y);
                }
                break;

            case Expose:
                if (m_pNotifier)
                {
                    m_pNotifier->requestPaintHMI();
                }
                break;

            case ClientMessage:
                {
                    if (event.xclient.data.l[0] == (INT)m_protocols)
                    {
                        if (m_pNotifier)
                        {
                            m_pNotifier->requestQuitHMI();
                        }
                        return;
                    }
                }
                break;

            default:
                break;
            }

//             // I think this should be mutually-exclusive with the touch event
//             if (XGetEventData(m_context.pDisplay, cookie))
//             {
//                 if (cookie->extension == xi_opcode && cookie->type == GenericEvent)
//                 {
//                     switch(cookie->evtype)
//                     {
//                         case XI_RawKeyPress:
//                             break;
//
//                         case XI_RawKeyRelease:
//                             break;
//
//                         case XI_RawButtonPress:
//                             break;
//
//                         case XI_RawButtonRelease:
//                             break;
//
//                         case XI_RawMotion:
//                             m_cursor.setID(1);
//                             m_cursor.setX((XIRawEvent*)cookie.event_x);  // event_x is double type
//                             m_cursor.setY((XIRawEvent*)cookie.event_y);
//                             m_cursor.setXSpeed(0);
//                             m_cursor.setYSpeed(0);
//                             m_cursor.setTouchStatus(CCursor::TS_UPDATE);
//
//                             CMsgScreenTouch *pMessage = new CMsgScreenTouch(m_pListener, m_cursor);
//                             if (m_pNotifier)
//                             {
//                                 m_pNotifier->dispatchScreenTouchMessage(pMessage);
//                             }
//                             break;
//
//                         case XI_RawTouchBegin:
//                             {
//                                 m_cursor.setID(1);
//                                 m_cursor.setX((XIRawEvent*)cookie.event_x);
//                                 m_cursor.setY((XIRawEvent*)cookie.event_x);
//                                 m_cursor.setXSpeed(0);
//                                 m_cursor.setYSpeed(0);
//                                 m_cursor.setTouchStatus(CCursor::TS_ADD);
//
//                                 CMsgScreenTouch *pMessage = new CMsgScreenTouch(m_pListener, m_cursor);
//                                 if (m_pNotifier)
//                                 {
//                                     m_pNotifier->dispatchScreenTouchMessage(pMessage);
//                                 }
//                             }
//                             break;
//
//                         case XI_RawTouchUpdate:
//                             break;
//
//                         case XI_RawTouchEnd:
//                             {
//                                 m_cursor.setID(1);
//                                 m_cursor.setX((XIRawEvent*)cookie.event_x);
//                                 m_cursor.setY((XIRawEvent*)cookie.event_x);
//                                 m_cursor.setXSpeed(800);
//                                 m_cursor.setYSpeed(800);
//                                 m_cursor.setTouchStatus(CCursor::TS_REMOVE);
//
//                                 CMsgScreenTouch *pMessage = new CMsgScreenTouch(m_pListener, m_cursor);
//                                 if (m_pNotifier)
//                                 {
//                                     m_pNotifier->dispatchScreenTouchMessage(pMessage);
//                                 }
//                             }
//
//                             break;
//
//                             //print_rawevent(cookie->data);
//                             //if (one_event)
//                             //{
//                                 //terminate = 1;
//                                 //  break;
//                             //}
//                         default:
//                             break;
//                     }
//                 }
//                 XFreeEventData(m_context.pDisplay, cookie);
//             }
        }
    }

#endif

#ifdef TECHNIQUE_FB
    BOOLEAN CScreenTouchServer::initialize()
    {
        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        //set screen touch thread priority;
        setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
        setName(L"ScreenTouch");
        start();

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();
        if (m_bSimulationUse)
        {
            m_pSimulationClient->prepareStop();
        }

        this->waitFor();
        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        m_context.reset();

        m_cursorManager.unInitialize();
    }

    void CScreenTouchServer::run()
    {
        nativeEventLoop();
    }

    void CScreenTouchServer::nativeEventLoop()
    {
        if (m_strDeviceNode.length() <= 0)
        {
            return;
        }

        char acDevice[1024];
        String::convertToMBCS(m_strDeviceNode, acDevice, 1024);

        CLogSDK::info(L"Device node begin to connect.");

        INT iTryCount = 0;
        INT iDevice = open(acDevice, O_RDWR);
        while (!m_bPrepareStop && iDevice < 0)
        {
            iTryCount++;

            if (iTryCount == 5)
            {
                CLogSDK::error2(L"CScreenTouchServer::nativeEventLoop: Device node fail to connect.");
                return;
            }

            iDevice = open(acDevice, O_RDWR);
            m_waitObject.wait(1000);
        }

        if (m_bPrepareStop)
        {
            return;
        }

        CLogSDK::info(L"Device node successfully connected.");

        INT iReadBytes;
        struct input_event event;

        while (!m_bPrepareStop)
        {
            iReadBytes = read(iDevice, &event, sizeof(struct input_event));
            if (iReadBytes != sizeof(struct input_event))
            {
                continue;
            }

            // parse event
            {
                if (EV_ABS == event.type)
                {
                    switch (event.code)
                    {
                    case ABS_MT_TRACKING_ID:
                        {
                            m_cursorEvent.uiID = (UINT)event.value;
                            //m_cursorEvent.ulTimeStamp = event.time.tv_sec * 1000 + ts.tv_usec / 1000;
                            m_cursorEvent.ulTimeStamp = event.time.tv_sec * 1000 + event.time.tv_usec / 1000;
                        }
                        break;

                    case ABS_MT_TOUCH_MAJOR:
                        {
                            m_cursorEvent.bAlive = (event.value > 0);
                        }
                        break;

                    case ABS_MT_POSITION_X:
                        {
                            m_cursorEvent.iX = event.value;
                        }
                        break;

                    case ABS_MT_POSITION_Y:
                        {
                            m_cursorEvent.iY = event.value;
                        }
                        break;

                    default:
                        ;
                    }
                }
                else if (EV_SYN == event.type)
                {
                    if (SYN_MT_REPORT == event.code)
                    {
                        if (m_cursorEvent.bAlive)
                        {
                            if (m_cursorManager.isExist(m_cursorEvent.uiID))
                            {
                                m_cursorManager.updateCursor(m_cursorEvent.iX, m_cursorEvent.iY, m_cursorEvent.ulTimeStamp);
                            }
                            else
                            {
                                m_cursorManager.addCursor(m_cursorEvent.iX, m_cursorEvent.iY, m_cursorEvent.ulTimeStamp);
                            }
                        }
                        else
                        {
                            m_cursorManager.removeCursor(m_cursorEvent.iX, m_cursorEvent.iY, m_cursorEvent.ulTimeStamp);
                        }
                    }
                    else if (SYN_REPORT == event.code)
                    {
                        ; // end event, do nothing.
                    }
                }
            }
        }

        close(iDevice);
    }
#endif

#ifdef TECHNIQUE_WAYLAND
    void CScreenTouchServer::keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int fd,
                                                    uint32_t size)
    {
    }

    void CScreenTouchServer::keyboard_handle_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                                                   struct wl_surface *surface, struct wl_array *keys)
    {
    }

    void CScreenTouchServer::keyboard_handle_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                                                   struct wl_surface *surface)
    {
    }

    void CScreenTouchServer::keyboard_handle_key(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                                                 uint32_t time, uint32_t key, uint32_t state)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::keyboard_handle_key: data is NULL");
            return;
        }

        pServer->getNotifier()->dispatchHardKeyMessage(key, state);
    }

    void CScreenTouchServer::keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial,
                                                       uint32_t mods_depressed, uint32_t mods_latched,
                                                       uint32_t mods_locked, uint32_t group)
    {
    }

    struct wl_keyboard_listener CScreenTouchServer::s_keyboardListener =
    {
        CScreenTouchServer::keyboard_handle_keymap,
        CScreenTouchServer::keyboard_handle_enter,
        CScreenTouchServer::keyboard_handle_leave,
        CScreenTouchServer::keyboard_handle_key,
        CScreenTouchServer::keyboard_handle_modifiers
    };

    void CScreenTouchServer::pointer_handle_enter(void *data, struct wl_pointer *pointer, uint32_t serial,
                                                  struct wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy)
    {
        //
    }

    void CScreenTouchServer::pointer_handle_leave(void *data, struct wl_pointer *pointer, uint32_t serial,
                                                  struct wl_surface *surface)
    {
        //
    }

    void CScreenTouchServer::pointer_handle_motion(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t sx,
                                                   wl_fixed_t sy)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::pointer_handle_motion: data is NULL");
            return;
        }

        g_iX = wl_fixed_to_int(sx);
        g_iY = wl_fixed_to_int(sy);

        if (g_bEnterButton)
        {
            if (CONFIG::getOutputTouch())
            {
                CLogSDK::info(String::format(L"CScreenTouchServer::pointer_handle_motion: Point(%d, %d)", g_iX, g_iY));
            }

            pServer->m_cursorManager.updateCursor(g_uiID, g_iX, g_iY, (ULONG) time);
        }
    }

    void CScreenTouchServer::pointer_handle_button(void *data, struct wl_pointer *pointer, uint32_t serial,
                                                   uint32_t time, uint32_t button, uint32_t state)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::pointer_handle_button: data is NULL");
            return;
        }

        if (CONFIG::getOutputTouch())
        {
            CLogSDK::info(String::format(L"CScreenTouchServer::pointer_handle_button: button %d is %d. Point(%d, %d)",
                                         button, state, g_iX, g_iY));
        }

        if (BTN_LEFT == button && state == WL_POINTER_BUTTON_STATE_PRESSED)
        {
            g_uiID++;
            g_bEnterButton = TRUE;
            pServer->m_cursorManager.addCursor(g_uiID, g_iX, g_iY, (ULONG) time);
        }
        else if (BTN_LEFT == button && state == WL_POINTER_BUTTON_STATE_RELEASED)
        {
            pServer->m_cursorManager.removeCursor(g_uiID, g_iX, g_iY, (ULONG) time);
            g_bEnterButton = FALSE;
        }
    }

    void CScreenTouchServer::pointer_handle_axis(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis,
                                                 wl_fixed_t value)
    {
    }

    struct wl_pointer_listener CScreenTouchServer::s_pointerListener =
    {
        CScreenTouchServer::pointer_handle_enter,
        CScreenTouchServer::pointer_handle_leave,
        CScreenTouchServer::pointer_handle_motion,
        CScreenTouchServer::pointer_handle_button,
        CScreenTouchServer::pointer_handle_axis
    };

    void CScreenTouchServer::touch_handle_down(void *data, struct wl_touch *touch, uint32_t serial, uint32_t time,
                                               struct wl_surface *surface, int32_t id, wl_fixed_t x, wl_fixed_t y)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::touch_handle_down: data is NULL");
            return;
        }

        if (CONFIG::getOutputTouch())
        {
            CLogSDK::info(String::format(L"CScreenTouchServer::touch_handle_down: Id %d Point(%d, %d)", id,
                                         wl_fixed_to_int(x), wl_fixed_to_int(y)));
        }

        pServer->m_cursorManager.addCursor((UINT) id, wl_fixed_to_int(x), wl_fixed_to_int(y), (ULONG) time);
    }

    void CScreenTouchServer::touch_handle_up(void *data, struct wl_touch *touch, uint32_t serial, uint32_t time,
                                             int32_t id)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::touch_handle_up: data is NULL");
            return;
        }

        if (CONFIG::getOutputTouch())
        {
            CLogSDK::info(String::format(L"CScreenTouchServer::touch_handle_up: Id %d", id));
        }

        pServer->m_cursorManager.removeCursor((UINT) id, time);
    }

    void CScreenTouchServer::touch_handle_motion(void *data, struct wl_touch *touch, uint32_t time, int32_t id,
                                                 wl_fixed_t x, wl_fixed_t y)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::touch_handle_motion: data is NULL");
            return;
        }

        if (CONFIG::getOutputTouch())
        {
            CLogSDK::info(String::format(L"CScreenTouchServer::touch_handle_motion: Id %d Point(%d, %d)", id,
                                         wl_fixed_to_int(x), wl_fixed_to_int(y)));
        }

        pServer->m_cursorManager.updateCursor((UINT) id, wl_fixed_to_int(x), wl_fixed_to_int(y), (ULONG) time);
    }

    void CScreenTouchServer::touch_handle_frame(void *data, struct wl_touch *touch)
    {
        //
    }

    void CScreenTouchServer::touch_handle_cancel(void *data, struct wl_touch *touch)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::touch_handle_cancel: data is NULL");
            return;
        }

        if (CONFIG::getOutputTouch())
        {
            CLogSDK::info(L"CScreenTouchServer::touch_handle_cancel");
        }
    }

    struct wl_touch_listener CScreenTouchServer::s_touchListener =
    {
        CScreenTouchServer::touch_handle_down,
        CScreenTouchServer::touch_handle_up,
        CScreenTouchServer::touch_handle_motion,
        CScreenTouchServer::touch_handle_frame,
        CScreenTouchServer::touch_handle_cancel
    };

    void CScreenTouchServer::seat_handle_capabilities(void *data, struct wl_seat *seat, uint32_t caps)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::seat_handle_capabilities: data is NULL");
            return;
        }

        if (caps & WL_SEAT_CAPABILITY_POINTER)
        {
            pServer->m_pWaylandContext->wlPointer = wl_seat_get_pointer(seat);
            wl_pointer_add_listener(pServer->m_pWaylandContext->wlPointer, &s_pointerListener, data);
        }
        else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && pServer->m_pWaylandContext->wlPointer != NULL)
        {
            wl_pointer_destroy(pServer->m_pWaylandContext->wlPointer);
            pServer->m_pWaylandContext->wlPointer = NULL;
        }

        if (caps & WL_SEAT_CAPABILITY_KEYBOARD)
        {
            pServer->m_pWaylandContext->wlKeyboard = wl_seat_get_keyboard(seat);
            wl_keyboard_add_listener(pServer->m_pWaylandContext->wlKeyboard, &s_keyboardListener, data);
        }
        else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && pServer->m_pWaylandContext->wlKeyboard != NULL)
        {
            wl_keyboard_destroy(pServer->m_pWaylandContext->wlKeyboard);
            pServer->m_pWaylandContext->wlKeyboard = NULL;
        }

        if (caps & WL_SEAT_CAPABILITY_TOUCH)
        {
            pServer->m_pWaylandContext->wlTouch = wl_seat_get_touch(seat);
            wl_touch_add_listener(pServer->m_pWaylandContext->wlTouch, &s_touchListener, data);
        }
        else if (!(caps & WL_SEAT_CAPABILITY_TOUCH) && pServer->m_pWaylandContext->wlTouch != NULL)
        {
            wl_touch_destroy(pServer->m_pWaylandContext->wlTouch);
            pServer->m_pWaylandContext->wlTouch = NULL;
        }
    }

    struct wl_seat_listener CScreenTouchServer::s_seatListener =
    {
        CScreenTouchServer::seat_handle_capabilities
    };

    void CScreenTouchServer::handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
    {
        wl_shell_surface_pong(shell_surface, serial);
    }

    void CScreenTouchServer::handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges,
                                              int32_t width, int32_t height)
    {
    }

    void CScreenTouchServer::handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
    {
        //
    }

    struct wl_shell_surface_listener CScreenTouchServer::s_shellSurfaceListener =
    {
        CScreenTouchServer::handle_ping,
        CScreenTouchServer::handle_configure,
        CScreenTouchServer::handle_popup_done
    };

    void CScreenTouchServer::global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
                                                     const char *interface, uint32_t version)
    {
        CScreenTouchServer *pServer = (CScreenTouchServer *) (data);

        String strInterface;
        String::convertFromMBCS(interface, strInterface);
        CLogSDK::debug2(String::format(L"WL-interface: %ls", (const CHAR*) strInterface));

        if (NULL == pServer)
        {
            CLogSDK::warning2(L"CScreenTouchServer::global_registry_handler: data is NULL");
            return;
        }

        if (0 == strcmp(interface, "wl_compositor"))
        {
            pServer->m_pWaylandContext->wlCompositor =
                (struct wl_compositor *) wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        }
        else if (0 == strcmp(interface, "wl_shell"))
        {
            pServer->m_pWaylandContext->wlShell =
                (struct wl_shell *) wl_registry_bind(registry, id, &wl_shell_interface, 1);
        }
        else if (0 == strcmp(interface, "wl_seat"))
        {
            pServer->m_pWaylandContext->wlSeat = (struct wl_seat *) wl_registry_bind(registry, id, &wl_seat_interface, 1);
            wl_seat_add_listener(pServer->m_pWaylandContext->wlSeat, &s_seatListener, data);
        }
        else if (0 == strcmp(interface, "wl_subcompositor"))
        {
            pServer->m_pWaylandContext->wlSubCompositor =
                (struct wl_subcompositor *) wl_registry_bind(registry, id, &wl_subcompositor_interface, 1);
        }
    }

    void CScreenTouchServer::global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
    {
        //
    }

    struct wl_registry_listener CScreenTouchServer::s_registryListener =
    {
        CScreenTouchServer::global_registry_handler,
        CScreenTouchServer::global_registry_remover
    };

    BOOLEAN CScreenTouchServer::initialize()
    {
        m_pWaylandContext = new tagWaylandContext();

        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
        setName(L"ScreenTouch");
        start();

        m_waitStart.wait();

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse    = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();

        destroyWL();

        if (m_bSimulationUse)
        {
            m_pSimulationClient->prepareStop();
        }

        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        m_cursorManager.unInitialize();

        m_context.reset();
        SAFE_DELETE(m_pWaylandContext);
    }

    void CScreenTouchServer::run()
    {
        char pcDisplayName[16]                    = {0};
        char pcScreenName[64]                     = {0};
        CConfigParser::tagScreenInfo *pScreenInfo = CConfigParser::getInstance()->getScreenInfo(m_uiIndex);

        String strName = String::format(L"wayland-%d", pScreenInfo->iDisplayID);
        String::convertToMBCS(strName, pcDisplayName, 16);
        String::convertToMBCS(pScreenInfo->strName, pcScreenName, 64);

        INT iParentDisplay  = CConfigParser::getInstance()->findParentScreenByDisplayID(m_uiIndex);
        BOOLEAN bIsSubSurface = FALSE;

        if (iParentDisplay == -1)
        {
            if (!getWLSereverReferences(pcDisplayName))
            {
                return;
            }
        }
        else
        {
#ifdef IVI_LAYERMANAGER
            if (!getWLSereverReferences(pcDisplayName))
            {
                return;
            }

            bIsSubSurface = FALSE;
#else
            CLogSDK::debug2(L"CScreenTouchServer::run [Wayland connect to parent screen]");

            CScreenTouchServer *pParentTouchServer = CScreenTouchServer::getInstance(iParentDisplay);
            if (pParentTouchServer)
            {
                m_context.wlDisplay                = pParentTouchServer->m_context.wlDisplay;
                m_pWaylandContext->wlCompositor    = pParentTouchServer->m_pWaylandContext->wlCompositor;
                m_pWaylandContext->wlSubCompositor = pParentTouchServer->m_pWaylandContext->wlSubCompositor;
                m_pWaylandContext->wlShell         = pParentTouchServer->m_pWaylandContext->wlShell;
                m_pWaylandContext->wlSeat          = pParentTouchServer->m_pWaylandContext->wlSeat;
                m_pWaylandContext->wlParentSurface = pParentTouchServer->m_pWaylandContext->wlSurface;
            }

            bIsSubSurface = TRUE;
#endif
        }

        m_pWaylandContext->wlSurface = wl_compositor_create_surface(m_pWaylandContext->wlCompositor);
        if (NULL == m_pWaylandContext->wlSurface)
        {
            CLogSDK::error2(L"CScreenTouchServer::run [Can't create compositor surface in wayland.]");
            destroyWL();
            return;
        }

        if (bIsSubSurface)
        {
            m_pWaylandContext->wlSubSurface = wl_subcompositor_get_subsurface(
                m_pWaylandContext->wlSubCompositor, m_pWaylandContext->wlSurface, m_pWaylandContext->wlParentSurface);
            if (NULL == m_pWaylandContext->wlSubSurface)
            {
                CLogSDK::error2(L"CScreenTouchServer::run [Can't create sub-surface in wayland.]");
                destroyWL();
                return;
            }

            wl_subsurface_set_position(m_pWaylandContext->wlSubSurface, pScreenInfo->iOffsetX, pScreenInfo->iOffsetY);
            wl_subsurface_place_above(m_pWaylandContext->wlSubSurface, m_pWaylandContext->wlParentSurface);
            wl_surface_commit(m_pWaylandContext->wlParentSurface);
        }
        else
        {
#ifdef IVI_LAYERMANAGER
            ilm_initWithNativedisplay((t_ilm_nativedisplay) m_context.wlDisplay);
            ilmClient_init((t_ilm_nativedisplay) m_context.wlDisplay);

            m_pWaylandContext->ilmSurfaceId = (t_ilm_surface) pScreenInfo->iSurfaceID;
            m_pWaylandContext->ilmLayerId   = (t_ilm_layer) pScreenInfo->iLayerID;

            t_ilm_surface ilmSurfaceId = m_pWaylandContext->ilmSurfaceId;
            if (ILM_FAILED == ilm_surfaceCreate((t_ilm_nativehandle) m_pWaylandContext->wlSurface,
                                                m_context.uiScreenWidth, m_context.uiScreenHeight,
                                                ILM_PIXELFORMAT_RGBA_8888,
                                                &ilmSurfaceId))
            {
                CLogSDK::error2(L"CScreenTouchServer::run [ilm_surfaceCreate failed.]");
                return;
            }

            m_pWaylandContext->ilmSurfaceId = ilmSurfaceId;

            ilm_layerAddSurface(m_pWaylandContext->ilmLayerId, m_pWaylandContext->ilmSurfaceId);
            ilm_surfaceSetVisibility(m_pWaylandContext->ilmSurfaceId, ILM_TRUE);
            ilm_surfaceSetOpacity(m_pWaylandContext->ilmSurfaceId, 1);

            ilmErrorTypes ilmErr = ilm_commitChanges();
            if (ilmErr != ILM_SUCCESS)
            {
                CLogSDK::error2(L"CScreenTouchServer::run [Layer Manager has commit failed.]");
            }
#else
            if (m_pWaylandContext->wlShell)
            {
                m_pWaylandContext->wlShellSurface =
                    wl_shell_get_shell_surface(m_pWaylandContext->wlShell, m_pWaylandContext->wlSurface);
            }
            if (m_pWaylandContext->wlShellSurface)
            {
                wl_shell_surface_add_listener(m_pWaylandContext->wlShellSurface, &s_shellSurfaceListener, this);
            }
            if (m_pWaylandContext->wlShellSurface)
            {
                wl_shell_surface_set_title(m_pWaylandContext->wlShellSurface, pcScreenName);
                wl_shell_surface_set_toplevel(m_pWaylandContext->wlShellSurface);
            }
#endif
        }

        m_context.wlEglWindow =
            wl_egl_window_create(m_pWaylandContext->wlSurface, m_context.uiScreenWidth, m_context.uiScreenHeight);
        if (NULL == m_context.wlEglWindow)
        {
            CLogSDK::error2(L"CScreenTouchServer::run [Can't create egl window in wayland.]");
            destroyWL();
            return;
        }

        m_waitStart.notify();

        if (!bIsSubSurface)
        {
            nativeEventLoop();
        }
    }

    BOOLEAN CScreenTouchServer::getWLSereverReferences(const char *pcDisplayName)
    {
        m_context.wlDisplay = wl_display_connect(pcDisplayName);
        if (m_context.wlDisplay == NULL)
        {
            m_context.wlDisplay = wl_display_connect(NULL);
            if (m_context.wlDisplay == NULL)
            {
                CLogSDK::error2(L"CScreenTouchServer::getWLSereverReferences [Can't connect to display in wayland.]");
                destroyWL();
                return FALSE;
            }
        }

        m_pWaylandContext->wlRegistry = wl_display_get_registry(m_context.wlDisplay);
        wl_registry_add_listener(m_pWaylandContext->wlRegistry, &s_registryListener, this);

        wl_display_dispatch(m_context.wlDisplay);
        wl_display_roundtrip(m_context.wlDisplay);

        if (m_pWaylandContext->wlCompositor == NULL)
        {
            CLogSDK::error2(L"CScreenTouchServer::getWLSereverReferences [Can't find compositor in wayland.]");
            destroyWL();
            return FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::destroyWL()
    {
        if (m_context.wlEglWindow)
        {
            wl_egl_window_destroy(m_context.wlEglWindow);
        }

        if (m_pWaylandContext->wlShellSurface)
        {
            wl_shell_surface_destroy(m_pWaylandContext->wlShellSurface);
        }

        if (m_pWaylandContext->wlSubSurface)
        {
            wl_subsurface_destroy(m_pWaylandContext->wlSubSurface);

            if (m_pWaylandContext->wlSurface)
            {
                wl_surface_destroy(m_pWaylandContext->wlSurface);
            }
        }
        else
        {
#if IVI_LAYERMANAGER
            ilm_surfaceRemove(m_pWaylandContext->ilmSurfaceId);

            ilm_destroy();
#endif
            if (m_pWaylandContext->wlSurface)
            {
                wl_surface_destroy(m_pWaylandContext->wlSurface);
            }

            if (m_pWaylandContext->wlShell)
            {
                wl_shell_destroy(m_pWaylandContext->wlShell);
            }

            if (m_pWaylandContext->wlSubCompositor)
            {
                wl_subcompositor_destroy(m_pWaylandContext->wlSubCompositor);
            }

            if (m_pWaylandContext->wlCompositor)
            {
                wl_compositor_destroy(m_pWaylandContext->wlCompositor);
            }

            if (m_pWaylandContext->wlRegistry)
            {
                wl_registry_destroy(m_pWaylandContext->wlRegistry);
            }

            if (m_context.wlDisplay)
            {
                wl_display_disconnect(m_context.wlDisplay);
            }
        }
    }

    void CScreenTouchServer::nativeEventLoop()
    {
        while (!m_bPrepareStop && (wl_display_dispatch(m_context.wlDisplay) != -1))
        {
            ;
        }
    }
#endif  // TECHNIQUE_WAYLAND

#endif  // OS_LINUX

#ifdef OS_ANDROID
  #ifdef TECHNIQUE_ACTIVITY
    void CScreenTouchServer::setAndroidApp(struct android_app *app)
    {
        m_app = app;
    }

    BOOLEAN CScreenTouchServer::initialize()
    {
        m_cursorManager.initialize();
        m_cursorManager.setScreenTouchServer(this);
        m_cursorManager.setScreenContext(getScreenContext());

        //set screen touch thread priority;
        setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
        setName(L"ScreenTouch");
        start();

        m_waitStart.wait();

        if (m_context.win == NULL)
        {
            return FALSE;
        }

        if (0 == m_uiIndex && m_bSimulationUse)
        {
            m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
            m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
            m_pSimulationClient->setName(L"SimulationInput");
            m_pSimulationClient->start(TRUE);
            m_pSimulationClient->setNeedSendHB(TRUE);
        }
        else
        {
            m_pSimulationClient = NULL;
            m_bSimulationUse = FALSE;
        }

        return TRUE;
    }

    void CScreenTouchServer::unInitialize()
    {
        prepareStop();

        if (m_bSimulationUse)
        {
            m_pSimulationClient->prepareStop();
        }

        if (m_bSimulationUse)
        {
            m_pSimulationClient->waitFor();
            delete m_pSimulationClient;
            m_pSimulationClient = NULL;
        }

        m_cursorManager.unInitialize();

        m_context.reset();
    }

    void CScreenTouchServer::run()
    {
        ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
        ALooper_addFd(looper, m_app->msgread, LOOPER_ID_MAIN, ALOOPER_EVENT_INPUT, NULL,
                        &m_app->cmdPollSource);
        m_app->looper = looper;

        nativeEventLoop();
    }

    void CScreenTouchServer::nativeEventLoop()
    {
        m_app->onAppCmd = CScreenTouchServer::handleCmd;
        m_app->onInputEvent = CScreenTouchServer::handleInput;

        // loop waiting for stuff to do.
        while (1)
        {
            // Read all pending events.
            int ident;
            int events;
            struct android_poll_source* source;

            if (m_bPrepareStop)
            {
                break;
            }

            // We will block forever waiting for events.
            while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)&source)) >= 0)
            {
                if (m_bPrepareStop)
                {
                    break;
                }

                // Process this event.
        		if (source != NULL)
				{
					source->process(m_app, source);
				}

				// If a sensor has data, process it now.
				if (ident == LOOPER_ID_USER)
				{
				}

				// Check if we are exiting.
				if (m_app->destroyRequested != 0)
				{
					if (m_pNotifier)
					{
						m_pNotifier->requestQuitHMI();
					}
					return;
				}
			}
		}
    }

	/**
	 * Process the next input event.
	 */
	int32_t CScreenTouchServer::handleInput(struct android_app* app, AInputEvent* event)
	{
		CScreenTouchServer *pServer = CScreenTouchServer::getInstance();
		if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
		{
			/* Indicates that the input event is a motion event. */
			float fX = 0;
			float fY = 0;
			int32_t iCount = AMotionEvent_getPointerCount(event);
			int32_t iAction = AMotionEvent_getAction(event);
			uint32_t uiFlag = iAction & AMOTION_EVENT_ACTION_MASK;

			if (CONFIG::getOutputTouch())
			{
				CLogSDK::info(String::format(L"Pointer count %d, action %d, flags %ld", iCount, iAction, uiFlag));
			}
			switch (uiFlag)
			{
				case AMOTION_EVENT_ACTION_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_DOWN:
					for (INT i = 0; i < iCount; ++i)
					{
					    fX = AMotionEvent_getX(event, i);
					    fY = AMotionEvent_getY(event, i);
                        pServer->m_cursorManager.addCursor(AMotionEvent_getPointerId(event, i), fX, fY);
                    }
					break;

                case AMOTION_EVENT_ACTION_CANCEL:
				case AMOTION_EVENT_ACTION_UP:
				case AMOTION_EVENT_ACTION_POINTER_UP:
                    for (INT i = 0; i < iCount; ++i)
                    {
					    fX = AMotionEvent_getX(event, i);
					    fY = AMotionEvent_getY(event, i);
					    pServer->m_cursorManager.removeCursor(AMotionEvent_getPointerId(event, i), fX, fY);
					}
					break;

				case AMOTION_EVENT_ACTION_MOVE:
                    for (INT i = 0; i < iCount; ++i)
                    {
                        fX = AMotionEvent_getX(event, i);
					    fY = AMotionEvent_getY(event, i);
					    pServer->m_cursorManager.updateCursor(AMotionEvent_getPointerId(event, i), fX, fY);
					}
					break;

				default:
					break;
			}

			return 1;
		}
		else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
		{
			/* Indicates that the input event is a key event. */
			INT iKeyStatus = (INT)AKeyEvent_getAction(event);
			UINT uiKeyCode = (UINT)AKeyEvent_getKeyCode(event);

			if (pServer->getNotifier())
			{
				pServer->getNotifier()->dispatchHardKeyMessage(uiKeyCode, iKeyStatus);
			}
		}

		return 0;
	}

	/**
	 * Process the next main command.
	 */
	void CScreenTouchServer::handleCmd(struct android_app* app, int32_t cmd)
	{
		CScreenTouchServer *pServer = CScreenTouchServer::getInstance();
		switch (cmd)
		{
			case APP_CMD_SAVE_STATE:
				// The system has asked us to save our current state. Do so.
				break;

			case APP_CMD_INIT_WINDOW:
				// The window is being shown, get it ready.
				if (app != NULL && app->window != NULL)
				{
					pServer->m_context.win = app->window;
					if (pServer->getNotifier())
					{
						pServer->getNotifier()->notifyRenderEngState(REST_NEEDINIT);
					}
					pServer->m_waitStart.notify();
				}
				break;

			case APP_CMD_TERM_WINDOW:
				// The window is being hidden or closed, clean it up.
				if (pServer->getNotifier())
				{
					pServer->getNotifier()->notifyRenderEngState(REST_NEEDUNINIT);
				}
				break;

			case APP_CMD_GAINED_FOCUS:
				break;

			case APP_CMD_LOST_FOCUS:
				break;

			default:
				break;
		}
	}
  #endif  // TECHNIQUE_ACTIVITY

  #ifdef TECHNIQUE_SURFACEFLINGER
	class OPENHMI_WINSYSTEM_API SurfaceInputListener : public ::android::NativeInputListener
	{
	private:
		CScreenTouchServer * mpServer;

	public:
		SurfaceInputListener()
		{
			mpServer = CScreenTouchServer::getInstance();
		}

		~SurfaceInputListener() {}
    	
		void notifyMotion(int64_t iTime, uint uiCount, ::android::Coordinate point[], ::android::NativeInputListener_Motion_Type eType)
		{
			CLogSDK::info(String::format(L"type %d", eType));

			switch (eType)
			{
			case ::android::NMT_PRESS:
			case ::android::NMT_POINT_DOWN:
			{
				for (INT i = 0; i < uiCount; ++i)
				{
					CLogSDK::info(String::format(L" x %f, y %f, id %d", point[i].fX, point[i].fY, point[i].iId));

					mpServer->m_cursorManager.addCursor(point[i].iId, point[i].fX, point[i].fY);
				}
				break;
			}

			case ::android::NMT_CANCLE:
			case ::android::NMT_RELEASE:
			case ::android::NMT_POINT_UP:
			{
				for (INT i = 0; i < uiCount; ++i)
				{
					CLogSDK::info(String::format(L" x %f, y %f, id %d", point[i].fX, point[i].fY, point[i].iId));
					mpServer->m_cursorManager.removeCursor(point[i].iId, point[i].fX, point[i].fY);
				}
				break;
			}

			case ::android::NMT_MOVE:
			{
				for (INT i = 0; i < uiCount; ++i)
				{
					CLogSDK::info(String::format(L" x %f, y %f, id %d", point[i].fX, point[i].fY, point[i].iId));
					mpServer->m_cursorManager.updateCursor(point[i].iId, point[i].fX, point[i].fY);
				}
				break;
			}

			default:
				break;
			}
		}
	};

	BOOLEAN CScreenTouchServer::initialize()
	{
		m_context.win = NULL;
		m_wfsHandle = NULL;
		m_pSurfaceInputListener = NULL;

		m_pSurfaceInputListener = new SurfaceInputListener();
		::android::TOUCH::nativeInputReaderSetListener((SurfaceInputListener*)m_pSurfaceInputListener);

		m_cursorManager.initialize();
		m_cursorManager.setScreenTouchServer(this);
		m_cursorManager.setScreenContext(getScreenContext());

		//set screen touch thread priority;
		setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriScreenTouch);
		setName(L"ScreenTouch");
		start();

		m_waitStart.wait();

		if (m_context.win == NULL)
		{
			return FALSE;
		}

		if (0 == m_uiIndex && m_bSimulationUse)
		{
			m_pSimulationClient = new CSimulationClient(this, m_strSimulationHost, m_uiSimulationPort);
			m_pSimulationClient->setPriority(CConfigParser::getInstance()->getSysInfo()->iThreadPriSimulationInput);
			m_pSimulationClient->setName(L"SimulationInput");
			m_pSimulationClient->start(TRUE);
			m_pSimulationClient->setNeedSendHB(TRUE);
		}
		else
		{
			m_pSimulationClient = NULL;
			m_bSimulationUse = FALSE;
		}

		return TRUE;
	}

	void CScreenTouchServer::unInitialize()
	{
		prepareStop();

		if (m_bSimulationUse)
		{
			m_pSimulationClient->prepareStop();
			m_pSimulationClient->waitFor();
			delete m_pSimulationClient;
			m_pSimulationClient = NULL;
		}

		// m_cursorManager.unInitialize();

		::android::TOUCH::nativeInputReaderStop();
		::android::TOUCH::nativeInputReaderClear();

		if (m_pSurfaceInputListener) {
			delete m_pSurfaceInputListener;
			m_pSurfaceInputListener = NULL;
		}
		m_cursorManager.unInitialize();
		m_context.reset();
	}

	void CScreenTouchServer::run()
	{
		nativeEventLoop();
	}

	void CScreenTouchServer::nativeEventLoop()
	{
		CConfigParser::tagScreenInfo *pInfo = CConfigParser::getInstance()->getScreenInfo(m_uiIndex);

		char acName[32];
		String::convertToMBCS(m_strScreenName, acName, 32);

		m_wfsHandle = ::android::WSF::wsfCreateDisplaySurface(pInfo->iOffsetX, pInfo->iOffsetY, m_context.uiScreenWidth, m_context.uiScreenHeight, pInfo->iLayerID, (const char*)acName);

		if (NULL != m_wfsHandle)
		{
			m_context.win = ::android::WSF::wsfGetEGLNativeWindow(m_wfsHandle);
		}

		m_waitStart.notify();

		::android::TOUCH::nativeInputReaderStart();

		while (1)
		{
			m_waitObject.wait();
			// loop for events
			if (m_bPrepareStop)
			{
				break;
			}
		}

		if (NULL != m_wfsHandle)
		{
			::android::WSF::wsfDestroySurface(m_wfsHandle);
			m_wfsHandle = NULL;
		}
	}
  #endif  // TECHNIQUE_SURFACEFLINGER
#endif  // OS_ANDROID
}

#ifdef OS_ANDROID
  #ifdef TECHNIQUE_ACTIVITY
	void free_saved_state(struct android_app* android_app)
	{
		pthread_mutex_lock(&android_app->mutex);
		if (android_app->savedState != NULL) {
			free(android_app->savedState);
			android_app->savedState = NULL;
			android_app->savedStateSize = 0;
		}
		pthread_mutex_unlock(&android_app->mutex);
	}

	static int8_t android_app_read_cmd(struct android_app* android_app)
	{
		int8_t cmd;
		if (read(android_app->msgread, &cmd, sizeof(cmd)) == sizeof(cmd)) {
			switch (cmd) {
			case APP_CMD_SAVE_STATE:
				free_saved_state(android_app);
				break;
			}
			return cmd;
		} else {
			OpenHMI::CLogSDK::info(L"No data on command pipe!");
		}
		return -1;
	}

	static void android_app_set_activity_state(struct android_app* android_app, int8_t iState)
	{
		OpenHMI::CLogSDK::info(OpenHMI::String::format(L"activityState=%d", iState));
		pthread_mutex_lock(&android_app->mutex);
		android_app->activityState = iState;
		pthread_cond_broadcast(&android_app->cond);
		pthread_mutex_unlock(&android_app->mutex);
	}

	static void android_app_pre_exec_cmd(struct android_app* android_app, int8_t cmd)
	{
        OpenHMI::CScreenTouchServer::IScreenTouchServerNotifier *pNotifier = OpenHMI::CScreenTouchServer::getInstance()->getNotifier();
		switch (cmd) {
		case APP_CMD_INPUT_CHANGED:
			OpenHMI::CLogSDK::info(L"APP_CMD_INPUT_CHANGED");
			pthread_mutex_lock(&android_app->mutex);
			if (android_app->inputQueue != NULL) {
				AInputQueue_detachLooper(android_app->inputQueue);
			}
			android_app->inputQueue = android_app->pendingInputQueue;
			if (android_app->inputQueue != NULL) {
				OpenHMI::CLogSDK::info(L"Attaching input queue to looper");
				AInputQueue_attachLooper(android_app->inputQueue,
					android_app->looper, LOOPER_ID_INPUT, NULL,
					&android_app->inputPollSource);
			}
			pthread_cond_broadcast(&android_app->cond);
			pthread_mutex_unlock(&android_app->mutex);
			break;

		case APP_CMD_INIT_WINDOW:
			OpenHMI::CLogSDK::info(L"APP_CMD_INIT_WINDOW");
			pthread_mutex_lock(&android_app->mutex);
			android_app->window = android_app->pendingWindow;
			pthread_cond_broadcast(&android_app->cond);
			pthread_mutex_unlock(&android_app->mutex);
			break;

		case APP_CMD_TERM_WINDOW:
			OpenHMI::CLogSDK::info(L"APP_CMD_TERM_WINDOW");
			pthread_cond_broadcast(&android_app->cond);
			break;

		case APP_CMD_RESUME:
			android_app_set_activity_state(android_app, cmd);
			if (pNotifier)
			{
				pNotifier->notifyActivityState(OpenHMI::CScreenTouchServer::AS_RESUME);
			}
			break;

		case APP_CMD_START:
			android_app_set_activity_state(android_app, cmd);
			if (pNotifier)
			{
				pNotifier->notifyActivityState(OpenHMI::CScreenTouchServer::AS_START);
			}
			break;

		case APP_CMD_PAUSE:
			android_app_set_activity_state(android_app, cmd);
			if (pNotifier)
			{
				pNotifier->notifyActivityState(OpenHMI::CScreenTouchServer::AS_PAUSE);
			}
			break;

		case APP_CMD_STOP:
			android_app_set_activity_state(android_app, cmd);
			if (pNotifier)
			{
				pNotifier->notifyActivityState(OpenHMI::CScreenTouchServer::AS_STOP);
			}
			break;

		case APP_CMD_CONFIG_CHANGED:
			OpenHMI::CLogSDK::info(L"APP_CMD_CONFIG_CHANGED");
			AConfiguration_fromAssetManager(android_app->config,
				android_app->activity->assetManager);
			// print_cur_config(android_app);
			break;

		case APP_CMD_DESTROY:
			OpenHMI::CLogSDK::info(L"APP_CMD_DESTROY");
			android_app->destroyRequested = 1;
			break;

		default:
			break;
		}
	}

	static void android_app_post_exec_cmd(struct android_app* android_app, int8_t cmd)
	{
		switch (cmd) {
		case APP_CMD_TERM_WINDOW:
			pthread_mutex_lock(&android_app->mutex);
			android_app->window = NULL;
			pthread_cond_broadcast(&android_app->cond);
			pthread_mutex_unlock(&android_app->mutex);
			break;

		case APP_CMD_SAVE_STATE:
			OpenHMI::CLogSDK::info(L"APP_CMD_SAVE_STATE");
			pthread_mutex_lock(&android_app->mutex);
			android_app->stateSaved = 1;
			pthread_cond_broadcast(&android_app->cond);
			pthread_mutex_unlock(&android_app->mutex);
			break;

		case APP_CMD_RESUME:
			free_saved_state(android_app);
			break;

		default:
			break;
		}
	}

	void process_cmd(struct android_app* app, struct android_poll_source* source)
	{
		int8_t cmd = android_app_read_cmd(app);
		android_app_pre_exec_cmd(app, cmd);
		if (app->onAppCmd != NULL) app->onAppCmd(app, cmd);
		android_app_post_exec_cmd(app, cmd);
	}
  #endif
#endif
