#include <iostream>
#include <queue>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

//File written in JetBrains Toolbox CLion, if di po gumana sa vscode try niyo po sa Clion
using namespace std;

class Person {
public:
    string name;
    int ticket_number;

    Person(string n, int num) : name(n), ticket_number(num) {}
};


class Queue {
private:
    queue<Person> q;
    int ticket_counter = 1;
    mutable mutex mtx;

public:
    void Enqueue(const string& name) {
        lock_guard<mutex> lock(mtx);
        Person new_person(name, ticket_counter++);
        q.push(new_person);
        cout << name << " added to the queue with Ticket #" << new_person.ticket_number << endl;
    }

    void AutoDequeue() {
        lock_guard<mutex> lock(mtx);
        if (!q.empty()) {
            Person front_person = q.front();
            cout << "\nDequeue: " << front_person.name << " received a ticket (Ticket #" << front_person.ticket_number << ")" << endl;
            q.pop();
            cout << "Queue size: " << q.size() << endl;
            if (!q.empty()) {
                Person next_person = q.front();
                cout << "Next in line: " << next_person.name << " (Ticket #" << next_person.ticket_number << ")" << endl;
            }
        }
    }

    bool IsEmpty() const {
        lock_guard<mutex> lock(mtx);
        return q.empty();
    }

    int Size() const {
        lock_guard<mutex> lock(mtx);
        return q.size();
    }

    void Peek() const {
        lock_guard<mutex> lock(mtx);
        if (!IsEmpty()) {
            Person front_person = q.front();
            cout << "Next in line: " << front_person.name << " (Ticket #" << front_person.ticket_number << ")" << endl;
        } else {
            cout << "The queue is empty!" << endl;
        }
    }

    void Position(const string& name) const {
        lock_guard<mutex> lock(mtx);
        int position = 1;
        for (queue<Person> temp = q; !temp.empty(); temp.pop(), position++) {
            Person person = temp.front();
            if (person.name == name) {
                cout << "Your position in the queue is: " << position << endl;
                cout << "Your Ticket Number is: Ticket #" << person.ticket_number << endl;
                return;
            }
        }
        cout << name << " is not in the queue." << endl;
    }
};

// Auto dequeueu function
void autoDequeueTimer(Queue &queue) {
    while (true) {
        this_thread::sleep_for(chrono::seconds(30)); // Dequeue every 30 secs
        queue.AutoDequeue();
    }
}

int main() {
    Queue queue;
    int option;
    bool running = true;

    // Process for dequeueing
    thread autoDequeueThread(autoDequeueTimer, ref(queue));

    cout << "Welcome to Olivia Rodrigo's Concert Ticketing System!" << endl;

    while (running) {
        cout << "\n1. Enqueue a person\n2. Check your position in the queue.\n3. Exit\nChoose an option: ";
        cin >> option;

        switch (option) {
            case 1: {
                string name;
                cout << "Enter the name: ";
                cin >> name;
                queue.Enqueue(name);
                cout << "Queue size: " << queue.Size() << endl;
                break;
            }
            case 2: {
                string name;
                cout << "Enter your name: ";
                cin >> name;
                queue.Position(name);
                break;
            }
            case 3:
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }

    // Detatching the dequeue thread
    autoDequeueThread.detach();
    return 0;
}