#pragma once
#include <iostream>
#include <string>

namespace voi {
	constexpr size_t MAX_STATIC_SIZE = 100;

	template<typename T>
	class List {
	public:
		virtual inline size_t size() const = 0;
		virtual inline size_t capacity() const = 0;
		virtual inline bool empty() const = 0;
		virtual bool pushBack(T n) = 0;
		virtual T popBack() = 0;
		virtual bool pushFront(T n) = 0;
		virtual T popFront() = 0;
		virtual bool insert(size_t i, T n) = 0;
		virtual T remove(size_t i) = 0;
		virtual T& operator[] (size_t i) = 0;
	};
	class StaticData {
		virtual inline bool full() const = 0;
	};

	template<typename T>
	class ArrayList : public List<T>{
		T* data;
		size_t m_size = 0;
		size_t m_capacity = 0;

	public:
		ArrayList() {
			m_capacity = 4;
			data = (T*)malloc(sizeof(T) * m_capacity);
		}
		ArrayList(size_t p_capacity) {
			m_capacity = p_capacity;
			data = (T*)malloc(sizeof(T) * p_capacity);
		}

		~ArrayList() {
			free(data);
		}

		inline size_t size() const override { return m_size; }
		inline size_t capacity() const override { return m_capacity; }

		bool pushBack(T n) override {
			if (full()) {
				m_capacity *= 2;
				T* temp = (T*) malloc(sizeof(T) * m_capacity);

				for (size_t i = 0; i < m_size; i++) temp[i] = data[i];
				
				temp[m_size++] = n;

				free(data);
				data = temp;
			}
			else {
				data[m_size++] = n;
			}
			return true;
		}
		 
		T popBack() override {
			if (!empty()) {
				if (m_capacity > 4 && (m_size - 1) <= m_capacity / 4) {
					T n = data[--m_size];
					T* temp = (T*)malloc(sizeof(T) * (m_capacity / 4));

					for (size_t i = 0; i < m_size; i++) temp[i] = data[i];

					free(data);
					data = temp;
					m_capacity /= 4;

					return n;
				}
				return data[--m_size];
			}
			std::cout << "ERROR::EMPTY LIST" << std::endl;
			throw "ERROR::EMPTY LIST";
			return data[0];
		}

		bool pushFront(T n) override {
			return insert(0, n);
		}

		T popFront() override {
			return remove(0);
		}

		bool insert(size_t i, T n) override {
			this->operator[](i);
			if (full()) {
				m_capacity *= 2;
				T* temp = (T*)malloc(sizeof(T) * m_capacity);

				for (int j = 0; j < i; j++) temp[j] = data[j];
				temp[i] = n;
				for (int j = i; j < m_size; j++) temp[j + 1] = data[j];

				m_size++;

				free(data);
				data = temp;
			}
			else {
				T x = data[i];
				data[i] = n;
				for (int j = i; j < m_size; j++) {
					if (j % 2 == i % 2) {
						n = data[j + 1];
						data[j + 1] = x;
					}
					else {
						x = data[j + 1];
						data[j + 1] = n;
					}
				}

				m_size++;
			}
			return n;
		}

		T remove(size_t i) override {
			if (!empty()) {
				m_size--;
				T n = this->operator[](i);
				if (m_capacity > 4 && (m_size - 1) <= m_capacity / 4) {
					T* temp = (T*)malloc(sizeof(T) * (m_capacity / 4));

					for (int j = i; j < m_size; j++) temp[i] = data[j + 1];

					free(data);
					data = temp;
					m_capacity /= 4;
				}
				else {
					for (int j = i; j < m_size; j++) data[j] = data[j + 1];
				}
				return n;
			}
			std::cout << "ERROR::EMPTY LIST" << std::endl;
			throw "ERROR::EMPTY LIST";
			return data[0];
		}

		T& operator [](size_t i) override {
			if (i > m_size-1 || i < 0) {
				std::cout << "ERROR::OUT OF RANGE ACCESS" << std::endl;
				throw "ERROR::OUT OF RANGE ACCESS";
			}
			return data[i % m_size];
		}

		inline bool empty() const override { return m_size <= 0; }
		inline bool full() const{ return m_size >= m_capacity; }

		std::string toStr() const {
			std::string s;

			s += '[';
			for (int i = 0; i < m_size; i++) {
				s += std::to_string(data[i]);
				if (i != m_size - 1) s += ", ";
			}
			s += ']';

			return s;
		}
	};


	template<typename T>
	struct Node {
		T data; Node<T>* next, * prev;
	};
	template<typename T>
	class LinkedList : public List<T> {
		Node<T>* head = nullptr;
		Node<T>* tail = nullptr;
		size_t m_size = 0;

	public:

		/*~LinkedList() {
			Node<T>* temp;

			while (head != nullptr) {
				popBack();
			}
		}*/

		inline size_t size() const override { return m_size; }
		inline size_t capacity() const override { return m_size; }

		inline bool empty() const override { return m_size <= 0; }

		bool pushBack(T n) override {
			Node<T>* temp = new Node<T>;
			temp->data = n;
			if (empty()) {
				head = temp;
				tail = head;
			}
			else {
				tail->next = temp;
				temp->prev = tail;
				tail = temp;
			}
			m_size++;
			return true;
		}

		T popBack() override {
			T n = tail->data;

			if (head == tail) {
				head = nullptr;
				delete tail;
				tail = nullptr;
			}
			else {
				Node<T>* temp = tail;
				tail = tail->prev;
				tail->next = nullptr;
				delete temp;
			}
			m_size--;
			return n;
		}

		bool pushFront(T n) override {
			Node<T>* temp = new Node<T>;
			temp->data = n;
			if (empty()) {
				head = temp;
				tail = head;
			}
			else {
				head->prev = temp;
				temp->next = head;
				head = temp;
			}
			m_size++;
			return true;
		}

		T popFront() override {
			T n = head->data;

			if (head == tail) {
				tail = nullptr;
				delete head;
				head = nullptr;
			}
			else {
				Node<T>* temp = head;
				head = head->next;
				head->prev = nullptr;
				delete temp;
			}
			m_size--;
			return n;
		}

		bool insert(size_t i, T n) override {
			Node<T>* iNode = getNode(i);
			Node<T>* temp = new Node<T>;

			temp->data = n;
			temp->next = iNode;
			temp->prev = iNode->prev;
			iNode->prev = temp;
			temp->prev->next = temp;

			m_size++;

			return true;
		}

		T remove(size_t i) {
			if (i == 0) return popFront();
			if (i == m_size - 1) return popBack();
			Node<T>* temp = getNode(i);
			if (head == tail) {
				tail = nullptr;
				delete head;
				head = nullptr;
			}
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;

			T n = temp->data;
			delete temp;
			m_size--;

			return n;
		}

		T& operator[] (size_t i) {
			if (i > m_size - 1 || i < 0) {
				std::cout << "ERROR::OUT OF RANGE ACCESS" << std::endl;
				throw "ERROR::OUT OF RANGE ACCESS";
			}
			Node<T>* temp;
			if (i < m_size / 2) {
				temp = head;
				for (int j = 0; j < i; j++) {
					temp = temp->next;
				}
			}
			else {
				temp = tail;
				for (int j = m_size - 1; j > i; j--) {
					temp = temp->prev;
				}
			}

			return temp->data;
		}

		Node<T>* getNode(size_t i) {
			if (i > m_size - 1 || i < 0) {
				std::cout << "ERROR::OUT OF RANGE ACCESS" << std::endl;
				throw "ERROR::OUT OF RANGE ACCESS";
			}
			Node<T>* temp;
			if (i < m_size / 2) {
				temp = head;
				for (int j = 0; j < i; j++) {
					temp = temp->next;
				}
			}
			else {
				temp = tail;
				for (int j = m_size - 1; j > i; j--) {
					temp = temp->prev;
				}
			}

			return temp;
		}

		std::string toStr() const {
			std::string s;
			Node<T>* d = head;

			s += '[';
			for (int i = 0; i < m_size; i++) {
				s += std::to_string(d->data);
				if (i != m_size - 1) s += ", ";
				d = d->next;
			}
			s += ']';

			return s;
		}
	};



	template<typename T>
	class StaticList : public List<T>, public StaticData {
		T* data;
		size_t m_head = 0;
		size_t m_size = 0;
		size_t m_capacity = 0;

		inline size_t dataIndex(size_t i) const { return ((m_head + i) % m_capacity); }
	protected:
		void innit(size_t p_capacity) {
			m_capacity = p_capacity;
			data = (T*)malloc(sizeof(T) * p_capacity);
		}

		StaticList() {};

	public:
		StaticList(size_t p_capacity) {
			m_capacity = p_capacity;
			data = (T*)malloc(sizeof(T) * p_capacity);
		}

		~StaticList() {
			free(data);
		}

		inline size_t size() const override { return m_size; }
		inline size_t capacity() const override { return m_capacity; }

		inline bool empty() const override { return m_size <= 0; }
		inline bool full() const override { return m_size >= m_capacity; }

		bool pushBack(T n) override {
			if (full()) return false;

			data[dataIndex(m_size)] = n;
			m_size++;

			return true;
		}

		T popBack() override {
			if (!empty()) {
				m_size--;
				return data[dataIndex(m_size)];
			}
			std::cout << "ERROR::EMPTY LIST" << std::endl;
			throw "ERROR::EMPTY LIST";
			return data[0];
		}

		bool pushFront(T n) override {
			if (full()) return false;

			m_head = (m_head - 1) % m_capacity;
			data[m_head] = n;
			m_size++;

			return true;
		}

		T popFront() override {
			if (!empty()) {
				T n = data[m_head];
				m_head = (m_head + 1) % m_capacity;
				m_size--;
				return n;
			}
			std::cout << "ERROR::EMPTY LIST" << std::endl;
			throw "ERROR::EMPTY LIST";
			return data[0];
		}

		bool insert(size_t i, T n) override {
			if (full()) return false;

			T x = this->operator[](i);
			m_size++;
			data[dataIndex(i)] = n;

			for (int j = i; j < m_size; j++) {
				if (j % 2 == i % 2) {
					n = data[dataIndex(j + 1)];
					data[dataIndex(j + 1)] = x;
				}
				else {
					x = data[dataIndex(j + 1)];
					data[dataIndex(j + 1)] = n;
				}
			}

			return true;
		}

		T remove(size_t i) override {
			if (!empty()) {
				T n = this->operator[](i);
				m_size--;

				for (int j = i; j < m_size; j++) data[dataIndex(j)] = data[dataIndex(j+1)];
			}
			std::cout << "ERROR::EMPTY LIST" << std::endl;
			throw "ERROR::EMPTY LIST";
			return data[0];
		}

		T& operator[](size_t i) override {
			if (i > m_size - 1 || i < 0) {
				std::cout << "ERROR::OUT OF RANGE ACCESS" << std::endl;
				throw "ERROR::OUT OF RANGE ACCESS";
			}
			return data[(m_head + i) % m_capacity];
		}

		std::string toStr() const {
			std::string s;

			s += '[';
			for (int i = 0; i < m_size; i++) {
				s += std::to_string(data[dataIndex(i)]);
				if (i != m_size - 1) s += ", ";
			}
			s += ']';

			return s;
		}
	};

	template<typename T>
	class StaticQueue: public StaticList<T> {
	public:
		StaticQueue(){
			this->innit(MAX_STATIC_SIZE);
		}

		inline bool enqueue(T n) { return this->pushBack(n); }
		inline T dequeue() { return this->popFront(); }
		inline T& head() { return this->operator[](0); }
		inline T& tail() { return this->operator[](this->size() - 1); }
	};

	template<typename T>
	class StaticStack : public StaticList<T> {
		StaticStack(){
			this->innit(MAX_STATIC_SIZE);
		}

		inline bool push(T n) { return this->pushBack(n); }
		inline T pop() { return this->popBack(); }
		inline T& top() { return this->operator[](this->size() - 1); }
		inline T& bottom() { return this->operator[](0); }
	};
}
