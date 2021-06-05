#include "FileUtils/Allocation.h"
#define SUBALLOCATION_HEADER_LENGTH 1
#define ALLOCATION_OFFSET_SIZE 4
/**
 * Structure of data chunk is
 * header:
 * 1 byte | number of allocations
 * int [] | allocation offsets size is equal to number of allocations
 * body:
 * char []| data
 *
 * there should be something like InodeManager that will return Allocation
 * and then another thing will use allocations to
 * convert them into things like directories and file chunks
 */
Allocation::Allocation(unsigned char *d, unsigned char allocation) :
		data(d), allocation(allocation) {
	//intentionally left blank
}
Allocation::~Allocation() {
	delete this->data;
}

static inline unsigned int read_int(unsigned char *data, unsigned int offset) {
	return static_cast<unsigned int>(data[offset]) << 24
			| static_cast<unsigned int>(data[offset + 1]) << 16
			| static_cast<unsigned int>(data[offset + 2]) << 8
			| static_cast<unsigned int>(data[offset + 3]);
}

static inline unsigned int get_allocation_offset(unsigned char *data,
		unsigned char id) {
	unsigned int offset = (id * ALLOCATION_OFFSET_SIZE) + 1;
	return read_int(reinterpret_cast<unsigned char*>(data), offset);
}

unsigned char* Allocation::get_data() {
	return this->data + get_allocation_offset(this->data, this->allocation)
			+ SUBALLOCATION_HEADER_LENGTH;

}

AllocationType Allocation::get_type() {
	return static_cast<AllocationType>((this->data
			+ get_allocation_offset(this->data, this->allocation))[0]);
}

unsigned int Allocation::get_length() {
	unsigned int offset = get_allocation_offset(this->data, this->allocation);
	unsigned int next_allocation = get_allocation_offset(this->data,
			this->allocation + 1);
	unsigned int length_with_header = next_allocation - offset;

	return length_with_header == 0 ? 0 // suballocation has been freed
			: length_with_header - SUBALLOCATION_HEADER_LENGTH; // data_length = total_length - header length; header length for suballocation 1

}

#undef SUBALLOCATION_HEADER_LENGTH
