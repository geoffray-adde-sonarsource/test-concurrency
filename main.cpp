#include <iostream>
#include <mutex>

void f(std::mutex& m1, std::mutex& m2) {
    // std::try_lock tries to lock all passed mutexes. 
    // It returns -1 on success, or the 0-based index of the mutex it failed to lock.
    const int result = std::try_lock(m1, m2); // Noncompliant: neither is a lock object
    
    if (result == -1) {
        std::cout << "[Noncompliant] Both locks acquired successfully.\n";
        
        // Both m1 and m2 are locked, and must be released manually on all code paths.
        // This is dangerous because if an exception is thrown before unlock(), 
        // the mutexes remain locked forever (deadlock).
        m1.unlock(); // Noncompliant with S5506
        m2.unlock(); // Noncompliant with S5506
        std::cout << "[Noncompliant] Both locks released manually.\n";
    } else {
        std::cout << "[Noncompliant] Failed to acquire lock at index: " << result << "\n";
    }
}

int main() {
    std::mutex mutex1;
    std::mutex mutex2;

    std::cout << "Calling noncompliant function f()...\n";
    f(mutex1, mutex2);

    return 0;
}