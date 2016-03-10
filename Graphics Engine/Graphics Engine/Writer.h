#include "Object.h"
#include "Animation.h"

class Writer{

private:
	static Writer* instance;

	Writer() = default;
	~Writer() = default;

	void SaveObject(std::fstream* file, Object& _object);
	bool LoadObject(std::fstream* file, Object& _object);

public:

	void SaveObject(const char* fileName, Object& object);
	bool LoadObject(const char* fileName, Object& object);

	static Writer* GetInstance();
	static void Terminate();
};