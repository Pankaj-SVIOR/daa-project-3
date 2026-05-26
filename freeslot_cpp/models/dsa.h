// =====================================================
// dsa.h  —  All Data Structures for this project
// =====================================================
// Contains 4 data structures:
//   1. Stack       -> Undo last booking  (LIFO)
//   2. Queue       -> Booking requests   (FIFO)
//   3. Linked List -> Room booking history
//   4. Hash Table  -> Fast key-value lookup
// =====================================================

#ifndef DSA_H
#define DSA_H

#include <string>
#include <vector>

// =====================================================
// Simple struct to hold one booking's info
// Used by Stack and Queue
// =====================================================
struct Booking {
    int teacher_id;
    int room_id;
    std::string date;
    int slot;

    Booking() : teacher_id(0), room_id(0), slot(0) {}
    Booking(int tid, int rid, std::string d, int s)
        : teacher_id(tid), room_id(rid), date(d), slot(s) {}
};


// =====================================================
// 1. STACK  (Last In, First Out — LIFO)
// =====================================================
// Used for: UNDO the last booking
// Think of a stack of plates — you take from the top
// DAA Concept: Stack
// =====================================================
class BookingStack {
public:
    // Push = add booking to top of stack
    void push(Booking b) {
        // Using Stack (LIFO) — new item goes to top
        data.push_back(b);
    }

    // Pop = remove and return top booking
    // Returns false if stack is empty
    bool pop(Booking& result) {
        // Using Stack (LIFO) — remove from top
        if (data.empty()) return false;
        result = data.back();
        data.pop_back();
        return true;
    }

    bool isEmpty() {
        return data.empty();
    }

private:
    std::vector<Booking> data;  // vector used as stack storage
};


// =====================================================
// 2. QUEUE  (First In, First Out — FIFO)
// =====================================================
// Used for: Queue of pending booking requests
// Like a line at a counter — first come, first served
// DAA Concept: Queue
// =====================================================
class BookingQueue {
public:
    // Enqueue = add to back of the queue
    void enqueue(Booking b) {
        // Using Queue (FIFO) — new item goes to back
        data.push_back(b);
    }

    // Dequeue = remove from front of the queue
    bool dequeue(Booking& result) {
        // Using Queue (FIFO) — remove from front
        if (data.empty()) return false;
        result = data.front();
        data.erase(data.begin());
        return true;
    }

    bool isEmpty() {
        return data.empty();
    }

private:
    std::vector<Booking> data;
};


// =====================================================
// 3. LINKED LIST  (Singly Linked List)
// =====================================================
// Used for: Room booking history
// Each node stores one booking record and points to next
// DAA Concept: Linked List
// =====================================================

// A single node in the linked list
struct HistoryNode {
    std::string info;   // booking info like "2024-01-15 slot 3"
    HistoryNode* next;  // pointer to next node

    HistoryNode(std::string val) : info(val), next(NULL) {}
};

class BookingHistory {
public:
    BookingHistory() : head(NULL) {}

    // Add a new record at the end of the list
    void append(std::string info) {
        // Using Linked List — traverse to end, add new node
        HistoryNode* newNode = new HistoryNode(info);
        if (head == NULL) {
            head = newNode;
            return;
        }
        HistoryNode* curr = head;
        while (curr->next != NULL) {
            curr = curr->next;  // walk to end
        }
        curr->next = newNode;
    }

    // Convert linked list to vector of strings (for JSON output)
    std::vector<std::string> toList() {
        // Using Linked List — traverse from head to end
        std::vector<std::string> result;
        HistoryNode* curr = head;
        while (curr != NULL) {
            result.push_back(curr->info);
            curr = curr->next;
        }
        return result;
    }

    // Destructor: free all allocated memory
    ~BookingHistory() {
        HistoryNode* curr = head;
        while (curr != NULL) {
            HistoryNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }

private:
    HistoryNode* head;  // start of the chain
};


// =====================================================
// 4. HASH TABLE  (Key -> Value mapping)
// =====================================================
// Used for: Fast slot-number <-> time-string lookup
// O(1) average lookup instead of looping every time
// Uses chaining to handle collisions
// DAA Concept: Hash Table / Hash Map
// =====================================================
class SimpleHashTable {
public:
    SimpleHashTable(int size) : tableSize(size) {
        // Each bucket holds a list of (key, value) pairs
        buckets.resize(size);
    }

    // Store key -> value
    void set(std::string key, int value) {
        // Using Hash Table — compute bucket index with hash function
        int index = hashFunc(key);
        for (int i = 0; i < (int)buckets[index].size(); i++) {
            if (buckets[index][i].first == key) {
                buckets[index][i].second = value;  // update existing
                return;
            }
        }
        buckets[index].push_back(std::make_pair(key, value));
    }

    // Get value by key, returns -1 if not found
    int get(std::string key) {
        // Using Hash Table — O(1) average time lookup
        int index = hashFunc(key);
        for (int i = 0; i < (int)buckets[index].size(); i++) {
            if (buckets[index][i].first == key) {
                return buckets[index][i].second;
            }
        }
        return -1;  // key not found
    }

private:
    int tableSize;
    std::vector<std::vector<std::pair<std::string, int>>> buckets;

    // Simple hash: sum up ASCII values of all characters
    int hashFunc(const std::string& key) {
        int sum = 0;
        for (int i = 0; i < (int)key.size(); i++) {
            sum += (int)key[i];
        }
        return sum % tableSize;
    }
};

#endif // DSA_H
