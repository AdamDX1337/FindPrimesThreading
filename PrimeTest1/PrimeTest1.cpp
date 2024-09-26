#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx; 

void checkPrimes(int start, int end, std::vector<int>& primes, int& count) {
    for (int prime = start; prime < end; prime++) {
        bool isPrime = true;
        for (int i = 2; i * i <= prime; i++) {
            if (prime % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            
            mtx.lock();
            primes.push_back(prime); 
            count++;
            mtx.unlock(); 
        }
    }
}

int main() {
    int inf = 100000000; 
    int numThreads = std::thread::hardware_concurrency(); 
    std::vector<int> primes;
    std::vector<std::thread> threads;
    int count = 0; 

    int range = inf / numThreads; 
    for (int i = 0; i < numThreads; i++) {
        int start = i * range + 2; 
        int end = (i == numThreads - 1) ? inf : start + range; 
        threads.emplace_back(checkPrimes, start, end, std::ref(primes), std::ref(count)); 
    }

    for (auto& thr : threads) {
        thr.join(); 
    }

    
    for (size_t i = 0; i < primes.size(); i++) {
        if (i % 1000 == 0) { 
            std::cout << primes[i] << std::endl;
        }
    }

    return 0;
}


