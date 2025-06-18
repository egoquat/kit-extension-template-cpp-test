// Copyright (c) 2022, NVIDIA CORPORATION. All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto.  Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.
//

#define CARB_EXPORTS

#include <carb/PluginUtils.h>

#include <omni/ext/IExt.h>
#include <omni/kit/IApp.h>
#include <carb/eventdispatcher/IEventDispatcher.h>

const struct carb::PluginImplDesc pluginImplDesc = { "omni.example.cpp.hello_world.plugin",
                                                     "An example C++ extension.", "NVIDIA",
                                                     carb::PluginHotReload::eEnabled, "dev" };

CARB_PLUGIN_IMPL_DEPS(carb::eventdispatcher::IEventDispatcher)

using namespace carb::extras;

namespace omni
{
namespace example
{
namespace cpp
{
namespace hello_world
{

// When this extension is enabled, any class that derives from omni.ext.IExt
// will be instantiated and 'onStartup(extId)' called. When the extension is
// later disabled, a matching 'onShutdown()' call will be made on the object.
class ExampleCppHelloWorldExtension : public omni::ext::IExt
{
    //@TEST
    Timer TestStaticTimer; 
    const double TimeInterval = 1.0;
    double TimeIntervalElapsed = 0.0;
    double TimeElapsedLast = 0;
    double FramePerSecond = 0;
    //@TEST //
    
public:
    void onStartup(const char* extId) override
    {
        printf("ExampleCppHelloWorldExtension starting up (ext_id: %s).\n", extId);

        // Get the event dispatcher interface from the Carbonite Framework.
        if (carb::eventdispatcher::IEventDispatcher* eventDispatcher =
            carb::getCachedInterface<carb::eventdispatcher::IEventDispatcher>())
        {
            // Subscribe to update events.
            m_updateEventsSubscription = eventDispatcher->observeEvent(
                carb::RStringKey("Hello World Extension"), carb::eventdispatcher::kDefaultOrder,
                omni::kit::kGlobalEventPostUpdate,
                [this](const carb::eventdispatcher::Event& e) { onUpdate(); });
        }

        //@TEST
        TestStaticTimer.start();
    }

    void onShutdown() override
    {
        printf("ExampleCppHelloWorldExtension shutting down.\n");

        // Unsubscribe from update events.
        m_updateEventsSubscription.reset();
    }
    
    void onUpdate()
    {
        //@TEST
        double elapsedTimeUS = (double)TestStaticTimer.getElapsedTime(Timer::Scale::eMicroseconds);
        double elapsedTimeMS =  elapsedTimeUS / (double)1000;
        double elapsedSecond = elapsedTimeMS / (double)1000;

        double tickTime = elapsedSecond - TimeElapsedLast;
        TimeIntervalElapsed += tickTime;
        
        double fps = (double)1 / tickTime;

        if (TimeIntervalElapsed >= TimeInterval)
        {
            printf(" >>>>>>>> test ticktime: %lf / fps: %lf / fps tick time: %lf .\n", elapsedSecond , fps, tickTime);
            TimeIntervalElapsed = 0;
        }
        TimeElapsedLast = elapsedSecond;
        //@TEST //
        
        if (m_updateCounter % 1000 == 0)
        {
            printf("Hello from the omni.example.cpp.hello_world extension! %d updates counted.\n", m_updateCounter);
        }
        m_updateCounter++;
    }

private:
    carb::eventdispatcher::ObserverGuard m_updateEventsSubscription;
    int m_updateCounter = 0;
};

}
}
}
}

CARB_PLUGIN_IMPL(pluginImplDesc, omni::example::cpp::hello_world::ExampleCppHelloWorldExtension)

void fillInterface(omni::example::cpp::hello_world::ExampleCppHelloWorldExtension& iface)
{
}
