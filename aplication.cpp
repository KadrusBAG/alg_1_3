#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <fstream>

template <typename T>
class queue_t {
private:
	T* elements_;
	std::size_t size_;
	std::size_t capacity_;
	std::size_t head_;
	std::size_t tail_;

public:
	queue_t() {
		elements_ = nullptr;
		size_ = 0;
		capacity_ = 0;
		head_ = 0;
		tail_ = 0;
	}

	explicit queue_t(size_t& n) {
		size_ = 0;
		head_ = 0;
		tail_ = 0;
		capacity_ = n;
		elements_ = new T[capacity_];
	}

	~queue_t() {
		delete[] elements_;
	}

	size_t size() {
		return size_;
	}

	void push(const T& el) {
		if (size_ == 0) {
			elements_[head_] = el;
			tail_ = (tail_ + 1) % capacity_;
			++size_;
		}
		else {
			elements_[tail_] = el;
			tail_ = (tail_ + 1) % capacity_;
			++size_;
		}
	}

	T pop() {
		T el = elements_[head_];
		head_ = (head_ + 1) % capacity_;
		--size_;
		return el;
	}

	std::string print() {
		std::ostringstream stroka{};
		for (size_t i = 0, j = head_; i < size_ - 1; j = (j + 1) % capacity_, ++i) {
			stroka << elements_[j] << " ";
		}
		stroka << elements_[head_ + size_ - 1] << "\n";
		return stroka.str();
	}
};

bool is_digit(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

std::string main_(std::string file) {
	std::ifstream fin(file);
	if (fin.is_open()) {
		bool success = false;
		std::ostringstream exit{};
		std::string time{};
		std::size_t t{};
		while (std::getline(fin, time)) {
			time += '\n';
			std::istringstream stream(time);
			while (stream >> time) {
				if (time.empty()) {
					continue;
				}
				if (time == "set_size" && stream.peek() != '\n') {
					stream >> time;
					if (is_digit(time)) {
						t = std::stoi(time);
						success = true;
						break;
					}
					else {
						exit << "error\n";
						stream.ignore(256, '\n');
					}
				}
				else {
					exit << "error\n";
					stream.ignore(256, '\n');
				}
			}
			if (success) break;
		}

		queue_t<std::string> queue(t);
		std::string time_{};

		while (std::getline(fin, time)) {
			time += '\n';
			std::istringstream stream(time);
			while (stream >> time) {
				if (time.empty()) {
					continue;
				}
				else if (time == "push" && stream >> time_ && stream.peek() == '\n') {
					if (queue.size() < t) {
						queue.push(time_);
					}
					else {
						exit << "overflow\n";
					}
				}
				else if (time == "pop" && stream.peek() == '\n') {
					if (queue.size() == 0) {
						exit << "underflow\n";
					}
					else {
						exit << queue.pop() << '\n';
					}
				}
				else if (time == "print" && stream.peek() == '\n') {
					if (queue.size() == 0) {
						exit << "empty\n";
					}
					else {
						exit << queue.print();
					}
				}
				else {
					exit << "error\n";
					stream.ignore(256, '\n');
				}
			}
		}
		fin.close();
		return exit.str();
	}
}

int main(int argc, char *argv[]) {
	std::string path_1 = argv[1];
	std::string path_2 = argv[2];
	std::ofstream fout(path_2);
	if (fout.is_open()) {
		fout << main_(path_1);
		fout.close();
	}
}
