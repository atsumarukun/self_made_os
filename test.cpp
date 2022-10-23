#include <stdint.h>
#include <stdio.h>

template <typename T>
class Test {
	public:
		T Read() const {
			T tmp;
			for (int i = 0; i < size_; i++) {
				tmp.data[i] = value_.data[i];	
			}
			return tmp;
		}
		const int Size() const {return size_;}
		int* P() {return (int*) &size_;}
	private:
		volatile T value_;
		static const int size_ = sizeof(T) / sizeof(value_.data[0]);
};

union Data {
	uint32_t data[4];
	struct {
		uint32_t: 32;
		uint32_t: 32;
		uint32_t: 32;
		uint32_t: 32;
	} __attribute__((packed)) bits;
} __attribute__((packed));

union D {
	uint64_t data[1];
} __attribute__((packed));

struct S {
	Test<Data> test;
	Test<D> ttt;
} __attribute__((packed));

int main(void) {
	uint32_t d[12];
	d[0] = 0xff32u;
	d[6] = 2;
	S* s = (S*) &d;
	//printf("%p\n", &S);
	printf("%p\n", s->test.P());
	printf("%p\n", s->ttt.P());

	return 0;
}
