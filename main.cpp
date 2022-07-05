#include <vector>
#include <iostream>
#include "ElevatorService.h"

using namespace std;

void performCalculation(int capacity, vector<vector<int>> &queues, vector<int> &floorsList) {
    //здесь проводятся все расчеты и составляется floorsList
    ElevatorService elevator_service(capacity, queues.size(), queues);
    floorsList = elevator_service.run(queues);
}

int main() {
    //максимально допустимое количество людей в лифте
    int capacity = 5;

    //список очередей людей для всех этажей здания
    vector<vector<int>> queues
        {
            {}, // 1-ый этаж – людей нет
            {0, 2, 2, 0, 3}, // 2-ой этаж - 5 человек, которые хотят попасть на 1-ый, 3-ий, 3-ий, 1-ый и 4-ый этажи соответственно
            {},
            {0, 0, 0},
            {2},
            {3, 1, 1, 0}
        };
    //список всех индексов этажей, на которых остановился лифт в порядке посещения
    vector<int> floorsList;

    performCalculation(capacity, queues, floorsList);

    for (int floor: floorsList) {
        cout << floor << " ";
    }
    cout << endl;
    //floorsList = {1, 2, 3, 5, 4, 3, 1, 0, 5, 4, 2, 1, 0}

    return 0;
}
