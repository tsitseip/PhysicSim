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
        auto renderBodies  = physicsBodies;
        auto bufferBodies  = physicsBodies;

        if (!graphics || !space || !universe) {
            core::cerr << core::constants::errors::MODULE_INIT_FAIL << core::nl;
            return -1;
        }

        core::Timer timer;
        double dt = config.getDeltaTime();
        int batchSize = config.getBatchSize();

        universe->setCameraTarget(graphics->getCameraTarget());

        std::mutex bodyMutex;
        std::atomic<bool> running = true;

        std::condition_variable cv;
        std::mutex cvMutex;

        std::thread physicsThread([&]() {
            while(running) {

                std::unique_lock<std::mutex> lk(cvMutex);
                cv.wait(lk, [&] { return !running || !graphics->isPaused(); });

                for (int i=0; i < batchSize; ++i){
                    universe->step(physicsBodies, *space, dt);
                    timer.tick.upd();
                }

                for (size_t i = 0; i < physicsBodies.size(); ++i) {
                    bufferBodies[i].state = physicsBodies[i].state;
                    bufferBodies[i].trail = physicsBodies[i].trail;
                    bufferBodies[i].active = physicsBodies[i].active;
                }
            
                {
                    core::lock_guard<core::mutex> lock(bodyMutex);
                    std::swap(bufferBodies, renderBodies);
                }
                // std::this_thread::sleep_for(std::chrono::microseconds(1));
            }

        });

        while (graphics->isOpen()) {
            timer.frame.upd();
            graphics->clear();

            bool wasPaused = graphics->isPaused();
            {
                core::lock_guard<core::mutex> lock(bodyMutex);
                graphics->handleInput(renderBodies);
                graphics->render(renderBodies);
            }

            if (wasPaused != graphics->isPaused()) {
                cv.notify_one();
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
