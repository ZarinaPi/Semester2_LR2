#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <limits>

using namespace std;

class List {
public:
    int data;
    List* prev;
    List* next;
    List(int data) : data(data), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    List* head;
    List* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        clear();
    }

    void clear() {
        List* current = head;
        while (current != nullptr) {
            List* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    void fillRandom(int N) {
        clear();
        srand(time(0));
        for (int i = 0; i < N; ++i) {
            int value = rand() % 100;
            append(value);
        }
    }

    void fillFromInput() {
        clear();
        int value;
        std::cout << "Введите элементы (введите НЕ число для остановки): ";
        while (std::cin >> value) {
            append(value);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void append(int value) {
        List* newList = new List(value);
        if (tail == nullptr) {
            head = tail = newList;
        }
        else {
            tail->next = newList;
            newList->prev = tail;
            tail = newList;
        }
        ++size;
    }

    void insertAt(int index, int value) {
        if (index < 0 || index > size) {
            std::cout << "Некорректный индекс" << std::endl;
            return;
        }

        // Проверка value

        List* newList = new List(value);
        if (index == 0) {
            newList->next = head;
            if (head != nullptr) {
                head->prev = newList;
            }
            head = newList;
            if (tail == nullptr) {
                tail = newList;
            }
        }
        else if (index == size) {
            tail->next = newList;
            newList->prev = tail;
            tail = newList;
        }
        else {
            List* current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            newList->next = current;
            newList->prev = current->prev;
            current->prev->next = newList;
            current->prev = newList;
        }
        ++size;
    }

    void removeAt(int index) {
        if (head == nullptr) {
            std::cout << "Список пустой" << std::endl;
            return;
        }
        if (index < 0 || index >= size) {
            std::cout << "Некорректный индекс" << std::endl;
            return;
        }

        // Ошибка: пользователь может ввести не число!


        List* toDelete = head;
        if (index == 0) {
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            else {
                tail = nullptr;
            }
        }
        else if (index == size - 1) {
            toDelete = tail;
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            for (int i = 0; i < index; ++i) {
                toDelete = toDelete->next;
            }
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }
        delete toDelete;
        --size;
    }

    void removeByValue(int value) {
        if (head == nullptr) {
            std::cout << "Список пустой" << std::endl;
            return;
        }

        // Проверка value

        List* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                List* toDelete = current;
                if (current == head) {
                    head = head->next;
                    if (head != nullptr) {
                        head->prev = nullptr;
                    }
                    else {
                        tail = nullptr;
                    }
                }
                else if (current == tail) {
                    tail = tail->prev;
                    tail->next = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                current = current->next;
                delete toDelete;
                --size;
            }
            else {
                current = current->next;
            }
        }
    }

    void getAt(int index) {
        if (index < 0 || index >= size) {
            std::cout << "Некорректный индекс" << std::endl;
            return;
        }
        List* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        std::cout << "____________________________\n\n";
        std::cout << "Элемент по заданному индексу: " << current->data << std::endl;
        std::cout << "____________________________\n\n";
    }

    void getByValue(int value) {
        std::cout << "____________________________\n\n";
        List* current = head;
        bool founded = false;
        while (current != nullptr) {
            if (current->data == value) {
                founded = true;
                std::cout << "Элемент найден: " << current->data << std::endl;
                break;
            }
            current = current->next;
        }
        if (!founded)
            std::cout << "Элемент не найден" << std::endl;
        std::cout << "____________________________\n\n";
    }

    void swap(int index1, int index2) {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
            std::cout << "Некорректный индекс" << std::endl;
            return;
        }
        if (index1 == index2) {
            std::cout << "Индексы совпадают" << std::endl;
            return;
        }
        List* list1 = head;
        List* list2 = head;
        for (int i = 0; i < index1; ++i) {
            list1 = list1->next;
        }
        for (int i = 0; i < index2; ++i) {
            list2 = list2->next;
        }
        std::swap(list1->data, list2->data);
    }

    void print() {
        List* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }


    template <typename Func, typename... Args>
    void measureTime(Func func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        (this->*func)(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "-------------\n";
        std::cout << "Операция заняла: " << duration.count() << " секунд" << std::endl;
        std::cout << "-------------\n\n";
    }
};

int main() {
    setlocale(LC_ALL, "Rus");
    DoublyLinkedList dll;
    int choice, N, index, value;

    do {
        std::cout << "Меню:\n1. Заполнение списка случайными элементами\n2. Заполнение списка элементами с консоли\n3. Вставка элемента\n4. Удаление элемента по индексу\n5. Удаление элемента по значкеню\n6. Получение элемента по индексу\n7. Получение элемента по значению\n8. Обмен элементов\n9. Вывод списка\n10. Выход\nВыберите операцию: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Введите кол-во элементов: ";
            std::cin >> N;
            dll.measureTime(&DoublyLinkedList::fillRandom, N);
            break;
        case 2:
            dll.measureTime(&DoublyLinkedList::fillFromInput);
            break;
        case 3:
            std::cout << "Введите индекс и значение: ";
            std::cin >> index >> value;
            dll.measureTime(&DoublyLinkedList::insertAt, index, value);
            break;
        case 4:
            std::cout << "Введите индекс: ";
            std::cin >> index;
            dll.measureTime(&DoublyLinkedList::removeAt, index);
            break;
        case 5:
            std::cout << "Введите значение: ";
            std::cin >> value;
            dll.measureTime(&DoublyLinkedList::removeByValue, value);
            break;
        case 6:
            std::cout << "Введите индекс: ";
            std::cin >> index;
            dll.measureTime(&DoublyLinkedList::getAt, index);
            break;
        case 7:
            std::cout << "Введите значение: ";
            std::cin >> value;
            dll.measureTime(&DoublyLinkedList::getByValue, value);
            break;
        case 8:
            int index1, index2;
            std::cout << "Введите два индекса: ";
            std::cin >> index1 >> index2;
            dll.measureTime(&DoublyLinkedList::swap, index1, index2);
            break;
        case 9:
            std::cout << "____________________________\n\n";
            dll.print();
            std::cout << "____________________________\n\n";
            break;
        case 10:
            break;
        default:
            std::cout << "Некорректный ввод. Попробуйте снова." << std::endl;
        }
    } while (choice != 10);

    return 0;
}