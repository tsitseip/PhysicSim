#include "core/Configurator.h"
#include "core/Globals.h"
#include "core/Timer.h"

int main() {
    try {
        core::cout << core::format(core::constants::ui::START_MSG, DIMENSION) << core::nl;

        // 1. Load Config
        core::string configPath = "config.json";
        core::ifstream check(configPath);
        if (!check.good()) {
            configPath = "src/config.json"; 
        }
        core::Configurator<double, DIMENSION> config(configPath);

        // 2. Initialize Modules
        auto graphics = config.createGraphicsModule();
        auto space    = config.createSpace();
        auto universe = config.createUniverse();
        auto physicsBodies = config.loadBodies();
        auto renderBodies  = physicsBodies; // Clone for rendering

        if (!graphics || !space || !universe) {
            core::cerr << core::constants::errors::MODULE_INIT_FAIL << core::nl;
            return -1;
        }

        core::Timer timer;
        double dt = config.getDeltaTime();
        int batchSize = config.getBatchSize();

        universe->setCameraTarget(graphics->getCameraTarget());

        core::mutex bodyMutex;
        core::atomic<bool> running = true;

        core::thread physicsThread([&]() {
            while(running) {
                for (int i=0; i < batchSize; ++i){
                    universe->step(physicsBodies, *space, dt);
                    timer.tick.upd();
                }

                {
                    core::lock_guard<core::mutex> lock(bodyMutex);
                    for (size_t i = 0; i < physicsBodies.size(); ++i) {
                        renderBodies[i].state = physicsBodies[i].state;
                        renderBodies[i].trail = physicsBodies[i].trail;
                        renderBodies[i].active = physicsBodies[i].active;
                    }
                }
                // std::this_thread::sleep_for(std::chrono::microseconds(1));
            }

        });

        while (graphics->isOpen()) {
            timer.frame.upd();
            graphics->clear();

            {
                core::lock_guard<core::mutex> lock(bodyMutex);
                graphics->handleInput(renderBodies);
                graphics->render(renderBodies);
            }

            graphics->update();
            if (timer.tick_one_second()) {
                graphics->updateHUD(timer.frame.rate, timer.tick.rate);
            }
        }
        running = false;
        physicsThread.join();

    } catch (const core::exception& e) {
        core::cerr << core::format(core::constants::errors::GENERIC_CRITICAL, e.what()) << core::nl;
        return -1;
    }

    return 0;
}
