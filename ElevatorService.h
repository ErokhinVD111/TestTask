#include <vector>
using namespace std;

class ElevatorService {
    private:
        vector<int> floors_in_processing;
        vector<int> floors_history;

        struct actions_status {
            bool is_get_off;
            bool is_get_in;
            actions_status() : is_get_off(false), is_get_in(false) {}
        } actions;

        enum class Direction {
                UP,
                DOWN,
        } direction;

        int max_capacity;
        int max_floors;

    public:
        explicit ElevatorService(int max_capacity, int max_floors, vector<vector<int>> &queues)
        : direction(ElevatorService::Direction::UP)
        , max_capacity(max_capacity)
        , max_floors(max_floors) {}

        void set_default_actions_status() {
            actions.is_get_in = false;
            actions.is_get_off = false;
        }

        bool get_in_predicate(int floor, int number) {
            switch (direction) {
                case ElevatorService::Direction::UP:
                    return number > floor && floors_in_processing.size() < max_capacity;
                case ElevatorService::Direction::DOWN:
                    return number < floor && floors_in_processing.size() < max_capacity;
            }
        }

        bool is_full_predicate(int floor, int number) {
            switch (direction) {
                case ElevatorService::Direction::UP:
                    return number > floor && floors_in_processing.size() == max_capacity;
                case ElevatorService::Direction::DOWN:
                    return number < floor && floors_in_processing.size() == max_capacity;
            }
        }

        bool get_in_handler(int floor, vector<int> &floor_numbers_in_queue) {
            vector<int> remaining_numbers_on_floor;
            bool is_all_get_off = check_on_all_get_off();
            for (int number: floor_numbers_in_queue) {
                if (get_in_predicate(floor, number)) {
                    floors_in_processing.push_back(number);
                    actions.is_get_in = true;
                }
                else {
                    remaining_numbers_on_floor.push_back(number);
                }
                if (is_full_predicate(floor, number)) {
                    actions.is_get_in = true;
                }
            }
            floor_numbers_in_queue = remaining_numbers_on_floor;
            if (is_all_get_off)
                change_direction();
            return actions.is_get_in;
        }

        bool get_off_handler(int floor) {
            vector<int> remaining_numbers_in_elevator;
            for (int number: floors_in_processing) {
                if (number != floor)
                    remaining_numbers_in_elevator.push_back(number);
                else
                    actions.is_get_off = true;
            }
            floors_in_processing = remaining_numbers_in_elevator;
            return actions.is_get_off;
        }

        void main_handler(int floor, vector<int> &floor_numbers_in_queue) {
            bool is_changed = false;
            if (get_off_handler(floor)) {
                is_changed = true;
            }
            if (get_in_handler(floor, floor_numbers_in_queue)) {
                is_changed = true;
            }
            if (is_changed) {
                floors_history.push_back(floor);
            }
        }

        void change_direction() {
            if (direction == ElevatorService::Direction::UP) {
                direction = ElevatorService::Direction::DOWN;
            } else {
                direction = ElevatorService::Direction::UP;
            }
        }

        bool check_on_all_get_off() {
            if (actions.is_get_off && floors_in_processing.empty()) {
                change_direction();
                return true;
            }
            return false;
        }

        bool check_on_end_work(vector<vector<int>> &queues) {
            int check_on_empty_floors = 0;
            for (const auto& floor: queues) {
                if (floor.empty() && floors_in_processing.empty()) {
                    check_on_empty_floors++;
                }
            }
            return check_on_empty_floors == queues.size();
        }

        vector<int> run(vector<vector<int>> &queues) {
            for (int floor = 0;;) {
                set_default_actions_status();
                main_handler(floor, queues[floor]);
                switch (direction) {
                    case Direction::UP:floor++;
                        break;
                    case Direction::DOWN:floor--;
                        break;
                }
                if (floor == queues.size() - 1 || floor == 0) {
                    change_direction();
                }
                if (check_on_end_work(queues)) {
                    if (floors_history.back() != 0) {
                        floors_history.push_back(0);
                    }
                    return floors_history;
                }
            }
        }


};