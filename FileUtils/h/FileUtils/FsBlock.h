#ifndef FSBLOCK_H_INCLUDED
#define FSBLOCK_H_INCLUDED

/**
 *	\brief Filesystem block
 *
 *	The filesystem supports block_suballocations see https://en.wikipedia.org/wiki/Block_suballocation
 *	Reason behind: in the current approach each block is a file and we want it to be reasonably big.
 *	Suballocation seem to me as a good compromise between big files consisting of thousands chunks and
 *	small files that occupy whole chunk.
 *
 *	Drawback of this approach is that after updating small file the whole block has to be reuploaded
 *
 *	max number of chunks(suballocations) in the block is 255
 */
class FsBlock
{
	public:
		/**
		 * \param data data. should be already decrypted
		 */
		FsBlock(char* data);
		~FsBlock();
		unsigned char count_chunks() noexcept;
		/**
		 * returns chunks data. This <b>does not</b> copy memory. please do not edit this
		 * \param n chunk number
		 * \return chunk.
		 */
		char* read_chunk(unsigned char n);
		/**
		 * removes chunk. may lead to moving chunks after this chunk
		 */
		void remove_chunk(unsigned char n);
		unsigned char insert_chunk(unsigned int size);
		unsigned int free_bytes() noexcept;
		void clear();
		unsigned int chunk_size(unsigned char n) noexcept;
		unsigned int get_chunk_offset(unsigned char n) noexcept;
		void resize_chunk(unsigned char n, unsigned int size) noexcept;
	private:
		char* data;

		unsigned int header_size() noexcept;
		int get_first_skipped_chunk() noexcept;
		void set_chunk_offset(unsigned char n, unsigned int value) noexcept;
};

#endif // FSBLOCK_H_INCLUDED
