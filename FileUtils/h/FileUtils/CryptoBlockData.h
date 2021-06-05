#ifndef FILEUTILS_H_FILEUTILS_CRYPTOBLOCKDATA_H_
#define FILEUTILS_H_FILEUTILS_CRYPTOBLOCKDATA_H_



class CryptoBlockData
{
public:
	CryptoBlockData(const char* block, const char* iv);
	~CryptoBlockData();
	const char* block;
	const char* iv;
};

#endif /* FILEUTILS_H_FILEUTILS_CRYPTOBLOCKDATA_H_ */
